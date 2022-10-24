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
 * Filename: dbsqlitemaptbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITEMAPTBL_H
#define DBSQLITEMAPTBL_H

#include "dbsqlitetbl.h"

enum ItemMapStatus{
    ITEM_MAP_STATUS_NOT_READY = (1 << 0),
    ITEM_MAP_STATUS_INACTIVE = (1 << 1), // Map is inactive
    ITEM_MAP_STATUS_ACTIVE = (1 << 2), // Map is active

    ITEM_MAP_STATUS_UNKNOWN = (0),
};

class MapDbModel;

class DbSqliteMapTbl : public DbSqliteTbl
{
public:
    DbSqliteMapTbl(DbSqlite* db);
    DbSqliteMapTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);

    virtual const QString& getFieldNameUid1() const;
    virtual const QString& getFieldNameDbid1() const;
    virtual const QString& getFieldNameUid2() const;
    virtual const QString& getFieldNameDbid2() const;
protected:
    QString mFieldNameUid1;
    QString mFieldNameDbId1;
    QString mFieldNameUid2;
    QString mFieldNameDbId2;
};

#endif // DBSQLITEMAPTBL_H
