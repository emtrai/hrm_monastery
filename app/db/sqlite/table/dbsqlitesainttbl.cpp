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
#include "dbsqliteupdatebuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "modeldefs.h"

#define VERSION_CODE_1 VERSION_CODE(0,0,1)
#define VERSION_CODE_2 VERSION_CODE(0,0,2)

const qint32 DbSqliteSaintTbl::KVersionCode = VERSION_CODE_2;


DbSqliteSaintTbl::DbSqliteSaintTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableSaint, KTableSaint, KVersionCode,
                  KModelNameSaint)
{
    traced;
}

void DbSqliteSaintTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldFullName, TEXT);
    builder->addField(KFieldOriginName, TEXT);
    builder->addField(KFieldGender, INT32);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldCountry, TEXT);
    // From version 0.0.2
    builder->addField(KFieldCountryUid, TEXT);
    traceout;
}

ErrCode DbSqliteSaintTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    dbgd("insert table field for model '%s'", MODELSTR2CHA(item));
    ErrCode err = DbSqliteTbl::insertTableField(builder, item);
    if (err == ErrNone && IS_MODEL_NAME(item, KModelNameSaint)) {
        Saint* saint = (Saint*) item;
        builder->addValue(KFieldFullName, saint->fullName());
        builder->addValue(KFieldOriginName, saint->originName());
        builder->addValue(KFieldGender, saint->gender());
        builder->addValue(KFieldFeastDay, saint->feastDay());
        builder->addValue(KFieldCountry, saint->country());
        builder->addValue(KFieldCountryUid, saint->countryUid());
    }
    traceout;
    return err;
}

ErrCode DbSqliteSaintTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    dbgd("updateDbModelDataFromQuery model '%s'", MODELSTR2CHA(item));
    ErrCode err = ErrNone;
    err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    if (err == ErrNone && IS_MODEL_NAME(item, KModelNameSaint)) {
        Saint* saint = (Saint*) item;
        saint->setFullName(qry.value(KFieldFullName).toString());
        saint->setOriginName(qry.value(KFieldOriginName).toString());
        saint->setGender((Gender)qry.value(KFieldGender).toInt());
        saint->setFeastDay(qry.value(KFieldFeastDay).toInt());
        saint->setCountry(qry.value(KFieldCountry).toString());
        if (qry.value(KFieldCountryUid).isValid()) {
            saint->setCountryUid(qry.value(KFieldCountryUid).toString());
        }
    }
    traceout;
    return err;
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
    tracein;
    QList<QString> list;// TODO: make as class member?
    list.append(KFieldName);
    list.append(KFieldFullName);
    list.append(KFieldOriginName);
    return list;
}

ErrCode DbSqliteSaintTbl::search(const QString &keyword, QList<DbModel *> *outList,
                             qint64 dbStatus,
                             int from,
                             int noItems,
                             int* total)
{
    traced;
    return DbSqliteTbl::search(keyword, &Saint::build, outList, dbStatus, from, noItems, total);
}

ErrCode DbSqliteSaintTbl::onTblMigration(qint64 oldVer)
{
    tracein;
    ErrCode err = ErrNone;
    logi("tbl '%s' version upgrade, from version 0x%llx to 0x%x",
         STR2CHA(mName), oldVer, mVersionCode);
    switch (oldVer) {
    case VERSION_CODE_1:
    {
        QHash<QString, TableFieldDatatype_t> columnField;
        // From version 0.0.2
        columnField.insert(KFieldCountryUid, TEXT);
        err = addTableColumn(columnField);
    }
    break;
    default:
        logi("skip upgrading from version 0x%llx to 0x%x",
             oldVer, mVersionCode);
        break;
    };
    traceret(err);
    return err;
}

ErrCode DbSqliteSaintTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{

    tracein;
    ErrCode err = ErrNone;
    dbgd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        if (item->modelName() == KModelNameSaint) {
            Saint* comm = (Saint*) item;
            if (field == KItemFeastDay) {
                builder->addValue(KFieldFeastDay, comm->feastDay());

            } else if (field == KItemCountryUid) {
                builder->addValue(KFieldCountryUid, comm->countryUid());

            } else if (field == KItemGender) {
                builder->addValue(KFieldGender, comm->gender());

            } else if (field == KItemFullName) {
                builder->addValue(KFieldFullName, comm->fullName());

            } else if (field == KItemOriginName) {
                builder->addValue(KFieldOriginName, comm->originName());

            } else if (field == KItemCountry) {
                builder->addValue(KFieldCountry, comm->country());

            } else {
                err = DbSqliteTbl::updateBuilderFieldFromModel(builder, field, item);
            }
        } else {
            loge("Model name '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceret(err);
    return err;
}

