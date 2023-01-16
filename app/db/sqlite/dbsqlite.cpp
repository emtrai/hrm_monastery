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
#include "dbsqlitetablebuilder.h"
#include "table/dbsqlitepersontbl.h"
#include "table/dbsqlitecommunitytbl.h"
#include "table/dbsqlitesainttbl.h"
#include "table/dbsqliteedutbl.h"
#include "table/dbsqlitespecialisttbl.h"
#include "table/dbsqlitecountrytbl.h"
#include "table/dbsqliteprovincetbl.h"
#include "table/dbsqliteethnictbl.h"
#include "table/dbsqlitemissiontbl.h"
#include "table/dbsqliteareatbl.h"
#include "table/dbsqlitedeparttbl.h"
#include "table/dbsqlitecoursetbl.h"
#include "table/dbsqliteworktbl.h"
#include "table/dbsqlitestatustbl.h"
#include "table/dbsqlitepersoneventtbl.h"
#include "table/dbsqliteeventtbl.h"
#include "table/dbsqlitesaintpersonmaptbl.h"
#include "table/dbsqlitecommunitypersontbl.h"
#include "table/dbsqlitecommunitydeptmaptbl.h"
#include "table/dbsqlitedepartmentpersontbl.h"
#include "table/dbsqliteroletbl.h"
#include "table/dbsqliteareamgrtbl.h"
#include "table/dbsqlitespecialistpersontbl.h"
#include "table/dbsqlitecommunitymgrtbl.h"

#include "dbsqlitedefs.h"
#include "dbsqliteedu.h"
#include "dbsqlitesaint.h"
#include "dbsqlitecommunity.h"
#include "dbsqlitespecialist.h"
#include "dbsqlitecountry.h"
#include "dbsqliteethnic.h"
#include "dbsqliteprovince.h"
#include "dbsqlitemission.h"
#include "dbsqlitearea.h"
#include "dbsqlitedept.h"
#include "dbsqlitecourse.h"
#include "dbsqlitework.h"
#include "dbsqliteperson.h"
#include "dbsqlitestatus.h"
#include "dbsqlitepersonevent.h"
#include "dbsqliteevent.h"
#include "dbsqliterole.h"

#include "defs.h"

static const QString DRIVER("QSQLITE");

// TODO: enhance performance of db access via refer to https://lnj.gitlab.io/post/multithreaded-databases-with-qtsql/

DbSqlite* DbSqlite::gInstance = nullptr;
QThreadStorage<DatabaseConnection *> DbSqlite::mDatabaseConnections;

// ref: https://lnj.gitlab.io/post/multithreaded-databases-with-qtsql/
DatabaseConnection::DatabaseConnection(QString uri)
    //        : mName(QString::number(QRandomGenerator::global()->generate(), 36))
    : mName(QString::number((quintptr)QThread::currentThreadId()))
{
    traced;
    auto database = QSqlDatabase::addDatabase(DRIVER, mName);
    logd("create db connection name=%s, uri=%s", STR2CHA(mName), STR2CHA(uri));
    database.setDatabaseName(uri);
    database.open();
    tracede;
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
    traced;
    logd("copy constructor");
    value = item.value;
    dataType = item.dataType;
}

FieldValue::FieldValue(const QString &value, int dataType)
{
    traced;
    logd("constructor");
    this->value = value;
    this->dataType = dataType;
}

DbSqlite::DbSqlite()
{
    traced;
    setupTables();
    setupModelHandler();
}

void DbSqlite::setupTables()
{
    traced;
    // Append all tables need for db
    // TODO: append more table
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
    appendTable(new DbSqliteStatusTbl(this));
    appendTable(new DbSqlitePersonEventTbl(this));
    appendTable(new DbSqliteEventTbl(this));
    appendTable(new DbSqliteSaintPersonMapTbl(this));
    appendTable(new DbSqliteCommunityPersonTbl(this));
    appendTable(new DbSqliteCommunityMgrTbl(this));
    appendTable(new DbSqliteCommunityDeptMapTbl(this));
    appendTable(new DbSqliteDepartmentPersonTbl(this));
    appendTable(new DbSqliteAreaMgrTbl(this));
    appendTable(new DbSqliteSpecialistPersonTbl(this));
}

void DbSqlite::setupModelHandler()
{
    traced;
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
    appendModelHandler(new DbSqliteCommunity());
    appendModelHandler(new DbSqliteArea());
    appendModelHandler(new DbSqliteDept());
    appendModelHandler(new DbSqliteCourse());
    appendModelHandler(new DbSqliteWork());
    appendModelHandler(new DbSqliteStatus());
    appendModelHandler(new DbSqliteRole());
    appendModelHandler(new DbSqlitePerson());
    appendModelHandler(new DbSqliteEvent());
    appendModelHandler(new DbSqlitePersonEvent());
}

const QSqlDatabase &DbSqlite::db() const
{
    return mDb;
}

void DbSqlite::appendTable(DbSqliteTbl *tbl)
{
    traced;
    if (nullptr != tbl)
    {
        logd("Add table '%s'", tbl->name().toStdString().c_str());
        mListTbl[tbl->name()] = tbl;
    }
    else
    {
        // TODO: throw exception?????
    }
}


ErrCode_t DbSqlite::checkOrCreateAllTables()
{
    traced;
    ErrCode_t err = ErrCode_t::ErrNone;
//    err = openDb();
//    if (err == ErrNone) {
//        foreach (QString key, mListTbl.keys())
//        {
//            logd("check & create table '%s'", key.toStdString().c_str());
//            err = mListTbl[key]->checkOrCreateTable();
//            if (err != ErrNone) // TODO: should break? return critical error????
//                break;
//        }
//        closeDb();
//    } else {
//        loge("Failed to open db for create table");
//    }
    foreach (QString key, mListTbl.keys())
    {
        logd("check & create table '%s'", key.toStdString().c_str());
        err = mListTbl[key]->checkOrCreateTable();
        if (err != ErrNone) // TODO: should break? return critical error????
            break;
    }
    tracedr(err);
    return err;
}

void DbSqlite::appendModelHandler(DbModelHandler *hdl)
{
    traced;
    if (nullptr != hdl)
    {
        mModelHdlList[hdl->getName()] = hdl;
    }
    else
    {
        // TODO: throw exception?????
    }
}



DbSqliteTbl *DbSqlite::getTable(const QString &tblName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;

    if (mListTbl.contains(tblName)){
        tbl = mListTbl[tblName];
    }
    else{
        tbl = nullptr;
        loge("Not found table %s", tblName.toStdString().c_str());
        // TODO: reload system? or what should we do?
    }

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

DbModelHandler *DbSqlite::getModelHandler(const QString &name)
{
    DbModelHandler* hdl = nullptr;
    traced;
    if (mModelHdlList.contains(name)){
        hdl = mModelHdlList[name];
    }
    else{
        hdl = nullptr;
        loge("Not found model %s", name.toStdString().c_str());
        // TODO: reload system? or what should we do?
    }

    return hdl;
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
    traced;
    // TODO: handle race condition issue when access db? should use mutex???
    if (!mDatabaseConnections.hasLocalData()) {
        logd("db connection not exist, create new one");
        mDatabaseConnections.setLocalData(new DatabaseConnection(mDbUri));
    } else {
        logd("db connection already exist, use existing one");
    }
    return mDatabaseConnections.localData()->database();
}

QSqlQuery DbSqlite::createQuery()
{
    return QSqlQuery(currentDb());
}


DbSqlite* DbSqlite::getInstance(){
    if (gInstance == nullptr){
        gInstance = new DbSqlite();
    }
    return gInstance;
}


ErrCode_t DbSqlite::loadDb(const DbInfo* dbInfo){
    traced;
    ErrCode_t err = ErrNone;
    if (dbInfo){

        if(QSqlDatabase::isDriverAvailable(DRIVER))
        {
//            mDb = QSqlDatabase::addDatabase(DRIVER);
//            QString uri;
            if (dbInfo->uri().isNull())
                mDbUri = ":memory:";
            else
                mDbUri = dbInfo->uri();

            logd("Db connect to uri %s", mDbUri.toStdString().c_str());

//            mDb.setDatabaseName(mDbUri);
            mDb = currentDb();
            logd("Feature LastInsertId %d", mDb.driver()->hasFeature(QSqlDriver::LastInsertId));
            logd("Feature Transactions %d", mDb.driver()->hasFeature(QSqlDriver::Transactions));
            logd("Feature PreparedQueries %d", mDb.driver()->hasFeature(QSqlDriver::PreparedQueries));

//            if(!mDb.open()){
//                loge("Database connect ERROR %s", mDb.lastError().text().toStdString().c_str());
//                err = ErrFailed;
//            }
//            else{
//                logi("Connected to db %s", uri.toStdString().c_str());
//                err = ErrNone;
//            }

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

    tracedr(err);
    return err;
}

ErrCode_t DbSqlite::execQuery(const QString &sql)
{
    ErrCode_t err = ErrNone;
    traced;
    logd("Execute query %s", sql.toStdString().c_str());

    // TODO: re-implement this, race condition? resource leakage???
//    err = openDb();
    QSqlQuery qry(currentDb());
    if (err == ErrNone) {
        currentDb().transaction();

        qry.prepare(sql);
        if( !qry.exec() ) {

            loge( "Failed to execute %s", qry.executedQuery().toStdString().c_str() );
            loge( "Last error %s", qry.lastError().text().toStdString().c_str() );
            err = ErrFailSqlQuery;
        }

        if (!currentDb().commit()){
            currentDb().rollback();
        }
        currentDb().close();
    } else {
        loge("exe query failed, open db failed %d", err);
    }
    tracedr(err);
    return err;
}

ErrCode_t DbSqlite::execQuery(QSqlQuery *qry)
{

    ErrCode_t err = ErrNone;
    traced;

    startTransaction();
    err = execQueryNoTrans(qry);
    endTransaction();
    tracedr(err);
    return err;
}

ErrCode_t DbSqlite::execQueryNoTrans(QSqlQuery *qry)
{

    ErrCode_t err = ErrNone;
    traced;

    if( !qry->exec() ) {
        loge( "Failed to execQuery %s", qry->executedQuery().toStdString().c_str());
        loge( "Last error %s", qry->lastError().text().toStdString().c_str() );
        err = ErrFailSqlQuery;
    } else {
        logd( "Executed execQuery %s", qry->executedQuery().toStdString().c_str());
    }
    tracedr(err);
    return err;
}

ErrCode_t DbSqlite::startTransaction()
{
    traced;
    ErrCode err = ErrNone;
//    err = openDb();
    if (err == ErrNone)
        currentDb().transaction();
    else
        loge("Start transaction failed, open db failed %d", err);
    // TODO: re-implement this, race condition? resource leakage???
    tracede;
    return ErrNone;
}

ErrCode_t DbSqlite::endTransaction()
{
    traced;
    ErrCode err = ErrNone;
//    err = openDb();
    if (!currentDb().commit()){
        logd("rollback");
        currentDb().rollback();
    }
//    closeDb();
    // TODO: re-implement this, race condition? resource leakage???
    tracede;
    return ErrNone;
}

QSqlDatabase DbSqlite::getDbConnection()
{
    traced;
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
    tracede;
    return db;

}

