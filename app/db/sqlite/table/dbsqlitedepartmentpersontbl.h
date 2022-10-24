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
 * Filename: dbsqlitedepartmentpersontbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#ifndef DBSQLITEDEPARTMENTPERSONTBL_H
#define DBSQLITEDEPARTMENTPERSONTBL_H

#include "dbsqlitemaptbl.h"

class DbSqliteDepartmentPersonTbl : public DbSqliteMapTbl
{
public:
    DbSqliteDepartmentPersonTbl(DbSqlite* db);
    DbSqliteDepartmentPersonTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    QList<DbModel*> getListPerson(const QString& communityUid, int status = ITEM_MAP_STATUS_ACTIVE);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);


    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);

private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEDEPARTMENTPERSONTBL_H
