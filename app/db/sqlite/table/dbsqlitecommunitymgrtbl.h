/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dbsqlitecommunitymgrtbl.h
 * Author: Anh, Ngo Huy
 * Created date:1/17/2023
 * Brief:
 */
#ifndef DBSQLITECOMMUNITYMGRTBL_H
#define DBSQLITECOMMUNITYMGRTBL_H

#include "dbsqlitemaptbl.h"

class DbSqliteCommunityMgrTbl : public DbSqliteMapTbl
{
public:
    DbSqliteCommunityMgrTbl(DbSqlite* db);

protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITECOMMUNITYMGRTBL_H
