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
 * Filename: dbsqliteedu.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "dbsqliteedu.h"
#include "logger.h"
#include "dbsqliteedu.h"
#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "table/dbsqliteedutbl.h"

GET_INSTANCE_IMPL(DbSqliteEdu)

DbSqliteEdu::DbSqliteEdu():DbSqliteModelHandler(KModelHdlEdu)
{
    traced;
}

ErrCode DbSqliteEdu::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameEducation) {
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldEduUid, model->uid());
            itemToSet.insert(KFieldEduUid, ""); // update to null/empty

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

DbSqliteTbl *DbSqliteEdu::getMainTbl()
{
    return (DbSqliteEduTbl*)DbSqlite::getInstance()->getTable(KTableEdu);
}

DbModelBuilder DbSqliteEdu::getMainBuilder()
{
    return &Education::build;
}

