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
#include "dbdefs.h"

DbModel::DbModel():
    mDbId(0),
    mDbStatus(0),
    mValidateResult(nullptr),
    mMarkModified(false)
{
    traced;

}

DbModel::DbModel(const DbModel &model)
{
    traced;
    mDbId = model.dbId();
    mName = model.name();
    mUid = model.uid();
    mDbStatus = model.dbStatus();
    mHistory = model.history();
    mMarkModified = model.markModified();
    // TODO: mValidateResult
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

int DbModel::modelType() const
{
    return MODEL_UNKNOWN;
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
    traced;
    if (uid().isEmpty()){
        setUid(buildUid());
    }
}

QString DbModel::buildUid(const QString* seed)
{
    traced;
    QString uid = Utils::UidFromName(name(), UidNameConvertType::NO_VN_MARK_UPPER);
    if (seed != nullptr) {
        uid += "_" + *seed;
    }
    logd("uid: %s", uid.toStdString().c_str());
    return uid;
}

ErrCode DbModel::prepare2Save()
{
    traced;
    buildUidIfNotSet();
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

bool DbModel::markModified() const
{
    return mMarkModified;
}

void DbModel::setMarkModified(bool newMarkModified)
{
    logd("set mark modify %d", newMarkModified);
    mMarkModified = newMarkModified;
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
}

qint64 DbModel::createdTime() const
{
    return mCreatedTime;
}

void DbModel::setCreatedTime(qint64 newCreatedTime)
{
    mCreatedTime = newCreatedTime;
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

ErrCode DbModel::update()
{
    traced;
    ErrCode ret = ErrNone;
    // TODO
    tracedr(ret);
    return ret;
}

ErrCode DbModel::remove()
{
    traced;
    ErrCode ret = ErrNone;
    // TODO
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

const QString &DbModel::history() const
{
    return mHistory;
}

void DbModel::setHistory(const QString &newHistory)
{
    mHistory = newHistory;

    markItemAsModified(KItemDbHistory);
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
    logd("- modelName %s", modelName().toStdString().c_str());
    logd("- DbId %lld", dbId());
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- mCreatedTime %s", Utils::timeMsToDatestring(createdTime()).toStdString().c_str());
    logd("- mLastUpdatedTime %s", Utils::timeMsToDatestring(lastUpdatedTime()).toStdString().c_str());
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

ErrCode DbModel::validate()
{
    traced;
    logi("Should be implemented by derived class");
    return ErrNone;
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
