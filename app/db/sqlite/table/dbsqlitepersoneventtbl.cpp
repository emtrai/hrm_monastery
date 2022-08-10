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
 * Filename: dbsqlitepersoneventtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#include "dbsqlitepersoneventtbl.h"
#include "dbsqlitetablebuilder.h"

#include "defs.h"
#include "logger.h"

static const uint32_t TBL_VERSION_CODE = VERSION_CODE(0,0,1);
static const char* TBL_NAME = "PersonEvent";

DbSqlitePersonEventTbl::DbSqlitePersonEventTbl(DbSqlite* db):
    DbSqliteTbl(db, TBL_NAME, TBL_NAME, TBL_VERSION_CODE)
{}


QString DbSqlitePersonEventTbl::getSqlCmdCreateTable() {
    traced;
    QString sql = DbSqliteTableBuilder::build(name())
                      ->addField(KFieldPersonId, INT32)
                      ->addField(KFieldEventId, TEXT)
                      ->addField(KFieldDate, TEXT)
                      ->addField(KFieldTitle, TEXT)
                      ->addField(KFieldRemark, TEXT)

                      ->buildSqlStatement();
    logi("Create statement %s", sql.toStdString().c_str());

    return sql;
}
