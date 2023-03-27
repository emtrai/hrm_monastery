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
 * Filename: dbsqliteethnic.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "dbsqliteethnic.h"
#include "logger.h"
#include "defs.h"
#include "ethnic.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"

DbSqliteEthnic::DbSqliteEthnic()
{
    tracein;
}

const QString DbSqliteEthnic::getName()
{
    return KModelHdlEthnic;
}

DbModel *DbSqliteEthnic::getByUid(const QString &uid)
{
    tracein;
    return DbSqliteModelHandler::getByUid(uid, Ethnic::build);
}

DbSqliteTbl *DbSqliteEthnic::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableEthnic);
}

DbModelBuilder DbSqliteEthnic::getMainBuilder()
{
    return &Ethnic::build;
}
