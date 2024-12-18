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
 * Filename: mapdbmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "mapdbmodel.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "datetimeutils.h"
#include "defs.h"
#include "dbmodel.h"
#include "modeldefs.h"
#include "importtype.h"

MapDbModel::MapDbModel():DbModel(),
    mDbId1(0),
    mDbId2(0),
    mStartDate(0),
    mEndDate(0),
    mModelStatus(0)
{
    traced;
}

MapDbModel::~MapDbModel()
{
    traced;
}

MapDbModel::MapDbModel(const MapDbModel &model):MapDbModel(&model)
{}

MapDbModel::MapDbModel(const MapDbModel *model):DbModel((DbModel*) model)
{
    tracein;
    if (model) {
        // because docopy is not virtual, we need to call it here to make sure
        // it's called correctly
        docopy(*model);
    } else {
        loge("contruct failed, null pointer");
    }
    traceout;
}

void MapDbModel::dump() const
{
    tracein;
    DbModel::dump();
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE
    logd("- mUid1 %s", STR2CHA(mUid1));
    logd("- mUid2 %s", STR2CHA(mUid2));
    logd("- mParentUid %s", STR2CHA(mParentUid));
    logd("- mModelStatus %d", mModelStatus);
    logd("- mStartDate %lld - %s", mStartDate, STR2CHA(DatetimeUtils::date2String(mStartDate)));
    logd("- mEndDate %lld - %s", mEndDate, STR2CHA(DatetimeUtils::date2String(mEndDate)));
#endif //DEBUG_TRACE
    traceout;
}

DbModel *MapDbModel::buildMapModel(DbModelBuilder builder,
                                   const DbModel *item1, const DbModel *item2,
                                   int status, qint64 startdate, qint64 enddate,
                                   const QString &remark)
{
    tracein;
    MapDbModel* model = nullptr;
    ErrCode err = ErrNone;
    if (!builder || !item1 || !item2) {
        loge("Invalid argument, seem one of param is null");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        model = (MapDbModel*)builder();
        if (!model) {
            loge("No memory");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone && model->modelType() != MODEL_MAP) {
        loge("Model is not model map");
        err = ErrInvalidData;
    }
    if (err == ErrNone) {
        model->setDbId1(item1->dbId());
        model->setUid1(item1->uid());
        model->setDbId2(item2->dbId());
        model->setUid2(item2->uid());
        model->setModelStatus(status);
        model->setStartDate(startdate);
        model->setEndDate(enddate);
        if (!remark.isEmpty())
            model->setRemark(remark);
    } else {
        loge("Error, release resource err = %d", err);
        if (model) delete model;
        model = nullptr;
    }
    traceout;
    return model;
}

QString MapDbModel::buildUid(const QString *seed)
{
    tracein;
    QString tmp = uid1() + uid2();
    tmp += QString("%1").arg(startDate());
    tmp += QString("%1").arg(endDate());
    tmp += QString("%1").arg(modelStatus());
    if (seed) {
        tmp += *seed;
    }
    logd("value to cal uid %s", STR2CHA(tmp));
    QString uid = Utils::UidFromName(tmp,
                                     UidNameConvertType::HASH_NAME);
    logd("uid '%s'", uid.toStdString().c_str());
    return uid;
}

void MapDbModel::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemStartDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((MapDbModel*)model)->startDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemEndDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((MapDbModel*)model)->endDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
        return ((MapDbModel*)model)->modelStatusName();
    });
    mExportCallbacks.insert(KItemStatusId, [](const DbModel* model, const QString& item){
        return QString("%1").arg(((MapDbModel*)model)->modelStatus());
    });
    mExportCallbacks.insert(KItemChangeHistory, [](const DbModel* model, const QString& item){
        return ((MapDbModel*)model)->changeHistory();
    });
    traceout;

}

void MapDbModel::initImportFields()
{
    tracein;
    DbModel::initImportFields();
    mImportCallbacks.insert(KItemStartDate, [this](const QString& value){
        this->setStartDate(DatetimeUtils::dateFromString(value));
        return ErrNone;
    });
    mImportCallbacks.insert(KItemEndDate, [this](const QString& value){
        this->setEndDate(DatetimeUtils::dateFromString(value));
        return ErrNone;
    });
    mImportCallbacks.insert(KItemStatus, [this](const QString& value){
        if (this->modelStatusName().isEmpty()) {
            this->setModelStatusName(value);
        }
        return ErrNone;
    });
    mImportCallbacks.insert(KItemStatusId, [this](const QString& value){
        bool ok = false;
        ErrCode ret = ErrNone;
        quint64 status = value.toLong(&ok);
        logd("status id %s", STR2CHA(value));
        if (ok) {
            QString name = DbModel::modelStatus2Name((DbModelStatus)status);
            if (!name.isEmpty()) {
                this->setModelStatus(status);
                this->setModelStatusName(name);
            } else {
                loge("invalid status id %s", STR2CHA(value));
                ret = ErrInvalidData;
            }
        } else {
            loge("invalid status id %s", STR2CHA(value));
            ret = ErrInvalidData;
        }
        return ret;
    });
    mImportCallbacks.insert(KItemChangeHistory, [this](const QString& value){
        this->setChangeHistory(value);
        return ErrNone;
    });
    mImportItemsType.insert(KItemChangeHistory,
                            (IMPORT_ITEM_TYPE_UPDATE_DB));
    mImportItemsType.insert(KItemStatusId,
                            (IMPORT_ITEM_TYPE_MATCH_DB));
    mImportItemsType.insert(KItemStartDate,
                            (IMPORT_ITEM_TYPE_UPDATE_DB));
    mImportItemsType.insert(KItemEndDate,
                            (IMPORT_ITEM_TYPE_UPDATE_DB));
    traceout;
}

const QString &MapDbModel::uid1() const
{
    return mUid1;
}

void MapDbModel::setUid1(const QString &newUid1)
{
    CHECK_MODIFIED_THEN_SET(mUid1, newUid1, KItemUid1);
}

qint64 MapDbModel::dbId1() const
{
    return mDbId1;
}

void MapDbModel::setDbId1(qint64 newDbId1)
{
    mDbId1 = newDbId1;
}

const QString &MapDbModel::uid2() const
{
    return mUid2;
}

void MapDbModel::setUid2(const QString &newUid2)
{
    CHECK_MODIFIED_THEN_SET(mUid2, newUid2, KItemUid2);
}

qint64 MapDbModel::dbId2() const
{
    return mDbId2;
}

void MapDbModel::setDbId2(qint64 newDbId2)
{
    mDbId2 = newDbId2;
}


qint64 MapDbModel::startDate() const
{
    return mStartDate;
}

void MapDbModel::setStartDate(qint64 newStartDate)
{
    if (mStartDate != newStartDate) {
        mStartDate = newStartDate;
        markItemAsModified(KItemStartDate);
    }
}


qint64 MapDbModel::endDate() const
{
    return mEndDate;
}

void MapDbModel::setEndDate(qint64 newEndDate)
{
    if (mEndDate != newEndDate) {
        mEndDate = newEndDate;
        markItemAsModified(KItemEndDate);
    }
}

ErrCode MapDbModel::setEndDate(const QString &newEndDate)
{
    tracein;
    bool isOk = false;
    ErrCode err = ErrNone;
    logd("end date string '%s'", STR2CHA(newEndDate));
    qint64 dateval = DatetimeUtils::dateFromString(newEndDate, DEFAULT_FORMAT_YMD, &isOk);
    logd("mClosedDate 0x%llx, isOk %d", dateval, isOk);
    if (isOk) {
        setEndDate(dateval);
    } else {
        err = ErrInvalidArg;
        loge("invalid close date time '%s'", STR2CHA(newEndDate));
    }

    traceret(err);
    return err;
}

qint32 MapDbModel::modelStatus() const
{
    return mModelStatus;
}

void MapDbModel::setModelStatus(qint32 newStatus)
{
    if (mModelStatus != newStatus) {
        mModelStatus = newStatus;
        markItemAsModified(KItemStatus);
        setModelStatusName(DbModel::modelStatus2Name((DbModelStatus)newStatus));
    }
}

const QString &MapDbModel::parentUid() const
{
    return mParentUid;
}

void MapDbModel::setParentUid(const QString &newParentUid)
{
    if (mParentUid != newParentUid) {
        mParentUid = newParentUid;
        markItemAsModified(KItemParentModel);
    }
}

QString MapDbModel::modelName() const
{
    return KModelNameMap;
}

int MapDbModel::modelType() const
{
    return MODEL_MAP;
}

const QString &MapDbModel::changeHistory() const
{
    return mChangeHistory;
}

void MapDbModel::setChangeHistory(const QString &newChangeHistory)
{
    if (mChangeHistory != newChangeHistory) {
        mChangeHistory = newChangeHistory;
        markItemAsModified(KItemChangeHistory);
    }
}

void MapDbModel::docopy(const DbModel &model)
{
    tracein;
    const MapDbModel* mapModel = static_cast<const MapDbModel*>(&model);
    setUid1(mapModel->mUid1);
    setDbId1(mapModel->mDbId1);
    setUid2(mapModel->mUid2);
    setDbId2(mapModel->mDbId2);
    setStartDate(mapModel->mStartDate);
    setEndDate(mapModel->mEndDate);
    setModelStatus(mapModel->mModelStatus);
    setModelStatusName(mapModel->mModelStatusName);
    setParentUid(mapModel->mParentUid);
    setChangeHistory(mapModel->mChangeHistory);
    traceout;
}

void MapDbModel::copyData(const DbModel *model)
{
    tracein;
    if (model && model->modelType() == MODEL_MAP) {
        docopy(*model);
    }
    DbModel::copyData(model);
    traceout;
}

QString MapDbModel::modelStatusName()
{
    QString statusName = mModelStatusName;
    if (mModelStatusName.isEmpty() && mModelStatus != 0) {
        statusName = DbModel::modelStatus2Name((DbModelStatus)mModelStatus);
    }
    return statusName;
}

void MapDbModel::setModelStatusName(const QString &newModelStatusName)
{
    mModelStatusName = newModelStatusName;
}

QString MapDbModel::toString() const
{
    QString str = DbModel::toString();
    str += QString(":modeltype('%1')").arg(modelType());
    str += QString(":status('%1'-%2)")
               .arg(DbModel::modelStatus2Name((DbModelStatus)modelStatus()))
               .arg(modelStatus());
    str += QString(":uid1('%1'):uid2('%2')").arg(uid1(), uid2());
    str += QString(":start('%1')").arg(DatetimeUtils::date2String(startDate()));
    str += QString(":end('%2')").arg(DatetimeUtils::date2String(endDate()));
    return str;
}
