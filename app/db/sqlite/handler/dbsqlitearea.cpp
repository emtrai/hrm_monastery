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
 * Filename: dbsqlitearea.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#include "dbsqlitearea.h"
#include "area.h"
#include "community.h"
#include "logger.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "table/dbsqliteareamgrtbl.h"
#include "table/dbsqlitecommunitytbl.h"
#include "model/areaperson.h"
#include "modeldefs.h"


GET_INSTANCE_IMPL(DbSqliteArea)

DbSqliteArea::DbSqliteArea():DbSqliteModelHandler(KModelHdlArea)
{
    traced;
}

DbSqliteArea::~DbSqliteArea()
{
    traced;
}

QList<DbModel *> DbSqliteArea::getListContactPeople(const QString &areaUid, int status)
{
    tracein;
    QList<DbModel *> list;
    ErrCode err = ErrNone;
    DbSqliteAreaMgrTbl* tbl = nullptr;
    if (areaUid.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalig arg, no area uid");
    }
    if (err == ErrNone) {
        tbl = (DbSqliteAreaMgrTbl*)DbSqlite::table(KTableAreaPerson);
        if (!tbl) {
            loge("not found table %s", KTableAreaPerson);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        dbg(LOG_VERBOSE, "Get list contact people from area uid %s, status=%d",
                        STR2CHA(areaUid), status);
        list = tbl->getListPerson(areaUid, status);
        logd("found %lld item", list.size());
    }
    traceout;
    return list;
}

QList<DbModel *> DbSqliteArea::getListCommunities(const QString &areaUid, int status)
{
    tracein;
    QList<DbModel *> list;
    ErrCode err = ErrNone;
    DbSqliteCommunityTbl* tbl = nullptr;
    if (areaUid.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalig arg, no area uid");
    }
    if (err == ErrNone) {
        // there is no area community mapping, information is stored directly in community table
        tbl = (DbSqliteCommunityTbl*)DbSqlite::table(KTableCommunity);
        if (!tbl) {
            loge("not found table %s", KTableCommunity);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        dbg(LOG_VERBOSE, "Get list community from area uid %s, status=%d",
            STR2CHA(areaUid), status);
        list = tbl->getListCommunitiesInArea(areaUid, status);
        logd("found %lld item", list.size());
    }
    traceout;
    return list;
}

ErrCode DbSqliteArea::addContactPerson(const DbModel* area,
                                       const DbModel* person,
                                       int status,
                                       const QString& roleUid,
                                       qint64 startdate, qint64 enddate,
                                       const QString &remark)
{
    tracein;
    ErrCode err = ErrNone;
    AreaPerson* mapModel = (AreaPerson*)MapDbModel::buildMapModel(&AreaPerson::build,
                                                                area, person,
                                                                status, startdate,
                                                                enddate, remark);
    if (mapModel) {
        mapModel->setRoleUid(roleUid);
        logi("save map db model to db area '%s' person '%s', roleUid '%s'",
             MODELSTR2CHA(area), MODELSTR2CHA(person), STR2CHA(roleUid));
        err = mapModel->save();
    } else {
        err = ErrNoData;
        loge("Failed to create map db model");
    }

    FREE_PTR(mapModel);
    traceret(err);
    return err;
}

ErrCode DbSqliteArea::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (IS_MODEL_NAME(model, KModelNameAreaPerson)) {
            err = DbSqliteModelHandler::deleteHard(model, force, msg);
        } else if (IS_MODEL_NAME(model, KModelNameArea)){
            err = deleteHardArea(model, force, msg);
        } else {
            err = ErrNotSupport;
            loge("not support delete model '%s' here", MODELSTR2CHA(model));
        }
    }
    traceout;
    return err;
}

ErrCode DbSqliteArea::deleteHardArea(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!IS_MODEL_NAME(model, KModelNameArea)) {
        err = ErrInvalidArg;
        loge("Invalid model '%s'", MODELSTR2CHA(model));
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);


        QHash<QString, QString> itemToSearch; // for searching
        QHash<QString, QString> itemToSet; // for update
        QList<DbModel*> list;
        int count = 0;
        ErrCode tmpErr = ErrNone;
        bool errDependency = false;

        itemToSearch.insert(KFieldAreaUid, model->uid());
        itemToSet.insert(KFieldAreaUid, ""); // update to null/empty
        itemToSet.insert(KFieldAreaDbId, ""); // update to null/empty

        // clear all area uid to be deleted in community
        logd("Check to delete value in table %s", KTableCommunity);
        err = getListItems(itemToSearch, KTableCommunity, true, &count, &list, &Community::build);
        if (err == ErrNone && count > 0) {
            if (!force) {
                errDependency = true;
                if (msg) *msg += QString("Area '%1' existed in %2.").arg(model->name(), KTableCommunity);
            } else {
                foreach (DbModel* model, list) {
                    logd("force Update '%s'", STR2CHA(model->toString()));
                    tmpErr = update(model, itemToSet, KTableCommunity, false);
                    logd("Update result=%d", tmpErr);
                    // TODO: handler error???
                }
            }
        }
        RELEASE_LIST_DBMODEL(list);

        logd("Check to delete items in table %s", KTableAreaPerson);
        err = getListItems(itemToSearch, KTableAreaPerson, true, &count, &list, &AreaPerson::build);
        if (err == ErrNone && count > 0) {
            if (!force) {
                errDependency = true;
                if (msg) *msg += QString("Area '%1' existed in %2.").arg(model->name(), KTableAreaPerson);
            } else {
                foreach (DbModel* model, list) {
                    logd("force delete '%s'", STR2CHA(model->toString()));
                    tmpErr = DbSqliteModelHandler::deleteHard(model, force, msg);
                    logd("Delete result=%d", tmpErr);
                    // TODO: handler error???
                }
            }
        }
        RELEASE_LIST_DBMODEL(list);

        if (errDependency) {
            err = ErrDependency;
            loge("cannot delete, has dependency '%s'", msg?STR2CHA(*msg):"");
        } else {
            err = DbSqliteModelHandler::deleteHard(model, force, msg);
        }
    }
    traceout;
    return err;
}

DbSqliteTbl *DbSqliteArea::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableArea);
}

DbSqliteTbl *DbSqliteArea::getTable(const QString &modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("getTable modelname '%s'", STR2CHA(modelName));
    if (modelName.isEmpty() || modelName == KModelNameArea) {
        tbl = getMainTbl();
    } else if (modelName == KModelNameAreaPerson) {
        tbl = DbSqlite::table(KTableAreaPerson);
    } else {
        loge("Invalid model name '%s' for area model handler", STR2CHA(modelName));
        // TODO: raise exception??
    }
    traceout;
    return tbl;
}

DbModelBuilder DbSqliteArea::getMainBuilder()
{
    return &Area::build;
}

DbModelBuilder DbSqliteArea::getBuilder(const QString &modelName)
{
    tracein;
    DbModelBuilder buildr = nullptr;
    logd("get builder for model '%s'", STR2CHA(modelName));
    if (modelName.isEmpty() || modelName == KModelNameArea) {
        buildr = getMainBuilder();
    } else if (modelName == KModelNameAreaPerson) {
        buildr = &AreaPerson::build;
    } else {
        loge("Invalid model name '%s'", STR2CHA(modelName));
    }
    traceout;
    return buildr;
}

