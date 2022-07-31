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
 * Filename: dbsqlitedeparttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "dbsqlitedeparttbl.h"
#include "department.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "std.h"
#include "dbsqlitetablebuilder.h"

#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteDepartTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteDepartTbl::DbSqliteDepartTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommunity, KTableCommunity, KVersionCode)
{}

ErrCode_t DbSqliteDepartTbl::add(const Department *item)
{
    traced;
    ErrCode_t err = ErrNone;
    QString sql = DbSqliteInsertBuilder::build(name())
                      ->addValue(KFieldName, item->name())
//                      ->addValue(KFieldCreateDate, item->createDate())
//                      ->addValue(KFieldParentUid, item->parentUid())
//                      ->addValue(KFieldStatus, (qint32) item->getStatus())

                      ->buildSqlStatement();
    logi("insert sql statement %s", sql.toStdString().c_str());
    err = db()->execQuery(sql);

    return err;

}

QString DbSqliteDepartTbl::getSqlCmdCreateTable()
{
    traced;
    // TODO; support multi language
    QString sql = DbSqliteTableBuilder::build(name())
                      ->addField(KFieldName, TEXT)
//                      ->addField(KFieldCEOUid, TEXT)
//                      ->addField(KFieldChurchUid, TEXT)
//                      ->addField(KFieldAreaUid, TEXT)
//                      ->addField(KFieldLevel, INT32)
//                      ->addField(KFieldParentUid, TEXT)
//                      ->addField(KFieldCreateDate, INT64)
//                      ->addField(KFieldDateFormat, TEXT)
//                      ->addField(KFieldStatus, INT32) // stop, alive, etc.
//                      ->addField(KFieldPreset, INT32) // 0: custom, 1: preset (from json)

                      ->buildSqlStatement();
    logi("Create statement %s", sql.toStdString().c_str());

    return sql;
}
