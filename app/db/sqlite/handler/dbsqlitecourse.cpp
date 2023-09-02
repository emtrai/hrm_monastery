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
 * Filename: dbsqlitecourse.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#include "dbsqlitecourse.h"
#include "logger.h"
#include "course.h"
#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "persondept.h"
#include "areaperson.h"

GET_INSTANCE_IMPL(DbSqliteCourse)


DbSqliteCourse::DbSqliteCourse():DbSqliteModelHandler(KModelHdlCourse)
{
    tracein;
}

ErrCode DbSqliteCourse::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", MODELSTR2CHA(model), force);

        if (model->modelName() == KModelNameCourse) {
            // KFieldAreaUid delete map, community, person
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSet; // for update
            bool errDependency = false;

            itemToSearch.insert(KFieldCourseUid, model->uid());
            itemToSet.insert(KFieldCourseUid, ""); // update to null/empty

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                       msg, force,
                                       itemToSearch, itemToSet,
                                       KTablePerson, &Person::build);

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                           KTableCommDepartPerson, &PersonDept::build);
            }

            if (err == ErrNone && !errDependency) {
                CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency,
                                           msg, force,
                                           itemToSearch, itemToSet,
                                           KTableAreaPerson, &AreaPerson::build);
            }

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                logi("Delete model '%s'", MODELSTR2CHA(model));
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        }
    }
    traceret(err);
    return err;

}


DbSqliteTbl *DbSqliteCourse::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCourse);
}

DbModelBuilder DbSqliteCourse::getMainBuilder()
{
    return &Course::build;
}
