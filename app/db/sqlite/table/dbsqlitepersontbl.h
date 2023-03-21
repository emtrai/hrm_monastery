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
 * Filename: dbsqlitepersontbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITEPersonTBL_H
#define DBSQLITEPersonTBL_H

#include "dbsqlitetbl.h"

class Person;

class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class DbModel;
class QSqlQuery;
class DbSqliteCommunityPersonTbl;

class DbSqlitePersonTbl : public DbSqliteTbl
{
public:
    DbSqlitePersonTbl(DbSqlite* db);

    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual ErrCode updateTableField(DbSqliteUpdateBuilder* builder,
                                     const QList<QString>& updateField,
                                     const DbModel *item);
    virtual ErrCode updateCommunity(const QString& uid, const QString& communityUid);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);
    QHash<QString, int> getSearchFields();
    QList<QString> getNameFields();
    virtual QString getSearchQueryString(const QString& cond = nullptr);

    virtual QString getFilterQueryString(int fieldId, const QString& cond = nullptr);

//    ErrCode_t addPerson(const Person* person);

//protected:
//    virtual QString getSqlCmdCreateTable();

public:
    static const qint32 KVersionCode;

    // allow this class can call protected/private class
    // this is not good at OOP, but I want to re-use code, not want to waste my relax time...
    friend class DbSqliteCommunityPersonTbl;
};

#endif // DBSQLITEPersonTBL_H
