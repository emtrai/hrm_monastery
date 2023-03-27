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
 * Filename: dbsqlitesaint.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "dbsqlitesaint.h"
#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "table/dbsqlitesainttbl.h"
#include "logger.h"
#include "defs.h"
#include "saint.h"
DbSqliteSaint* DbSqliteSaint::gInstance = nullptr;


DbSqliteSaint::DbSqliteSaint():DbSqliteModelHandler()
//    mSaintTbl(nullptr)
{
    tracein;
}

DbSqliteSaint *DbSqliteSaint::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new DbSqliteSaint();
    }

    return gInstance;
}

const QString DbSqliteSaint::getName()
{
    return KModelHdlSaint;
}


DbModel *DbSqliteSaint::getByUid(const QString &name)
{
    tracein;
    return DbSqliteModelHandler::getByUid(name, &Saint::build);
}

DbSqliteTbl *DbSqliteSaint::getMainTbl()
{
    return (DbSqliteSaintTbl*)DbSqlite::getInstance()->getTable(KTableSaint);
}

DbModelBuilder DbSqliteSaint::getMainBuilder()
{
    return &Saint::build;
}

