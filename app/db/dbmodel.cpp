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
#include "iexporter.h"

DbModel::DbModel():
    mDbId(0),
    mDbStatus(0)
{
    traced;

}

DbModel::DbModel(const DbModel &model)
{
    traced;
    setDbId(model.dbId());
    setName(model.name());
    setUid(model.uid());
    setDbStatus(model.dbStatus());
    setHistory(model.history());
}

DbModel::DbModel(const DbModel *model):DbModel(*model)
{
    traced;
}

DbModel::~DbModel()
{
    traced;
}


QString DbModel::modelName() const
{
    return "DbModel";
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

const QString &DbModel::uid() const
{
    return mUid;
}

void DbModel::setUid(const QString &newUid)
{
    mUid= newUid;
}

void DbModel::buildUidIfNotSet()
{
    traced;
    if (uid().isEmpty()){
        setUid(Utils::UidFromName(name(), UidNameConvertType::NO_VN_MARK_UPPER));
    }
}

ErrCode DbModel::save()
{
    traced;
    ErrCode ret = ErrNone;
    DbModelHandler* dbModelHdl = getDbModelHandler();
    if (dbModelHdl != nullptr){
        buildUidIfNotSet();
        ret = dbModelHdl->add(this);
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
    traced;
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE
    logd("- DbId %d", dbId());
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
#endif //DEBUG_TRACE
}

QString DbModel::toString()
{
    return QString("%1,%2").arg(uid(), name());
}

void DbModel::setNameId(const QString &newNameId)
{
    traced;
    logd("Set name id %s", newNameId.toStdString().c_str());
//    mUid = Utils::UidFromName(newNameId);
    // TODO: should hash or use original value???
    mUid = newNameId;
}

IExporter *DbModel::getExporter()
{
    return nullptr;
}
