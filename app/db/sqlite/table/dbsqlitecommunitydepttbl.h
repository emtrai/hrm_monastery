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
 * Filename: dbsqlitecommunitydeptmaptbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITECOMMUNITYDEPTTBL_H
#define DBSQLITECOMMUNITYDEPTTBL_H

#include "dbsqlitetbl.h"
#include "dbmodel.h"

class DbSqliteCommunityDeptTbl : public DbSqliteTbl
{
public:
    DbSqliteCommunityDeptTbl(DbSqlite* db);
    QList<DbModel *> getListDept(const QString &commUid, int status = MODEL_ACTIVE);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
//    virtual QString getSearchQueryStringWithTag(const QString& cond = nullptr, const QString& condTag = nullptr);

    virtual DbModelBuilder mainModelBuilder();
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITECOMMUNITYDEPTTBL_H
