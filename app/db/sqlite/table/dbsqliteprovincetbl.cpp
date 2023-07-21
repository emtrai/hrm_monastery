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
 * Filename: dbsqliteprovincetbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "dbsqliteprovincetbl.h"

#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "province.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteProvinceTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteProvinceTbl::DbSqliteProvinceTbl():
    DbSqliteProvinceTbl(nullptr)
{}
DbSqliteProvinceTbl::DbSqliteProvinceTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableProvince, KTableProvince, KVersionCode)
{
    tracein;
}

void DbSqliteProvinceTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldCountryDbId, INT64); // DB ID
    builder->addField(KFieldParentDbId, INT64); // DB ID
    builder->addField(KFieldParentUid, TEXT); // DB ID
    traceout;
}

ErrCode DbSqliteProvinceTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item);

    Province* model = (Province*) item;
    builder->addValue(KFieldCountryUid, model->countryUid());
    builder->addValue(KFieldCountryDbId, model->countryDbId());
    builder->addValue(KFieldParentDbId, model->parentDbId());
    builder->addValue(KFieldParentUid, model->parentUid());
    traceout;
    return ErrNone;
}

ErrCode DbSqliteProvinceTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    Province* model = (Province*) item;
    model->setCountryUid(qry.value(KFieldCountryUid).toString());
    model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());
    model->setParentDbId(qry.value(KFieldParentDbId).toInt());
    model->setParentUid(qry.value(KFieldParentUid).toString());
    traceout;
    return err;
}
