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
 * Filename: dbsqlitedept.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "dbsqlitedept.h"
#include "logger.h"
#include "defs.h"
#include "department.h"
#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "communitydept.h"
#include "modeldefs.h"

GET_INSTANCE_IMPL(DbSqliteDept);


DbSqliteDept::DbSqliteDept():DbSqliteModelHandler(KModelHdlDept)
{
    tracein;
}

ErrCode DbSqliteDept::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameDepartment) {
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldDepartmentUid, model->uid());
            itemToSet.insert(KFieldDepartmentUid, ""); // update to null/empty
            if (model->dbId() > 0) {
                logi("Set db id to clear %lld", model->dbId());
                itemToSearch.insert(KFieldDepartmentDbId, QString::number(model->dbId()));
                itemToSet.insert(KFieldDepartmentDbId, ""); // update to null/empty
            }

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTableCommDept, &CommunityDept::build);

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

DbSqliteTbl *DbSqliteDept::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableDepartment);
}

DbModelBuilder DbSqliteDept::getMainBuilder()
{
    return &Department::build;
}

