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
 * Filename: dbsqliteareamgrtbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITEAREAMGRTBL_H
#define DBSQLITEAREAMGRTBL_H

#include "dbsqlitecommdeptpersontbl.h"

class DbSqliteAreaMgrTbl : public DbSqliteMapTbl
{
public:
    DbSqliteAreaMgrTbl(DbSqlite* db);
    QList<DbModel*> getListPerson(const QString& areaUid, int status = MODEL_STATUS_MAX);

protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEAREAMGRTBL_H
