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
    builder->addField(KFieldOriginName, TEXT);
    builder->addField(KFieldGender, INT32);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldCountry, TEXT);
    builder->addField(KFieldRemark, TEXT);
}

void DbSqliteSaintTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);
    Saint* saint = (Saint*) item;
    builder->addValue(KFieldFullName, saint->fullName());
    builder->addValue(KFieldOriginName, saint->originName());
    builder->addValue(KFieldGender, saint->gender());
    builder->addValue(KFieldFeastDay, saint->feastDay());
    builder->addValue(KFieldCountry, saint->country());
    builder->addValue(KFieldRemark, saint->remark());
}

void DbSqliteSaintTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Saint* saint = (Saint*) item;
    saint->setFullName(qry.value(KFieldFullName).toString());
    saint->setOriginName(qry.value(KFieldOriginName).toString());
    saint->setGender((Gender)qry.value(KFieldGender).toInt());
    saint->setFeastDay(qry.value(KFieldFeastDay).toInt());
    saint->setCountry(qry.value(KFieldCountry).toString());
    saint->setRemark(qry.value(KFieldRemark).toString());
}

QHash<QString, int> DbSqliteSaintTbl::getSearchFields()
{
    QHash<QString, int> inFields;// TODO: make as class member?
    inFields[KFieldName] = TEXT;
    inFields[KFieldFullName] = TEXT;
    inFields[KFieldOriginName] = TEXT;
    return inFields;
}

QList<QString> DbSqliteSaintTbl::getNameFields()
{
    traced;
    QList<QString> list;// TODO: make as class member?
    list.append(KFieldName);
    list.append(KFieldFullName);
    list.append(KFieldOriginName);
    return list;
}

int DbSqliteSaintTbl::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    return DbSqliteTbl::search (keyword, &Saint::build, outList);
//    QHash<QString, int> inFields;
//    inFields[KFieldName] = TEXT;
//    inFields[KFieldFullName] = TEXT;
//    inFields[KFieldOriginName] = TEXT;
//    logi("Search Saint '%s'", keyword.toStdString().c_str());
//    qint32 cnt = 0;
//    cnt = DbSqliteTbl::search (keyword, inFields, &Saint::build, outList);
//    traced;
//    // TODO: implement it
//    // TODO: exact and not exact match???
//    QSqlQuery qry;
//    qint32 cnt = 0;
//    logi("Search Saint '%s'", keyword.toStdString().c_str());
//    QString queryString = QString("SELECT * "
//                                  "FROM %1 WHERE lower(%2) like :name OR "
//                                  "lower(%3) like :name OR "
//                                  "lower(%4) like :name")
//                              .arg(name(), KFieldName, KFieldFullName, KFieldOriginName);
//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());

//    // TODO: check sql injection issue
//    qry.bindValue( ":name", QString("%%1%").arg(keyword.trimmed().toLower()) );
//    cnt = runQuery(qry, &Saint::build, outList);
////    if( qry.exec() )
////    {
////        while (qry.next()) {
////            // qry.size may not support, so cannot use here
////            // TODO: check if any better way to get the number of items;
////            cnt++;
////            if (outList != nullptr){
////                Saint* item = (Saint*)Saint::build();
////                updateModelFromQuery(item, qry);
////                outList->append(item); // TODO: when it cleaned up?????
////            }
////        }
////    }
////    else {
////        loge( "Failed to execute %s", queryString.toStdString().c_str() );
////    }

//    logi("Found %d", cnt);

//    return cnt;
}

