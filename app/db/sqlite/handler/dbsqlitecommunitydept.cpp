/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: DbSqliteCommunityDeptdept.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/1/2023
 * Brief:
 */
#include "dbsqlitecommunitydept.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "logger.h"
#include "model/communitydept.h"
#include "table/dbsqlitecommdeptpersontbl.h"
#include "table/dbsqlitecommunitydepttbl.h"
#include "persondept.h"

GET_INSTANCE_IMPL(DbSqliteCommunityDept);


DbSqliteCommunityDept::DbSqliteCommunityDept():DbSqliteModelHandler(KModelHdlCommDept)
{
    traced;
}

DbSqliteTbl *DbSqliteCommunityDept::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCommDept);
}


QList<DbModel *> DbSqliteCommunityDept::getListPerson(const QString &commDeptUid, int status, bool* ok)
{
    tracein;
    DbSqliteCommDeptPersonTbl* tbl = (DbSqliteCommDeptPersonTbl*)DbSqlite::table(KTableCommDepartPerson);
    if (ok) *ok = true; // TODO: handle error case
    return tbl->getListPerson(commDeptUid, status);
}

QList<DbModel *> DbSqliteCommunityDept::getListDept(const QString &communityUid, int status, bool* ok)
{
    DbSqliteCommunityDeptTbl* tbl = (DbSqliteCommunityDeptTbl*)DbSqlite::table(KTableCommDept);
    if (ok) *ok = true; // TODO: handle error case
    return tbl->getListDept(communityUid, status);
}

ErrCode DbSqliteCommunityDept::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (IS_MODEL_NAME(model, KModelNameCommDept)){
            QHash<QString, QString> itemToSearch; // for searching
            ErrCode tmpErr = ErrNone;
            bool errDependency = false;
            logi("Check to delete item '%s' in dept manager table '%s'",
                 MODELSTR2CHA(model), KTableCommDepartPerson);
            itemToSearch.insert(KFieldCommDeptUid, model->uid());

            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch,
                                   KTableCommDepartPerson, &PersonDept::build);

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        } else if (IS_MODEL_NAME(model, KModelNamePersonDept)) {
            err = DbSqliteModelHandler::deleteHard(model, force, msg);
        } else {
            err = ErrInvalidData;
            loge("invalid model '%s'", MODELSTR2CHA(model));
        }
    }

    logi("Delete '%s' ret %d", MODELSTR2CHA(model), err);

    traceret(err);
    return err;

}

DbSqliteTbl *DbSqliteCommunityDept::getTable(const QString &modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName.isEmpty() || modelName == KModelNameCommDept) {
        tbl = getMainTbl();
    } else if (modelName == KModelNameCommPerson) {
        tbl = DbSqlite::table(KTableCommPerson);
    } else if (modelName == KModelNamePersonDept) {
        logd("get table '%s'", KTableCommDepartPerson);
        tbl = (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCommDepartPerson);
    } else { // TODO: check & implement more??
        loge("model name '%s' not support", STR2CHA(modelName));
    }
    traceout;
    return tbl;
}

DbModelBuilder DbSqliteCommunityDept::getMainBuilder()
{
    return &CommunityDept::build;
}


