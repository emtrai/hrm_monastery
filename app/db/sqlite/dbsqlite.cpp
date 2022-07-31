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

#include <QSqlDatabase>
#include "dbsqlitetablebuilder.h"
#include "table/dbsqlitepersontbl.h"
#include "table/dbsqlitecommunitytbl.h"
#include "table/dbsqlitesainttbl.h"
#include "dbsqlitedefs.h"

static const QString DRIVER("QSQLITE");

DbSqlite* DbSqlite::gInstance = nullptr;

DbSqlite::DbSqlite()
{
    traced;

    // Append all tables need for db
    // TODO: append more table
    appendTable(new DbSqlitePersonTbl(this));
    appendTable(new DbSqliteCommunityTbl(this));
    appendTable(new DbSqliteSaintTbl(this));
}

void DbSqlite::appendTable(DbSqliteTbl *tbl)
{
    if (nullptr != tbl)
    {
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

    foreach (QString key, mListTbl.keys())
    {
        err = mListTbl[key]->checkOrCreateTable();
        if (err != ErrNone)
            break;
    }

    tracedr(err);
    return err;
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


DbSqlite* DbSqlite::getInstance(){
    if (gInstance == nullptr){
        gInstance = new DbSqlite();
    }
    return gInstance;
}


ErrCode_t DbSqlite::addPerson(const Person* person)
{
    traced;
    ErrCode_t err = ErrNone;

    if (mListTbl.contains(KTablePerson)){
        err = ((DbSqlitePersonTbl*)mListTbl[KTablePerson])->addPerson(person);
    }
    else{
        err = ErrNotFound;
        loge("Not found table %s", KTablePerson);
        // TODO: reload system? or what should we do?
    }

    return err;
}

ErrCode_t DbSqlite::loadDb(const DbInfo* dbInfo){
    traced;
    ErrCode_t err = ErrNone;
    if (dbInfo){

        if(QSqlDatabase::isDriverAvailable(DRIVER))
        {
            QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
            QString uri;
            if (dbInfo->uri().isNull())
                uri = ":memory:";
            else
                uri = dbInfo->uri();

            logd("Db connect to uri %s", uri.toStdString().c_str());

            db.setDatabaseName(uri);

            if(!db.open()){
                loge("Database connect ERROR %s", db.lastError().text().toStdString().c_str());
                err = ErrFailed;
            }
            else{
                logi("Connected to db %s", uri.toStdString().c_str());
                err = ErrNone;
            }

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
    QSqlQuery qry;
    ErrCode_t err = ErrNone;
    traced;
    logd("Execute query %s", sql.toStdString().c_str());
    qry.prepare(sql);
    if( !qry.exec() ) {
        qFatal( "Failed to execute %s", sql.toStdString().c_str() );
        err = ErrFailSqlQuery;
    }
    return err;
}

