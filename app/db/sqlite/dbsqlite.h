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
 * Filename: dbsqlite.h
 * Author: Anh, Ngo Huy
 * Created date:7/19/2022
 * Brief:
 */
#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <idatabase.h>
#include "errcode.h"
#include "person.h"
#include <QSqlQuery>
#include <QMap>
#include <QSqlDatabase>
#include <QThreadStorage>

#define SQLITE (DbSqlite::getInstance())

// TODO: make it as library????
class DbSqliteTbl;

// https://lnj.gitlab.io/post/multithreaded-databases-with-qtsql/
class DatabaseConnection
{
    Q_DISABLE_COPY(DatabaseConnection)
public:
    DatabaseConnection(QString uri);

    virtual ~DatabaseConnection();

    QSqlDatabase database();

private:
    QString mName;
};
// THIS IS ONE INTERFACE FOR ALL OPERATION RELATING TO DB
// TODO: should separate person, community, saint, etc. into separate class????
// It's quite stupid when putting everything here
class DbSqlite : public IDatabase
{
public:

//    virtual ErrCode_t addPerson(const Person* person);
    /**
    * Load database
    */

    virtual ErrCode_t loadDb(const DbInfo* dbInfo);

    virtual ErrCode_t execQuery(const QString& sql);
    virtual ErrCode_t execQuery(QSqlQuery* qry);
    virtual ErrCode_t execQueryNoTrans(QSqlQuery* qry);
    virtual ErrCode_t startTransaction();
    virtual ErrCode_t endTransaction();
    QSqlDatabase getDbConnection();
//    virtual ErrCode_t addCommunity(const Community* comm);
    virtual DbSqliteTbl* getTable(const QString& tblName);
    virtual DbModelHandler *getEduModelHandler();
    virtual DbModelHandler *getSaintModelHandler();
    virtual DbModelHandler* getSpecialistModelHandler();
    virtual DbModelHandler* getCommunityModelHandler();
    virtual DbModelHandler* getModelHandler(const QString& name);

    static DbSqliteTbl* table(const QString& tblName);
    static DbModelHandler* handler(const QString& name);
    virtual ErrCode openDb();
    virtual void closeDb();

    QSqlDatabase currentDb();
    QSqlQuery createQuery();

private:

    DbSqlite();


    /**
    * Append table to List
    */
    void appendTable(DbSqliteTbl* tbl);
    void setupTables();

    /**
    * Check & create all tables if not exist
    */
    ErrCode_t checkOrCreateAllTables();

    void appendModelHandler(DbModelHandler* tbl);
    void setupModelHandler();

private:
    static DbSqlite* gInstance;
    QMap<QString, DbSqliteTbl*> mListTbl;
    QMap<QString, DbModelHandler*> mModelHdlList;
    QSqlDatabase mDb;
    QSqlDatabase mCurrentDb; // TODO: race condition???
    QString mDbUri;

public:
    static DbSqlite* getInstance();

    const QSqlDatabase &db() const;

public:
    static QThreadStorage<DatabaseConnection *> mDatabaseConnections;
};

#endif // DBSQLITE_H
