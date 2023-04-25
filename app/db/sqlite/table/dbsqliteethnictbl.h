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
 * Filename: dbsqliteethnictbl.h
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#ifndef DBSQLITEETHNICTBL_H
#define DBSQLITEETHNICTBL_H

#include "dbsqlitetbl.h"

class DbSqliteEthnicTbl : public DbSqliteTbl
{
public:
    DbSqliteEthnicTbl();
public:
    DbSqliteEthnicTbl(DbSqlite *db);
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
protected:

    virtual QString getSearchQueryString(const QString& cond = nullptr);
    virtual ErrCode updateTableField(DbSqliteUpdateBuilder* builder,
                                     const QList<QString>& updateField,
                                     const DbModel *item);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEETHNICTBL_H
