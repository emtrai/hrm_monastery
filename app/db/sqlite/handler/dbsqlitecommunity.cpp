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
#include "table/dbsqlitecommunitymanagertbl.h"
#include "dbsqlite.h"
#include "logger.h"
#include "communityperson.h"
#include "communitymanager.h"
#include "person.h"
#include "community.h"
#include "dbpersonmodelhandler.h"
#include "communitydept.h"
#include "exception.h"
#include "modeldefs.h"

// if defined, only one mapping community-person allow
// else, can exist multiple same mapping community-person
//#define SINGLE_COMMUNITY_PERSON_MAP

GET_INSTANCE_IMPL(DbSqliteCommunity)

// maximum number of times to check duplicate items
#define MAX_CHECK_DUP_TIME 1024

DbSqliteCommunity::DbSqliteCommunity():DbSqliteModelHandler(KModelHdlCommunity),
    mRootCommunity(nullptr)
{
    traced;
    mRootCommunity = nullptr;
}

DbSqliteCommunity::~DbSqliteCommunity()
{
    tracein;
    FREE_PTR(mRootCommunity);
    traceout;
}

ErrCode DbSqliteCommunity::add(DbModel *model, bool notify)
{
    tracein;
    ErrCode_t err = ErrNone;
    Community* comm = nullptr;
    if (!model) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    if (err == ErrNone) {
        logi("Add community '%s' to db", MODELSTR2CHA(model));
        err = DbSqliteModelHandler::add(model, false);
    }
    if (err == ErrNone) {
        err = check2UpdateCEO(model);
    }

    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_ADD, err);
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteCommunity::update(DbModel *model, bool notify)
{
    tracein;
    ErrCode_t err = ErrNone;
    if (!model) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    if (err == ErrNone) {
        logi("Add community '%s' to db", MODELSTR2CHA(model));
        err = DbSqliteModelHandler::update(model, false);
    }
    if (err == ErrNone) {
        err = check2UpdateCEO(model);
    }

    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_UPDATE, err);
    }
    traceret(err);
    return err;

}

ErrCode DbSqliteCommunity::update(DbModel *model, const QHash<QString, QString> &inFields,
                                  const QString &tableName, bool notify)
{
    return DbSqliteModelHandler::update(model, inFields, tableName, notify);
}


DbSqliteTbl *DbSqliteCommunity::getMainTbl()
{
    return (DbSqliteCommunityTbl*)DbSqlite::table(KTableCommunity);
}

DbSqliteTbl *DbSqliteCommunity::getTable(const QString &modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", STR2CHA(modelName));
    if (modelName.isEmpty() || modelName == KModelNameCommunity) {
        tbl = getMainTbl();
    } else if (modelName == KModelNameCommPerson) {
        tbl = DbSqlite::table(KTableCommPerson);
    } else if (modelName == KModelNameCommDept) {
        tbl = DbSqlite::table(KTableCommDept);
    } else if (modelName == KModelNameCommManager) {
        tbl = DbSqlite::table(KTableCommManager);
    } else { // TODO: check & implement more??
        loge("unsupport model name '%s'", STR2CHA(modelName));
    }
    traceout;
    return tbl;
}

DbModelBuilder DbSqliteCommunity::getMainBuilder()
{
    return &Community::build;
}

ErrCode DbSqliteCommunity::check2UpdateCEO(DbModel *model)
{
    tracein;
    ErrCode_t err = ErrNone;
    Community* comm = nullptr;
    if (!model) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    if (err == ErrNone && IS_MODEL_NAME(model, KModelNameCommunity)) {
        comm = static_cast<Community*>(model);
        if (comm->newCommMgr()) {
            CommunityManager* newComm = comm->newCommMgr();
            logi("Update community CEO '%s'", MODELSTR2CHA(comm->newCommMgr()));
            QList<DbModel *> currentCEOList;
            logi("Get current active CEO for comm '%s'", MODELSTR2CHA(comm));
            err = getCEOList(comm->uid(), currentCEOList, MODEL_STATUS_ACTIVE);
            logi("found '%lld' ceo", currentCEOList.size());
            if (err == ErrNone && currentCEOList.size() > 0) {
                foreach (DbModel* ceo, currentCEOList) {
                    if (IS_MODEL_NAME(ceo, KModelNameCommManager)) {
                        logi("Change current ceo '%s' to inactive", MODELSTR2CHA(ceo));
                        CommunityManager* currMgr = static_cast<CommunityManager*>(ceo);
                        currMgr->setMarkModified(true);
                        currMgr->setModelStatus(MODEL_STATUS_INACTIVE);
                        currMgr->setEndDate(DatetimeUtils::currentTimeToDatestring());
                        err = currMgr->update(false, false);
                        if (err != ErrNone) {
                            break;
                        }
                    }
                }
            }
            if (err == ErrNone) {
                logi("save new ceo '%s'", MODELSTR2CHA(comm->newCommMgr()));
                if (!newComm->markModified()) {
                    newComm->setMarkModified(true);
                }
                newComm->setCommunityUid(comm->uid());
                newComm->setModelStatus(MODEL_STATUS_ACTIVE);
                err = comm->newCommMgr()->save(false);
            }
            RELEASE_LIST_DBMODEL(currentCEOList);
        } else {
            logi("Skip updating CEO for comm '%s'", MODELSTR2CHA(comm));
        }
    }

    traceret(err);
    return err;
}

ErrCode DbSqliteCommunity::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameCommunity){
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update

            bool errDependency = false;

            itemToSearch.insert(KFieldCommunityUid, model->uid());
            itemToSet.insert(KFieldCommunityUid, ""); // update to null/empty

            // community can exist in: person, community-person mapping, community-dept mapping

            // clear community info on person
            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency, msg, force, itemToSearch, itemToSet,
                                       KTablePerson, &Person::build);
            // clear mapping community - person
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch,
                                   KTableCommPerson, &CommunityPerson::build);
            // clear mapping community - manager
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch,
                                   KTableCommManager, &CommunityManager::build);
            // delete community - dept
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch,
                                   KTableCommDept, &CommunityDept::build);

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        } else {
            err = DbSqliteModelHandler::deleteHard(model, force, msg);
        }
    }
    traceout;
    return err;
}


QList<Person *> DbSqliteCommunity::getListPerson(const QString &commUid,
                                                 int modelStatus,
                                                 const QString* perStatusUid)
{
    tracein;
    QList<Person *> list;
    logd("get list person of uid '%s'", STR2CHA(commUid));
    if(!commUid.isEmpty()) {
        // Get from commumity&person mapping table, how about community uid info in person tble??
        // TODO: need to sync information with communityuid on person table
        // RISK OF INCONSITANT!!!!!!!
        DbSqliteCommunityPersonTbl* tbl =
            static_cast<DbSqliteCommunityPersonTbl*>(DbSqlite::table(KTableCommPerson));
        if (tbl) {
            list = tbl->getListPerson(commUid, modelStatus, perStatusUid);
        } else {
            THROWEX("Not found table '%s'", KTableCommPerson);
        }
    } else {
        loge("invalid commUid '%s'", STR2CHA(commUid));
    }
    traceout;
    return list;
}

QList<DbModel *> DbSqliteCommunity::getListCommunityPerson(const QString &commUid,
                                                           int modelStatus,
                                                           const QString *perStatusUid)
{
    tracein;
    UNUSED(perStatusUid);
    QList<DbModel *> list;
    logd("get list person of uid '%s'", STR2CHA(commUid));
    if(!commUid.isEmpty()) {
        // Get from commumity&person mapping table, how about community uid info in person tble??
        // TODO: need to sync information with communityuid on person table
        // RISK OF INCONSITANT!!!!!!!
        DbSqliteCommunityPersonTbl* tbl =
            static_cast<DbSqliteCommunityPersonTbl*>(DbSqlite::table(KTableCommPerson));
        if (tbl) {
            list = tbl->getListCommunityPerson(commUid, modelStatus);
        } else {
            THROWEX("Not found table '%s'", KTableCommPerson);
        }
    } else {
        loge("invalid commUid '%s'", STR2CHA(commUid));
    }
    traceout;
    return list;

}

QList<DbModel *> DbSqliteCommunity::getListCommunityPersonOfPerson(const QString &perUid,
                                                                   int modelStatus)
{
    tracein;
    QList<DbModel *> list;
    logd("get list comm-person for perUid '%s'", STR2CHA(perUid));
    if(!perUid.isEmpty()) {
        DbSqliteCommunityPersonTbl* tbl =
            static_cast<DbSqliteCommunityPersonTbl*>(DbSqlite::table(KTableCommPerson));
        if (tbl) {
            list = tbl->getListCommunityPerson(perUid, QString(), modelStatus);
        } else {
            THROWEX("Not found table '%s'", KTableCommPerson);
        }
    } else {
        loge("invalid perUid '%s'", STR2CHA(perUid));
    }
    traceout;
    return list;
}

QList<DbModel *> DbSqliteCommunity::getListActiveCommunityPersonOfPerson(const QString &perUid)
{
    return getListCommunityPersonOfPerson(perUid, MODEL_STATUS_ACTIVE);
}

ErrCode DbSqliteCommunity::addPerson2Community(const Community *comm,
                                               const Person *per,
                                               bool updateCommPer, // update mapping comm & per
                                               int modelstatus,
                                               qint64 startdate, qint64 enddate,
                                               const QString &remark,
                                               bool notify)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteCommunityPersonTbl* tblCommPer = nullptr;
    logd("Build map object");

    if (!comm || !per) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    tblCommPer = (DbSqliteCommunityPersonTbl*)DbSqlite::table(KTableCommPerson);
    if (!tblCommPer) {
        loge("not found table '%s'", KTableCommPerson);
        err = ErrNoTable;
    }
    dbg(LOG_VERBOSE, "add person '%s' to comm '%s', modelstatus 0x%x, updateCommPer=%d",
        MODELSTR2CHA(per), MODELSTR2CHA(comm), modelstatus, updateCommPer);
    if (err == ErrNone && IS_STATUS(modelstatus, MODEL_STATUS_ACTIVE)) {
        // update community info in person info
        // only update if model status is active.
        DbPersonModelHandler* hdl =
            dynamic_cast<DbPersonModelHandler*>(DbSqlite::handler(KModelHdlPerson));
        if (hdl) {
            logi("update community '%s' for per '%s'",
                 STR2CHA(comm->toString()), STR2CHA(per->toString()));
            err = hdl->updateCommunity(per->uid(), comm->uid());
        } else {
            err = ErrInvalidData;
            loge("not found handler %s", KModelHdlPerson);
        }
    }
    if (!updateCommPer) {
        logi("update community, but skip updating comm per mapping here");
    }
    // start updating person - community mapping
    if (err == ErrNone && updateCommPer) {
        bool newAdd = true;
#ifdef SINGLE_COMMUNITY_PERSON_MAP
        QList<DbModel*> listCommunitiesOfPerson =
            tblCommPer->getListItemsUids(per->uid(), comm->uid(), &CommunityPerson::build);
        // check if mapping exist or not, if exit, set it active again
        // TODO: if person is moved arround in multiple time?
        if (listCommunitiesOfPerson.size() > 0) {
            logd("found %lld community which person uid belong to, set it as active",
                 listCommunitiesOfPerson.size());
            foreach (DbModel* model, listCommunitiesOfPerson) {
                newAdd = false;
                tblCommPer->updateModelStatusInDb(model->uid(), MODEL_STATUS_ACTIVE);
            }
        }
        RELEASE_LIST_DBMODEL(listCommunitiesOfPerson);
#endif //SINGLE_COMMUNITY_PERSON_MAP
        logd("newAdd=%d", newAdd);
        if (newAdd) {
            if (IS_STATUS(modelstatus, MODEL_STATUS_ACTIVE)) {
                // search active community list of person, change to inactive status
                QList<CommunityPerson*> listCommunitiesOfPerson =
                    tblCommPer->getListCommunityOfPerson(per->uid(), MODEL_STATUS_ACTIVE);
                if (listCommunitiesOfPerson.size() > 0) {
                    dbg(LOG_DEBUG, "found %lld community which person uid belong to currently"
                        ", change status to inactive", listCommunitiesOfPerson.size());
                    foreach (CommunityPerson* model, listCommunitiesOfPerson) {
                        tblCommPer->updateModelStatusInDb(model->uid(), MODEL_STATUS_INACTIVE);
                    }
                }
                RELEASE_LIST(listCommunitiesOfPerson, CommunityPerson);
            }
            logi("Save mapping community '%s' and person '%s'",
                 MODELSTR2CHA(comm), MODELSTR2CHA(per));
            // TODO: update modelstatus of old one?
            CommunityPerson* mapModel = (CommunityPerson*)MapDbModel::buildMapModel(&CommunityPerson::build,
                                                                            comm, per,
                                                                            modelstatus, startdate,
                                                                            enddate, remark);
            if (mapModel) {
                dbg(LOG_DEBUG, "save '%s'", MODELSTR2CHA(mapModel));
                err = mapModel->save(notify);
                FREE_PTR(mapModel);
            } else {
                err = ErrNoMemory;
                loge("no memory?");
            }
        }
    }

    if (err == ErrNone && notify) {
        logd("notify per '%s' for change", STR2CHA(per->toString()));
        notifyDataChange((DbModel*)per, DBMODEL_CHANGE_UPDATE, err);

        logd("notify comm '%s' for change", STR2CHA(comm->toString()));
        notifyDataChange((DbModel*)comm, DBMODEL_CHANGE_UPDATE, err);
    }
    traceret(err);
    return err;
}

const Community *DbSqliteCommunity::getRootCommunity()
{
    tracein;
    DbModel* model = nullptr;
    // TODO: should we mark if root community is loaded? to avoid re-query data multiple
    // times when no root community found.
    // but there is a case that root community can be added later, so mark as loadded may cause
    // it not up-to-date
    // solution can be when  community is added, check if it's root community, then
    // clear flag, but a little bit complicated, so do it later.
    if (!mRootCommunity) {
        logd("query root model with name '%s'", KModelNameIdRootCommunity);
        model = getByNameId(KModelNameIdRootCommunity, &Community::build);
        if (IS_MODEL_NAME(model, KModelNameCommunity)) {
            logd("found root community '%s'", MODELSTR2CHA(model));
            mRootCommunity = static_cast<Community*>(model);
        } else {
            loge("not found root community with name '%s' or invalid model '%s'",
                 KModelNameIdRootCommunity, MODELSTR2CHA(model));
        }
    } else {
        logd("use existing root community '%s'", MODELSTR2CHA(mRootCommunity));
    }
    logd("mRootCommunity '%s", MODELSTR2CHA(mRootCommunity));
    traceout;
    return mRootCommunity;
}

ErrCode DbSqliteCommunity::getManagersList(const QString &communityUid,
                                           QList<DbModel *> &outList, qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteCommunityManagerTbl* tbl = nullptr;

    if (err == ErrNone) {
        tbl = (DbSqliteCommunityManagerTbl*)DbSqlite::table(KTableCommManager);
        if (!tbl) {
            loge("not found table %s", KTableCommManager);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        logd("Get list community mgr from communityUid %s, status=%lld",
             STR2CHA(communityUid), modelStatus);
        outList = tbl->getListPerson(communityUid, modelStatus);
        logd("found %lld item", outList.size());
    }
    traceout;
    return err;

}

ErrCode DbSqliteCommunity::getManagersListWithRole(const QString &communityUid,
                                                   QList<DbModel *> &outList,
                                                   const QString &roleUid,
                                                   qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteCommunityManagerTbl* tbl = nullptr;
    dbgtrace;

    if (err == ErrNone) {
        tbl = (DbSqliteCommunityManagerTbl*)DbSqlite::table(KTableCommManager);
        if (!tbl) {
            loge("not found table %s", KTableCommManager);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        logd("Get list community mgr from communityUid '%s', roleUid '%s', status=%lld",
             STR2CHA(communityUid), STR2CHA(roleUid), modelStatus);
        outList = tbl->getListPerson(communityUid, roleUid, modelStatus);
        logd("found %lld item", outList.size());
    }
    logife(err, "getManagersListWithRole failed");
    traceout;
    return err;

}

ErrCode DbSqliteCommunity::getAllManagersList(QList<DbModel *> &outList, qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteCommunityManagerTbl* tbl = nullptr;

    if (err == ErrNone) {
        tbl = (DbSqliteCommunityManagerTbl*)DbSqlite::table(KTableCommManager);
        if (!tbl) {
            loge("not found table %s", KTableCommManager);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        logd("Get list community mgr status=%lld", modelStatus);
        outList = tbl->getListPersonAll(modelStatus);
        logd("found %lld item", outList.size());
    }
    traceout;
    return err;
}


ErrCode DbSqliteCommunity::getCEOList(const QString &communityUid,
                                 QList<DbModel *> &outList,
                                 qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel *> items;
    DbModel* role = nullptr;
    logd("KRoleCEODefaultNameId '%s'", KRoleCEODefaultNameId);
    role = SQLITE->getRoleModelHandler()->getByNameId(KRoleCEODefaultNameId);
    if (role) {
        dbgd("found ceo role '%s'", MODELSTR2CHA(role));
    } else {
        logw("not found role with name id '%s'", KRoleCEODefaultNameId);
    }

    if (err == ErrNone && !role) {
        loge("not found CEO role");
        err = ErrNotFound;
    }


    if (err == ErrNone) {
        dbgi("get list of person for communityUid '%s', role '%s', status 0x%llx",
             STR2CHA(communityUid), MODELSTR2CHA(role), modelStatus);
        err = getManagersListWithRole(communityUid, items, role->uid(), modelStatus);
        if (items.size() > 0) {
            outList.append(items);
        } else {
            logw("not found list person of communityUid '%s', role '%s'",
                 STR2CHA(communityUid), MODELSTR2CHA(role));
        }
    }
    logife(err, "Get list of community manager '%s' failed", STR2CHA(communityUid));

    FREE_PTR(role);
    traceout;
    return err;
}

ErrCode DbSqliteCommunity::getCurrentCEO(const QString &communityUid,
                                         Person** ceo, bool* isActiveCEO)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel *> outList;
    logi("Get current CEO for comm '%s'", STR2CHA(communityUid));
    err = getCEOList(communityUid, outList, MODEL_STATUS_MAX);
    if (err == ErrNone && outList.size() == 0) {
        err = ErrNoData;
        loge("not found current CEO for commuid '%s'", STR2CHA(communityUid));
    }
    if (err == ErrNone) {
        logi("found %lld current ceo", outList.size());
        CommunityManager* mgr = nullptr;
        CommunityManager* activemgr = nullptr;
        qint64 maxStartDate = 0;
        qint64 maxActiveStartDate = 0;
        foreach (DbModel* iter, outList) {
            if (IS_MODEL_NAME(iter, KModelNameCommManager)) {
                CommunityManager* tmpMgr = static_cast<CommunityManager*>(iter);
                if (tmpMgr->modelStatus() == MODEL_STATUS_ACTIVE) {
                    if (tmpMgr->startDate() >= maxActiveStartDate) {
                        maxActiveStartDate = tmpMgr->startDate();
                        activemgr = tmpMgr;
                    }
                } else {
                    if (tmpMgr->startDate() >= maxStartDate) {
                        maxStartDate = tmpMgr->startDate();
                        mgr = tmpMgr;
                    }
                }
            }
        }
        dbgd("activemgr '%s'", MODELSTR2CHA(activemgr));
        dbgd("mgr '%s'", MODELSTR2CHA(mgr));
        if (!mgr && !activemgr) {
            err = ErrNoData;
            loge("not found current CEO for commuid '%s'", STR2CHA(communityUid));
        }
        if (err == ErrNone) {
            // get 1st element, other is skipped
            // if more than 1 active element, data something bad!
            if (activemgr) {
                if (ceo) *ceo = CLONE_MODEL(activemgr->person(), Person);
                if (isActiveCEO) *isActiveCEO = true;
            } else {
                if (ceo) *ceo = CLONE_MODEL(mgr->person(), Person);
                if (isActiveCEO) *isActiveCEO = false;
            }
        }
    }
    RELEASE_LIST_DBMODEL(outList);
    traceret(err);
    return err;
}
