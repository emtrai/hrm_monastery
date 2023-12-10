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
 * Filename: dbsqlitespecialist.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#include "dbsqlitespecialist.h"
#include "logger.h"
#include "dbsqlitespecialist.h"
#include "dbsqlite.h"
#include "table/dbsqlitespecialisttbl.h"
#include "table/dbsqlitespecialistpersontbl.h"
#include "dbsqlitedefs.h"
#include "specialist.h"

#include "specialistperson.h"
#include "modeldefs.h"

GET_INSTANCE_IMPL(DbSqliteSpecialist)

DbSqliteSpecialist::DbSqliteSpecialist():DbSqliteModelHandler(KModelHdlSpecialist)
{
    traced;
}


QList<DbModel *> DbSqliteSpecialist::getListPerson(const QString &specialistUid)
{
    tracein;
    DbSqliteSpecialistPersonTbl* tbl =
        (DbSqliteSpecialistPersonTbl*)DbSqlite::getInstance()->getTable(KTableSpecialistPerson);
    // assume main tbl is not null, if not programming error,
    // and require override search function
    ASSERT2(tbl != nullptr);
    QList<DbModel *> list = tbl->getListPerson(specialistUid);
    logd("found %lld", list.count());
    traceout;
    return list;
}

ErrCode DbSqliteSpecialist::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameSpecialist) {
            
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldSpecialistUid, model->uid());
            itemToSet.insert(KFieldSpecialistUid, ""); // update to null/empty

            CHECK_REMOVE_TO_DELETE( err, errDependency,
                                    msg, force,
                                    itemToSearch,
                                    KTableSpecialistPerson, &SpecialistPerson::build);
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

DbSqliteTbl *DbSqliteSpecialist::getMainTbl()
{
    return (DbSqliteSpecialistTbl*)DbSqlite::getInstance()->getTable(KTableSpecialist);
}

DbSqliteTbl *DbSqliteSpecialist::getTable(const QString &modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameSpecialistPerson) {
        tbl = DbSqlite::table(KTableSpecialistPerson);
    } else {
        tbl = getMainTbl();
    }
    traceout;
    return tbl;
}

DbModelBuilder DbSqliteSpecialist::getMainBuilder()
{
    return (DbModelBuilder)Specialist::build;
}

