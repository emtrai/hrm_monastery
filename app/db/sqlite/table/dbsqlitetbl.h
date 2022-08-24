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
 * Filename: dbsqlitetbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITETBL_H
#define DBSQLITETBL_H

#include <QString>
#include "errcode.h"

#include "dbsqlitedefs.h"
#include "dbmodel.h"
#include <QHash>
class DbSqlite;
class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class QSqlQuery;

class DbSqliteTbl
{
public:
    DbSqliteTbl(DbSqlite* db);
    DbSqliteTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    virtual ~DbSqliteTbl();

    virtual const QString &baseName() const;
    virtual void setBaseName(const QString &newBaseName);

    virtual const QString &name() const;
    virtual void setName(const QString &newName);

    virtual uint32_t versionCode() const;
    virtual void setVersionCode(uint32_t newVersionCode);

    virtual ErrCode add(const DbModel* item);

    virtual bool isExist(const DbModel* item);
    virtual QList<DbModel*> getAll(const DbModelBuilder& builder);
    virtual DbModel* getModel(qint64 dbId, const DbModelBuilder& builder);


    virtual ErrCode checkOrCreateTable();
    virtual ErrCode onDbMigration(int oldVer, int newVer);
    virtual ErrCode onTblMigration(int oldVer, int newVer);
protected:
    virtual QString getSqlCmdCreateTable();
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual void insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
public:

    virtual DbSqlite *db() const;

protected: // allow derived class can get info
    QString mBaseName;
    QString mName;
    uint32_t mVersionCode;
    DbSqlite* mDb;
};

#endif // DBSQLITETBL_H
