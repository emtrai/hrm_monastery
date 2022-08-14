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

DbModel::DbModel():
    mDbId(0),
    mDbStatus(0)
{
    traced;

}

DbModel::DbModel(const DbModel &model)
{
    setDbId(model.dbId());
    setName(model.name());
    setUuid(model.uuid());
}

DbModel::~DbModel()
{
    traced;
}

QString DbModel::nameid() const
{
    QString hash = Crypto::hashString(name().simplified().toLower());
    return hash;
}

const QString &DbModel::name() const
{
    return mName;
}

void DbModel::setName(const QString &newName)
{
    mName = newName;
}

qint64 DbModel::dbId() const
{
    return mDbId;
}

void DbModel::setDbId(qint64 newDbId)
{
    mDbId = newDbId;
}

const QString &DbModel::uuid() const
{
    return mUuid;
}

void DbModel::setUuid(const QString &newUuid)
{
    mUuid = newUuid;
}

ErrCode DbModel::save()
{
    traced;
    ErrCode ret = ErrNone;
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        ret = dbModelHdl->add(this);
        if (ret == ErrExisted){ // alrady exist, judge as ok
            ret = ErrNone;
            logi("%s already exist", name().toStdString().c_str());
        }
    }
    else{
        ret = ErrDbNotReady;
        loge("DbSaint not ready");
    }
    tracedr(ret);
    return ret;
}

qint32 DbModel::dbStatus() const
{
    return mDbStatus;
}

void DbModel::setDbStatus(qint32 newDbStatus)
{
    mDbStatus = newDbStatus;
}

const QString &DbModel::history() const
{
    return mHistory;
}

void DbModel::setHistory(const QString &newHistory)
{
    mHistory = newHistory;
}

bool DbModel::isValid()
{
    // TODO: add more checking for valid info
    return !name().isEmpty();
}

void DbModel::dump()
{
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE

    logd("DUMP:");
    logd("- Uuid %s", nameid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
#endif //DEBUG_TRACE
}

QString DbModel::toString()
{
    return QString("%1,%2").arg(uuid(), name());
}
