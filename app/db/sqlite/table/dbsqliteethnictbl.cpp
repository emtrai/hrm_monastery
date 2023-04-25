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

const qint32 DbSqliteEthnicTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteEthnicTbl::DbSqliteEthnicTbl():
    DbSqliteEthnicTbl(nullptr)
{}
DbSqliteEthnicTbl::DbSqliteEthnicTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableEthnic, KTableEthnic, KVersionCode)
{
    tracein;
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
    DbSqliteTbl::insertTableField(builder, item);

    Ethnic* model = (Ethnic*) item;
    builder->addValue(KFieldCountryUid, model->countryUid());
    builder->addValue(KFieldCountryDbId, model->countryDbId());
    traceout;
    return ErrNone;
}

ErrCode DbSqliteEthnicTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Ethnic* model = (Ethnic*) item;
    model->setCountryUid(qry.value(KFieldCountryUid).toString().trimmed());
    model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());
    model->setCountryName(qry.value(KFieldCountryName).toString().trimmed());
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
    queryString += " ORDER BY name ASC";
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

ErrCode DbSqliteEthnicTbl::updateTableField(DbSqliteUpdateBuilder *builder, const QList<QString> &updateField, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!builder || !item) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateTableField(builder, updateField, item);
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNameEthnic) {
            Ethnic* comm = (Ethnic*) item;
            foreach (QString field, updateField) {
                logd("Update field %s", STR2CHA(field));
                if (field == KItemCountry) {
                    builder->addValue(KFieldCountryUid, comm->countryUid());

                } else {
                    logw("Field '%s' not support here", STR2CHA(field));
                }
            }
        } else {
            logw("Model name '%s' is no support",
                 STR2CHA(item->modelName()));
        }
    }
    traceret(err);
    return err;

}
