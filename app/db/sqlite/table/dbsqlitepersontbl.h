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
 * Filename: dbsqlitepersontbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITEPersonTBL_H
#define DBSQLITEPersonTBL_H

#include "dbsqlitetbl.h"
#include "errcode.h"
#include "std.h"

class Person;

class DbSqlitePersonTbl : public DbSqliteTbl
{
public:
    DbSqlitePersonTbl(DbSqlite* db);

    ErrCode_t addPerson(const Person* person);

protected:
    virtual QString getSqlCmdCreateTable();

public:
    static const qint32 KVersionCode;


};

#endif // DBSQLITEPersonTBL_H
