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
 * Filename: dbsqlitemaptbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitemaptbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include "model/mapdbmodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "dbdefs.h"
#include "dbctl.h"
#include "dbsqlite.h"
#include "exception.h"

DbSqliteMapTbl::DbSqliteMapTbl(DbSqlite* db):DbSqliteTbl(db)
{
    tracein;
}

DbSqliteMapTbl::DbSqliteMapTbl(DbSqlite *db,
                               const QString &baseName,
                               const QString &name,
                               qint32 versionCode):
    DbSqliteTbl(db, baseName, name, versionCode)
{
    mFieldNameUid1 = "UID1";
    mFieldNameUid2 = "UID2";
    mFieldNameDbId1 = "DBID1";
    mFieldNameDbId2 = "DBID2";
}

DbSqliteMapTbl::DbSqliteMapTbl(DbSqlite *db, const QString &baseName,
                               const QString &name, qint32 versionCode,
                               const QString &modelName):
    DbSqliteMapTbl(db, baseName, name, versionCode)
{
    mHandleModelName = modelName;
}

/**
 * SELECT * FROM "mapTblName"
 *  JOIN "modelTblName"
 *  ON "mapTblName"."fieldUid2Join" = "modelTblName"."fieldModelUid"
 *  WHERE "mapTblName"."fieldUid1Cond" = :uid
 */
QList<DbModel *> DbSqliteMapTbl::getListItemsWithUid(const QString &mapTblName,
                                              const QString &modelTblName,
                                              const QString &fieldUid2Join,
                                              const QString &fieldModelUid,
                                              const QString &fieldUid1Cond,
                                              const DbModelBuilder &builder,
                                              const QString &uid, int status,
                                              const QString& selectedField)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    QString cond;
    QList<DbModel *> outList;
    if (uid.isEmpty()){
        loge("Invalid uid");
        return QList<DbModel*>();
    }
    logd("uid '%s'", STR2CHA(uid));
    if (!uid.isEmpty()) {
        cond = QString("%1.%2 = '%3'").arg(mapTblName, fieldUid1Cond,uid);
    } else {
        cond = QString("%1.%2 IS NULL OR %2 = ''").arg(mapTblName, fieldUid1Cond);
    }
    logd("cond '%s'", STR2CHA(cond));
    outList = getListItemsWithCond(mapTblName, modelTblName, fieldUid2Join, fieldModelUid, builder,
                           cond, status, selectedField);
    traceout;
    return outList;
}

QString DbSqliteMapTbl::getListItemsQueryString(const QString &mapTblName,
                                                const QString &modelTblName,
                                                const QString &fieldUid2Join,
                                                const QString &fieldModelUid,
                                                const QString &cond,
                                                int status,
                                                const QString &selectedField)
{
    tracein;
    QString condStr = cond;
    logd("cond '%s'", STR2CHA(cond));
    QString queryString = QString("SELECT %5 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(mapTblName, modelTblName) // 1 & 2
                              .arg(fieldUid2Join, fieldModelUid) // 3 & 4
                              .arg(selectedField) // 5
        ;
    appendModelStatusCond(condStr, status);
    logd("condition String '%s'", STR2CHA(condStr));
    if (!condStr.isEmpty()) {
        queryString += " WHERE " + condStr;
    }
    logd("Query String '%s'", STR2CHA(queryString));
    traceout;
    return queryString;
}

QList<DbModel *> DbSqliteMapTbl::getListItemsWithCond(const QString &mapTblName,
                                              const QString &modelTblName,
                                              const QString &fieldUid2Join,
                                              const QString &fieldModelUid,
                                              const DbModelBuilder &builder,
                                              const QString &cond,
                                              int status,
                                              const QString &selectedField)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    QString condStr = cond;
    logd("cond '%s'", STR2CHA(cond));
    QString queryString = getListItemsQueryString(mapTblName, modelTblName,
                                                  fieldUid2Join, fieldModelUid,
                                                  cond, status, selectedField);
    dbg(LOG_DEBUG, "Query String '%s'", STR2CHA(queryString));
    qry.prepare(queryString);

    // TODO: status check???
    QList<DbModel *> outList;
    cnt = runQuery(qry, builder, &outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

QList<DbModel *> DbSqliteMapTbl::getListItemsUids(const QString &uid1, const QString &uid2,
                                                  const DbModelBuilder &builder,
                                                  int modelStatus)
{
    tracein;
    //    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    QString uid1Cond;
    QString uid2Cond;
    QString cond;
    logi("Get list model of uid1='%s', uid2='%s'", STR2CHA(uid1), STR2CHA(uid2));
    if (!uid1.isEmpty()){
        uid1Cond = QString(":uid1 = %1").arg(getFieldNameUid1());
    } else {
        uid1Cond = "1";
    }
    if (!uid2.isEmpty()){
        uid2Cond = QString(":uid2 = %1").arg(getFieldNameUid2());
    } else {
        uid2Cond = "1";
    }
    logi("uid1Cond='%s', uid2Cond='%s'", STR2CHA(uid1Cond), STR2CHA(uid2Cond));

    cond = QString("%1 AND %2").arg(uid1Cond, uid2Cond);
    appendModelStatusCond(cond, modelStatus);
    logi("cond='%s'", STR2CHA(cond));
    QString queryString = QString("SELECT * FROM %1 WHERE %2 ORDER BY NAME ASC")
                              .arg(name(), cond)
        ;

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    if (!uid1.isEmpty()){
        logd("Bind uid1='%s'", STR2CHA(uid1));
        qry.bindValue( ":uid1", uid1);
    }
    if (!uid2.isEmpty()){
        logd("Bind uid2='%s'", STR2CHA(uid2));
        qry.bindValue( ":uid2", uid2);
    }
    // TODO: status check???
    QList<DbModel *> outList;
    cnt = runQuery(qry, builder, &outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

ErrCode DbSqliteMapTbl::updateModelStatusInDb(const QString &uid, int status)
{
    tracein;
    ErrCode err = ErrNone;
    QHash<QString, QString> updateField;
    logd("update model status %d for uid '%s'", status, STR2CHA(uid));
    updateField.insert(KFieldModelStatus, QString::number(status));
    err = update(uid, updateField);
    traceret(err);
    return err;
}

void DbSqliteMapTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(getFieldNameUid1(), TEXT);
    builder->addField(getFieldNameDbid1(), INT64);
    builder->addField(getFieldNameUid2(), TEXT);
    builder->addField(getFieldNameDbid2(), INT64);
    builder->addField(KFieldModelStatus, INT64);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldChangeHistory, TEXT);
    builder->addField(KFieldParentUid, TEXT);
}

ErrCode DbSqliteMapTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode ret = ErrNone;
    ret = DbSqliteTbl::insertTableField(builder, item);
    if (ret == ErrNone) {
        MapDbModel* model = (MapDbModel*) item;
        builder->addValue(getFieldNameUid1(), model->uid1());
        builder->addValue(getFieldNameDbid1(), model->dbId1());
        builder->addValue(getFieldNameUid2(), model->uid2());
        builder->addValue(getFieldNameDbid2(), model->dbId2());
        builder->addValue(KFieldModelStatus, model->modelStatus());
        builder->addValue(KFieldStartDate, model->startDate());
        builder->addValue(KFieldEndDate, model->endDate());
        builder->addValue(KFieldChangeHistory, model->changeHistory());
        builder->addValue(KFieldParentUid, model->parentUid());
        // TODO: implement change history by event, i.e. IN:<datetime>, OUT:<datetime.
    }
    traceret(ret);
    return ret;
}

ErrCode DbSqliteMapTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    }
    if (err == ErrNone) {
        logd("update for map model '%s'", item->modelName().toStdString().c_str());
        if (item->modelType() == MODEL_MAP) {
            logd("update for map model");
            MapDbModel* model = (MapDbModel*) item;
            model->setUid1(qry.value(getFieldNameUid1()).toString());
            model->setDbId1(qry.value(getFieldNameDbid1()).toInt());
            model->setUid2(qry.value(getFieldNameUid2()).toString());
            model->setDbId2(qry.value(getFieldNameDbid2()).toInt());
            model->setModelStatus(qry.value(KFieldModelStatus).toInt());
            model->setModelStatusName(DbModel::modelStatus2Name((DbModelStatus)model->modelStatus()));

            model->setStartDate(qry.value(KFieldStartDate).toInt());
            model->setEndDate(qry.value(KFieldEndDate).toInt());
            model->setChangeHistory(qry.value(KFieldChangeHistory).toString());
            model->setParentUid(qry.value(KFieldParentUid).toString());
        } else {
            loge("Invalid mapp model '%s', type %d, do nothing",
                 STR2CHA(item->modelName()), item->modelType());
//            err = ErrInvalidData;
        }
    }
    traceret(err);
    return err;
}

QHash<QString, QString> DbSqliteMapTbl::getFieldsCheckExists(const DbModel *item)
{
    tracein;
    QHash<QString, QString> list;
    if (item) {
        QString modelName = item->modelName();
        int modelType = item->modelType();
        logd("modelName %s", STR2CHA(modelName));
        logd("modelType %d", modelType);
        if (modelType == MODEL_MAP) {
            const MapDbModel* model = (MapDbModel*)item;
            // TODO: make as class member?

            list[getFieldNameUid1()] = model->uid1();
            list[getFieldNameUid2()] = model->uid2();
            list[KFieldStartDate] = QString("%1").arg(model->startDate());
            list[KFieldEndDate] = QString("%1").arg(model->endDate());
            list[KFieldModelStatus] = QString("%1").arg(model->modelStatus());
        } else {
            THROWEX("Invalid modelName %s", STR2CHA(modelName));
        }
    } else {
        THROWEX("Invalid parameter");
    }

    return list;
}

const QString& DbSqliteMapTbl::getFieldNameUid1() const
{
    return mFieldNameUid1;
}

const QString& DbSqliteMapTbl::getFieldNameDbid1() const
{
    return mFieldNameDbId1;
}

const QString& DbSqliteMapTbl::getFieldNameUid2() const
{
    return mFieldNameUid2;
}

const QString& DbSqliteMapTbl::getFieldNameDbid2() const
{
    return mFieldNameDbId2;

}

ErrCode DbSqliteMapTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
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
        if (item->modelType() == MODEL_MAP) {
            MapDbModel* comm = (MapDbModel*) item;
            if (field == KItemChangeHistory) {
                builder->addValue(KFieldChangeHistory, comm->changeHistory());
            }else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->modelStatus());
            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());
            } else if (field == KItemStartDate) {
                builder->addValue(KFieldStartDate, comm->startDate());
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

ErrCode DbSqliteMapTbl::filterFieldCond(int fieldId, int operatorId,
                                        QString fieldValueName, const DbModel *parentModel,
                                        QString &cond, int &dataType, bool &isExact)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::filterFieldCond(fieldId, operatorId, fieldValueName,
                                       parentModel, cond, dataType, isExact);
    if (err == ErrNone && parentModel && !parentModel->uid().isEmpty()) {
        logd("Append uid '%s'", STR2CHA(parentModel->uid()));
        cond = QString("((%1) AND (%2.%3 = '%4'))").arg(cond, name(), getFieldNameUid1(), parentModel->uid());
    }
    logd("cond: '%s'", STR2CHA(cond));
    traceret(err);
    return err;
}

ErrCode DbSqliteMapTbl::filterFieldCond(const QList<FilterKeyworkItem *> &filters,
                                        QString &cond, QHash<QString, QString> &bindValues,
                                        const DbModel *parentModel)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::filterFieldCond(filters, cond, bindValues, nullptr);
    if (err == ErrNone && parentModel && !parentModel->uid().isEmpty()) {
        logd("Append uid '%s'", STR2CHA(parentModel->uid()));
        cond = QString("((%1) AND (%2.%3 = '%4'))").arg(cond, name(), getFieldNameUid1(), parentModel->uid());
    }
    logd("cond: '%s'", STR2CHA(cond));
    traceret(err);
    return err;
}

