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
 * Filename: dbsqlitestatus.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/6/2022
 * Brief:
 */
#include "dbsqlitepersonstatus.h"
#include "logger.h"
#include "personstatus.h"
#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "person.h"

GET_INSTANCE_IMPL(DbSqlitePersonStatus)

DbSqlitePersonStatus::DbSqlitePersonStatus():DbSqliteModelHandler(KModelHdlPersonStatus)
{
    traced;
}

ErrCode DbSqlitePersonStatus::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNamePersonStatus) {
            // KFieldAreaUid delete map, community, person
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldPersonStatusUid, model->uid());
            itemToSet.insert(KFieldPersonStatusUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTablePerson, &Person::build);

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

DbSqliteTbl *DbSqlitePersonStatus::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableStatus);
}

DbModelBuilder DbSqlitePersonStatus::getMainBuilder()
{
    return &PersonStatus::build;
}
