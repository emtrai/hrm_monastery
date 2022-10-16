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

DbSqliteMapTbl::DbSqliteMapTbl(DbSqlite* db):DbSqliteTbl(db)
{
    traced;
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

void DbSqliteMapTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(getFieldNameUid1(), TEXT);
    builder->addField(getFieldNameDbid1(), INT64);
    builder->addField(getFieldNameUid2(), TEXT);
    builder->addField(getFieldNameDbid2(), INT64);
    builder->addField(KFieldStatus, INT64);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldRemark, TEXT);
    builder->addField(KFieldParentUid, TEXT);
}

void DbSqliteMapTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);
    MapDbModel* model = (MapDbModel*) item;
    builder->addValue(getFieldNameUid1(), model->uid1());
    builder->addValue(getFieldNameDbid1(), model->dbId1());
    builder->addValue(getFieldNameUid2(), model->uid2());
    builder->addValue(getFieldNameDbid2(), model->dbId2());
    builder->addValue(KFieldStatus, model->status());
    builder->addValue(KFieldStartDate, model->startDate());
    builder->addValue(KFieldEndDate, model->endDate());
    builder->addValue(KFieldRemark, model->remark());
    builder->addValue(KFieldParentUid, model->parentUid());
}

void DbSqliteMapTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    logd("update for map model '%s'", item->modelName().toStdString().c_str());
    if (item->modelName() == KModelNameMap)
    {
        logd("update for map model");
        MapDbModel* model = (MapDbModel*) item;
        model->setUid1(qry.value(getFieldNameUid1()).toString());
        model->setDbId1(qry.value(getFieldNameDbid1()).toInt());
        model->setUid2(qry.value(getFieldNameUid2()).toString());
        model->setDbId2(qry.value(getFieldNameDbid2()).toInt());
        model->setStatus(qry.value(KFieldStatus).toInt());
        model->setStartDate(qry.value(KFieldStartDate).toInt());
        model->setEndDate(qry.value(KFieldEndDate).toInt());
        model->setRemark(qry.value(KFieldRemark).toString());
        model->setParentUid(qry.value(KFieldParentUid).toString());
    } else {
        loge("Invalid mapp model '%s', do nothing", item->modelName().toStdString().c_str());
    }
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

