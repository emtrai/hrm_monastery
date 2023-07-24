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
#include "areactl.h"
#include "person.h"
#include "personctl.h"
#include "areaperson.h"
#include "rolectl.h"
#include "course.h"
#include "coursectl.h"
#include "utils.h"
#include "dbmodelutils.h"
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
                                                         QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                             .arg(KTablePerson, KFieldNameId)
                                                             .arg(KFieldPersonNameId)
                                                             .arg(KFieldUid)
                                                             .arg(KFieldPersonUid)
                                                             .arg(KFieldLastName)
                                                             .arg(KFieldFirstName)
                                                             .arg(KFieldFullName)
                                                         );

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

ErrCode DbSqliteAreaMgrTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = DbSqliteMapTbl::updateDbModelDataFromQuery(item, qry);
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
                    areaPerson->setRoleNameId(model->nameId());
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
                    Course* course = (Course*)model;
                    areaPerson->setCourseNameId(model->nameId());
                    areaPerson->setCourseName(model->name());
                    if (!areaPerson->startDate()) {
                        areaPerson->setStartDate(course->startDate());
                    }
                    if (!areaPerson->endDate()) {
                        areaPerson->setEndDate(course->endDate());
                    }
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

            if (!qry.isNull(KFieldPersonUid)) {
                areaPerson->setPersonUid(qry.value(KFieldPersonUid).toString());
                Person* person = nullptr;
                if (!qry.isNull(KFieldPersonNameId)) {
                    person = (Person*)Person::build();
                    if (person) {
                        tblPerson->updateDbModelDataFromQuery(person, qry);
                    } else {
                        err = ErrNoMemory;
                        loge("no memory, cannot allocat person");
                    }
                } else {
                    person = (Person*)PERSONCTL->getModelByUid(areaPerson->personUid());
                    if (!person) {
                        logw("not found personUid '%s'", STR2CHA(areaPerson->personUid()));
                    }
                }
                if ((err == ErrNone) && person) {
                    areaPerson->setPersonName(person->displayName());
                    areaPerson->setPersonNameId(person->nameId());
                    // TODO: risk of no sync up data of holly name!!!
                    areaPerson->setPersonUid(person->uid());
                    areaPerson->setPersonEmail(person->emailString());
                    areaPerson->setPersonTel(person->telString());
                }
                FREE_PTR(person);
            } else {
                logw("not found field '%s'", KFieldPersonUid);
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

//ErrCode DbSqliteAreaMgrTbl::updateBuilderFromModel(DbSqliteUpdateBuilder *builder,
//                                             const QList<QString> &updateField,
//                                             const DbModel *item)
//{
//    tracein;
//    ErrCode err = ErrNone;
//    logd("update table for model '%s'", MODELSTR2CHA(item));
//    if (!builder || !item) {
//        err = ErrInvalidArg;
//        loge("invalid arg");
//    }

//    if (err == ErrNone) {
//        err = DbSqliteMapTbl::updateBuilderFromModel(builder, updateField, item);
//    }

//    if (err == ErrNone) {
//        if (item->modelName() == KModelNameAreaPerson) {
//            AreaPerson* comm = (AreaPerson*) item;
//            foreach (QString field, updateField) {
//                logd("Update field %s", STR2CHA(field));
//                if (field == KItemRole) {
//                    err = DbModelUtils::updateField(builder,
//                                                    KFieldRoleUid,
//                                                    KModelHdlArea,
//                                                    comm->roleUid(),
//                                                    comm->roleNameId(),
//                                                    comm->roleName());
//                } else if (field == KItemCourse) {
//                    err = DbModelUtils::updateField(builder,
//                                                    KFieldCourseUid,
//                                                    KModelHdlCourse,
//                                                    comm->courseUid(),
//                                                    comm->courseNameId(),
//                                                    comm->courseName());
////                    builder->addValue(KFieldCourseUid, comm->courseUid());
//                } else if (field == KItemChangeHistory) {
//                    builder->addValue(KFieldChangeHistory, comm->changeHistory());
//                } else if (field == KItemPerson) {

//                    err = DbModelUtils::updateField(builder,
//                                                    KFieldPersonUid,
//                                                    KModelHdlPerson,
//                                                    comm->personUid(),
//                                                    comm->personNameId(),
//                                                    comm->personName());
////                    builder->addValue(KFieldPersonUid, comm->personUid());
//                } else if (field == KItemArea) {

//                    err = DbModelUtils::updateField(builder,
//                                                    KFieldAreaUid,
//                                                    KModelHdlArea,
//                                                    comm->areaUid(),
//                                                    comm->areaNameId(),
//                                                    comm->areaName());
////                    builder->addValue(KFieldAreaUid, comm->areaUid());
//                } else if (field == KItemStatus) {
//                    builder->addValue(KFieldModelStatus, comm->modelStatus());
//                } else if (field == KItemEndDate) {
//                    builder->addValue(KFieldEndDate, comm->endDate());
//                } else if (field == KItemStartDate) {
//                    builder->addValue(KFieldStartDate, comm->startDate());
//                } else {
//                    logw("Field '%s' not support here", STR2CHA(field));
//                }
//                if (err != ErrNone) {
//                    loge("not found '%s', err=%d, field '%s'",
//                         MODELSTR2CHA(comm), err, STR2CHA(field));
//                    break;
//                }
//            }
//        } else {
//            logw("Model name '%s' is no support",
//                 STR2CHA(item->modelName()));
//        }
//    }
//    traceret(err);
//    return err;
//}

ErrCode DbSqliteAreaMgrTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
                                                        const QString &field,
                                                        const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNameAreaPerson) {
            AreaPerson* comm = (AreaPerson*) item;
            if (field == KItemRole) {
                err = DbModelUtils::updateField(builder,
                                                KFieldRoleUid,
                                                KModelHdlRole,
                                                comm->roleUid(),
                                                comm->roleNameId(),
                                                comm->roleName());
            } else if (field == KItemCourse) {
                err = DbModelUtils::updateField(builder,
                                                KFieldCourseUid,
                                                KModelHdlCourse,
                                                comm->courseUid(),
                                                comm->courseNameId(),
                                                comm->courseName());
            } else if (field == KItemChangeHistory) {
                builder->addValue(KFieldChangeHistory, comm->changeHistory());
            } else if (field == KItemPerson) {

                err = DbModelUtils::updateField(builder,
                                                KFieldPersonUid,
                                                KModelHdlPerson,
                                                comm->personUid(),
                                                comm->personNameId(),
                                                comm->personName());
            } else if (field == KItemArea) {

                err = DbModelUtils::updateField(builder,
                                                KFieldAreaUid,
                                                KModelHdlArea,
                                                comm->areaUid(),
                                                comm->areaNameId(),
                                                comm->areaName());
            } else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->modelStatus());
            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());
            } else if (field == KItemStartDate) {
                builder->addValue(KFieldStartDate, comm->startDate());
            } else {
                err = DbSqliteMapTbl::updateBuilderFieldFromModel(builder, field, item);
            }
        } else {
            loge("Model '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceret(err);
    return err;
}

QString DbSqliteAreaMgrTbl::getSearchQueryStringWithTag(const QString &cond, const QString &tag)
{
    tracein;
    QString queryStr = getListItemsQueryString(KTableAreaPerson,
                                            KTablePerson,
                                            KFieldPersonUid,
                                            KFieldUid,
                                            cond,
                                            MODEL_STATUS_MAX, // TODO: status?
                                            QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                .arg(KTablePerson)
                                                .arg(KFieldNameId)
                                                .arg(KFieldPersonNameId)
                                                .arg(KFieldUid)
                                                .arg(KFieldPersonUid)
                                                .arg(KFieldLastName)
                                                .arg(KFieldFirstName)
                                                .arg(KFieldFullName)
                                            );
    traceout;
    return queryStr;
}
