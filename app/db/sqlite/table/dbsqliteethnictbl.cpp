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
#include "dbsqliteupdatebuilder.h"
#include "modeldefs.h"

const qint32 DbSqliteEthnicTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteEthnicTbl::DbSqliteEthnicTbl():
    DbSqliteEthnicTbl(nullptr)
{}
DbSqliteEthnicTbl::DbSqliteEthnicTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableEthnic, KTableEthnic, KVersionCode,
                  KModelNameEthnic)
{
    traced;
}

void DbSqliteEthnicTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldCountryDbId, INT64); // DB ID
    traceout;
}

ErrCode DbSqliteEthnicTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::insertTableField(builder, item);
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameEthnic)) {
        loge("not supported model '%s', expect '%s'",
             MODELSTR2CHA(item), KModelNameEthnic);
        err = ErrNotSupport;
    }
    if (err == ErrNone) {
        Ethnic* model = (Ethnic*) item;
        builder->addValue(KFieldCountryUid, model->countryUid());
        builder->addValue(KFieldCountryDbId, model->countryDbId());
    }
    traceout;
    return err;
}

ErrCode DbSqliteEthnicTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameEthnic)) {
        loge("not supported model '%s', expect '%s'",
             MODELSTR2CHA(item), KModelNameEthnic);
        err = ErrNotSupport;
    }
    if (err == ErrNone) {
        Ethnic* model = (Ethnic*) item;
        model->setCountryUid(qry.value(KFieldCountryUid).toString().trimmed());
        model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());
        model->setCountryName(qry.value(KFieldCountryName).toString().trimmed());
    }
    traceout;
    return err;
}

QString DbSqliteEthnicTbl::getSearchQueryString(const QString &cond)
{
    tracein;
    QString queryString = QString("SELECT *, %2.%4 AS %7, %2.%5 AS %6 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableCountry) // 1 & 2
                              .arg(KFieldCountryUid, KFieldUid) // 3 & 4
                              .arg(KFieldName, KFieldCountryName) // 5 & 6
                              .arg(KFieldCountryUid) // 7
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    dbg(LOG_DEBUG, "queryString: %s", STR2CHA(queryString));
    return queryString;
}

ErrCode DbSqliteEthnicTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
                                                       const QString &field,
                                                       const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        if (item->modelName() == KModelNameEthnic) {
            Ethnic* comm = (Ethnic*) item;
            if (field == KItemCountry) {
                builder->addValue(KFieldCountryUid, comm->countryUid());

            } else {
                err = DbSqliteTbl::updateBuilderFieldFromModel(builder, field, item);
            }
        } else {
            loge("Model '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceret(err);
    return err;

}
