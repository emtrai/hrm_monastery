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
 * Filename: dbsqliteareamgrtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqliteareamgrtbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "area.h"
#include "person.h"
#include "areaperson.h"
#include "rolectl.h"

const qint32 DbSqliteAreaMgrTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteAreaMgrTbl::DbSqliteAreaMgrTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableAreaPerson, KTableAreaPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldAreaUid;
    mFieldNameDbId1 = KFieldAreaDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<DbModel *> DbSqliteAreaMgrTbl::getListPerson(const QString &areaUid, int status)
{
    traced;
    logi("areaUid '%s'", areaUid.toStdString().c_str());
    /*
     * SELECT * FROM "KTableAreaPerson"
     *  JOIN "KTablePerson"
     *  ON "KTableAreaPerson"."KFieldPersonUid" = "KTablePerson"."KFieldUid"
     *  WHERE "KTableAreaPerson"."KFieldAreaUid" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItems(KTableAreaPerson,
                                                         KTablePerson,
                                                         KFieldPersonUid,
                                                         KFieldUid,
                                                         KFieldAreaUid,
                                                         &AreaPerson::build,
                                                         areaUid,
                                                         status);

    tracede;
    return list;
}

void DbSqliteAreaMgrTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);// TODO: use this????
    tracede;
}

ErrCode DbSqliteAreaMgrTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = DbSqliteMapTbl::updateModelFromQuery(item, qry);
    }
    if (err == ErrNone) {
        QString modelName = item->modelName();
        AreaPerson* areaPerson = nullptr;
        DbModel* person = nullptr;
        DbModel* area = nullptr;
        logd("update for map model '%s'", STR2CHA(modelName));
        if (modelName == KModelNameAreaPerson) {
            areaPerson = (AreaPerson*)item;
            logd("update for person model");
            person = Person::build();
            area = Area::build();
            if (!person || !area) {
                err = ErrNoMemory;
                loge("no memory for person or arae");
            }
        } else {
            loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
            err = ErrInvalidData;
        }
        if (err == ErrNone) {
            logd("Update person info");
            DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
            if (tbl) {
                err = tbl->updateModelFromQuery(person, qry);
            } else {
                loge("not found table %s", KTablePerson);
                err = ErrNotFound;
            }
        }
        if (err == ErrNone) {
            logd("update arae info");
            DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTableArea));
            if (tbl) {
                err = tbl->updateModelFromQuery(area, qry);
            } else {
                loge("not found table %s", KTableArea);
                err = ErrNotFound;
            }
        }

        if (err == ErrNone) {
            areaPerson->setRoleUid(qry.value(KFieldRoleUid).toString());
            areaPerson->setPerson(person);
            areaPerson->setArea(area);
            if (!areaPerson->roleUid().isEmpty()){
                logd("search role uid '%s'", STR2CHA(areaPerson->roleUid()));
                DbModel* model = ROLECTL->getModelByUid(areaPerson->roleUid());
                if (model) {
                    areaPerson->setRoleName(model->name());
                    delete model;
                } else {
                    logw("not found role uid '%s'", STR2CHA(areaPerson->roleUid()));
                }
            } else {
                logw("Role uid empty");
            }
        } else {
            loge("update db from model failed, err = %d", err);
            if (person) delete person;
            if (area) delete area;
        }
    }
    tracede;
    return err;
}
