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
 * Filename: dbsqliteethnictbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "dbsqliteethnictbl.h"

#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "ethnic.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteEthnicTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteEthnicTbl::DbSqliteEthnicTbl():
    DbSqliteEthnicTbl(nullptr)
{}
DbSqliteEthnicTbl::DbSqliteEthnicTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableEthnic, KTableEthnic, KVersionCode)
{
    traced;
}

void DbSqliteEthnicTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldCountryDbId, INT64); // DB ID
    tracede;
}

ErrCode DbSqliteEthnicTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);

    Ethnic* model = (Ethnic*) item;
    builder->addValue(KFieldCountryUid, model->countryUid());
    builder->addValue(KFieldCountryDbId, model->countryDbId());
    tracede;
    return ErrNone;
}

void DbSqliteEthnicTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Ethnic* model = (Ethnic*) item;
    model->setCountryUid(qry.value(KFieldCountryUid).toString());
    model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());
    model->setCountryName(qry.value(KFieldCountryName).toString());
    tracede;
}

QString DbSqliteEthnicTbl::getSearchQueryString(const QString &cond)
{
    traced;
    QString queryString = QString("SELECT *, %2.%4 AS %7, %2.%5 AS %6 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableCountry) // 1 & 2
                              .arg(KFieldCountryUid, KFieldUid) // 3 & 4
                              .arg(KFieldName, KFieldCountryName) // 5 & 6
                              .arg(KFieldCountryUid) // 7
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}
