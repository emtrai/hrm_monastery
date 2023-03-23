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

DbModel::DbModel():
    mDeletable(true)
    , mDbId(0)
    , mValidateResult(nullptr)
    , mDbStatus(0)
    , mMarkModified(false)
    , mUpdateAllFields(false)
    , mCreatedTime(0)
    , mLastUpdatedTime(0)

{
    traced;
//    init(); // TODO: should call init here?
}

DbModel::DbModel(const DbModel &model):DbModel()
{
    traced;
    copy(model);
    // TODO: mValidateResult
    tracede;
}

DbModel::DbModel(const DbModel *model)
{
    traced;
    if (model) {
        DbModel(*model);
        copy(*model);
    }
    tracede;
}

void DbModel::copy(const DbModel &model)
{
    traced;
    mDeletable = model.mDeletable;
    mDbId = model.dbId();
    mUid = model.uid();
    mName = model.name();
    mRemark = model.remark();
    mNameId = model.nameId();
    mDbStatus = model.dbStatus();
    mDbHistory = model.dbHistory();
    mCreatedTime = model.createdTime();
    mLastUpdatedTime = model.lastUpdatedTime();
    mMarkModified = model.markModified();
    mUpdateAllFields = model.updateAllFields();
    mExportCallbacks = model.mExportCallbacks;
    mImportCallbacks = model.mImportCallbacks;
    tracede;
}


DbModel::~DbModel()
{
    traced;
}

void DbModel::init()
{
    traced;
    initExportFields();
    initImportFields();
    tracede;
}

void DbModel::clone(const DbModel *model)
{
    traced;
    if (model) {
        copy(*model);
    } else {
        loge("clone failed, null model");
    }
    tracede;
}

DbModel *DbModel::clone()
{
    traced;
    DbModel* model = getBuilder()(); // TODO: check builder null or not???
    model->clone(this);
    tracede;
    return model;
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
    traced;
    mExportCallbacks.insert(KItemName, [this](const QString& item){
        return this->name();

    });
    mExportCallbacks.insert(KItemNameId, [this](const QString& item){
        return this->nameId();

    });
    mExportCallbacks.insert(KItemUid, [this](const QString& item){
        return this->uid();

    });
    mExportCallbacks.insert(KItemRemark, [this](const QString& item){
        return this->remark();

    });
    tracede;
}

void DbModel::initImportFields()
{
    traced;
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
    tracede;
}

const QHash<int, QString> *DbModel::getStatusIdNameMap()
{
    traced;
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
    tracede;
    return &map;
}

QString DbModel::status2Name(DbModelStatus status)
{
    const QHash<int, QString>* statuses = getStatusIdNameMap();
    QString ret;
    traced;
    logd("Status to get name %d", status);
    if (statuses->contains(status)){
        ret = statuses->value(status);
    } else {
        loge("invalid status %d", status);
        ret = "Không rõ"; // TODO: translate???
    }
    tracede;
    return ret;
}

const QList<int>& DbModel::getStatusList()
{
    traced;
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
    tracede;
    return statusList;
}

DbModelBuilder DbModel::getBuilderByModelName(const QString& modelName)
{
    traced;
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
    tracede;
    return builder;
}

const QString &DbModel::name() const
{
    return mName.isEmpty()?mNameId:mName;
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
    traced;
    // TODO: is this this function neccessary anymore?
    if (uid().isEmpty()){
        setUid(buildUid());
    }
}

QString DbModel::buildUid(const QString* seed)
{
    traced;
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
    traced;
//    buildUidIfNotSet();
    // TODO: consider again should build uid here? by default, uid will be calc later after inserting to db successfull
    return ErrNone;
}

void DbModel::markItemAsModified(const QString &itemName)
{
    traced;
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

    tracede;
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
    traced;
    ErrCode ret = ErrNone;
    QString fname = QString("%1.html").arg(uid());
    *fpath = FileCtl::getTmpDataDir(fname);
    ret = ExportFactory::exportTo(getExporter(),
                                  *fpath, type);
    // TODO: how about exportTo() method??
    return ret;
}

const QString DbModel::exportTemplatePath(FileExporter* exporter, QString* ftype) const
{
    traced;
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_HTML:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltCommonTemplateFileName);
        };
    }
    tracede;
    return QString();
}

const QStringList DbModel::getListExportKeyWord() const
{
    traced;

    return mExportCallbacks.keys();
}

ErrCode DbModel::getExportDataString(const QString &item, QString *data) const
{
    ErrCode ret = ErrNone;
    traced;
    logd("item %s", item.toStdString().c_str());
    if (mExportCallbacks.contains(item)){
        ExportCallbackFunc func = mExportCallbacks.value(item);
        if (func != nullptr) *data = func(item);
    }
    // TODO: raise exception when error occur???

    return ret;
}

ErrCode DbModel::onImportDataItem(const QString& importName, int importFileType,
                              const QString &keyword, const QString &value,
                              quint32 idx, QList<DbModel *>* outList)
{
    traced;
    ErrCode ret = ErrNone;
    logd("importFileType %d", importFileType);

    // TODO: raise exception when error occur???
    logd("keyword %s", keyword.toStdString().c_str());
    if (mImportCallbacks.contains(keyword)){
        ImportCallbackFunc func = mImportCallbacks.value(keyword);
        if (func != nullptr) ret = func(value);
    }
    tracedr(ret);
    return ret;
}

const QList<QString> &DbModel::updatedField() const
{
    return mUpdatedField;
}

void DbModel::resetAllModifiedMark()
{
    traced;
    logd("Total marked item: %d", mUpdatedField.count());
    mUpdatedField.clear();
    logd("Total marked item after clear: %d", mUpdatedField.count());
    tracede;
}

qint64 DbModel::lastUpdatedTime() const
{
    return mLastUpdatedTime;
}

void DbModel::setLastUpdatedTime(qint64 newLastUpdatedTime)
{
    mLastUpdatedTime = newLastUpdatedTime;
    markItemAsModified(KItemLastUpdateTime);
}

qint64 DbModel::createdTime() const
{
    return mCreatedTime;
}

void DbModel::setCreatedTime(qint64 newCreatedTime)
{
    mCreatedTime = newCreatedTime;
    markItemAsModified(KItemCreateTime);
}

ErrCode DbModel::save()
{
    traced;
    ErrCode ret = ErrNone;
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        ret = prepare2Save();
        if (ret == ErrNone) {
            ret = dbModelHdl->add(this);
        }
        if (ret == ErrExisted){ // alrady exist, judge as ok
            ret = ErrNone;
            logi("%s already exist", name().toStdString().c_str());
        }
    }
    else{
        ret = ErrDbNotReady;
        loge("db not ready");
    }
    tracedr(ret);
    return ret;
}

DbModel *DbModel::addUpdate(const QString &field)
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
    traced;
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
    tracedr(ret);
    return ret;
}

bool DbModel::allowRemove(QString* msg)
{
    return mDeletable;
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
    traced;
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
    tracedr(ret);
    return ret;
}

ErrCode DbModel::markRemove()
{
    traced;
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
    tracedr(ret);
    return ret;
}

ErrCode DbModel::exportTo(const QString &fpath, ExportType type)
{
    traced;
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
    traced;
    bool isValid = !nameId().isEmpty();
    logd("isValid = %d", isValid);
    tracede;
    return isValid;
}

void DbModel::dump()
{
    traced;
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE
    logd("- modelName %s", modelName().toStdString().c_str());
    logd("- DbId %lld", dbId());
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- mCreatedTime %s", Utils::timeMsToDatestring(createdTime()).toStdString().c_str());
    logd("- mLastUpdatedTime %s", Utils::timeMsToDatestring(lastUpdatedTime()).toStdString().c_str());
#endif //DEBUG_TRACE
}

QString DbModel::toString() const
{
    return QString("%1:%2:%3:%4").arg(modelName(), uid(), nameId(),name());
}

void DbModel::setNameId(const QString &newNameId)
{
    traced;
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

ErrCode DbModel::validateAllFields()
{
    traced;
    logi("Should be implemented by derived class");
    return ErrNone;
}

bool DbModel::isExist()
{
    bool isExist = false;
    traced;
    DbModelHandler* hdlr = this->getDbModelHandler();
    if (hdlr->exist(this)) {
        loge("Model already existed");
        isExist = true;
    }
    tracedr(isExist);
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
    traced;
    logd("App validate msg '%s'", newValidateMsg.toStdString().c_str());
    mValidateMsg.append(newValidateMsg + ";");
}

void DbModel::cleanValidateResult()
{
    traced;
    mValidateMsg.clear();
    if (mValidateResult){
        mValidateResult->clear();
    }
}
