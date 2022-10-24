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
 * Filename: dbsqlitedept.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "dbsqlitedept.h"
#include "logger.h"
#include "defs.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "table/dbsqlitedepartmentpersontbl.h"

DbSqliteDept::DbSqliteDept()
{
    traced;
}

const QString DbSqliteDept::getName()
{
    return KModelHdlDept;
}

QList<DbModel *> DbSqliteDept::getListPerson(const QString &deptUid)
{
    traced;
    DbSqliteDepartmentPersonTbl* tbl = (DbSqliteDepartmentPersonTbl*)DbSqlite::getInstance()
                                          ->getTable(KTableDepartPerson);
    return tbl->getListPerson(deptUid);
}

DbSqliteTbl *DbSqliteDept::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableDepartment);
}

DbSqliteTbl *DbSqliteDept::getTable(const QString &modelName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;
    logd("Get model modelName %s", modelName.toStdString().c_str());
    if (modelName == KModelNamePersonDept) {
        logd("get table '%s'", KTableDepartPerson);
        tbl = (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableDepartPerson);
    } else {
        logd("get main table");
        tbl = getMainTbl();
    }
    tracede;
    return tbl;
}
