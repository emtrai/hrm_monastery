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
 * Filename: dbsqlitecommunitytbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "dbsqlitecommunitytbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "defs.h"
#include "dbsqlitetablebuilder.h"
#include "community.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include <QSqlQuery>
#include "logger.h"
#include "errcode.h"
#include "dbpersonmodelhandler.h"
#include "dbsqlite.h"
#include "dbsqliteperson.h"

const qint32 DbSqliteCommunityTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityTbl::DbSqliteCommunityTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommunity, KTableCommunity, KVersionCode)
{}

QList<DbModel *> DbSqliteCommunityTbl::getListCommunitiesInArea(const QString &areaUid, int status)
{
    tracein;
    QList<DbModel *> olist;
    ErrCode ret = ErrNone;
    QHash<QString, int> fields;
    fields.insert(KFieldAreaUid, TEXT);
    // TODO: check status???
    logd("Start search area uid %s", areaUid.toStdString().c_str());
    ret = search(areaUid, fields, &Community::build, &olist, true);
    logd("ret=%d", ret);
    traceout;
    return olist;
}


ErrCode DbSqliteCommunityTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    Community* cmm = (Community*) item;
    // TODO: community code: to be use later???
    // TODO: check if root community exist (via level value), there is only one root community
    builder->addValue(KFieldBrief, cmm->brief());
    builder->addValue(KFieldFullInfo, cmm->fullInfo());
    builder->addValue(KFieldHistory, cmm->history());
    builder->addValue(KFieldMissionUid, cmm->missionUidString());
    builder->addValue(KFieldAddr, cmm->addr());
    builder->addValue(KFieldTel, cmm->tel());
    builder->addValue(KFieldEmail, cmm->email());
    builder->addValue(KFieldCreateDate, cmm->createDate());
    builder->addValue(KFieldCloseDate, cmm->closeDate());
    builder->addValue(KFieldFeastDay, cmm->feastDate());
    builder->addValue(KFieldParentUid, cmm->parentUid());
    builder->addValue(KFieldAreaUid, cmm->areaUid());
    builder->addValue(KFieldAreaDbId, cmm->areaDbId());
    builder->addValue(KFieldModelStatus, (qint32) cmm->getStatus());
    // TODO: update status accordingly???
    builder->addValue(KFieldCountryUid, cmm->countryUid());
    builder->addValue(KFieldChurchAddr, cmm->church());
    builder->addValue(KFieldImgPath, cmm->imgPath());
    builder->addValue(KFieldCEOUid, cmm->currentCEOUid());
    builder->addValue(KFieldContact, cmm->contact());
    // TODO: calculate level basing on parent uid chain
    // Level should be auto-updated, not set by user, which may cause mistake
    builder->addValue(KFieldLevel, cmm->level());

    // TODO: need to search Area Uid basing on Area Code
    // as there is no field of Area Code in community table, just Area UID
    // because code can be changed/update, so should not be used as key

    // TODO: need to search parent community Uid basing on Community Code
    // as there is no field of Parent Community Code in table, just Community UID
    // because code can be changed/update, so should not be used as key

    // TODO: need to search CEO Uid basing on CEO Code
    // as there is no field of CEO Code in table, just UID
    // because code can be changed/update, so should not be used as key

    return ErrNone;
}

ErrCode DbSqliteCommunityTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Community* cmm = (Community*) item;
    cmm->setBrief(qry.value(KFieldBrief).toString());
    cmm->setFullInfo(qry.value(KFieldFullInfo).toString());
    cmm->setHistory(qry.value(KFieldHistory).toString());
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setCloseDate(qry.value(KFieldCloseDate).toInt());
    cmm->setImgPath(qry.value(KFieldImgPath).toString());
    cmm->setLevel(qry.value(KFieldLevel).toInt());
    cmm->setParentUid(qry.value(KFieldParentUid).toString().trimmed());
    if (!cmm->parentUid().isEmpty() && cmm->parentUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCommunityModelHandler()->getByUid(cmm->parentUid());
        if (model) {
            cmm->setParentNameId(model->nameId());
            cmm->setParentName(model->name());
            delete model;
        } else {
            logw("not found parent uid '%s'", STR2CHA(cmm->parentUid()));
        }
    }
    // TODO: parent nameid, parent name???
    cmm->setAreaUid(qry.value(KFieldAreaUid).toString());
    cmm->setAreaDbId(qry.value(KFieldAreaDbId).toInt());
    if (!cmm->areaUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getAreaModelHandler()->getByUid(cmm->areaUid());
        if (model) {
            cmm->setAreaNameId(model->nameId());
            cmm->setAreaName(model->name());
            delete model;
        } else {
            logw("not found area uid '%s'", STR2CHA(cmm->areaUid()));
        }
    }

    cmm->setCountryUid(qry.value(KFieldCountryUid).toString());
    if (!cmm->countryUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCountryModelHandler()->getByUid(cmm->countryUid());
        if (model) {
            cmm->setCountryNameId(model->nameId());
            cmm->setCountryName(model->name());
            delete model;
        } else {
            logw("not found countryUid uid '%s'", STR2CHA(cmm->countryUid()));
        }
    }
    cmm->setChurch(qry.value(KFieldChurchAddr).toString());
    if (qry.value(KFieldAreaName).isValid())
        cmm->setAreaName(qry.value(KFieldAreaName).toString());
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setFeastDate(qry.value(KFieldFeastDay).toInt());
    cmm->setStatus((DbModelStatus)qry.value(KFieldModelStatus).toInt());
    cmm->setCurrentCEOUid(qry.value(KFieldCEOUid).toString().trimmed()); // TODO: check and set name as well
    if (!cmm->currentCEOUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        Person* per = (Person*)SQLITE->getPersonModelHandler()->getByUid(cmm->currentCEOUid());
        if (per) {
            cmm->setCurrentCEOName(per->getFullName());
            delete per;
        }
    }
    cmm->setContact(qry.value(KFieldContact).toString());
    cmm->setMissionUid(qry.value(KFieldMissionUid).toString().trimmed());
    if (!cmm->missionUid().isEmpty()) {
        foreach (QString uid, cmm->missionUid()) {
            DbModel* model = SQLITE->getMissionModelHandler()->getByUid(uid);
            if (model) {
                cmm->addMissionName(model->name());
                cmm->addMissionNameId(model->nameId());
                delete model;
            } else {
                logw("not found mission uid '%s'", STR2CHA(uid));
            }
        }
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler

    }
    traceout;
    return err;
}

QString DbSqliteCommunityTbl::getSearchQueryString(const QString &cond)
{
    tracein;
    QString queryString = QString("SELECT *, %2.%4 AS %7, %2.%5 AS %6 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableArea) // 1 & 2
                              .arg(KFieldAreaUid, KFieldUid) // 3 & 4
                              .arg(KFieldName, KFieldAreaName) // 5 & 6
                              .arg(KFieldAreaUid) // 7
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

ErrCode DbSqliteCommunityTbl::updateTableField(DbSqliteUpdateBuilder *builder, const QList<QString> &updateField, const DbModel *item)
{
    tracein;
    ErrCode err = DbSqliteTbl::updateTableField(builder, updateField, item);
    if (err == ErrNone) {
        Community* comm = (Community*) item;
        foreach (QString field, updateField) {
            logd("Update field %s", STR2CHA(field));
            if (field == KItemFeastDay) {
                builder->addValue(KFieldFeastDay, comm->feastDate());
            } else if (field == KItemArea) {
                builder->addValue(KFieldAreaUid, comm->areaUid());
                builder->addValue(KFieldAreaDbId, comm->areaDbId());
            } else if (field == KItemTel) {
                builder->addValue(KFieldTel, comm->tel());
            } else if (field == KItemAddress) {
                builder->addValue(KFieldAddr, comm->addr());
            } else if (field == KItemEmail) {
                builder->addValue(KFieldEmail, comm->email());
            } else if (field == KItemCountry) {
                builder->addValue(KFieldCountryUid, comm->countryUid());
            } else if (field == KItemChurchAddress) {
                builder->addValue(KFieldChurchAddr, comm->church());
            } else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->getStatus());
            } else if (field == KItemCreateTime) {
                builder->addValue(KFieldCreateDate, comm->createDate());
            } else if (field == KItemParentCommunity) {
                builder->addValue(KFieldParentUid, comm->parentUid());
            } else if (field == KItemContact) {
                builder->addValue(KFieldContact, comm->contact());
            } else if (field == KItemBrief) {
                builder->addValue(KFieldBrief, comm->brief());
            } else {
                logw("Field '%s' not support here", STR2CHA(field));
            }
        }
    }
    traceout;
    return err;
}

DbModelBuilder DbSqliteCommunityTbl::mainModelBuilder()
{
    return &Community::build;
}

void DbSqliteCommunityTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    // TODO: community code: to be use later???
    // TODO: purpose of community code: it's human readable
    // can be changed/updated later
    // can be used as identification which is more dynamic than UID
    // UID: need to fix, must not changed, as it'll impact to many table/field
    builder->addField(KFieldImgPath, TEXT);
    builder->addField(KFieldBrief, TEXT);
    builder->addField(KFieldFullInfo, TEXT);
    builder->addField(KFieldHistory, TEXT);
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldContact, TEXT);
    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldMissionUid, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldCEOUid, TEXT);
    builder->addField(KFieldChurchAddr, TEXT);
    builder->addField(KFieldAreaUid, TEXT);
    builder->addField(KFieldAreaDbId, TEXT);
    builder->addField(KFieldLevel, INT32); // TODO: do we need level here????
    builder->addField(KFieldParentUid, TEXT);
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldCloseDate, INT64);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldDateFormat, TEXT);
    builder->addField(KFieldModelStatus, INT32); // stop, alive, etc.
    builder->addField(KFieldPreset, INT32); // 0: custom, 1: preset (from json)

    // THIS IS IMPORTANT NOTE, DON'T REMOVE IT
    // - ANY UPDATE ON THIS, MUST UPDATE Community::clone() as well
}
