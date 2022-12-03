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
 * Filename: dbsqlitespecialistpersontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:11/3/2022
 * Brief:
 */
#include "dbsqlitespecialistpersontbl.h"

#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/specialistperson.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "persondept.h"
#include "table/dbsqlitepersontbl.h"
#include "table/dbsqlitespecialisttbl.h"
#include "specialist.h"
#include "dbctl.h"

const qint32 DbSqliteSpecialistPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteSpecialistPersonTbl::DbSqliteSpecialistPersonTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableSpecialistPerson, KTableSpecialistPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldPersonUid;
    mFieldNameDbId1 = KFieldPersonDbId;
    mFieldNameUid2 = KFieldSpecialistUid;
    mFieldNameDbId2 = KFieldSpecialistDbId;
}

DbSqliteSpecialistPersonTbl::DbSqliteSpecialistPersonTbl(DbSqlite *db,
                                                         const QString &baseName,
                                                         const QString &name,
                                                         qint32 versionCode)
    : DbSqliteMapTbl(db, baseName, name, versionCode)
{
    traced;
    mFieldNameUid1 = KFieldPersonUid;
    mFieldNameDbId1 = KFieldPersonDbId;
    mFieldNameUid2 = KFieldSpecialistUid;
    mFieldNameDbId2 = KFieldSpecialistDbId;
}

QList<DbModel *> DbSqliteSpecialistPersonTbl::getListPerson(
    const QString &specialistUid, int status)
{
    traced;
    QSqlQuery qry;
    qint32 cnt = 0;
    if (specialistUid.isEmpty()){
        loge("Invalid specialistUid uid");
        return QList<DbModel*>();
    }
    logi("specialistUid '%s'", specialistUid.toStdString().c_str());
    /*
     * SELECT * FROM "mapTblName"
     *  JOIN "modelTblName"
     *  ON "mapTblName"."fieldUid2Join" = "modelTblName"."fieldModelUid"
     *  WHERE "mapTblName"."fieldUid1Cond" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItems(name(), /*mapTblName*/
                                                         KTablePerson, /*modelTblName*/
                                                         KFieldPersonUid,/*fieldUid2Join*/
                                                         KFieldUid,/*fieldModelUid*/
                                                         KFieldSpecialistUid,/*fieldUid1Cond*/
                                                         &Person::build, /*builder*/
                                                         specialistUid, /*uid*/
                                                         status
                                                         );

    tracede;
    return list;
}

QList<DbModel *> DbSqliteSpecialistPersonTbl::getListSpecialist(const QString &personUid, int status)
{
    traced;
    QSqlQuery qry;
    qint32 cnt = 0;
    if (personUid.isEmpty()){
        loge("Invalid personUid uid");
        return QList<DbModel*>();
    }
    logi("personUid '%s'", personUid.toStdString().c_str());
    /*
     * SELECT * FROM "mapTblName"
     *  JOIN "modelTblName"
     *  ON "mapTblName"."fieldUid2Join" = "modelTblName"."fieldModelUid"
     *  WHERE "mapTblName"."fieldUid1Cond" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItems(name(), /*mapTblName*/
                                                         KTableSpecialist, /*modelTblName*/
                                                         KFieldSpecialistUid,/*fieldUid2Join*/
                                                         KFieldUid,/*fieldModelUid*/
                                                         KFieldPersonUid,/*fieldUid1Cond*/
                                                         &Specialist::builder, /*builder*/
                                                         personUid, /*uid*/
                                                         status,
                                                         QString("*, %1.%2 AS %3, %1.%4 AS %5")
                                                             .arg(KTableSpecialist, KFieldName)
                                                             .arg(KFieldSpecialistName)
                                                             .arg(KFieldUid)
                                                             .arg(KFieldSpecialistUid)
                                                         );

    tracede;
    return list;
}


void DbSqliteSpecialistPersonTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldExperienceHistory, TEXT);
    tracede;
}

ErrCode DbSqliteSpecialistPersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    ErrCode ret = ErrNone;
    QString modelName = item->modelName();
    logd("model name to insert '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameSpecialistPerson ) {
        ret = DbSqliteMapTbl::insertTableField(builder, item);
        SpecialistPerson* model = (SpecialistPerson*) item;
        builder->addValue(KFieldExperienceHistory, model->experienceHistory());
    } else {
        ret = ErrInvalidArg;
        loge("Invali model name '%s'", modelName.toStdString().c_str());
    }
    tracedr(ret);
    return ret;
}

void DbSqliteSpecialistPersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePerson )
    {
        logd("update for person model");
        DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
        tbl->updateModelFromQuery(item, qry);
    } else if (modelName == KModelNameSpecialist) {
        logd("update for specialist model");
        DbSqliteSpecialistTbl* tbl = dynamic_cast<DbSqliteSpecialistTbl*>(DbSqlite::table(KTableSpecialist));
        tbl->updateModelFromQuery(item, qry);
    } else if (modelName == KModelNameSpecialistPerson) {
        logd("update for specialist person model");
        DbSqliteMapTbl::updateModelFromQuery(item, qry);
        SpecialistPerson* model = (SpecialistPerson*) item;
        model->setExperienceHistory(qry.value(KFieldExperienceHistory).toString());
    } else {
        loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
    }
    tracede;
}
