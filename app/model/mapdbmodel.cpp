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
#include "defs.h"

#include "dbdefs.h"

MapDbModel::MapDbModel():DbModel()
{
    tracein;
}

MapDbModel::~MapDbModel()
{
    tracein;
}

MapDbModel::MapDbModel(const MapDbModel &model):DbModel((DbModel*)&model)
{
    tracein;
    copy(model);
    traceout;
}

MapDbModel::MapDbModel(const MapDbModel *model):DbModel((DbModel*) model)
{
    tracein;
    if (model) {
        copy(*model);
    } else {
        loge("contruct failed, null pointer");
    }
    traceout;
}

void MapDbModel::clone(const DbModel *model)
{
    tracein;
    if (model) {
        DbModel::clone(model);
        copy((*(MapDbModel*)model));
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

DbModel *MapDbModel::clone() const
{
    tracein;
    DbModel::clone();
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
        model->setStatus(status);
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
    tmp += QString("%1").arg(status());
    if (seed) {
        tmp += *seed;
    }
    logd("value to cal uid %s", STR2CHA(tmp));
    QString uid = Utils::UidFromName(tmp,
                                     UidNameConvertType::HASH_NAME);
    logd("uid '%s'", uid.toStdString().c_str());
    return uid;
}

const QString &MapDbModel::uid1() const
{
    return mUid1;
}

void MapDbModel::setUid1(const QString &newUid1)
{
    mUid1 = newUid1;
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
    mUid2 = newUid2;
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
    mStartDate = newStartDate;
}


qint64 MapDbModel::endDate() const
{
    return mEndDate;
}

void MapDbModel::setEndDate(qint64 newEndDate)
{
    mEndDate = newEndDate;
}

qint32 MapDbModel::status() const
{
    return mStatus;
}

void MapDbModel::setStatus(qint32 newStatus)
{
    mStatus = newStatus;
}

const QString &MapDbModel::parentUid() const
{
    return mParentUid;
}

void MapDbModel::setParentUid(const QString &newParentUid)
{
    mParentUid = newParentUid;
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
    mChangeHistory = newChangeHistory;
}

void MapDbModel::copy(const MapDbModel &model)
{
    tracein;
    mUid1 = model.mUid1;
    mDbId1 = model.mDbId1;
    mUid2 = model.mUid2;
    mDbId2 = model.mDbId2;
    mStartDate = model.mStartDate;
    mEndDate = model.mEndDate;
    mStatus = model.mStatus;
    mParentUid = model.mParentUid;
    mChangeHistory = model.mChangeHistory;
    traceout;
}