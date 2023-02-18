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
 * Filename: dbsqliteareatbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#include "dbsqliteareatbl.h"
#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "area.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteAreaTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteAreaTbl::DbSqliteAreaTbl():
    DbSqliteAreaTbl(nullptr)
{}
DbSqliteAreaTbl::DbSqliteAreaTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableArea, KTableArea, KVersionCode)
{
    traced;
}

void DbSqliteAreaTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldAreaCode, TEXT); // DB ID
    builder->addField(KFieldCountryUid, TEXT); // DB ID
    builder->addField(KFieldCountryDbId, INT64); // DB ID
    builder->addField(KFieldPersonDbId, INT64); // DB ID
    builder->addField(KFieldPersonUid, TEXT); //
    tracede;
}

ErrCode DbSqliteAreaTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code

    Area* model = (Area*) item;
    builder->addValue(KFieldAreaCode, model->areaCode());
    builder->addValue(KFieldCountryUid, model->countryUid());
    builder->addValue(KFieldCountryDbId, model->countryDbId());
    builder->addValue(KFieldPersonDbId, model->personDbId());
    builder->addValue(KFieldPersonUid, model->personUid());
    tracede;
    return ErrNone;
}

void DbSqliteAreaTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Area* model = (Area*) item;
    model->setAreaCode(qry.value(KFieldAreaCode).toString());
    model->setCountryUid(qry.value(KFieldCountryUid).toString());//TODO: load country obj
    model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());//TODO: load country obj
    model->setPersonDbId(qry.value(KFieldPersonDbId).toInt());
    model->setPersonUid(qry.value(KFieldPersonUid).toString());
    if (qry.value(KFieldPersonName).isValid())
        model->setPersonName(qry.value(KFieldPersonName).toString());
    if (qry.value(KFieldCountryName).isValid())
        model->setCountryName(qry.value(KFieldCountryName).toString());

    tracede;
}

QString DbSqliteAreaTbl::getSearchQueryString(const QString &cond)
{
    traced;
    QString queryString = QString("SELECT *, %2.%5 AS %6 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableCountry)
                              .arg(KFieldCountryUid, KFieldUid)
                              .arg(KFieldName, KFieldCountryName);
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}
