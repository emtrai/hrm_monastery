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
 * Filename: dbsqlitestatustbl.h
 * Author: Anh, Ngo Huy
 * Created date:9/5/2022
 * Brief:
 */
#ifndef DBSQLITEPERSONSTATUSTBL_H
#define DBSQLITEPERSONSTATUSTBL_H

#include "dbsqlitetbl.h"

class DbSqlitePersonStatusTbl : public DbSqliteTbl
{
public:
    DbSqlitePersonStatusTbl();
public:
    DbSqlitePersonStatusTbl(DbSqlite *db);

private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEPERSONSTATUSTBL_H
