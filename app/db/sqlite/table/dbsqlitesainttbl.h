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
 * Filename: dbsqlitesainttbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#ifndef DBSQLITESAINTTBL_H
#define DBSQLITESAINTTBL_H

#include "dbsqlitetbl.h"


class DbSqlite;
class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class DbModel;
class QSqlQuery;

class DbSqliteSaintTbl : public DbSqliteTbl
{
public:
    DbSqliteSaintTbl(DbSqlite* db);

    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual ErrCode updateDbModelDataFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual QHash<QString, int> getSearchFields();
    virtual QList<QString> getNameFields();

    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual ErrCode search(const QString& keyword, QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);


    virtual ErrCode onTblMigration(qint64 oldVer);
    virtual ErrCode updateBuilderFieldFromModel(DbSqliteUpdateBuilder* builder,
                                                const QString& field,
                                                const DbModel *item);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITESAINTTBL_H
