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
#include "dbsqliteupdatebuilder.h"
#include "dbmodel.h"

/**
 * VERSION 0.0.1:
 * + KFieldCountryUid
 * + KFieldCountryDbId
 * VERSION 0.0.2: Add:
 * + KFieldAddress
 * + KFieldEmail
 * + KFieldTel
 * + KFieldModelStatus
 * + KFieldStartDate
 * + KFieldEndDate
 * + KFieldChangeHistory
 */

#define VERSION_CODE_1 VERSION_CODE(0,0,1)
#define VERSION_CODE_2 VERSION_CODE(0,0,2)

const qint32 DbSqliteAreaTbl::KVersionCode = VERSION_CODE_2;

DbSqliteAreaTbl::DbSqliteAreaTbl():
    DbSqliteAreaTbl(nullptr)
{}
DbSqliteAreaTbl::DbSqliteAreaTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableArea, KTableArea, KVersionCode, KModelNameArea)
{
    tracein;
}

void DbSqliteAreaTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCountryUid, TEXT); // DB ID
    builder->addField(KFieldCountryDbId, INT64); // DB ID

    // from 0.0.2
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldModelStatus, INT64);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldChangeHistory, TEXT);
    traceout;
}

ErrCode DbSqliteAreaTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                           const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code

    Area* model = (Area*) item;
    builder->addValue(KFieldCountryUid, model->countryUid());
    builder->addValue(KFieldCountryDbId, model->countryDbId());

    builder->addValue(KFieldAddr, model->addr());
    builder->addValue(KFieldEmail, model->email());
    builder->addValue(KFieldTel, model->tel());
    builder->addValue(KFieldModelStatus, model->modelStatus());
    builder->addValue(KFieldStartDate, model->startDate());
    builder->addValue(KFieldEndDate, model->endDate());
    builder->addValue(KFieldChangeHistory, model->changeHistory());

    traceout;
    return ErrNone;
}

ErrCode DbSqliteAreaTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
        if (item->modelName() == KModelNameArea) {
            Area* model = (Area*) item;
            model->setCountryUid(qry.value(KFieldCountryUid).toString());//TODO: load country obj
            model->setCountryDbId(qry.value(KFieldCountryDbId).toInt());//TODO: load country obj
            if (qry.value(KFieldCountryName).isValid()) {
                model->setCountryName(qry.value(KFieldCountryName).toString());
            }
            if (qry.value(KFieldCountryNameId).isValid()) {
                model->setCountryNameId(qry.value(KFieldCountryNameId).toString());
            }
            model->setAddr(qry.value(KFieldAddr).toString());
            model->setEmail(qry.value(KFieldEmail).toString());
            model->setTel(qry.value(KFieldTel).toString());
            model->setModelStatus(qry.value(KFieldModelStatus).toInt());
            model->setModelStatusName(DbModel::modelStatus2Name((DbModelStatus)model->modelStatus()));

            model->setStartDate(qry.value(KFieldStartDate).toInt());
            model->setEndDate(qry.value(KFieldEndDate).toInt());
            model->setChangeHistory(qry.value(KFieldChangeHistory).toString());
        } else {
            logw("Not support model '%s'", STR2CHA(item->modelName()));
        }
    } else {
        loge("Invalid argument");
        err = ErrInvalidArg;
    }
    traceout;
    return err;
}

QString DbSqliteAreaTbl::getSearchQueryString(const QString &cond)
{
    tracein;
    QString queryString = QString("SELECT *, %2.%5 AS %6, %2.%7 AS %8, %2.%4 AS %9 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableCountry) // 1, 2
                              .arg(KFieldCountryUid, KFieldUid) // 3, 4
                              .arg(KFieldName, KFieldCountryName) // 5, 6
                              .arg(KFieldNameId, KFieldCountryNameId) // 7, 8
                              .arg(KFieldCountryUid) // 9
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    queryString += " ORDER BY name ASC";
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

ErrCode DbSqliteAreaTbl::onTblMigration(qint64 oldVer)
{
    tracein;
    ErrCode err = ErrNone;
    logi("tbl '%s' version upgrade, from version 0x%lx to 0x%lx",
         STR2CHA(mName), oldVer, mVersionCode);
    switch (oldVer) {
        case VERSION_CODE_1:
        {
            QHash<QString, TableFieldDatatype_t> columnField;
            // From version 0.0.2
            columnField.insert(KFieldAddr, TEXT);
            columnField.insert(KFieldEmail, TEXT);
            columnField.insert(KFieldTel, TEXT);
            columnField.insert(KFieldModelStatus, INT64);
            columnField.insert(KFieldStartDate, INT64);
            columnField.insert(KFieldEndDate, INT64);
            columnField.insert(KFieldChangeHistory, TEXT);
            loge("Add column fields");
            err = addTableColumn(columnField);
        }
        break;
        default:
            break;
    };
    traceret(err);
    return err;
}

ErrCode DbSqliteAreaTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{

    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
            err = ErrInvalidArg;
            loge("invalid arg");
    }
    if (err == ErrNone) {
        if (item->modelName() == KModelNameArea) {
            Area* comm = (Area*) item;
            if (field == KItemCountry) {
                builder->addValue(KFieldCountryUid, comm->countryUid());

            } else if (field == KItemAddress) {
                builder->addValue(KFieldAddr, comm->addr());

            } else if (field == KItemEmail) {
                builder->addValue(KFieldEmail, comm->email());

            } else if (field == KItemTel) {
                builder->addValue(KFieldTel, comm->tel());

            } else if (field == KItemChangeHistory) {
                builder->addValue(KFieldChangeHistory, comm->changeHistory());

            } else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->modelStatus());

            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());

            } else if (field == KItemStartDate) {
                builder->addValue(KFieldStartDate, comm->startDate());

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

//ErrCode DbSqliteAreaTbl::updateBuilderFromModel(DbSqliteUpdateBuilder *builder, const QList<QString> &updateField, const DbModel *item)
//{
//    tracein;
//    ErrCode err = ErrNone;
//    if (!builder || !item) {
//            err = ErrInvalidArg;
//            loge("invalid arg");
//    }
//    if (err == ErrNone) {
//            err = DbSqliteTbl::updateBuilderFromModel(builder, updateField, item);
//    }

//    if (err == ErrNone) {
//            if (item->modelName() == KModelNameArea) {
//            Area* comm = (Area*) item;
//            foreach (QString field, updateField) {
//                logd("Update field %s", STR2CHA(field));
//                if (field == KItemCountry) {
//                    builder->addValue(KFieldCountryUid, comm->countryUid());

//                } else if (field == KItemAddress) {
//                    builder->addValue(KFieldAddr, comm->addr());

//                } else if (field == KItemEmail) {
//                    builder->addValue(KFieldEmail, comm->email());

//                } else if (field == KItemTel) {
//                    builder->addValue(KFieldTel, comm->tel());

//                } else if (field == KItemChangeHistory) {
//                    builder->addValue(KFieldChangeHistory, comm->changeHistory());

//                } else if (field == KItemStatus) {
//                    builder->addValue(KFieldModelStatus, comm->modelStatus());

//                } else if (field == KItemEndDate) {
//                    builder->addValue(KFieldEndDate, comm->endDate());

//                } else if (field == KItemStartDate) {
//                    builder->addValue(KFieldStartDate, comm->startDate());

//                } else {
//                    logw("Field '%s' not support here", STR2CHA(field));
//                }
//            }
//            } else {
//            logw("Model name '%s' is no support",
//                 STR2CHA(item->modelName()));
//            }
//    }
//    traceret(err);
//    return err;
//}
