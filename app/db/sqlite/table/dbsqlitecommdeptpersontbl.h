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
#ifndef DBSQLITECOMMDEPTPERSONTBL_H
#define DBSQLITECOMMDEPTPERSONTBL_H

#include "dbsqlitemaptbl.h"

class DbSqliteCommDeptPersonTbl : public DbSqliteTbl
{
public:
    DbSqliteCommDeptPersonTbl(DbSqlite* db);
    QList<DbModel*> getListPerson(const QString& communityUid, int status = MODEL_STATUS_MAX);
protected:
    virtual QString getSearchQueryStringWithTag(const QString& cond = nullptr, const QString& condTag = nullptr);
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);

    virtual ErrCode updateDbModelDataFromQuery(DbModel* item, const QSqlQuery& qry);

    virtual ErrCode updateBuilderFieldFromModel(DbSqliteUpdateBuilder* builder,
                                                const QString& field,
                                                const DbModel *item);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITECOMMDEPTPERSONTBL_H
