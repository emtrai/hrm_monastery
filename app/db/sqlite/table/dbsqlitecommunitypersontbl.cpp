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
#include "filter.h"

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
                                                           int modelStatus,
                                                          const QString* perStatusUid)
{
    tracein;
    QSqlQuery qry (SQLITE->currentDb());
    qint32 cnt = 0;
    QString cond;

    if (communityUid.isEmpty()){
        loge("Invalid community uid");
        return QList<Person*>();
    }
    logi("CommunityUid '%s'", communityUid.toStdString().c_str());
    QString queryString = QString(" SELECT *, %2.%3 AS %4, %2.%5 AS %6"
                                  " FROM %1"
                                  " LEFT JOIN %2"
                                  " ON %1.%4 = %2.%3")
                              .arg(name(), KTablePerson) // 1 & 2
                              .arg(KFieldUid, KFieldPersonUid) // 3 & 4
                              .arg(KFieldNameId, KFieldPersonNameId) // 5 & 6
                              ;
    cond = QString("%1.%2 = :uid").arg(name(), KFieldCommunityUid);
    if (perStatusUid) {
        if (!cond.isEmpty())
            cond += " AND ";
        cond += QString("%1.%2 = :perstatus").arg(KTablePerson, KFieldPersonStatusUid);
    }
    appendModelStatusCond(cond, modelStatus);
    if (!cond.isEmpty()) {
        queryString += " WHERE " + cond;
    }
    queryString += " ORDER BY name ASC";
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", communityUid);
    logd("bind uid '%s'", STR2CHA(communityUid));
    if (perStatusUid) {
        qry.bindValue( ":perstatus", *perStatusUid);
        logd("bind uid '%s'", STR2CHA(*perStatusUid));
    }

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

//QList<CommunityPerson *> DbSqliteCommunityPersonTbl::getListCommunityPerson(
//                                        const QString &communityUid, int modelStatus)
//{
//    tracein;
//    QSqlQuery qry(SQLITE->currentDb());
//    qint32 cnt = 0;
//    QString cond;
//    logd("Get list model of communityUid='%s'", STR2CHA(communityUid));
//    if (!communityUid.isEmpty()){
//        cond = QString("%1.%2 = :communityUid").arg(name(), getFieldNameUid1());
//    } else {
//        cond = "1";
//    }
//    appendModelStatusCond(cond, modelStatus);
//    logi("cond='%s'", STR2CHA(cond));
//    QString queryString = QString("SELECT *, %2.%5 AS %6, %2.%7 AS %8, "
//                                  "%2.%9 AS %10, %2.%11 AS %12 "
//                                  "FROM %1 LEFT JOIN %2 ON %1.%4 = %2.%5 "
//                                  "WHERE %3 ORDER BY NAME ASC")
//                                .arg(name(), KTablePerson, cond) // 1, 2, 3
//                                .arg(getFieldNameUid2(), KFieldUid, KFieldPersonUid) // 4, 5, 6
//                                .arg(KFieldNameId, KFieldPersonNameId) // 7, 8
//                                .arg(KFieldId, KFieldPersonDbId) // 9, 10
//                                .arg(KFieldRemark, KFieldPersonRemark) // 11, 12
//        ;

//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());

//    // TODO: check sql injection issue
//    if (!communityUid.isEmpty()){
//        logd("Bind communityUid='%s'", STR2CHA(communityUid));
//        qry.bindValue( ":communityUid", communityUid);
//    }
//    // TODO: status check???
//    QList<DbModel *> items;
//    QList<CommunityPerson *> outList;
//    cnt = runQuery(qry, &CommunityPerson::build, &items);

//    logi("Found %d", cnt);
//    if (cnt > 0) {
//        outList = CLONE_LIST_FROM_DBMODEL(items, CommunityPerson);
//    }
//    RELEASE_LIST_DBMODEL(items);
//    traceout;
//    return outList;
//}

QList<DbModel *> DbSqliteCommunityPersonTbl::getListCommunityPerson(
    const QString &communityUid, int modelStatus)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    QString cond;
    logd("Get list model of communityUid='%s'", STR2CHA(communityUid));
    if (!communityUid.isEmpty()){
        cond = QString("%1.%2 = :communityUid").arg(name(), getFieldNameUid1());
    } else {
        cond = "1";
    }
    appendModelStatusCond(cond, modelStatus);
    logi("cond='%s'", STR2CHA(cond));
    QString queryString = QString("SELECT * FROM %1 "
                                  "WHERE %2")
                              .arg(name(), cond) // 1, 2
        ;

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    if (!communityUid.isEmpty()){
        logd("Bind communityUid='%s'", STR2CHA(communityUid));
        qry.bindValue( ":communityUid", communityUid);
    }
    // TODO: status check???
    QList<DbModel *> items;
//    QList<CommunityPerson *> outList;
    cnt = runQuery(qry, &CommunityPerson::build, &items);

    logi("Found %d", cnt);
//    if (cnt > 0) {
//        outList = CLONE_LIST_FROM_DBMODEL(items, CommunityPerson);
//    }
//    RELEASE_LIST_DBMODEL(items);
    traceout;
    return items;
}
QList<DbModel *> DbSqliteCommunityPersonTbl::getListCommunityPerson(const QString &personUid,
                                                                    const QString &commuid,
                                                                    int modelStatus)
{
    traced;
    return getListItemsUids(commuid, personUid,  &CommunityPerson::build, modelStatus);
}

ErrCode DbSqliteCommunityPersonTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    DbSqlitePersonTbl* tblPerson = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
    DbModelHandler* commHdl = DB->getCommunityModelHandler();
    DbModelHandler* personHdl = DB->getPersonModelHandler();
    err = DbSqliteMapTbl::updateDbModelDataFromQuery(item, qry);
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
    if (err == ErrNone && !personHdl) {
        err = ErrNoHandler;
        loge("not found handler personHdl");
    }
    if (err == ErrNone) {
        if (modelName == KModelNamePerson) {
            logd("update for person model");
            if (tblPerson) {
                tblPerson->updateDbModelDataFromQuery(item, qry);
            } else {
                err = ErrNoTable;
                loge("not found table person");
            }
        } else if (modelName == KModelNameCommPerson) {
            CommunityPerson* commPer = (CommunityPerson*)item;
//            Person* person = (Person*)Person::build();
//            if (person) {
//                tblPerson->updateDbModelDataFromQuery(person, qry);
//                commPer->setPerson(person);
//                delete person;
//            } else {
//                err = ErrNoMemory;
//                loge("no memory, cannot allocat person");
//            }

            if (!commPer->personUid().isEmpty()) {
                logd("Search person uid '%s'", STR2CHA(commPer->personUid()));
                DbModel* model = personHdl->getByUid(commPer->personUid());
                if (model) {
                    logd("Found person '%s'", STR2CHA(model->toString()));
                    commPer->setPerson((Person*)model);
                    delete model;
                } else {
                    loge("Not found person uid '%s'", STR2CHA(commPer->personUid()));
                    err = ErrNotFound;
                }
            } else {
                loge("Person uid is empty!");
                err = ErrNoData;
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

QString DbSqliteCommunityPersonTbl::getFilterQueryString(int fieldId, const QString &cond)
{
    tracein;
    QString queryString;
    logd("fieldId %d", fieldId);
    if (fieldId == FILTER_FIELD_FULL_NAME) {

        queryString = QString(  " SELECT *, "
                                " (%2.%6 || ' ' || %2.%7) AS %8, "
                                " %2.%9 AS %9a"
                                " FROM %1 "
                                " LEFT JOIN %2 ON %1.%4 = %2.%5"
                                " WHERE (%3)")
                          .arg(name(), KTablePerson) // 1, 2
                          .arg(cond) // 3
                          .arg(KFieldPersonUid, KFieldUid) // 4, 5
                          .arg(KFieldLastName, KFieldFirstName) // 6, 7
                          .arg(KFieldFullName) // 8
                          .arg(KFieldCommunityUid) // 9

            ;
    } else {
        logd("call default filer query for fieldId %d", fieldId);
        queryString = DbSqliteMapTbl::getFilterQueryString(fieldId, cond);
    }
    logd("queryString '%s'", STR2CHA(queryString));
    traceout;
    return queryString;
}

QString DbSqliteCommunityPersonTbl::getFilterQueryString(const QList<FilterKeyworkItem *> &filters,
                                                         const QString &cond)
{
    tracein;
    QString queryString;
    queryString = QString(  " SELECT *, "
                          " (%2.%6 || ' ' || %2.%7) AS %8, "
                          " %2.%9 AS %9a"
                          " FROM %1 "
                          " LEFT JOIN %2 ON %1.%4 = %2.%5"
                          " WHERE (%3)")
                      .arg(name()) // 1
                      .arg(KTablePerson) // 2
                      .arg(cond) // 3
                      .arg(KFieldPersonUid) // 4
                      .arg(KFieldUid) // 5
                      .arg(KFieldLastName) // 6
                      .arg(KFieldFirstName) // 7
                      .arg(KFieldFullName) // 8
                      .arg(KFieldCommunityUid) // 9
        ;
    logd("queryString '%s'", STR2CHA(queryString));
    traceout;
    return queryString;
}

//ErrCode DbSqliteCommunityPersonTbl::filterFieldCond(int fieldId, int operatorId,
//                                                    QString fieldValueName,
//                                                    const DbModel *parentModel,
//                                                    QString &cond, int &dataType,
//                                                    bool &isExact)
//{
//    tracein;
//    ErrCode err = ErrNone;
//    if (fieldId == FILTER_FIELD_FULL_NAME) {
//        queryString = QString("SELECT * LEFT JOIN %1 ON WHERE %1").arg()
//            ;
//    } else {
//        err = DbSqliteMapTbl::filterFieldCond(fieldId, operatorId, fieldValueName,
//                                              parentModel, cond, dataType, isExact);
//    }
//    traceret(err);
//    return err;
//}
