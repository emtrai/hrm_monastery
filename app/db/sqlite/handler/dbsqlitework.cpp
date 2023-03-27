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
 * Filename: dbsqlitework.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/2/2022
 * Brief:
 */
#include "dbsqlitework.h"
#include "logger.h"
#include "defs.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "work.h"

GET_INSTANCE_IMPL(DbSqliteWork)


DbSqliteWork::DbSqliteWork()
{
    tracein;
}

const QString DbSqliteWork::getName()
{
    return KModelHdlWork;
}

DbModelBuilder DbSqliteWork::getMainBuilder()
{
    return (DbModelBuilder)Work::build;
}

DbSqliteTbl *DbSqliteWork::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableWork);
}
