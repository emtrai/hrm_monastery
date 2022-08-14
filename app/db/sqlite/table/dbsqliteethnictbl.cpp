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
    builder->addField(KFieldCountryShortName, TEXT);
    builder->addField(KFieldCountryDbId, INT64); // DB ID
    builder->addField(KFieldRemark, TEXT);
    tracede;
}

void DbSqliteEthnicTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);

    Ethnic* model = (Ethnic*) item;
    builder->addValue(KFieldCountryShortName, model->countryShortName());
    builder->addValue(KFieldCountryDbId, model->countryDbId());
    builder->addValue(KFieldRemark, model->remark());
    tracede;
}

void DbSqliteEthnicTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Ethnic* model = (Ethnic*) item;
    model->setCountryShortName(qry.value(KFieldCountryShortName).toString());
    model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());
    model->setRemark(qry.value(KFieldRemark).toString());
    tracede;
}
