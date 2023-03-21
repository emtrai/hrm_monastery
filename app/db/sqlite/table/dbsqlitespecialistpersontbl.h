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
 * Filename: dbsqlitespecialistpersontbl.h
 * Author: Anh, Ngo Huy
 * Created date:11/3/2022
 * Brief:
 */
#ifndef DBSQLITESPECIALISTPERSONTBL_H
#define DBSQLITESPECIALISTPERSONTBL_H

#include "dbsqlitemaptbl.h"

class DbSqliteSpecialistPersonTbl : public DbSqliteMapTbl
{
public:
    DbSqliteSpecialistPersonTbl(DbSqlite* db);
    DbSqliteSpecialistPersonTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    QList<DbModel*> getListPerson(const QString& specialistUid, int status = MODEL_ACTIVE);
    QList<DbModel*> getListSpecialist(const QString& personUid, int status = MODEL_ACTIVE);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);

    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);

private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITESPECIALISTPERSONTBL_H
