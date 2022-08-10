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
 * Filename: dbsqlitecommunitytbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "dbsqlitecommunitytbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "defs.h"
#include "dbsqlitetablebuilder.h"
#include "community.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include "logger.h"

const qint32 DbSqliteCommunityTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityTbl::DbSqliteCommunityTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommunity, KTableCommunity, KVersionCode)
{}

//ErrCode_t DbSqliteCommunityTbl::add(const DbModel *param)
//{
//    traced;
//    ErrCode_t err = ErrNone;
//    Community* item = (Community*)param;
//    QString sql = DbSqliteInsertBuilder::build(name())
//                      ->addValue(KFieldName, item->name())
//                      ->addValue(KFieldCreateDate, item->createDate())
//                      ->addValue(KFieldParentUid, item->parentUid())
//                      ->addValue(KFieldStatus, (qint32) item->getStatus())

//                      ->buildSqlStatement();
//    logi("insert sql statement %s", sql.toStdString().c_str());
//    err = db()->execQuery(sql);

//    return err;

//}

void DbSqliteCommunityTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);
    Community* cmm = (Community*) item;
    builder->addValue(KFieldCreateDate, cmm->createDate());
    builder->addValue(KFieldParentUid, cmm->parentUid());
    builder->addValue(KFieldStatus, (qint32) cmm->getStatus());
}

void DbSqliteCommunityTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Community* cmm = (Community*) item;
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setParentUid(qry.value(KFieldParentUid).toString());
    cmm->setStatus((CommunityStatus)qry.value(KFieldStatus).toInt());
}

//QString DbSqliteCommunityTbl::getSqlCmdCreateTable()
//{
//    traced;
//    // TODO; support multi language
//    QString sql = DbSqliteTableBuilder::build(name())
//                      ->addField(KFieldName, TEXT)
//                      ->addField(KFieldCEOUid, TEXT)
//                      ->addField(KFieldChurchUid, TEXT)
//                      ->addField(KFieldAreaUid, TEXT)
//                      ->addField(KFieldLevel, INT32)
//                      ->addField(KFieldParentUid, TEXT)
//                      ->addField(KFieldCreateDate, INT64)
//                      ->addField(KFieldDateFormat, TEXT)
//                      ->addField(KFieldStatus, INT32) // stop, alive, etc.
//                      ->addField(KFieldPreset, INT32) // 0: custom, 1: preset (from json)

//                      ->buildSqlStatement();
//    logi("Create statement %s", sql.toStdString().c_str());

//    return sql;
//}

void DbSqliteCommunityTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCEOUid, TEXT);
    builder->addField(KFieldChurchUid, TEXT);
    builder->addField(KFieldAreaUid, TEXT);
    builder->addField(KFieldLevel, INT32);
    builder->addField(KFieldParentUid, TEXT);
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldDateFormat, TEXT);
    builder->addField(KFieldStatus, INT32); // stop, alive, etc.
    builder->addField(KFieldPreset, INT32); // 0: custom, 1: preset (from json)
}
