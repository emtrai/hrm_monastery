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
 * Filename: dbsqlitesainttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "dbsqlitesainttbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "saint.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteSaintTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteSaintTbl::DbSqliteSaintTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableSaint, KTableSaint, KVersionCode)
{

}

void DbSqliteSaintTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldFullName, TEXT);
    builder->addField(KFieldGender, INT32);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldCountry, TEXT);
}

void DbSqliteSaintTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);
    Saint* saint = (Saint*) item;
    builder->addValue(KFieldFullName, saint->fullName());
    builder->addValue(KFieldGender, saint->gender());
    builder->addValue(KFieldFeastDay, saint->feastDay());
    builder->addValue(KFieldCountry, saint->country());
}

void DbSqliteSaintTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Saint* saint = (Saint*) item;
    saint->setFullName(qry.value(KFieldFullName).toString());
    saint->setGender((Gender)qry.value(KFieldGender).toInt());
    saint->setFeastDay(qry.value(KFieldFeastDay).toInt());
    saint->setCountry(qry.value(KFieldCountry).toString());
}


//ErrCode_t DbSqliteSaintTbl::add(const Saint *item)
//{
//    traced;
//    ErrCode_t err = ErrNone;
//    QString sql = DbSqliteInsertBuilder::build(name())
//                      ->addValue(KFieldNameId, item->nameid())
//                      ->addValue(KFieldName, item->name())
//                      ->addValue(KFieldGender, item->gender())
//                      ->addValue(KFieldFeastDay, item->feastDay())
//                      ->addValue(KFieldCountry, item->country())
//                      ->addValue(KFieldHistory, item->history())

//                      ->buildSqlStatement();
//    logi("insert sql statement %s", sql.toStdString().c_str());
//    err = db()->execQuery(sql);

//    return err;

//}

//QHash<QString, Saint*> DbSqliteSaintTbl::getListSaint()
//{
//    QSqlQuery qry;

//    traced;
//    QString queryString = QString("SELECT * FROM %1").arg(name());
//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());
//    QHash<QString, Saint*> list;

//    if( qry.exec() )
//    {
//        while (qry.next()) {
//            Saint* saint = new Saint();
//            // TODO: validate value before, i.e. toInt return ok
//            saint->setName(qry.value(KFieldName).toString());
//            saint->setDbId(qry.value(KFieldId).toInt());
//            saint->setGender((Gender)qry.value(KFieldId).toInt());
//            saint->setFeastDay(qry.value(KFieldFeastDay).toInt());
//            list.insert(saint->name(), saint);
//        }
//    }
//    else {
//        loge( "Failed to execute %s", queryString.toStdString().c_str() );
//    }

//    logd("Found %d", list.size());
//    return list;
//}

//QString DbSqliteSaintTbl::getSqlCmdCreateTable()
//{
//    traced;
//    // TODO; support multi language
//    QString sql = DbSqliteTableBuilder::build(name())
//                      ->addField(KFieldNameId, TEXT)
//                      ->addField(KFieldName, TEXT)
//                      ->addField(KFieldGender, TEXT)
//                      ->addField(KFieldFeastDay, INT64)
//                      ->addField(KFieldCountry, TEXT)
//                      ->addField(KFieldHistory, TEXT)

//                      ->buildSqlStatement();
//    logi("Create statement %s", sql.toStdString().c_str());

//    return sql;
//}
