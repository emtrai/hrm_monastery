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
 * Filename: dbsqlite.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/19/2022
 * Brief:
 */
#include "dbsqlite.h"
#include "errcode.h"
#include "logger.h"
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QThreadStorage>

#include <QSqlDatabase>
#include "table/dbsqlitepersontbl.h"
#include "table/dbsqlitecommunitytbl.h"
#include "table/dbsqlitesainttbl.h"
#include "table/dbsqliteedutbl.h"
#include "table/dbsqlitespecialisttbl.h"
#include "table/dbsqlitecountrytbl.h"
#include "table/dbsqliteethnictbl.h"
#include "table/dbsqlitemissiontbl.h"
#include "table/dbsqliteareatbl.h"
#include "table/dbsqlitedeparttbl.h"
#include "table/dbsqlitecoursetbl.h"
#include "table/dbsqliteworktbl.h"
#include "table/dbsqlitepersonstatustbl.h"
#include "table/dbsqlitepersoneventtbl.h"
#include "table/dbsqliteeventtbl.h"
#include "table/dbsqlitesaintpersonmaptbl.h"
#include "table/dbsqlitecommunitypersontbl.h"
#include "table/dbsqlitecommunitydepttbl.h"
#include "table/dbsqlitecommdeptpersontbl.h"
#include "table/dbsqliteroletbl.h"
#include "table/dbsqliteareamgrtbl.h"
#include "table/dbsqlitespecialistpersontbl.h"
#include "table/dbsqlitecommunitydepttbl.h"
#include "table/dbsqlitecommunitymanagertbl.h"
#include "table/dbmetadatatbl.h"
#include "table/dbsqlsequencetbl.h"

#include "dbsqlitedefs.h"
#include "dbsqliteedu.h"
#include "dbsqlitesaint.h"
#include "dbsqlitecommunity.h"
#include "dbsqlitespecialist.h"
#include "dbsqlitecountry.h"
#include "dbsqliteethnic.h"
#include "dbsqlitemission.h"
#include "dbsqlitearea.h"
#include "dbsqlitedept.h"
#include "dbsqlitecourse.h"
#include "dbsqlitework.h"
#include "dbsqliteperson.h"
#include "dbsqlitepersonstatus.h"
#include "dbsqlitepersonevent.h"
#include "dbsqliteevent.h"
#include "dbsqliterole.h"
#include "dbsqlitecommunitydept.h"

#include "defs.h"
#include "exception.h"

static const QString DRIVER("QSQLITE");

// TODO: enhance performance of db access via refer to https://lnj.gitlab.io/post/multithreaded-databases-with-qtsql/

DbSqlite* DbSqlite::gInstance = nullptr;
QThreadStorage<DatabaseConnection *> DbSqlite::mDatabaseConnections;

// ref: https://lnj.gitlab.io/post/multithreaded-databases-with-qtsql/
DatabaseConnection::DatabaseConnection(QString uri)
    //        : mName(QString::number(QRandomGenerator::global()->generate(), 36))
    : mName(QString::number((quintptr)QThread::currentThreadId()))
{
    tracein;
    auto database = QSqlDatabase::addDatabase(DRIVER, mName);
    logd("create db connection name=%s, uri=%s", STR2CHA(mName), STR2CHA(uri));
    database.setDatabaseName(uri);
    database.open();
    traceout;
}

DatabaseConnection::~DatabaseConnection()
{
    logd("remote db connection %s", STR2CHA(mName));
    QSqlDatabase::removeDatabase(mName);
}


QSqlDatabase DatabaseConnection::database()
{
    logd("db connection name=%s", STR2CHA(mName));
    return QSqlDatabase::database(mName);
}


FieldValue::FieldValue()
{
    dataType = TEXT;
}

FieldValue::FieldValue(const FieldValue &item)
{
    tracein;
    logd("copy constructor");
    value = item.value;
    mIntValue = item.mIntValue;
    dataType = item.dataType;
}

FieldValue::FieldValue(const QString &value, int dataType)
{
    tracein;
    logd("constructor");
    this->value = value;
    this->dataType = dataType;
    if (dataType == INT32 || dataType == INT64) {
        bool ok = false;
        qint64 longValue = 0;
        if (!value.isEmpty()) {
            longValue = value.toLong(&ok);
            if (!ok) {
                THROWEX("Failed to convert '%s' to long", STR2CHA(value));
            } else {
                logd("convert '%s' to long %lld", STR2CHA(value), longValue);
            }
        } // empty, set default to zero
        this->mIntValue = longValue;
    }
    traceout;
}

FieldValue::FieldValue(qint64 value)
{
    this->mIntValue = value;
    this->dataType = INT64;
}

qint64 FieldValue::valueInt()
{
    return this->mIntValue;
}

const QString &FieldValue::valueString()
{
    return this->value;
}

DbSqlite::DbSqlite()
{
    tracein;
    setupTables();
    setupModelHandler();
    traceout;
}

void DbSqlite::setupTables()
{
    tracein;
    dbgd("add tables");
    // Append all tables need for db
    // TODO: append more table
    appendTable(new DbMetadataTbl(this));
    appendTable(new DbSqlSequenceTbl(this));
    appendTable(new DbSqlitePersonTbl(this));
    appendTable(new DbSqliteSaintTbl(this));
    appendTable(new DbSqliteEduTbl(this));
    appendTable(new DbSqliteSpecialistTbl(this));
    appendTable(new DbSqliteCountryTbl(this));
#ifndef SKIP_PERSON_PROVINE
    appendTable(new DbSqliteProvinceTbl(this));
#endif
    appendTable(new DbSqliteEthnicTbl(this));
    appendTable(new DbSqliteMissionTbl(this));
    appendTable(new DbSqliteCommunityTbl(this));
    appendTable(new DbSqliteAreaTbl(this));
    appendTable(new DbSqliteDepartTbl(this));
    appendTable(new DbSqliteCourseTbl(this));
    appendTable(new DbSqliteWorkTbl(this));
    appendTable(new DbSqliteRoleTbl(this));
    appendTable(new DbSqlitePersonStatusTbl(this));
    appendTable(new DbSqlitePersonEventTbl(this));
    appendTable(new DbSqliteEventTbl(this));
    appendTable(new DbSqliteSaintPersonMapTbl(this));
    appendTable(new DbSqliteCommunityPersonTbl(this));
    appendTable(new DbSqliteCommunityDeptTbl(this));
    appendTable(new DbSqliteCommDeptPersonTbl(this));
    appendTable(new DbSqliteAreaMgrTbl(this));
    appendTable(new DbSqliteSpecialistPersonTbl(this));
    appendTable(new DbSqliteCommunityManagerTbl(this));
    traceout;
}

void DbSqlite::setupModelHandler()
{
    tracein;
    dbgd("add model handler");
    appendModelHandler(new DbSqliteSpecialist());
    appendModelHandler(new DbSqliteEdu());
    appendModelHandler(new DbSqliteSaint());
    appendModelHandler(new DbSqliteSpecialist());
    appendModelHandler(new DbSqliteCountry());
    appendModelHandler(new DbSqliteEthnic());
#ifndef SKIP_PERSON_PROVINE
    appendModelHandler(new DbSqliteProvince());
#endif
    appendModelHandler(new DbSqliteMission());
    appendModelHandler(new DbSqliteDept());
    appendModelHandler(new DbSqliteCommunity());
    appendModelHandler(new DbSqliteArea());
    appendModelHandler(new DbSqliteCourse());
    appendModelHandler(new DbSqliteWork());
    appendModelHandler(new DbSqlitePersonStatus());
    appendModelHandler(new DbSqliteRole());
    appendModelHandler(new DbSqlitePerson());
    appendModelHandler(new DbSqliteEvent());
    appendModelHandler(new DbSqlitePersonEvent());
    appendModelHandler(new DbSqliteCommunityDept());
    traceout;
}

DbMetadataTbl *DbSqlite::tableMetadata()
{
    return (DbMetadataTbl*)getTable(KTableMetadata);
}

DbSqlSequenceTbl *DbSqlite::tblSqlSequence()
{
    return (DbSqlSequenceTbl*)getTable(KTableSqliteSequence);
}

const QSqlDatabase &DbSqlite::db() const
{
    return mDb;
}

void DbSqlite::appendTable(DbSqliteTbl *tbl)
{
    tracein;
    if (nullptr != tbl) {
        logd("Add table '%s'", tbl->name().toStdString().c_str());
        mListTbl[tbl->name()] = tbl;
    } else
    {
        // TODO: throw exception?????
    }
    traceout;
}


ErrCode_t DbSqlite::checkOrCreateAllTables()
{
    tracein;
    ErrCode_t err = ErrCode_t::ErrNone;
    QStringList tables = currentDb().tables();
    logd("List of table: \n %s", STR2CHA(tables.join("\n")));
    bool ok = false;
    foreach (QString key, mListTbl.keys())
    {
        DbSqliteTbl* tbl = nullptr;
        logd("check & create table '%s'", STR2CHA(key));

        if (tables.contains(key)) {
            logd("table '%s' existed", STR2CHA(key));
        }
        tbl = mListTbl[key];
        if (!tbl) {
            logw("Table is not exist");
            err = ErrNotExist;
            break;
        }
        // we still need to call this function so that table can build up table fields map to get proper processing
        logd("Initialize table '%s'", STR2CHA(tbl->name()));
        err = tbl->checkOrCreateTable();
        if (err != ErrNone) // TODO: should break? return critical error????
            break;
#ifdef DEBUG_TRACE
        qint64 seq = 0;
        seq = getDbSeqNumber(key, &ok);
        logd("Get seq number of tbl '%s', seq = %d, ok = %d", STR2CHA(key), seq, ok);
#endif
        qint64 tblVer = mMetaDbInfo.tableVersion(key, &ok);
        if (ok) {
            if (tblVer > 0 && (tblVer != tbl->versionCode())) {
                logi("Different version from db file and current one: 0x%llx --> 0x%x",
                     tblVer, tbl->versionCode());
                err = tbl->onTblMigration(tblVer);
            }
        }
        if (err == ErrNone) {
            mMetaDbInfo.addTableVersion(key, tbl->versionCode());
        } else {
            loge("Something wrong when creating table, err = %d", err);
        }
        // TODO: validate version code;
    }
    traceret(err);
    return err;
}

void DbSqlite::appendModelHandler(DbModelHandler *hdl)
{
    tracein;
    if (nullptr != hdl) {
        logd("Append handler '%s'", STR2CHA(hdl->getName()));
        mModelHdlList[hdl->getName()] = hdl;
    } else {
        // TODO: throw exception?????
    }
}



DbSqliteTbl *DbSqlite::getTable(const QString &tblName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;

    if (mListTbl.contains(tblName)){
        tbl = mListTbl[tblName];
    }
    else{
        tbl = nullptr;
        loge("Not found table %s", tblName.toStdString().c_str());
        // TODO: reload system? or what should we do?
    }
    traceout;
    return tbl;
}

DbSqliteTbl *DbSqlite::getTableFromModelName(const QString &name)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("get table for model name '%s'", STR2CHA(name));
    if (!name.isEmpty()) {
        foreach (DbSqliteTbl* item, mListTbl) {
            if (item && item->getHandleModelName() == name) {
                tbl = item;
            }
        }
    } else {
        loge("invalid model name to search table");
    }
    if (!tbl){
        loge("Not found table for model '%s'", STR2CHA(name));
    }
    traceout;
    return tbl;
}

DbModelHandler *DbSqlite::getEduModelHandler()
{
    return getModelHandler(KModelHdlEdu);
}

DbModelHandler *DbSqlite::getSaintModelHandler()
{
    return getModelHandler(KModelHdlSaint);
}

DbModelHandler *DbSqlite::getSpecialistModelHandler()
{
    return getModelHandler(KModelHdlSpecialist);
}

DbModelHandler *DbSqlite::getCommunityModelHandler()
{
    return getModelHandler(KModelHdlCommunity);
}

DbModelHandler *DbSqlite::getPersonModelHandler()
{
    return getModelHandler(KModelHdlPerson);

}

DbModelHandler *DbSqlite::getMissionModelHandler()
{
    return getModelHandler(KModelHdlMission);
}

DbModelHandler *DbSqlite::getAreaModelHandler()
{
    return getModelHandler(KModelHdlArea);

}

DbModelHandler *DbSqlite::getCountryModelHandler()
{
    return getModelHandler(KModelHdlCountry);

}

DbModelHandler *DbSqlite::getModelHandler(const QString &name)
{
    DbModelHandler* hdl = nullptr;
    tracein;
    if (mModelHdlList.contains(name)){
        hdl = mModelHdlList[name];
    } else{
        hdl = nullptr;
        loge("Not found model %s", STR2CHA(name));
        // TODO: reload system? or what should we do?
    }
    traceout;
    return hdl;
}

DbModelHandler *DbSqlite::getDepartmentModelHandler()
{
    return getModelHandler(KModelHdlDept);

}

DbModelHandler *DbSqlite::getWorkModelHandler()
{
    return getModelHandler(KModelHdlWork);

}

DbModelHandler *DbSqlite::getPersonStatusModelHandler()
{
    return getModelHandler(KModelHdlPersonStatus);

}

DbModelHandler *DbSqlite::getCourseModelHandler()
{
    return getModelHandler(KModelHdlCourse);

}

DbModelHandler *DbSqlite::getEthnicModelHandler()
{
    return getModelHandler(KModelHdlEthnic);

}

DbModelHandler *DbSqlite::getRoleModelHandler()
{
    return getModelHandler(KModelHdlRole);

}

DbSqliteTbl *DbSqlite::table(const QString &tblName)
{
    return DbSqlite::getInstance()->getTable(tblName);
}

DbModelHandler *DbSqlite::handler(const QString &name)
{
    return DbSqlite::getInstance()->getModelHandler(name);
}

QSqlDatabase DbSqlite::currentDb()
{
    tracein;
    // TODO: handle race condition issue when access db? should use mutex???
    if (!mDatabaseConnections.hasLocalData()) {
        logd("db connection not exist, create new one");
        mDatabaseConnections.setLocalData(new DatabaseConnection(mDbUri));
    } else {
        logd("db connection already exist, use existing one");
    }
    if (mDatabaseConnections.localData()) {
        return mDatabaseConnections.localData()->database();
    } else {
        THROWEX("NULL local data");
    }
    traceout;
}

QSqlQuery DbSqlite::createQuery()
{
    return QSqlQuery(currentDb());
}

quint64 DbSqlite::getDbVersion()
{
    return DB_VERSION;
}

quint64 DbSqlite::getDbVersionInMetadata(bool* isOk)
{
    tracein;
    qint64 ver = 0;
    ErrCode err = getMetadataValue(KMetadataKeyVersion, ver);
    if (err == ErrNone) {
        logd("Version = 0x%llx", ver);
    } else {
        loge("value for key '%s' failed, err=%d", KMetadataKeyVersion, err);
    }
    if (isOk) *isOk = (err == ErrNone);
    traceout;
    return ver;
}

ErrCode DbSqlite::updateDbVersionInMetadata(qint64 value)
{
    tracein;
    ErrCode err = updateMetadataValue(KMetadataKeyVersion, value);
    traceout;
    return err;
}

ErrCode DbSqlite::getMetadataValue(const QString& key, QString& value)
{
    tracein;
    DbMetadataTbl *tbl = (DbMetadataTbl*)tableMetadata();
    ErrCode err = ErrNone;
    if (key.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone) {
        logd("Get value for key '%s'", STR2CHA(key));
        if (tbl) {
            err = tbl->getMetadataValue(key, value);
        } else {
            err = ErrNoTable;
            loge("not found table of Metadata");
        }
    }
    traceret(err);
    return err;
}

ErrCode DbSqlite::getMetadataValue(const QString &key, qint64 &outvalue)
{
    tracein;
    ErrCode err = ErrNone;
    QString valueStr;
    qint64 value = 0;
    bool ok = false;
    err = getMetadataValue(key, valueStr);
    if (err == ErrNone) {
        if (!valueStr.isEmpty()) {
            value = valueStr.toInt(&ok);
            if (ok) {
                logd("value = 0x%llx", value);
                outvalue = value;
            } else {
                loge("convert '%s' to in failed", STR2CHA(valueStr));
                value = 0;
                err = ErrFailedConvert;
            }
        } else {
            outvalue = 0;
            logd("No data for key '%s'", STR2CHA(key));
        }
    } else {
        loge("value for key '%s' failed, err=%d", STR2CHA(key), err);
    }
    traceret(err);
    return err;
}

ErrCode DbSqlite::updateMetadataValue(const QString &key, qint64 value)
{
    tracein;
    ErrCode err = updateMetadataValue(key, QString("%1").arg(value));
    traceout;
    return err;
}

ErrCode DbSqlite::updateMetadataValue(const QString &key, const QString &value)
{
    tracein;
    DbMetadataTbl *tbl = (DbMetadataTbl*)tableMetadata();
    ErrCode err = ErrNone;
    QString currValue;
    if (key.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone) {
        logd("Get value for key '%s'", STR2CHA(key));
        if (tbl) {
            err = tbl->getMetadataValue(key, currValue);
        } else {
            err = ErrNoTable;
            loge("not found table of Metadata");
        }
    }
    if (err == ErrNone) {
        err = tbl->updateMetadataValue(key, value);
    } else if (err == ErrNotExist) {
        err = tbl->addMetadataValue(key, value);
    } else {
        loge("Get valued failed, err=%d", err);
    }
    if (err != ErrNone) {
        loge("Updated value for key '%s' failed, err = %d", STR2CHA(key), err);
    }
    traceret(err);
    return err;
}

quint64 DbSqlite::getCurrentPersonCodeNumber(bool* isOk)
{
    tracein;
    qint64 seq = 0;
    seq = getDbSeqNumber(KTablePerson, isOk);
    traceout;
    return seq;
}

quint64 DbSqlite::getDbSeqNumber(const QString &tblName, bool *ok)
{
    tracein;
    DbSqlSequenceTbl *tbl = (DbSqlSequenceTbl*)tblSqlSequence();
    ErrCode err = ErrNone;
    qint64 value = 0;
    if (tblName.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone) {
        logd("Get value for tblName '%s'", STR2CHA(tblName));
        if (tbl) {
            err = tbl->getValue(tblName, value);
        } else {
            err = ErrNoTable;
            loge("not found table of seq number");
        }
    }
    if (ok) *ok = (err == ErrNone);
    traceret(value);
    return value;
}

ErrCode DbSqlite::getAvailableNameId(const QString &modelName,
                                     const QString &initNameId,
                                     QString &availableNameId)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl* tbl = getTableFromModelName(modelName);
    QString nameId = initNameId;
    int i = 1;
    if (initNameId.isEmpty() || modelName.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument, init nameid '%s' or model name '%s' is empty",
             STR2CHA(initNameId),STR2CHA(modelName));
    }
    if (err == ErrNone && !tbl) {
        err = ErrNoHandler;
        loge("not found valid tbl");
    }
    if (err == ErrNone) {
        logd("initNameId to search '%s'", STR2CHA(initNameId));
        for (; i < MAX_CHECK_DUP_TIME; i++) {
            if (!tbl->isNameidExist(nameId)) {
                dbg(LOG_DEBUG, "name id '%s' not exist", STR2CHA(nameId));
                availableNameId = nameId;
                break;
            } else {
                dbg(LOG_DEBUG, "Name id '%s' existed", STR2CHA(nameId));
                nameId = QString("%1_%2").arg(initNameId).arg(i);
                logd("Try next nameid '%s'", STR2CHA(nameId));
            }
        }
    }
    if (i >= MAX_CHECK_DUP_TIME || nameId.isEmpty()) {
        err = ErrNotFound;
        loge("not found any suitable nameid for '%s', tried %d time",
             STR2CHA(initNameId), i);
    }

    traceret(err);
    return err;

}


DbSqlite* DbSqlite::getInstance(){
    if (gInstance == nullptr){
        gInstance = new DbSqlite();
    }
    return gInstance;
}


ErrCode_t DbSqlite::loadDb(const DbInfo* dbInfo){
    tracein;
    ErrCode_t err = ErrNone;
    if (dbInfo){
        if(QSqlDatabase::isDriverAvailable(DRIVER)) {
            if (dbInfo->uri().isNull())
                mDbUri = ":memory:";
            else
                mDbUri = dbInfo->uri();

            logd("Db connect to uri %s", mDbUri.toStdString().c_str());

            mDb = currentDb();
            logd("Feature LastInsertId %d", mDb.driver()->hasFeature(QSqlDriver::LastInsertId));
            logd("Feature Transactions %d", mDb.driver()->hasFeature(QSqlDriver::Transactions));
            logd("Feature PreparedQueries %d", mDb.driver()->hasFeature(QSqlDriver::PreparedQueries));

            if (ErrNone == err){
                err = checkOrCreateAllTables();
            }

        }
        else{
            err = ErrNotAvail;
            loge("SQl driver %s not available", DRIVER.toStdString().c_str());
        }
    }
    else{
        err = ErrInvalid;
        loge("dbinfo is NULL");
    }

    if (err == ErrNone) {
        bool ok = false;
        quint64 dbVersion = getDbVersion();
        quint64 versionInDb = getDbVersionInMetadata(&ok);
        logi("dbversion = 0x%08llx, versionInDb = 0x%08llx", dbVersion, versionInDb);
        bool updateversionindb = false;
        if (ok) {
            if (dbVersion == versionInDb) {
                logi("Version is identical = 0x%08llx", dbVersion);
            } else if (versionInDb == 0){
                updateversionindb = true;
            } else {
                logw("VERSION ARE DIFFERENT!! version = 0x%08llx, versionInDb = 0x%08llx",
                        dbVersion, versionInDb);
                // TODO: NEED TO DO ANY MIGRATION????
            }
        } else {
            updateversionindb = true;
        }
        if (updateversionindb) {
            updateDbVersionInMetadata(dbVersion);
        }
    }

    if (err == ErrNone) {
        mMetaDbInfo.saveJson(dbInfo->metaUri());
    }

    traceret(err);
    return err;
}

ErrCode DbSqlite::validateDbInfo(const DbInfo *dbInfo)
{
    tracein;
    ErrCode err = ErrNone;
    if (!dbInfo) {
        err = ErrInvalidArg;
        loge("Invalid agument");
    }
    if (err == ErrNone) {
        QString metaUri = dbInfo->metaUri();
        logi("meta uri = '%s'", STR2CHA(metaUri));
        if (!metaUri.isEmpty()) {
            if (!QFile::exists(metaUri)) {
                logi("Meta db '%s' not exist, generate new one", STR2CHA(metaUri));
                mMetaDbInfo.setAppVersion(APP_VERSION_CODE);
                mMetaDbInfo.setAppVersionString(APP_VERSION);
                mMetaDbInfo.setDbVersion(DB_VERSION);
            } else {
                logi("meta db exist, load it");
                err = mMetaDbInfo.fromJsonFile(metaUri);
            }
        } else {
            loge("invalid meta uri");
            err = ErrInvalidData;
        }
    }

    if (err == ErrNone) {
        if (mMetaDbInfo.dbVersion() != DB_VERSION) {
            logw("DB version change, from %lld to %d", mMetaDbInfo.dbVersion(), DB_VERSION);
            // TODO: handle Db change version here
        }
    }
    traceret(err);
    return err;
}

bool DbSqlite::isDbExist()
{
    bool exist = false;
    tracein;
    logd("mDbUri '%s'", STR2CHA(mDbUri));
    if (!mDbUri.isEmpty() && QFile::exists(mDbUri)) {
        exist = true;
    } else {
        dbgd("db '%s' not exist", STR2CHA(mDbUri));
        exist = false;
    }
    traceret(exist);
    return exist;
}

ErrCode_t DbSqlite::execQuery(const QString &sql)
{
    ErrCode_t err = ErrNone;
    tracein;
    logd("Execute query %s", sql.toStdString().c_str());

    // TODO: re-implement this, race condition? resource leakage???
    QSqlQuery qry(currentDb());
    if (err == ErrNone) {
        currentDb().transaction();

        qry.prepare(sql);
        if( !qry.exec() ) {

            loge( "Failed to execute %s", STR2CHA(qry.executedQuery()) );
            loge( "Last error %s", STR2CHA(qry.lastError().text()) );
            err = ErrFailSqlQuery;
        }

        if (!currentDb().commit()){
            currentDb().rollback();
        }
        currentDb().close();
    } else {
        loge("exe query failed, open db failed %d", err);
    }
    traceret(err);
    return err;
}

ErrCode_t DbSqlite::execQuery(QSqlQuery *qry)
{

    ErrCode_t err = ErrNone;
    tracein;

    startTransaction();
    err = execQueryNoTrans(qry);
    endTransaction();
    traceret(err);
    return err;
}

ErrCode_t DbSqlite::execQueryNoTrans(QSqlQuery *qry)
{

    ErrCode_t err = ErrNone;
    tracein;

    if( !qry->exec() ) {
        loge( "Failed to execQuery %s", qry->executedQuery().toStdString().c_str());
        loge( "Last error %s", qry->lastError().text().toStdString().c_str() );
        err = ErrFailSqlQuery;
    } else {
        logd( "Executed execQuery %s", qry->executedQuery().toStdString().c_str());
    }
    traceret(err);
    return err;
}

ErrCode_t DbSqlite::startTransaction()
{
    tracein;
    ErrCode err = ErrNone;
    if (err == ErrNone)
        currentDb().transaction();
    else
        loge("Start transaction failed, open db failed %d", err);
    // TODO: re-implement this, race condition? resource leakage???
    traceout;
    return ErrNone;
}

ErrCode_t DbSqlite::endTransaction()
{
    tracein;
    ErrCode err = ErrNone;
//    err = openDb();
    if (!currentDb().commit()){
        logd("rollback");
        currentDb().rollback();
    }
//    closeDb();
    // TODO: re-implement this, race condition? resource leakage???
    traceout;
    return err;
}

QSqlDatabase DbSqlite::getDbConnection()
{
    tracein;
    // Starting with Qt 5.11, sharing the same connection between threads is not allowed.
    // Use a dedicated connection for each thread requiring access to the database,
    // using the thread address as connection name.

    const QString connName = QString::number((quintptr)QThread::currentThreadId());
    logd("conntion name %s", STR2CHA(connName));
    QSqlDatabase db = QSqlDatabase::database(connName);
    if (!db.isOpen() || !db.isValid()) {
        db = QSqlDatabase::cloneDatabase(mDb, connName);
        if (!db.open()) {
            qCritical() << "Failed to open db connection" + connName;
        }
    }
    traceout;
    return db;

}

