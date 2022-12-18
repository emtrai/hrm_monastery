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
 * Filename: dbsqlitecommunitytbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#ifndef DBSQLITECOMMUNITYTBL_H
#define DBSQLITECOMMUNITYTBL_H

#include "dbsqlitetbl.h"

class DbSqlite;
class DbModel;
class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class QSqlQuery;

class DbSqliteCommunityTbl : public DbSqliteTbl
{
public:
    DbSqliteCommunityTbl(DbSqlite* db);

    QList<DbModel *> getListCommunitiesInArea(const QString &areaUid, int status = 0);
protected:
//    virtual QString getSqlCmdCreateTable();
//    virtual ErrCode_t add(const DbModel* item);
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual QString getSearchQueryString(const QString& cond = nullptr);

    virtual DbModelBuilder mainModelBuilder();
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITECOMMUNITYTBL_H
