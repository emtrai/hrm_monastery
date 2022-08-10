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
 * Filename: dbsqlitespecialisttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#include "dbsqlitespecialisttbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "specialist.h"
#include "defs.h"
#include "logger.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteSpecialistTbl::KVersionCode = VERSION_CODE(0,0,1);


DbSqliteSpecialistTbl::DbSqliteSpecialistTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableSpecialist, KTableSpecialist, KVersionCode)
{
    traced;
}

//ErrCode DbSqliteSpecialistTbl::add(const Specialist *item)
//{
//    traced;
//    ErrCode_t err = ErrNone;
//    QString sql = DbSqliteInsertBuilder::build(name())
//                      ->addValue(KFieldUuid, item->nameid())
//                      ->addValue(KFieldName, item->name())
//                      ->buildSqlStatement();
//    logi("insert sql statement %s", sql.toStdString().c_str());
//    err = db()->execQuery(sql);

//    return err;
//}

//bool DbSqliteSpecialistTbl::isExist(const Specialist *item)
//{
//    QSqlQuery qry;
//    bool exist = false;
//    traced;
//    QString queryString = QString("SELECT COUNT(*) "
//                                  "FROM %1 WHERE %2 = :uuid").arg(name(), KFieldUuid);
//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());

//    // TODO: check sql injection issue
//    qry.bindValue( ":uuid", item->nameid() );
//    if( qry.exec() )
//    {
//        int rows= 0;
//        if (qry.next()) {
//            rows = qry.value(0).toInt();
//            exist = rows > 0;
//        }
//        else {
//            logd("Not found any items");
//            exist = false;
//        }
//    }
//    else {
//        loge( "Failed to execute %s", queryString.toStdString().c_str() );
//        exist = false;
//    }

//    logd("Exist %d", exist);
//    return exist;
//}

//QList<Specialist *> DbSqliteSpecialistTbl::getAll()
//{
//    QSqlQuery qry;

//    traced;
//    // TODO: check record status????
//    QString queryString = QString("SELECT * FROM %1").arg(name());
//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());
//    QList<Specialist*>list;

//    if( qry.exec() )
//    {
//        while (qry.next()) {
//            Specialist* item = new Specialist();
//            // TODO: validate value before, i.e. toInt return ok
//            item->setName(qry.value(KFieldName).toString());
//            item->setDbId(qry.value(KFieldId).toInt());
//            item->setUuid(qry.value(KFieldUuid).toString());
//            list.append(item); // TODO: when it cleaned up?????
//        }
//    }
//    else {
//        loge( "Failed to execute %s", queryString.toStdString().c_str() );
//    }

//    logd("Found %d", list.size());
//    return list;
//}

//QString DbSqliteSpecialistTbl::getSqlCmdCreateTable()
//{
//    // TODO; support multi language
//    QString sql = DbSqliteTableBuilder::build(name())
//                      ->addField(KFieldUuid, TEXT)
//                      ->addField(KFieldName, TEXT)
//                      ->buildSqlStatement();
//    logi("Create statement %s", sql.toStdString().c_str());

//    return sql;
//}
