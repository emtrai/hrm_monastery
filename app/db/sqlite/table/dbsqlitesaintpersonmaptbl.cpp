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
 * Filename: dbsqlitesaintpersonmaptbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/4/2022
 * Brief:
 */
#include "dbsqlitesaintpersonmaptbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/saintperson.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteSaintPersonMapTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteSaintPersonMapTbl::DbSqliteSaintPersonMapTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableSaintPerson, KTableSaintPerson, KVersionCode,
                  KModelNameSaintPerson)
{
    traced;
}

void DbSqliteSaintPersonMapTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldSaintUid, TEXT);
    builder->addField(KFieldSaintDbId, INT64);
    builder->addField(KFieldPersonUid, TEXT);
    builder->addField(KFieldPersonDbId, INT64);
    traceout;
}

ErrCode DbSqliteSaintPersonMapTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    dbgd("insert table field for model '%s'", MODELSTR2CHA(item));
    ErrCode err = DbSqliteTbl::insertTableField(builder, item);
    if (err == ErrNone && IS_MODEL_NAME(item, KModelNameSaintPerson)) {
        SaintPerson* saint = (SaintPerson*) item;
        builder->addValue(KFieldSaintUid, saint->saintUid());
        builder->addValue(KFieldSaintDbId, saint->saintDbId());
        builder->addValue(KFieldPersonUid, saint->personUid());
        builder->addValue(KFieldPersonDbId, saint->personDbId());
    }
    traceout;
    return err;
}

ErrCode DbSqliteSaintPersonMapTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    dbgd("updateDbModelDataFromQuery model '%s'", MODELSTR2CHA(item));
    err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);

    if (err == ErrNone && IS_MODEL_NAME(item, KModelNameSaintPerson)) {
        SaintPerson* saint = (SaintPerson*) item;
        saint->setSaintDbId(qry.value(KFieldSaintDbId).toInt());
        saint->setSaintUid(qry.value(KFieldSaintUid).toString());
        saint->setPersonDbId(qry.value(KFieldPersonDbId).toInt());
        saint->setPersonUid(qry.value(KFieldPersonUid).toString());
    }
    traceret(err);
    return err;
}

