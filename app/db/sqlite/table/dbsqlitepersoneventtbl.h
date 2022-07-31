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
 * Filename: dbsqlitepersoneventtbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITEPersonEVENTTBL_H
#define DBSQLITEPersonEVENTTBL_H

#include "dbsqlitetbl.h"
#include "errcode.h"
#include "std.h"

class DbSqlite;

/**
* Table for events of a person
*/
class DbSqlitePersonEventTbl : public DbSqliteTbl
{
public:
    DbSqlitePersonEventTbl(DbSqlite* db);


protected:
    virtual QString getSqlCmdCreateTable();

private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEPersonEVENTTBL_H
