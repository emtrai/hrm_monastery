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
 * Filename: dbsqlitesaint.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "dbsqlitesaint.h"
#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "table/dbsqlitesainttbl.h"
#include "logger.h"
#include "saint.h"
#include "person.h"
#include "saintperson.h"
#include "modeldefs.h"

GET_INSTANCE_IMPL(DbSqliteSaint)

DbSqliteSaint::DbSqliteSaint():DbSqliteModelHandler(KModelHdlSaint)
{
    tracein;
}

ErrCode DbSqliteSaint::deleteHard(DbModel *model, bool force, QString *msg)
{

    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameSaint) {

            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldSaintUid, model->uid());
            itemToSet.insert(KFieldSaintUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTablePerson, &Person::build);

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force,
                                       itemToSearch,
                                       KTableSaintPerson,
                                       &SaintPerson::build);
            }

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                logi("Delete model '%s'", MODELSTR2CHA(model));
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        } else {
            err = ErrInvalidData;
            loge("invalid model '%s'", MODELSTR2CHA(model));
        }
    }
    traceret(err);
    return err;
}

DbSqliteTbl *DbSqliteSaint::getMainTbl()
{
    return (DbSqliteSaintTbl*)DbSqlite::getInstance()->getTable(KTableSaint);
}

DbModelBuilder DbSqliteSaint::getMainBuilder()
{
    return &Saint::build;
}

