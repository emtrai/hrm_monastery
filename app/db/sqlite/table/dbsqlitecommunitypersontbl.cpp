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
 * Filename: dbsqlitecommunitypersontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitecommunitypersontbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/saintperson.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "dbctl.h"
#include "dbsqlite.h"
#include "communityperson.h"

const qint32 DbSqliteCommunityPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityPersonTbl::DbSqliteCommunityPersonTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableCommPerson, KTableCommPerson, KVersionCode)
{
    tracein;

    mFieldNameUid1 = KFieldCommunityUid;
    mFieldNameDbId1 = KFieldCommunityDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<Person *> DbSqliteCommunityPersonTbl::getListPerson(const QString &communityUid,
                                                           int modelStatus, const QString* perStatusUid)
{
    tracein;
//    DB->openDb();
    QSqlQuery qry (SQLITE->currentDb());
    qint32 cnt = 0;
    QString cond;

    if (communityUid.isEmpty()){
        loge("Invalid community uid");
        return QList<Person*>();
    }
    logi("CommunityUid '%s'", communityUid.toStdString().c_str());
    QString queryString = QString("SELECT *, %2.%3 AS %4, %2.%5 AS %6 FROM %1 LEFT JOIN %2 ON %1.%4 = %2.%3")
                              .arg(name(), KTablePerson) // 1 & 2
                              .arg(KFieldUid, KFieldPersonUid) // 3 & 4
                              .arg(KFieldNameId, KFieldPersonNameId) // 5 & 6
                              ;
    cond = QString("%1.%5 = :uid").arg(name(), KFieldCommunityUid);
    appendModelStatusCond(cond, modelStatus);
    if (!cond.isEmpty()) {
        queryString += " WHERE " + cond;
    }
    queryString += " ORDER BY name ASC";
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", communityUid);
    // TODO: add query status
//    qry.bindValue( ":status", status); // TODO: support multiple status???
    QList<Person *> outList;
    cnt = runQueryT<Person>(qry, &Person::build, outList);

    logi("Found %d", cnt);
    traceout;
    return outList;
}

QList<CommunityPerson *> DbSqliteCommunityPersonTbl::getListCommunityOfPerson(const QString &personUid,
                                                                      int modelStatus)
{
    tracein;
    logd("get community of personUid '%s', status 0x%x", STR2CHA(personUid), modelStatus);
    QList<DbModel*> list = getListItemsUids(nullptr, personUid, &CommunityPerson::build, modelStatus);
    QList<CommunityPerson*> outList;
    logd("found %d", list.size());
    if (list.size() > 0) {
        foreach (DbModel* item, list) {
            if (item) {
                logd("item '%s'", MODELSTR2CHA(item));
                outList.append((CommunityPerson*)item);
            } else {
                logw("something stupid, null item in list!!!");
            }
        }
    } else {
        logi("not found community of per uid '%s' in list", STR2CHA(personUid));
    }
    traceout;
    return outList;
}

QList<CommunityPerson *> DbSqliteCommunityPersonTbl::getListPersonOfCommunity(const QString &community, int modelStatus)
{
    tracein;
    logd("get person of community '%s'", STR2CHA(community));
    QList<DbModel*> list = getListItemsUids(community, nullptr, &CommunityPerson::build, modelStatus);
    QList<CommunityPerson*> outList;
    logd("found %d", list.size());
    if (list.size() > 0) {
        foreach (DbModel* item, list) {
            outList.append((CommunityPerson*)item);
        }
    }
    traceout;
    return outList;
}

QList<DbModel *> DbSqliteCommunityPersonTbl::getListItems(const QString &personUid, const QString &commuid,
                                                                      const DbModelBuilder &builder)
{
    traced;
    return getListItemsUids(commuid, personUid, builder);
}

ErrCode DbSqliteCommunityPersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    DbSqlitePersonTbl* tblPerson = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
    DbModelHandler* commHdl = DB->getCommunityModelHandler();
    err = DbSqliteMapTbl::updateModelFromQuery(item, qry);
    if (err == ErrNone && !item) {
        loge("invalid input model, it's NULL!");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && !tblPerson) {
        err = ErrNoTable;
        loge("not found table person");
    }
    if (err == ErrNone && !commHdl) {
        err = ErrNoHandler;
        loge("not found handler community");
    }
    if (err == ErrNone) {
        if (modelName == KModelNamePerson) {
            logd("update for person model");
            if (tblPerson) {
                tblPerson->updateModelFromQuery(item, qry);
            } else {
                err = ErrNoTable;
                loge("not found table person");
            }
        } else if (modelName == KModelNameCommPerson) {
            CommunityPerson* commPer = (CommunityPerson*)item;
            Person* person = (Person*)Person::build();
            if (person) {
                tblPerson->updateModelFromQuery(person, qry);
                commPer->setPerson(person);
                delete person;
            } else {
                err = ErrNoMemory;
                loge("no memory, cannot allocat person");
            }

            if (err == ErrNone) {
                if (!commPer->communityUid().isEmpty()) {
                    logd("Search community uid '%s'", STR2CHA(commPer->communityUid()));
                    DbModel* model = commHdl->getByUid(commPer->communityUid());
                    if (model) {
                        logd("Found community '%s'", STR2CHA(model->toString()));
                        commPer->setCommunity((Community*)model);
                        delete model;
                    } else {
                        loge("Not found community uid '%s'", STR2CHA(commPer->communityUid()));
                        err = ErrNotFound;
                    }
                } else {
                    loge("Community uid is empty!");
                    err = ErrNoData;
                }
            }
        } else {
            loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
            err = ErrNotSupport;
        }
    }
    traceout;
    return err;
}
