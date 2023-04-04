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
#include "communitydept.h"
#include "exception.h"

GET_INSTANCE_IMPL(DbSqliteCommunity)

#define MAX_CHECK_DUP_TIME 1024

DbSqliteCommunity::DbSqliteCommunity():DbSqliteModelHandler(KModelHdlCommunity)
{
    tracein;
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

ErrCode DbSqliteCommunity::add(DbModel *model, bool notify)
{
    tracein;
    ErrCode err = ErrNone;
    DbModelHandler* hdlDept = nullptr;
    DbModelHandler* hdlCommDept = nullptr;
    if (!model) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        logi("Add model '%s' to db", STR2CHA(model->toString()));
        err = DbSqliteModelHandler::add(model, false);
        logd("add model res=%d", err);
    }

    if (err == ErrNone) {
        hdlDept = DbSqlite::handler(KModelHdlDept);
        if (!hdlDept) {
            err = ErrNoHandler;
            loge("not found dept handler");
        }
    }
    if (err == ErrNone) {
        hdlCommDept = DbSqlite::handler(KModelHdlCommDept);
        if (!hdlCommDept) {
            err = ErrNoHandler;
            loge("not found comm dept handler");
        }
    }

    // Add management dept to community so that we can add comminity's manager to
    if (err == ErrNone) {
        logd("Check to add management dept for community");
        // TODO: FIXME we're trying to fix management name id with this, but trouble
        // if it's not in json prebuilt file... TAKE CARE
        DbModel* dept = hdlDept->getByNameId(KManagementDeptNameId);
        if (dept) {
            QString nameId;
            int i = 0;
            for (; i < MAX_CHECK_DUP_TIME; i++) {
                // TODO: search if comm & dept mapping existed???
                nameId = QString("%1_%2").arg(model->nameId(), dept->nameId());
                if (i > 0) {
                    nameId += QString("_%1").arg(i);
                }
                if (!hdlCommDept->isNameidExist(nameId)) {
                    logd("not found name id '%s'", STR2CHA(nameId));
                    break;
                } else {
                    logi("Name id '%s' existed", STR2CHA(nameId));
                    nameId.clear();
                }
            }
            if (i >= MAX_CHECK_DUP_TIME || nameId.isEmpty()) {
                err = ErrExisted;
                logd("not found any suitable nameid, tried %d time", i);
            }
            if (err == ErrNone) {
                logd("Add mapping comm vs dept with nameid '%s'", STR2CHA(nameId));
                CommunityDept* commdept = (CommunityDept*)CommunityDept::build();
                if (commdept) {
                    commdept->setNameId(nameId);
                    commdept->setCommunityUid(model->uid());
                    commdept->setDepartmentUid(dept->uid());
                    commdept->setName(dept->name());
                    commdept->setStatus(MODEL_ACTIVE);
                    logi("Add dept '%s' to community '%s'", STR2CHA(dept->toString()), STR2CHA(model->toString()));
                    err = commdept->save();
                    delete commdept;
                    logd("add result=%d", err);
                } else {
                    err = ErrNoMemory;
                    loge("no memory");
                }
            }
            delete dept;
        } else {
            logw("not found dept '%s' for community", KManagementDeptNameId);
        }
    }
    // TODO: well, if adding comm to db ok, but update comm and management dept failed
    // what should we do next???
    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_ADD, err);
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
        logd("Delete hard model '%s', force %d", STR2CHA(model->toString()), force);

        if (model->modelName() == KModelNameCommunity){
            // KFieldAreaUid delete map, community, person
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            QList<DbModel*> list;
            int count = 0;
            ErrCode tmpErr = ErrNone;
            bool errDependency = false;

            itemToSearch.insert(KFieldCommunityUid, model->uid());
            itemToSet.insert(KFieldCommunityUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency, msg, force, itemToSearch, itemToSet, KTablePerson, &Person::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableCommPerson, &CommunityPerson::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableCommDept, &CommunityDept::build);

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        }
    }
    traceout;
    return err;

}


QList<DbModel *> DbSqliteCommunity::getListPerson(const QString &commUid, int modelStatus, const QString* perStatusUid)
{
    tracein;
    QList<DbModel *> list;
    logd("get list person of uid '%s'", STR2CHA(commUid));
    if(!commUid.isEmpty()) {
        // Get from commumity&person mapping table, how about community uid info in person tble??
        // TODO: need to sync information with communityuid on person table
        // RISK OF INCONSITANT!!!!!!!
        DbSqliteCommunityPersonTbl* tbl = (DbSqliteCommunityPersonTbl*)DbSqlite::table(KTableCommPerson);
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

ErrCode DbSqliteCommunity::addPerson2Community(const Community *comm,
                                               const Person *per, int status,
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

    if (err == ErrNone) {
        DbPersonModelHandler* hdl = dynamic_cast<DbPersonModelHandler*>(DbSqlite::handler(KModelHdlPerson));
        if (hdl) {
            logi("update community '%s' for per '%s'",
                 STR2CHA(comm->toString()), STR2CHA(per->toString()));
            err = hdl->updateCommunity(per->uid(), comm->uid());
        } else {
            err = ErrInvalidData;
            loge("not found handler %s", KModelHdlPerson);
        }
    }

    if (err == ErrNone) {
        bool newAdd = true;
        QList<DbModel*> listCommunitiesOfPerson =
            tblCommPer->getListItemsUids(per->uid(), comm->uid(), &CommunityPerson::build);
        if (listCommunitiesOfPerson.size() > 0) {
            logd("found %d community which person uid belong to, set it as active", listCommunitiesOfPerson.size());
            foreach (DbModel* model, listCommunitiesOfPerson) {
                newAdd = false;
                tblCommPer->updateModelStatus(model->uid(), MODEL_ACTIVE);
            }
        }
        RELEASE_LIST_DBMODEL(listCommunitiesOfPerson);
        logd("newAdd=%d", newAdd);
        if (newAdd) {
            QList<DbModel*> listCommunitiesOfPerson =
                tblCommPer->getListCommunityOfPerson(per->uid(), &CommunityPerson::build);
            if (listCommunitiesOfPerson.size() > 0) {
                logd("found %d community which person uid belong to currently, change status to inactive", listCommunitiesOfPerson.size());
                foreach (DbModel* model, listCommunitiesOfPerson) {
                    tblCommPer->updateModelStatus(model->uid(), MODEL_INACTIVE);
                }
            }
            RELEASE_LIST_DBMODEL(listCommunitiesOfPerson);
            logi("Save mapping community '%s' and person '%s'",
                 STR2CHA(comm->toString()), STR2CHA(per->toString()));
            // TODO: update status of old one?
            CommunityPerson* mapModel = (CommunityPerson*)MapDbModel::buildMapModel(&CommunityPerson::build,
                                                                            comm, per,
                                                                            status, startdate,
                                                                            enddate, remark);
            if (mapModel) {
                mapModel->setStatus(MODEL_ACTIVE); // TODO: set active here is suitable???
                err = mapModel->save();
                delete mapModel;
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


