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

#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbctl.h"
#include "dbsqlite.h"


const qint32 DbSqliteDepartTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteDepartTbl::DbSqliteDepartTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableDepartment, KTableDepartment, KVersionCode)
{
    traced;
}


void DbSqliteDepartTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    // TODO: check again if it maps with json
    builder->addField(KFieldParentDbId, INT64);
    builder->addField(KFieldParentUid, TEXT);
    builder->addField(KFieldCommunityDbId, INT64);
    builder->addField(KFieldCommunityUid, TEXT);
    builder->addField(KFieldPersonId, INT64); // DB ID
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldBrief, TEXT);
    builder->addField(KFieldRemark, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldStatus, INT32);//status of dept (active, closed, etc.)
    tracede;
}

ErrCode DbSqliteDepartTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                       const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code

    Department* model = (Department*) item;
    builder->addValue(KFieldShortName, model->shortName());
    builder->addValue(KFieldBrief, model->brief());
    builder->addValue(KFieldAddr, model->addr());
    builder->addValue(KFieldEmail, model->email());
    builder->addValue(KFieldTel, model->tel());
    builder->addValue(KFieldPersonId, model->hoDPersonId());
    builder->addValue(KFieldRemark, model->remark());
    builder->addValue(KFieldCommunityDbId, model->communityDbId());
    builder->addValue(KFieldCommunityUid, model->communityUid());
    tracede;
    return ErrNone;
}

void DbSqliteDepartTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Department* model = (Department*) item;
    model->setBrief(qry.value(KFieldBrief).toString());//TODO: load country obj
    model->setCommunityDbId(qry.value(KFieldCommunityDbId).toInt());//TODO: load country obj
    model->setCommunityUid(qry.value(KFieldCommunityUid).toString());//TODO: load country obj
    model->setCommunityName(qry.value(KFieldCommunityName).toString());//TODO: load country obj
    model->setHoDPersonId(qry.value(KFieldPersonId).toInt());
    model->setRemark(qry.value(KFieldRemark).toString());
    tracede;
}

QSqlQuery *DbSqliteDepartTbl::getAllQuery()
{
    traced;
//    DB->openDb();
    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());

    traced;
    // TODO: check record status????
    QString queryString = QString("SELECT *, %2.%5 AS %6 FROM %1 JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableCommunity)
                              .arg(KFieldCommunityUid, KFieldUid)
                              .arg(KFieldName, KFieldCommunityName);
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    return qry;
}
