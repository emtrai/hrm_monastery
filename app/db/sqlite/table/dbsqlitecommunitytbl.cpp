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
    traced;
    QList<DbModel *> olist;
    int ret = 0;
    QHash<QString, int> fields;
    fields.insert(KFieldAreaUid, TEXT);
    // TODO: check status???
    logd("Start search area uid %s", areaUid.toStdString().c_str());
    ret = search(areaUid, fields, &Community::build, &olist, true);
    logd("ret=%d", ret);
    tracede;
    return olist;
}


ErrCode DbSqliteCommunityTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    Community* cmm = (Community*) item;
    // TODO: community code: to be use later???
    // TODO: check if root community exist (via level value), there is only one root community
    builder->addValue(KFieldCommunityCode, cmm->communityCode());
    builder->addValue(KFieldAddr, cmm->addr());
    builder->addValue(KFieldTel, cmm->tel());
    builder->addValue(KFieldEmail, cmm->email());
    builder->addValue(KFieldCreateDate, cmm->createDate());
    builder->addValue(KFieldFeastDay, cmm->feastDate());
    builder->addValue(KFieldParentUid, cmm->parentUid());
    builder->addValue(KFieldAreaUid, cmm->areaUid());
    builder->addValue(KFieldAreaDbId, cmm->areaDbId());
    builder->addValue(KFieldStatus, (qint32) cmm->getStatus());
    // TODO: update status accordingly???
    builder->addValue(KFieldCountryUid, cmm->countryUid());
    builder->addValue(KFieldChurchAddr, cmm->church());
    builder->addValue(KFieldImgPath, cmm->imgPath());
    builder->addValue(KFieldCEOUid, cmm->currentCEOUid());
    builder->addValue(KFieldIntro, cmm->intro());
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

void DbSqliteCommunityTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Community* cmm = (Community*) item;
    cmm->setCommunityCode(qry.value(KFieldCommunityCode).toString());
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setImgPath(qry.value(KFieldImgPath).toString());
    cmm->setLevel(qry.value(KFieldLevel).toInt());
    cmm->setParentUid(qry.value(KFieldParentUid).toString());
    cmm->setAreaUid(qry.value(KFieldAreaUid).toString());
    cmm->setAreaDbId(qry.value(KFieldAreaDbId).toInt());
    cmm->setCountryUid(qry.value(KFieldCountryUid).toString());
    cmm->setChurch(qry.value(KFieldChurchAddr).toString());
    if (qry.value(KFieldAreaName).isValid())
        cmm->setAreaName(qry.value(KFieldAreaName).toString());
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setFeastDate(qry.value(KFieldFeastDay).toInt());
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setStatus((CommunityStatus)qry.value(KFieldStatus).toInt());
    cmm->setCurrentCEOUid(qry.value(KFieldCEOUid).toString()); // TODO: check and set name as well
    if (!cmm->currentCEOUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        Person* per = (Person*)SQLITE->getPersonModelHandler()->getByUid(cmm->currentCEOUid());
        if (per) {
            cmm->setCurrentCEO(per->getFullName());
        }
    }
    cmm->setIntro(qry.value(KFieldIntro).toString());
    cmm->setContact(qry.value(KFieldContact).toString());
}

QString DbSqliteCommunityTbl::getSearchQueryString(const QString &cond)
{
    traced;
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

DbModelBuilder DbSqliteCommunityTbl::mainModelBuilder()
{
    return &Community::build;
}

void DbSqliteCommunityTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    // TODO: community code: to be use later???
    // TODO: purpose of community code: it's human readable
    // can be changed/updated later
    // can be used as identification which is more dynamic than UID
    // UID: need to fix, must not changed, as it'll impact to many table/field
    builder->addField(KFieldCommunityCode, TEXT);
    builder->addField(KFieldImgPath, TEXT);
    builder->addField(KFieldIntro, TEXT); // TODO:translation????
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldContact, TEXT);
    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldCEOUid, TEXT);
    builder->addField(KFieldChurchAddr, TEXT);
    builder->addField(KFieldAreaUid, TEXT);
    builder->addField(KFieldAreaDbId, TEXT);
    builder->addField(KFieldLevel, INT32); // TODO: do we need level here????
    builder->addField(KFieldParentUid, TEXT);
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldDateFormat, TEXT);
    builder->addField(KFieldStatus, INT32); // stop, alive, etc.
    builder->addField(KFieldPreset, INT32); // 0: custom, 1: preset (from json)

    // THIS IS IMPORTANT NOTE, DON'T REMOVE IT
    // - ANY UPDATE ON THIS, MUST UPDATE Community::clone() as well
}
