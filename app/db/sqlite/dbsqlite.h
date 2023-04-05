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
#include "dbsqlitedefs.h"
#include "table/dbmetadatatbl.h"
#include "table/dbsqlsequencetbl.h"
#include "dbmetainfo.h"

#define DB_VERSION_MAJOR    0
#define DB_VERSION_MINOR    0
#define DB_VERSION_PATCH    1
#define DB_VERSION      ((DB_VERSION_MAJOR << 24) | (DB_VERSION_MINOR << 8) | DB_VERSION_PATCH)

#define SQLITE (DbSqlite::getInstance())

// TODO: make it as library????
class DbSqliteTbl;
class DbSqlitePerson;

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

struct FieldValue
{
    FieldValue();
    FieldValue(const FieldValue& item);
    FieldValue(const QString& value, int dataType = TEXT);
    FieldValue(qint64 value);
    qint64 valueInt();
    const QString& valueString();
    QString value;
    qint64 mIntValue;
    int dataType; // TODO: make it private?
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
    virtual ErrCode validateDbInfo(const DbInfo* dbInfo);

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
    virtual DbModelHandler* getPersonModelHandler();
    virtual DbModelHandler* getMissionModelHandler();
    virtual DbModelHandler* getAreaModelHandler();
    virtual DbModelHandler* getCountryModelHandler();
    virtual DbModelHandler* getModelHandler(const QString& name);
    virtual DbModelHandler* getDepartmentModelHandler();
    virtual DbModelHandler* getWorkModelHandler();
    virtual DbModelHandler* getPersonStatusModelHandler();
    virtual DbModelHandler* getCourseModelHandler();
    virtual DbModelHandler* getEthnicModelHandler();

    static DbSqliteTbl* table(const QString& tblName);
    static DbModelHandler* handler(const QString& name);

    QSqlDatabase currentDb();
    QSqlQuery createQuery();


    virtual quint64 getDbVersion();
    virtual quint64 getDbVersionInMetadata(bool* ok = nullptr);
    virtual ErrCode updateDbVersionInMetadata(qint64 value);
    virtual ErrCode getMetadataValue(const QString& key, QString& value);
    virtual ErrCode getMetadataValue(const QString& key, qint64& value);
    virtual ErrCode updateMetadataValue(const QString& key, qint64 value);
    virtual ErrCode updateMetadataValue(const QString& key, const QString& value);
    virtual quint64 getCurrentPersonCodeNumber(bool* ok = nullptr);
    virtual quint64 getDbSeqNumber(const QString& tblName, bool* ok = nullptr);
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
    DbMetadataTbl* tableMetadata();
    DbSqlSequenceTbl* tblSqlSequence();
private:
    static DbSqlite* gInstance;
    QMap<QString, DbSqliteTbl*> mListTbl;
    QMap<QString, DbModelHandler*> mModelHdlList;
    QSqlDatabase mDb;
    QSqlDatabase mCurrentDb; // TODO: race condition???
    QString mDbUri;

    DbMetaInfo mMetaDbInfo;

public:
    static DbSqlite* getInstance();

    const QSqlDatabase &db() const;

public:
    static QThreadStorage<DatabaseConnection *> mDatabaseConnections;
};

#endif // DBSQLITE_H
