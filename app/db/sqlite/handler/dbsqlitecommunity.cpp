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
 * Filename: dbsqlitecommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "dbsqlitecommunity.h"
#include "dbsqlitedefs.h"
#include "table/dbsqlitecommunitytbl.h"
#include "table/dbsqlitecommunitypersontbl.h"
#include "dbsqlite.h"
#include "logger.h"
#include "defs.h"
#include "model/communityperson.h"
#include "person.h"
#include "community.h"
#include "handler/dbsqliteperson.h"
#include "dbpersonmodelhandler.h"

DbSqliteCommunity* DbSqliteCommunity::gInstance = nullptr;

DbSqliteCommunity::DbSqliteCommunity():DbSqliteModelHandler()
{
    traced;
}

DbSqliteTbl *DbSqliteCommunity::getMainTbl()
{
    return (DbSqliteCommunityTbl*)DbSqlite::getInstance()->getTable(KTableCommunity);
}

DbSqliteTbl *DbSqliteCommunity::getTable(const QString &modelName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameCommPerson) {
        tbl = DbSqlite::table(KTableCommPerson);
    } else {
        tbl = getMainTbl();
    }
    tracede;
    return tbl;
}

DbModelBuilder DbSqliteCommunity::getMainBuilder()
{
    return Community::builder;
}

DbSqliteCommunity *DbSqliteCommunity::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new DbSqliteCommunity();
    }

    return gInstance;
}

const QString DbSqliteCommunity::getName()
{
    return KModelHdlCommunity;
}

QList<DbModel *> DbSqliteCommunity::getListPerson(const QString &uid)
{
    traced;
    // Get from commumity&person mapping table, how about community uid info in person tble??
    // TODO: need to sync information with communityuid on person table
    // RISK OF INCONSITANT!!!!!!!
    DbSqliteCommunityPersonTbl* tbl = (DbSqliteCommunityPersonTbl*)DbSqlite::getInstance()
                                          ->getTable(KTableCommPerson);
    return tbl->getListPerson(uid);
}

ErrCode DbSqliteCommunity::addPerson2Community(const Community *comm, const Person *per, int status, qint64 startdate, qint64 enddate, const QString &remark)
{
    traced;
    ErrCode err = ErrNone;
//    DbSqliteCommunityPersonTbl* tbl = (DbSqliteCommunityPersonTbl*)DbSqlite::getInstance()
//                                          ->getTable(KTableCommPerson);
    logd("Build map object");
//    CommunityPerson* model = new CommunityPerson();
    // TODO: err on macro SAVE_MAP_MODEL
    DbPersonModelHandler* hdl = dynamic_cast<DbPersonModelHandler*>(DbSqlite::handler(KModelHdlPerson));
    if (hdl) {
        logd("update community uid of each person first");
        err = hdl->updateCommunity(per->uid(), comm->uid());
    } else {
        err = ErrInvalidData;
        loge("not found handler %s", KModelHdlPerson);
    }
    if (err == ErrNone) {
        logd("Save mapping community and person");
        // TODO: update status of old one?
        SAVE_MAP_MODEL(CommunityPerson, comm, per, status, startdate, enddate, remark);
    }
//    model->setDbId1(comm->dbId());
//    model->setUid1(comm->uid());
//    model->setDbId2(per->dbId());
//    model->setUid2(per->uid());
//    model->setStatus(status);
//    model->setStartDate(startdate);
//    model->setEndDate(enddate);
//    if (!remark.isEmpty())
//        model->setRemark(remark);

//    logd("Add to db");
//    err = model->save();

//    delete model;
//    model = nullptr;
    tracedr(err);
    return err;
}


