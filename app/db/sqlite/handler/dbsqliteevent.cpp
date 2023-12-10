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
 * Filename: dbsqliteevent.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/10/2022
 * Brief:
 */
#include "dbsqliteevent.h"
#include "logger.h"
#include "defs.h"
#include "event.h"
#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "personevent.h"
#include "modeldefs.h"

GET_INSTANCE_IMPL(DbSqliteEvent)

DbSqliteEvent::DbSqliteEvent():DbSqliteModelHandler(KModelHdlEvent)
{
    traced;
}

ErrCode DbSqliteEvent::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameEvent) {
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldEventUid, model->uid());
            itemToSet.insert(KFieldEventUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTablePersonEvent, &PersonEvent::build);

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


DbSqliteTbl *DbSqliteEvent::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableEvent);
}

DbModelBuilder DbSqliteEvent::getMainBuilder()
{
    return &Event::build;
}
