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
 * Filename: dbsqlitecountrytbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/12/2022
 * Brief:
 */
#include "dbsqlitecountrytbl.h"


#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "country.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteCountryTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCountryTbl::DbSqliteCountryTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCountry, KTableCountry, KVersionCode)
{
    traced;
}

void DbSqliteCountryTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldRegion, TEXT);
    builder->addField(KFieldShortName, TEXT);
    builder->addField(KFieldContinent, TEXT);
}

ErrCode DbSqliteCountryTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code

    Country* model = (Country*) item;
    // TODO: check if shortname exist???
    builder->addValue(KFieldRegion, model->region());
    builder->addValue(KFieldShortName, model->shortName());
    builder->addValue(KFieldContinent, model->continent());
    return ErrNone;
}

void DbSqliteCountryTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Country* model = (Country*) item;
    model->setRegion(qry.value(KFieldRegion).toString());
    model->setShortName(qry.value(KFieldShortName).toString());
    model->setContinent(qry.value(KFieldContinent).toString());
}

