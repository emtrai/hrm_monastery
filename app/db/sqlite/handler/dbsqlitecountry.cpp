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
 * Filename: dbsqlitecountry.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/12/2022
 * Brief:
 */
#include "dbsqlitecountry.h"
#include "logger.h"
#include "defs.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "country.h"
#include "area.h"
#include "community.h"
#include "ethnic.h"
#include "saint.h"


GET_INSTANCE_IMPL(DbSqliteCountry);

DbSqliteCountry::DbSqliteCountry():DbSqliteModelHandler(KModelHdlCountry)
{
    traced;
}

ErrCode DbSqliteCountry::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameCountry) {
            // KFieldAreaUid delete map, community, person
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldCountryUid, model->uid());
            itemToSet.insert(KFieldCountryUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTablePerson, &Person::build);

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                           KTableCommunity, &Community::build);
            }

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                           KTableSaint, &Saint::build);
            }

            if (model->dbId() > 0) {
                logi("Set db id to clear %lld", model->dbId());
                itemToSearch.insert(KFieldCountryDbId, QString::number(model->dbId()));
                itemToSet.insert(KFieldCountryDbId, ""); // update to null/empty
            }

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                           KTableArea, &Area::build);
            }

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                            KTableEthnic, &Ethnic::build);
            }

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

DbSqliteTbl *DbSqliteCountry::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCountry);
}

DbModelBuilder DbSqliteCountry::getMainBuilder()
{
    return &Country::build;

}
