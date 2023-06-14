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
#include "crypto.h"
#include "dbmodelhandler.h"
#include "utils.h"
#include "dataexporter.h"
#include "dbdefs.h"
#include "filectl.h"

#include "person.h"
#include "community.h"
#include "specialist.h"
#include "prebuiltdefs.h"
#include "stringdefs.h"

DbModel::DbModel():
    mDeletable(true)
    , mDbId(0)
    , mValidateResult(nullptr)
    , mDbStatus(0)
    , mMarkModified(false)
    , mUpdateAllFields(false)
    , mDbCreatedTime(0)
    , mLastDbUpdatedTime(0)

{
    tracein;
    mRefCnt.storeRelease(0);
//    init(); // TODO: should call init here?
}

DbModel::DbModel(const DbModel &model):DbModel()
{
    tracein;
    copy(model);
    // TODO: mValidateResult
    traceout;
}

DbModel::DbModel(const DbModel *model):DbModel()
{
    tracein;
    if (model) {
        copy(*model);
    }
    traceout;
}

void DbModel::copy(const DbModel &model)
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
    traceout;
}


DbModel::~DbModel()
{
    tracein;
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
        copy(*model);
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
        mName = model->name();
        markItemAsModified(KItemName);
        mRemark = model->remark();
        markItemAsModified(KItemRemark);
//        mDeletable = model->mDeletable;
//        mDbId = model->dbId();
//        mUid = model->uid();
//        mNameId = model->nameId();
//        mDbStatus = model->dbStatus();
//        mDbHistory = model->dbHistory();
//        mDbCreatedTime = model->dbCreatedTime();
//        mLastDbUpdatedTime = model->lastDbUpdatedTime();
//        mMarkModified = model->markModified();
//        mUpdateAllFields = model->updateAllFields();
//        mExportCallbacks = model->mExportCallbacks;
//        mImportCallbacks = model->mImportCallbacks;
//        mUpdatedField = model->mUpdatedField;
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
        map.insert(MODEL_NOT_READY, QObject::tr("Không rõ"));
        map.insert(MODEL_INACTIVE, QObject::tr("Không hoạt động"));
        map.insert(MODEL_ACTIVE, QObject::tr("Đang hoạt động"));
        map.insert(MODEL_BUILDING, QObject::tr("Đang xây dựng"));
        isInitiedStatusNameMap = true;
    }
    // TODO: make it as static to load once only???
    traceout;
    return &map;
}

QString DbModel::modelStatus2Name(DbModelStatus status)
{
    const QHash<int, QString>* statuses = getModelStatusIdNameMap();
    QString ret;
    tracein;
    logd("Status to get name %d", status);
    if (statuses->contains(status)){
        ret = statuses->value(status);
    } else {
        loge("invalid status %d", status);
        ret = STR_UNKNOWN; // TODO: translate???
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
        statusList.push_back(MODEL_NOT_READY);
        statusList.push_back(MODEL_INACTIVE);
        statusList.push_back(MODEL_ACTIVE);
        statusList.push_back(MODEL_BUILDING);
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

DbModelBuilder DbModel::getBuilderByModelName(const QString& modelName)
{
    tracein;
    static bool isInited = false;
    static QHash<QString, DbModelBuilder> map;
    DbModelBuilder builder = nullptr;
    if (!isInited) {
        map[KModelNamePerson] = &Person::build;
        map[KModelNameCommunity] = &Community::build;
        map[KModelHdlSpecialist] = &Specialist::build;
        isInited = true;
    }
    if (map.contains(modelName)) {
        builder = map.value(modelName);
    }
    // TODO: make it as static to load once only???
    traceout;
    return builder;
}

const QString &DbModel::name() const
{
    return mName;
}

void DbModel::setName(const QString &newName)
{
    mName = newName;

    markItemAsModified(KItemName);
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
    mUid = newUid;
    markItemAsModified(KItemUid);
}

void DbModel::buildUidIfNotSet()
{
    tracein;
    // TODO: is this this function neccessary anymore?
    if (uid().isEmpty()){
        setUid(buildUid());
    }
}

QString DbModel::buildUid(const QString* seed)
{
    tracein;
    QString value = QString("%1%2%3").arg(nameId(), name()).arg(Utils::currentTimeMs());
    if (seed != nullptr) {
        value += "_" + *seed;
    }
    logd("value for uid calc: %s", STR2CHA(value));
    QString uid = Utils::UidFromName(value, UidNameConvertType::HASH_NAME);
    logd("uid: %s", uid.toStdString().c_str());
    return uid;
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
    tracein;
    if (markModified()) {
        logd("mark item %s as modified", itemName.toStdString().c_str());

        if (!mUpdatedField.contains(itemName)) {
            mUpdatedField.append(itemName);
        } else {
            logd("Item already existed");
        }
    } else {
        logd("Skip mark modified for %s", itemName.toStdString().c_str());
    }

    traceout;
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

const QString &DbModel::remark() const
{
    return mRemark;
}

void DbModel::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
    markItemAsModified(KItemRemark);
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

ErrCode DbModel::exportToFile(ExportType type, QString *fpath)
{
    tracein;
    ErrCode ret = ErrNone;
    QString fname = QString("%1.html").arg(uid());
    logd("fname '%s'", STR2CHA(fname));
    *fpath = FileCtl::getTmpDataFile(fname);
    logd("fpath '%s'", STR2CHA((*fpath)));
    ret = ExportFactory::exportTo(getExporter(),
                                  *fpath, type);
    // TODO: how about exportTo() method??
    return ret;
}

const QString DbModel::exportTemplatePath(FileExporter* exporter, QString* ftype) const
{
    tracein;
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_HTML:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltCommonTemplateFileName);
        };
    }
    traceout;
    return QString();
}

const QStringList DbModel::getListExportKeyWord() const
{
    tracein;

    return mExportCallbacks.keys();
}

ErrCode DbModel::getExportDataString(const QString &item, QString *data) const
{
    ErrCode ret = ErrNone;
    tracein;
    logd("item %s", item.toStdString().c_str());
    if (mExportCallbacks.contains(item)){
        ExportCallbackFunc func = mExportCallbacks.value(item);
        if (func != nullptr) *data = func(this, item);
    }
    // TODO: raise exception when error occur???

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
    logd("keyword %s", keyword.toStdString().c_str());
    if (mImportCallbacks.contains(keyword)){
        ImportCallbackFunc func = mImportCallbacks.value(keyword);
        if (func != nullptr) ret = func(value);
    }
    traceret(ret);
    return ret;
}

const QList<QString> &DbModel::updatedField() const
{
    return mUpdatedField;
}

bool DbModel::isFieldUpdated(const QString &itemField)
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
    logd("Total marked item: %d", mUpdatedField.count());
    mUpdatedField.clear();
    logd("Total marked item after clear: %d", mUpdatedField.count());
    traceout;
}

qint64 DbModel::lastDbUpdatedTime() const
{
    return mLastDbUpdatedTime;
}

void DbModel::setLastDbUpdatedTime(qint64 newLastUpdatedTime)
{
    mLastDbUpdatedTime = newLastUpdatedTime;
    markItemAsModified(KItemLastUpdateTime);
}

qint64 DbModel::dbCreatedTime() const
{
    return mDbCreatedTime;
}

void DbModel::setDbCreatedTime(qint64 newCreatedTime)
{
    mDbCreatedTime = newCreatedTime;
    markItemAsModified(KItemCreateTime);
}

ErrCode DbModel::save()
{
    tracein;
    ErrCode ret = ErrNone;
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        ret = prepare2Save();
        if (ret == ErrNone) {
            ret = dbModelHdl->add(this);
        }
//        if (ret == ErrExisted){ // alrady exist, judge as ok
//            ret = ErrNone;
//            logi("%s already exist", name().toStdString().c_str());
//        }
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

ErrCode DbModel::update(bool allFields)
{
    tracein;
    ErrCode ret = ErrNone;
    // TODO: support "allFields"
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        ret = dbModelHdl->update(this);
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
    mDbHistory = newDbHistory;

    markItemAsModified(KItemDbHistory);
}

ErrCode DbModel::remove(bool force, QString* msg)
{
    tracein;
    ErrCode ret = ErrNone;
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
    ErrCode ret = ErrNone;

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
    mDbStatus = newDbStatus;
    markItemAsModified(KItemDbStatus);
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

void DbModel::dump()
{
    tracein;
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE
    logd("- modelName %s", modelName().toStdString().c_str());
    logd("- DbId %lld", dbId());
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- NameId %s", nameId().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- mDbCreatedTime %s", Utils::timeMsToDatestring(dbCreatedTime()).toStdString().c_str());
    logd("- mLastDbUpdatedTime %s", Utils::timeMsToDatestring(lastDbUpdatedTime()).toStdString().c_str());
#endif //DEBUG_TRACE
}

QString DbModel::toString() const
{
    return QString("%1:%2:%3:%4").arg(modelName(), uid(), nameId(),name());
}

void DbModel::setNameId(const QString &newNameId)
{
    tracein;
    logd("Set name id %s", newNameId.toStdString().c_str());
//    mUid = Utils::UidFromName(newNameId);
    // TODO: should hash or use original value???
//    mUid = newNameId;
    mNameId = newNameId;
    markItemAsModified(KItemNameId);
}

DataExporter *DbModel::getExporter()
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
