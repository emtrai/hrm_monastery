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

/*
 * SELECT * FROM "mapTblName"
 *  JOIN "modelTblName"
 *  ON "mapTblName"."fieldUid2Join" = "modelTblName"."fieldModelUid"
 *  WHERE "mapTblName"."fieldUid1Cond" = :uid
 */
QList<DbModel *> DbSqliteMapTbl::getListItems(const QString &mapTblName,
                                              const QString &modelTblName,
                                              const QString &fieldUid2Join,
                                              const QString &fieldModelUid,
                                              const QString &fieldUid1Cond,
                                              const DbModelBuilder &builder,
                                              const QString &uid, int status,
                                              const QString& selectedField)
{
    tracein;
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    if (uid.isEmpty()){
        loge("Invalid uid");
        return QList<DbModel*>();
    }
    logi("uid '%s'", uid.toStdString().c_str());
    QString queryString = QString("SELECT %6 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4 WHERE %1.%5 = :uid")
                              .arg(mapTblName, modelTblName)
                              .arg(fieldUid2Join, fieldModelUid, fieldUid1Cond)
                              .arg(selectedField)
        ;

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", uid);
    // TODO: status check???
    QList<DbModel *> outList;
    cnt = runQuery(qry, builder, &outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

QList<DbModel *> DbSqliteMapTbl::getListItemsOfUid2(const QString &uid2, const DbModelBuilder &builder)
{
    tracein;
    //    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    if (uid2.isEmpty()){
        loge("Invalid uid");
        return QList<DbModel*>();
    }
    logi("Get list model of uid2 '%s'", uid2.toStdString().c_str());
    QString queryString = QString("SELECT * FROM %1 WHERE %2 = :uid")
                              .arg(name(), getFieldNameUid2())
        ;

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    logd("Bind uid2='%s'", STR2CHA(uid2));
    qry.bindValue( ":uid", uid2);
    // TODO: status check???
    QList<DbModel *> outList;
    cnt = runQuery(qry, builder, &outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

QList<DbModel *> DbSqliteMapTbl::getListItemsUids(const QString &uid1, const QString &uid2,
                                                  const DbModelBuilder &builder)
{
    tracein;
    //    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    if (uid2.isEmpty()){
        loge("Invalid uid");
        return QList<DbModel*>();
    }
    logi("Get list model of uid1='%s', uid2='%s'", STR2CHA(uid1), STR2CHA(uid2));
    QString queryString = QString("SELECT * FROM %1 WHERE %2 = :uid1 AND %3 = :uid2")
                              .arg(name(), getFieldNameUid1(), getFieldNameUid2())
        ;

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    logd("Bind uid1='%s'", STR2CHA(uid1));
    qry.bindValue( ":uid1", uid1);
    logd("Bind uid2='%s'", STR2CHA(uid2));
    qry.bindValue( ":uid2", uid2);
    // TODO: status check???
    QList<DbModel *> outList;
    cnt = runQuery(qry, builder, &outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

ErrCode DbSqliteMapTbl::updateModelStatus(const QString &uid, int status)
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

ErrCode DbSqliteMapTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateModelFromQuery(item, qry);
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

