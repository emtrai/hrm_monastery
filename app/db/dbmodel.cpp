/*
 * Copyright (C) 2022 Ngo Huy Anh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Filename: dbmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "dbmodel.h"
#include "logger.h"
#include "dbmodelhandler.h"
#include "utils.h"
#include "dataexporter.h"
#include "dbdefs.h"
#include "filectl.h"

#include "prebuiltdefs.h"
#include "stringdefs.h"
#include "datetimeutils.h"
#include <QUuid>

#define MAX_HISTORY_LENGTH 500

DbModel::DbModel():
    mDeletable(true)
    , mValidateResult(nullptr)
    , mDbId(0)
    , mMarkModified(false)
    , mDbStatus(0)
    , mUpdateAllFields(false)
    , mDbCreatedTime(0)
    , mLastDbUpdatedTime(0)
    , mLoadedAllData(false)

{
    traced;
    mRefCnt.storeRelease(0);
//    init(); // TODO: should call init here?
}

DbModel::DbModel(const DbModel &model):DbModel()
{
    tracein;
    docopy(model);
    // TODO: mValidateResult
    traceout;
}

DbModel::DbModel(const DbModel *model):DbModel()
{
    tracein;
    if (model) {
        docopy(*model);
    }
    traceout;
}

void DbModel::docopy(const DbModel &model)
{
    tracein;
    mDeletable = model.mDeletable;
    mDbId = model.dbId();
    mUid = model.uid();
    mName = model.name();
    mRemark = model.remark();
    mNameId = model.nameId();
    mDbStatus = model.dbStatus();
    mDbHistory = model.dbHistory();
    mDbCreatedTime = model.dbCreatedTime();
    mLastDbUpdatedTime = model.lastDbUpdatedTime();
    mMarkModified = model.markModified();
    mUpdateAllFields = model.updateAllFields();
    mExportCallbacks = model.mExportCallbacks;
    mImportCallbacks = model.mImportCallbacks;
    mUpdatedField = model.mUpdatedField;
    mLoadedAllData = model.mLoadedAllData;
    traceout;
}

void DbModel::copy(const DbModel *model)
{
    UNUSED(model);
    // do nothing
}


DbModel::~DbModel()
{
    traced;
}

void DbModel::init()
{
    tracein;
    initExportFields();
    initImportFields();
    traceout;
}

void DbModel::clone(const DbModel *model)
{
    tracein;
    if (model) {
        logd("clone from model '%s'", STR2CHA(model->toString()));
        docopy(*model);
        copy(model);
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

DbModel *DbModel::clone() const
{
    tracein;
    DbModel* model = getBuilder()(); // TODO: check builder null or not???

    logd("clone new model '%s'", STR2CHA(toString()));
    model->clone((DbModel*)this);
    traceout;
    return model;
}

ErrCode DbModel::copyData(const DbModel *model)
{
    tracein;
    ErrCode err = ErrNone;
    if (model) {

        mUpdateAllFields = model->updateAllFields();
        setName(model->name());
        setRemark(model->remark());
        setNameId(model->nameId());
        setUid(model->uid());
        setDbId(model->dbId());
        setDbStatus(model->dbStatus());
        setDbHistory(model->dbHistory());
        setDbCreatedTime(model->dbCreatedTime());
        setLastDbUpdatedTime(model->lastDbUpdatedTime());
        mDeletable = model->mDeletable;

        mExportCallbacks = model->mExportCallbacks;
        mImportCallbacks = model->mImportCallbacks;
        mImportItemsType = model->mImportItemsType;
        mImportFieldRequired = model->mImportFieldRequired;
        mUpdatedField = model->updatedField();
        mUpdateAllFields = model->mUpdateAllFields;
        mLoadedAllData = model->mLoadedAllData;
    } else {
        loge("copy data failed, empty model");
        err = ErrInvalidArg;
    }
    traceret(err);
    return err;
}

void DbModel::incRef()
{
    mRefCnt.ref();
    logd("mRefCnt %d", mRefCnt.loadAcquire());
}

void DbModel::decRef()
{
    if (mRefCnt > 0) {
        mRefCnt.deref();
    }
    logd("mRefCnt %d", mRefCnt.loadAcquire());
}


QString DbModel::modelName() const
{
    return "DbModel";
}

int DbModel::modelType() const
{
    return MODEL_UNKNOWN;
}

void DbModel::initExportFields()
{
    tracein;
    mExportCallbacks.insert(KItemName, [](const DbModel* model, const QString& item){
        return model->name();

    });
    mExportCallbacks.insert(KItemNameId, [](const DbModel* model, const QString& item){
        return model->nameId();

    });
    mExportCallbacks.insert(KItemUid, [](const DbModel* model, const QString& item){
        return model->uid();

    });
    mExportCallbacks.insert(KItemRemark, [](const DbModel* model, const QString& item){
        return model->remark();

    });
    traceout;
}

void DbModel::initImportFields()
{
    tracein;
    mImportCallbacks.insert(KItemName, [this](const QString& value){
        this->setName(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemNameId, [this](const QString& value){
        this->setNameId(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemUid, [this](const QString& value){
        this->setUid(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemRemark, [this](const QString& value){
        this->setRemark(value);
        return ErrNone;
    });
    traceout;
}

const QHash<int, QString> *DbModel::getModelStatusIdNameMap()
{
    tracein;
    static bool isInitiedStatusNameMap = false;
    static QHash<int, QString> map;
    if (!isInitiedStatusNameMap) {
        map.insert(MODEL_STATUS_UNKNOWN, STR_UNKNOWN);
        map.insert(MODEL_STATUS_NOT_READY, STR_NOT_READY); // TODO: change to suitable name???
        map.insert(MODEL_STATUS_INACTIVE, STR_INACTIVE);
        map.insert(MODEL_STATUS_ACTIVE, STR_ACTIVE);
        map.insert(MODEL_STATUS_BUILDING, STR_BUILDING);
        isInitiedStatusNameMap = true;
    }
    // TODO: make it as static to load once only???
    traceout;
    return &map;
}

QString DbModel::modelStatus2Name(DbModelStatus status, bool* ok)
{
    const QHash<int, QString>* statuses = getModelStatusIdNameMap();
    QString ret;
    tracein;
    logd("Status to get name %d", status);
    if (statuses->contains(status)){
        ret = statuses->value(status);
        if (ok) *ok = true;
    } else {
        loge("invalid status %d", status);
        ret = STR_UNKNOWN; // TODO: translate???
        if (ok) *ok = false;
    }
    traceout;
    return ret;
}

const QList<int>* DbModel::getModelStatusList()
{
    tracein;
    static bool isInitiedStatus = false;
    static QList<int> statusList;
    if (!isInitiedStatus) {
        statusList.push_back(MODEL_STATUS_UNKNOWN);
        statusList.push_back(MODEL_STATUS_NOT_READY);
        statusList.push_back(MODEL_STATUS_INACTIVE);
        statusList.push_back(MODEL_STATUS_ACTIVE);
        statusList.push_back(MODEL_STATUS_BUILDING);
        isInitiedStatus = true;
    }
    // TODO: make it as static to load once only???
    traceout;
    return &statusList;
}

const QList<int>* DbModel::getDbStatusList()
{
    tracein;
    static bool isInitiedDbStatus = false;
    static QList<int> dbstatusList;
    if (!isInitiedDbStatus) {
        dbstatusList.push_back(DB_RECORD_NOT_READY);
        dbstatusList.push_back(DB_RECORD_ACTIVE);
        dbstatusList.push_back(DB_RECORD_DElETED);
        isInitiedDbStatus = true;
    }
    // TODO: make it as static to load once only???
    traceout;
    return &dbstatusList;
}

const QString &DbModel::name() const
{
    return mName;
}

QString DbModel::fullName() const
{
    return name();
}

void DbModel::setName(const QString &newName)
{
    CHECK_MODIFIED_THEN_SET(mName, newName, KItemName);
}

qint64 DbModel::dbId() const
{
    return mDbId;
}

void DbModel::setDbId(qint64 newDbId)
{
    mDbId = newDbId;
}

const QString &DbModel::uid() const
{
    return mUid;
}

void DbModel::setUid(const QString &newUid)
{
    CHECK_MODIFIED_THEN_SET(mUid, newUid, KItemUid);
}

void DbModel::buildUidIfNotSet()
{
    tracein;
    // TODO: is this this function neccessary anymore?
    if (uid().isEmpty()){
        setUid(buildUid());
    }
    traceout;
}

QString DbModel::buildUid(const QString* seed)
{
    tracein;
    QUuid uuid = QUuid::createUuid();
    QString value = QString("%1%2%3").arg(nameId(), name()).arg(DatetimeUtils::currentTimeMs());
    if (!uuid.isNull()) {
        value += uuid.toString();
    }
    if (seed != nullptr) {
        value += "_" + *seed;
    }
    logd("value for uid calc: %s", STR2CHA(value));
    QString uid = Utils::UidFromName(value, UidNameConvertType::HASH_NAME);
    dbgd("built uid: %s", STR2CHA(uid));
    traceout;
    return uid;
}

QString DbModel::buildNameId(const QString *seed, bool* ok)
{
    QString name = this->name();
    QString nameid;
    tracein;
    if (!name.isEmpty()) {
        nameid = Utils::UidFromName(name, NO_VN_MARK_UPPER);
    } else {
        logw("no name to build nameid");
    }
    if (seed && !seed->isEmpty()) {
        logd("set seed");
        if (!nameid.isEmpty()) {
            nameid += "_";
        }
        nameid += Utils::UidFromName(*seed, NO_VN_MARK_UPPER);
    }
    if (ok) *ok = !nameid.isEmpty();
    logd("built nameid %s", STR2CHA(nameid));
    traceout;
    return nameid;
}

ErrCode DbModel::prepare2Save()
{
    tracein;
//    buildUidIfNotSet();
    // TODO: consider again should build uid here? by default, uid will be calc later after inserting to db successfull
    return ErrNone;
}

void DbModel::markItemAsModified(const QString &itemName)
{
    if (markModified()) {
        logd("mark item %s as modified", itemName.toStdString().c_str());

        if (!mUpdatedField.contains(itemName)) {
            mUpdatedField.append(itemName);
        } else {
        }
    }// avoid print log here, as there will be a lot of logs
}



void DbModel::checkModifiedThenSet(QString &cur, const QString &next, const QString &itemName)
{
    CHECK_MODIFIED_THEN_SET(cur, next, itemName);
}

void DbModel::checkModifiedThenSet(qint32 &cur, qint32 next, const QString &itemName)
{
    CHECK_MODIFIED_THEN_SET(cur, next, itemName);
}

void DbModel::checkModifiedThenSet(qint64 &cur, qint64 next, const QString &itemName)
{
    CHECK_MODIFIED_THEN_SET(cur, next, itemName);
}

void DbModel::appendDbHistory(const QString &msg)
{
    tracein;
    QString history = dbHistory();
    history += QString("%1:%2;")
                   .arg(DatetimeUtils::currentTimeToDatestring(QT_DATE_FORMAT_DMYHMS),
                        msg);
    if (history.length() > MAX_HISTORY_LENGTH) {
        history = history.right(MAX_HISTORY_LENGTH);
    }
    setDbHistory(history);
    traceout;
}

const QString &DbModel::remark() const
{
    return mRemark;
}

void DbModel::setRemark(const QString &newRemark)
{
    CHECK_MODIFIED_THEN_SET(mRemark, newRemark, KItemRemark);
}

const QString &DbModel::nameId() const
{
    return mNameId;
}

bool DbModel::markModified() const
{
    return mMarkModified;
}

void DbModel::setMarkModified(bool newMarkModified)
{
    logd("set mark modify %d, olde %d", newMarkModified, mMarkModified);
    // start mark modified, reset all previous info if any
    resetAllModifiedMark();
    mMarkModified = newMarkModified;
}

ErrCode DbModel::exportToFile(ExportType type, QString *fpath) const
{
    tracein;
    ErrCode ret = ErrNone;
    QString path;
    if (fpath && !fpath->isEmpty()) {
        path = *fpath;
    } else {
        QString fname = QString("%1.html").arg(uid());
        logd("fname '%s'", STR2CHA(fname));
        path = FileCtl::getTmpDataFile(fname);
        if (fpath) *fpath = path;
    }
    logd("fpath '%s'", STR2CHA(path));
    ret = ExportFactory::exportTo(getExporter(),
                                  getName(),
                                  path, type);
    // TODO: how about exportTo() method??
    return ret;
}

QString DbModel::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltCommonTemplateFileName;
}

ErrCode DbModel::exportTemplatePath(FileExporter* exporter,
                                    const QString& name,
                                    QString& fpath, QString* ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(ftype);
    QString templateFname;
    if (exporter) {
        ExportType type = exporter->getExportType();
        logd("export type '%d', name '%s'", type, STR2CHA(name));
        switch (type) {
        case EXPORT_HTML:
            templateFname = exportHtmlTemplateFile(name);
            logd("templateFname '%s'", STR2CHA(templateFname));
            fpath = FileCtl::getPrebuiltDataFilePath(templateFname);
            break;
        default:
            err = ErrNotSupport;
            loge("export type %d not support", type);
        };
    }
    traceout;
    return err;
}

const QStringList DbModel::getListExportKeyWord() const
{
    tracein;

    return mExportCallbacks.keys();
}

ErrCode DbModel::getExportDataString(const QString &item,
                                     const FileExporter* fileexporter,
                                     const QString& datatype,
                                     QString *data) const
{
    ErrCode ret = ErrNone;
    tracein;
    logd("item %s", item.toStdString().c_str());
    if (mExportCallbacks.contains(item)){
        ExportCallbackFunc func = mExportCallbacks.value(item);
        if (func != nullptr) *data = func(this, item);
    }
    // TODO: raise exception when error occur???
    traceret(ret);
    return ret;
}

ErrCode DbModel::onImportParseDataItem(const QString& importName, int importFileType,
                              const QString &keyword, const QString &value,
                              quint32 idx, QList<DbModel *>* outList)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("importFileType %d", importFileType);

    // TODO: raise exception when error occur???
    logd("keyword %s", STR2CHA(keyword));
    if (!keyword.isEmpty()) {
        if (mImportCallbacks.contains(keyword)){
            ImportCallbackFunc func = mImportCallbacks.value(keyword);
            if (func != nullptr) {
                if (!value.isEmpty()) {
                    ret = func(value);
                } else {
                    logw("empty value for keyword '%s' for model '%s'",
                        STR2CHA(keyword), STR2CHA(modelName()));
                }
            } else {
                logw("keyword '%s' has no callback in model '%s'",
                     STR2CHA(keyword), STR2CHA(modelName()));
            }
        } else {
            logw("keyword '%s' not support in model '%s'",
                 STR2CHA(keyword), STR2CHA(modelName()));
        }
    } else {
        logw("keywork is empty");
    }
    traceret(ret);
    return ret;
}

const QList<QString> &DbModel::updatedField() const
{
    return mUpdatedField;
}

bool DbModel::isFieldUpdated(const QString &itemField) const
{
    tracein;
    bool updated = false;
    if (!itemField.isEmpty()) {
        if (markModified()) {
            logd("item field to be check: %s", STR2CHA(itemField));
            if (mUpdatedField.contains(itemField)){
                updated = true;
            } else {
                logw("Field '%s' is not exist", STR2CHA(itemField));
            }
        } else {
            logw("Model is not enable modified flag");
        }
    } else {
        logw("Invalid argument");
    }
    logd("updated=%d", updated);
    traceret(updated);
    return updated;
}

void DbModel::resetAllModifiedMark()
{
    tracein;
    logd("Total marked item: %lld", mUpdatedField.count());
    mUpdatedField.clear();
    logd("Total marked item after clear: %lld", mUpdatedField.count());
    traceout;
}

qint64 DbModel::lastDbUpdatedTime() const
{
    return mLastDbUpdatedTime;
}

void DbModel::setLastDbUpdatedTime(qint64 newLastUpdatedTime)
{
    CHECK_MODIFIED_THEN_SET(mLastDbUpdatedTime, newLastUpdatedTime, KItemLastUpdateTime);
}

qint64 DbModel::dbCreatedTime() const
{
    return mDbCreatedTime;
}

void DbModel::setDbCreatedTime(qint64 newCreatedTime)
{
    CHECK_MODIFIED_THEN_SET(mDbCreatedTime, newCreatedTime, KItemCreateTime);
}

ErrCode DbModel::save(bool notifyDataChange)
{
    tracein;
    ErrCode ret = ErrNone;
    dbgtrace;
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        ret = prepare2Save();
        if (ret == ErrNone) {
            appendDbHistory("Add");
            logi("Save model '%s'", MODELSTR2CHA(this));
            ret = dbModelHdl->add(this, notifyDataChange);
        }
    }
    else{
        ret = ErrDbNotReady;
        loge("db not ready");
    }
    traceret(ret);
    return ret;
}

DbModel *DbModel::addFieldToBeUpdated(const QString &field)
{
    if (!mUpdatedField.contains(field)) {
        mUpdatedField.append(field);
    } else {
        logd("Item already existed");
    }
    return this;
}

ErrCode DbModel::update(bool allFields, bool notifyDataChange)
{
    tracein;
    UNUSED(allFields);
    ErrCode ret = ErrNone;
    dbgtrace;
    // TODO: support "allFields"
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        appendDbHistory("Update");
        logi("Update model '%s'", MODELSTR2CHA(this));
        ret = dbModelHdl->update(this, notifyDataChange);
    }
    else{
        ret = ErrDbNotReady;
        loge("db not ready");
    }
    traceret(ret);
    return ret;
}

bool DbModel::allowRemove(QString* msg)
{
    return mDeletable;
}

ErrCode DbModel::loadAllData()
{
    // do nothing here, derived class will implement this
    return ErrNone;
}

void DbModel::check2LoadAllData()
{
    if (!mLoadedAllData) {
        dbgd("All data not load yet, load all");
        ErrCode err = loadAllData();
        dbgd("load all data ret=%d", err);
        if (err == ErrNone) {
            mLoadedAllData = true;
        }
    }
}

bool DbModel::isNameIdUpdated() const
{
    return isFieldUpdated(KItemNameId);
}

quint32 DbModel::refCnt() const
{
    return mRefCnt;
}

bool DbModel::updateAllFields() const
{
    return mUpdateAllFields;
}

const QString &DbModel::dbHistory() const
{
    return mDbHistory;
}

void DbModel::setDbHistory(const QString &newDbHistory)
{
    CHECK_MODIFIED_THEN_SET(mDbHistory, newDbHistory, KItemDbHistory);
}

ErrCode DbModel::remove(bool force, QString* msg)
{
    tracein;
    ErrCode ret = ErrNone;
    dbgtrace;
    logi("Remove model '%s', force=%d", MODELSTR2CHA(this), force);
    if (!allowRemove()) {
        ret = ErrDenied;
        loge("No allow delete?");
    }
    if (ret == ErrNone) {
        DbModelHandler* dbModelHdl = getDbModelHandler();
        if (dbModelHdl != nullptr){
            ret = dbModelHdl->deleteHard(this, force, msg);
        }
        else{
            ret = ErrDbNotReady;
            loge("db not ready");
        }
    }
    traceret(ret);
    return ret;
}

ErrCode DbModel::markRemove()
{
    tracein;
    ErrCode ret = ErrNone;
    logi("markRemove model '%s'", MODELSTR2CHA(this));
    if (!allowRemove()) {
        ret = ErrDenied;
        loge("No allow delete?");
    }
    if (ret == ErrNone) {
        DbModelHandler* dbModelHdl = getDbModelHandler();
        if (dbModelHdl != nullptr){
            ret = dbModelHdl->deleteSoft(this);
        }
        else{
            ret = ErrDbNotReady;
            loge("db not ready");
        }
    }
    traceret(ret);
    return ret;
}

ErrCode DbModel::exportTo(const QString &fpath, ExportType type)
{
    tracein;

    UNUSED(fpath);
    UNUSED(type);
    loge("Not support export here");
    return ErrNotSupport;
}

qint32 DbModel::dbStatus() const
{
    return mDbStatus;
}

void DbModel::setDbStatus(qint32 newDbStatus)
{
    CHECK_MODIFIED_THEN_SET(mDbStatus, newDbStatus, KItemDbStatus);
}

bool DbModel::isValid()
{
    // TODO: add more checking for valid info
    tracein;
    bool isValid = !nameId().isEmpty();
    logd("isValid = %d", isValid);
    traceout;
    return isValid;
}

void DbModel::dump() const
{
    tracein;
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE
    logd("- modelName %s", STR2CHA(modelName()));
    logd("- DbId %lld", dbId());
    logd("- Uid %s", STR2CHA(uid()));
    logd("- NameId %s", STR2CHA(nameId()));
    logd("- Name %s", STR2CHA(name()));
    logd("- mDbCreatedTime %s", STR2CHA(DatetimeUtils::timeMsToDatestring(dbCreatedTime())));
    logd("- mLastDbUpdatedTime %s", STR2CHA(DatetimeUtils::timeMsToDatestring(lastDbUpdatedTime())));
#endif //DEBUG_TRACE
    traceout;
}

QString DbModel::toString() const
{
    return QString("modelName('%1'):uid('%2'):nameid('%3'):name('%4')")
        .arg(modelName(), uid(), nameId(),name());
}

QString DbModel::getName() const
{
    return modelName();
}

void DbModel::setNameId(const QString &newNameId)
{
    tracein;
    logd("Set name id %s", newNameId.toStdString().c_str());
//    mUid = Utils::UidFromName(newNameId);
    // TODO: should hash or use original value???
//    mUid = newNameId;
    CHECK_MODIFIED_THEN_SET(mNameId, newNameId,KItemNameId);
}

const DataExporter *DbModel::getExporter() const
{
    return this;
}

ErrCode DbModel::validateAllFields(bool checkExist)
{
    tracein;
    ErrCode err = ErrNone;
    if (checkExist && isExist()) {
        err = ErrExisted;
        logd("already existed");
    }
    traceret(err);
    return err;
}

bool DbModel::isExist()
{
    bool isExist = false;
    tracein;
    DbModelHandler* hdlr = this->getDbModelHandler();
    if (hdlr->exist(this)) {
        loge("Model already existed");
        isExist = true;
    }
    traceret(isExist);
    return isExist;
}

ErrCode DbModel::checkMatch(int &perc)
{
    tracein;
    ErrCode err = ErrNone;
    DbModelHandler* hdlr = this->getDbModelHandler();
    if (hdlr) {
        err = hdlr->checkMatch(this, perc);
    } else {
        err = ErrNoHandler;
        loge("not found handler");
    }
    traceret(err);
    return err;
}

QHash<QString, ErrCode> *DbModel::validateResult() const
{
    return mValidateResult;
}

const QString &DbModel::validateMsg() const
{
    return mValidateMsg;
}

void DbModel::appendValidateResult(const QString& item, ErrCode res)
{
    if (mValidateResult == nullptr) {
        mValidateResult = new QHash<QString, ErrCode>();
    }

    logd("App validate item '%s':%d", item.toStdString().c_str(), res);
    (*mValidateResult)[item] = res;
}

void DbModel::appendValidateMsg(const QString &newValidateMsg)
{
    tracein;
    logd("App validate msg '%s'", newValidateMsg.toStdString().c_str());
    mValidateMsg.append(newValidateMsg + ";");
}

void DbModel::cleanValidateResult()
{
    tracein;
    mValidateMsg.clear();
    if (mValidateResult){
        mValidateResult->clear();
    }
}
