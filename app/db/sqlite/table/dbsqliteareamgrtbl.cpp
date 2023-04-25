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
#include "dbsqliteupdatebuilder.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "area.h"
#include "areactl.h"
#include "person.h"
#include "areaperson.h"
#include "rolectl.h"
#include "role.h"
#include "course.h"
#include "coursectl.h"
#include "dbctl.h"

/**
 * VERSION 0.0.1:
 * + KFieldAreaUid
 * + KFieldAreaDbId
 * + KFieldPersonUid
 * + KFieldPersonDbId
 * + KFieldRoleUid
 * VERSION 0.0.2: Add:
 * + KFieldCourseUid
 */

#define VERSION_CODE_1 VERSION_CODE(0,0,1)
#define VERSION_CODE_2 VERSION_CODE(0,0,2)
const qint32 DbSqliteAreaMgrTbl::KVersionCode = VERSION_CODE_2;

DbSqliteAreaMgrTbl::DbSqliteAreaMgrTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableAreaPerson, KTableAreaPerson, KVersionCode)
{
    tracein;

    mFieldNameUid1 = KFieldAreaUid;
    mFieldNameDbId1 = KFieldAreaDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<DbModel *> DbSqliteAreaMgrTbl::getListPerson(const QString &areaUid, int status)
{
    tracein;
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
                                                         status,
                                                         QString("*, %1.%2 AS %3, %1.%4 AS %5")
                                                             .arg(KTablePerson, KFieldNameId)
                                                             .arg(KFieldPersonNameId)
                                                             .arg(KFieldUid)
                                                             .arg(KFieldPersonUid));

    traceout;
    return list;
}

ErrCode DbSqliteAreaMgrTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode ret = ErrNone;

    ret = DbSqliteMapTbl::insertTableField(builder, item); // TODO: handle error code
    QString modelName = item->modelName();
    logd("model name to insert '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameAreaPerson ) {
        AreaPerson* model = (AreaPerson*) item;

        builder->addValue(KFieldRoleUid, model->roleUid());
        builder->addValue(KFieldCourseUid, model->courseUid());

    } else {
        ret = ErrInvalidArg;
        loge("Invali model name '%s'", modelName.toStdString().c_str());
    }
    traceret(ret);
    return ret;
}

void DbSqliteAreaMgrTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);// TODO: use this????

    /* From version 0.0.2 */
    builder->addField(KFieldCourseUid, TEXT);

    traceout;
}

ErrCode DbSqliteAreaMgrTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
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
        if (modelName == KModelNameAreaPerson) {
            AreaPerson* areaPerson = (AreaPerson*)item;
            DbSqlitePersonTbl* tblPerson = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
            areaPerson->setRoleUid(qry.value(KFieldRoleUid).toString());
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

            areaPerson->setCourseUid(qry.value(KFieldCourseUid).toString());
            if (!areaPerson->courseUid().isEmpty()){
                logd("search courseUid '%s'", STR2CHA(areaPerson->courseUid()));
                DbModel* model = COURSECTL->getModelByUid(areaPerson->courseUid());
                if (model) {
                    areaPerson->setCourseName(model->name());
                    delete model;
                } else {
                    logw("not found courseUid '%s'", STR2CHA(areaPerson->courseUid()));
                }
            } else {
                logw("courseUid empty");
            }


            areaPerson->setAreaUid(qry.value(KFieldAreaUid).toString());
            if (!areaPerson->areaUid().isEmpty()){
                logd("search areaUid '%s'", STR2CHA(areaPerson->areaUid()));
                DbModel* model = AREACTL->getModelByUid(areaPerson->areaUid());
                if (model) {
                    areaPerson->setAreaName(model->name());
                    areaPerson->setAreaNameId(model->nameId());
                    delete model;
                } else {
                    logw("not found areaUid '%s'", STR2CHA(areaPerson->areaUid()));
                }
            } else {
                logw("areaUid empty");
            }

            areaPerson->setPersonUid(qry.value(KFieldPersonUid).toString());
            Person* person = (Person*)Person::build();
            if (person) {
                tblPerson->updateModelFromQuery(person, qry);
                areaPerson->setPersonName(person->fullName());
                areaPerson->setPersonNameId(person->nameId());
                areaPerson->setHollyName(person->hollyName());
                // TODO: risk of no sync up data of holly name!!!
                areaPerson->setPersonUid(person->uid());
                areaPerson->setPersonEmail(person->emailString());
                areaPerson->setPersonTel(person->telString());
                delete person;
            } else {
                err = ErrNoMemory;
                loge("no memory, cannot allocat person");
            }

        } else {
            loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
            err = ErrInvalidData;
        }
    }
    traceout;
    return err;
}
/**
 * @brief DbSqliteAreaMgrTbl::onTblMigration
 * @param oldVer
 * @return
 */
ErrCode DbSqliteAreaMgrTbl::onTblMigration(qint64 oldVer)
{
    tracein;
    ErrCode err = ErrNone;
    logi("tbl '%s' version upgrade, from version 0x%lx to 0x%lx", STR2CHA(mName), oldVer, mVersionCode);
    switch (oldVer) {
        case VERSION_CODE_1:
        {
            QHash<QString, TableFieldDatatype_t> columnField;
            // From version 0.0.2
            columnField.insert(KFieldCourseUid, TEXT);
            loge("Add column fields");
            err = addTableColumn(columnField);
        }
        break;
        default:
            break;
    };
    traceret(err);
    return err;
}

ErrCode DbSqliteAreaMgrTbl::updateTableField(DbSqliteUpdateBuilder *builder,
                                             const QList<QString> &updateField, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!builder || !item) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        err = DbSqliteMapTbl::updateTableField(builder, updateField, item);
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNameAreaPerson) {
            AreaPerson* comm = (AreaPerson*) item;
            foreach (QString field, updateField) {
                logd("Update field %s", STR2CHA(field));
                if (field == KItemRole) {
                    builder->addValue(KFieldRoleUid, comm->roleUid());
                } else if (field == KItemCourse) {
                    builder->addValue(KFieldCourseUid, comm->courseUid());
                } else if (field == KItemChangeHistory) {
                    builder->addValue(KFieldChangeHistory, comm->changeHistory());
                } else if (field == KItemPerson) {
                    builder->addValue(KFieldPersonUid, comm->personUid());
                } else if (field == KItemArea) {
                    builder->addValue(KFieldAreaUid, comm->areaUid());
                } else if (field == KItemStatus) {
                    builder->addValue(KFieldModelStatus, comm->modelStatus());
                } else if (field == KItemEndDate) {
                    builder->addValue(KFieldEndDate, comm->endDate());
                } else if (field == KItemStartDate) {
                    builder->addValue(KFieldStartDate, comm->startDate());
                } else {
                    logw("Field '%s' not support here", STR2CHA(field));
                }
            }
        } else {
            logw("Model name '%s' is no support",
                 STR2CHA(item->modelName()));
        }
    }
    traceret(err);
    return err;
}
