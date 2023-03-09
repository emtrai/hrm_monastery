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
 * Filename: dbsqliteedu.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "dbsqliteedu.h"
#include "logger.h"
#include "dbsqliteedu.h"
#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "table/dbsqliteedutbl.h"
#include "defs.h"
DbSqliteEdu* DbSqliteEdu::gInstance = nullptr;

DbSqliteEdu::DbSqliteEdu()
{
    traced;
}

DbSqliteEdu *DbSqliteEdu::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new DbSqliteEdu();
    }

    return gInstance;
}

const QString DbSqliteEdu::getName()
{
    return KModelHdlEdu;
}

DbSqliteTbl *DbSqliteEdu::getMainTbl()
{
    return (DbSqliteEduTbl*)DbSqlite::getInstance()->getTable(KTableEdu);
}

DbModelBuilder DbSqliteEdu::getMainBuilder()
{
    return &Education::build;
}

