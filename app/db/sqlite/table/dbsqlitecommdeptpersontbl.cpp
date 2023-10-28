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
 * Filename: DbSqliteCommDeptPersonTbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#include "dbsqlitecommdeptpersontbl.h"

#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "persondept.h"
#include "dbctl.h"
#include "role.h"
#include "course.h"
#include "dbsqlite.h"

const qint32 DbSqliteCommDeptPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommDeptPersonTbl::DbSqliteCommDeptPersonTbl(DbSqlite *db):
    DbSqliteTbl(db, KTableCommDepartPerson, KTableCommDepartPerson, KVersionCode,
                  KModelNamePersonDept)
{
    traced;
}

QList<DbModel *> DbSqliteCommDeptPersonTbl::getListPerson(const QString &commDeptUid, int status)
{
    tracein;
    QList<DbModel *> olist;
    ErrCode ret = ErrNone;
    QHash<QString, FieldValue> fields;
    fields.insert(KFieldCommDeptUid, FieldValue(commDeptUid));
    fields.insert(KFieldModelStatus, FieldValue(status));
    // TODO: check status???
    logd("Start search commDeptUid %s", commDeptUid.toStdString().c_str());
    ret = search(fields, true, nullptr, &PersonDept::build, &olist, true);
    logd("ret=%d", ret);
    traceout;
    return olist;
}

QString DbSqliteCommDeptPersonTbl::getSearchQueryStringWithTag(const QString &cond,
                                                               const QString &condTag)
{
    tracein;
    // TODO: check with condTag????
    QString queryString = QString("SELECT *"
                                  ", %2.%5 AS %6 "
                                  ", %2.%7 AS %8 "
                                  ", %2.%9 AS %10 "
                                  "FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(KTableCommDepartPerson, KTablePerson) // 1 & 2
                              .arg(KFieldPersonUid, KFieldUid) // 3 & 4
                              .arg(KFieldNameId, KFieldPersonNameId) // 5 & 6
                              .arg(KFieldEmail, KFieldPersonEmail) // 7 & 8
                              .arg(KFieldTel, KFieldPersonTel) // 9 & 10
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    queryString += " ORDER BY name ASC";
    dbg(LOG_DEBUG, "queryString: %s", STR2CHA(queryString));
    return queryString;
}

void DbSqliteCommDeptPersonTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCommDeptUid, TEXT);
    builder->addField(KFieldPersonUid, TEXT);
    builder->addField(KFieldRoleUid, TEXT);
    builder->addField(KFieldCourseUid, TEXT);
    builder->addField(KFieldModelStatus, INT64);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldChangeHistory, TEXT); // history of changing role/person in dept
    traceout;
}

ErrCode DbSqliteCommDeptPersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode ret = ErrNone;

    ret = DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    QString modelName = item->modelName();
    logd("model name to insert '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePersonDept ) {
        PersonDept* model = (PersonDept*) item;

        builder->addValue(KFieldCommDeptUid, model->commDeptUid());
        builder->addValue(KFieldPersonUid, model->personUid());
        builder->addValue(KFieldRoleUid, model->roleUid());
        builder->addValue(KFieldCourseUid, model->courseUid());
        builder->addValue(KFieldModelStatus, model->modelStatus());
        builder->addValue(KFieldStartDate, model->startDate());
        builder->addValue(KFieldEndDate, model->endDate());
        builder->addValue(KFieldChangeHistory, model->changeHistory());

    } else {
        ret = ErrInvalidArg;
        loge("Invalid model name '%s'", modelName.toStdString().c_str());
    }
    traceret(ret);
    return ret;
}

ErrCode DbSqliteCommDeptPersonTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    QString modelName;
    if (!item) {
        loge("Invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    }
    if (err == ErrNone) {
        modelName = item->modelName();
    }

    logd("update for map model '%s'", STR2CHA(modelName));
    if (err == ErrNone) {
        if (modelName == KModelNamePerson ) {
            logd("update for person model");
            DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
            ASSERT2((tbl != nullptr));
            err = tbl->updateDbModelDataFromQuery(item, qry);
        } else if (modelName == KModelNamePersonDept) {
            PersonDept* model = (PersonDept*) item;
            DbModelHandler* hdl = nullptr;
            hdl = DB->getModelHandler(KModelHdlCommDept);
            ASSERT2(hdl != nullptr);

            model->setCommDeptUid(qry.value(KFieldCommDeptUid).toString());
            if (!model->commDeptUid().isEmpty()) {
                dbg(LOG_DEBUG, "commDeptUid '%s'", STR2CHA(model->commDeptUid()));
                DbModel* commDept = hdl->getByUid(model->commDeptUid());
                if (commDept) {
                    model->setCommDeptName(commDept->name());
                    model->setCommDeptNameId(commDept->nameId());
                    delete commDept;
                } else {
                    logw("not found Comm Dept uid '%s'", STR2CHA(model->commDeptUid()));
                }
            }
            model->setRoleUid(qry.value(KFieldRoleUid).toString());
            hdl = DB->getModelHandler(KModelHdlRole);
            ASSERT2(hdl != nullptr);
            if (!model->roleUid().isEmpty()) {
                dbg(LOG_DEBUG, "roleUid '%s'", STR2CHA(model->roleUid()));
                DbModel* tmp = hdl->getByUid(model->roleUid(), &Role::build);
                if (tmp != nullptr) {
                    model->setRoleName(tmp->name());
                    model->setRoleNameId(tmp->nameId());
                    delete tmp;
                } else {
                    logw("Not found role uid");
                }
            }

            model->setPersonUid(qry.value(KFieldPersonUid).toString());
            model->setPersonNameId(qry.value(KFieldPersonNameId).toString());
            model->setPersonHollyName(qry.value(KFieldHollyName).toString());
            model->setPersonEmail(qry.value(KFieldPersonEmail).toString());
            model->setPersonTel(qry.value(KFieldPersonTel).toString());
            model->setPersonName(FULLNAME(qry.value(KFieldFirstName).toString(),
                                          qry.value(KFieldLastName).toString()));

            model->setCourseUid(qry.value(KFieldCourseUid).toString());
            // TODO: make below code be re-used????
            hdl = DB->getModelHandler(KModelHdlCourse);
            ASSERT2(hdl != nullptr);
            if (!model->courseUid().isEmpty()) {
                dbg(LOG_DEBUG, "courseUid '%s'", STR2CHA(model->courseUid()));
                DbModel* tmp = hdl->getByUid(model->courseUid(), &Course::build);
                if (tmp != nullptr) {
                    model->setCourseName(tmp->name());
                    model->setCourseNameId(tmp->nameId());
                    delete tmp;
                } else {
                    logw("Not found course uid");
                }
            }

            model->setModelStatus(qry.value(KFieldModelStatus).toInt());
            model->setModelStatusName(DbModel::modelStatus2Name((DbModelStatus)model->modelStatus()));

            model->setStartDate(qry.value(KFieldStartDate).toInt());
            model->setEndDate(qry.value(KFieldEndDate).toInt());
            model->setChangeHistory(qry.value(KFieldChangeHistory).toString());

        } else {
            loge("Invalid mapp model '%s', do nothing", STR2CHA(modelName));
            err = ErrInvalidModel;
        }
    }
    traceout;
    return err;
}

ErrCode DbSqliteCommDeptPersonTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNamePersonDept) {
            PersonDept* comm = (PersonDept*) item;
            if (field == KItemRole) {
                builder->addValue(KFieldRoleUid, comm->roleUid());
            } else if (field == KItemCourse) {
                builder->addValue(KFieldCourseUid, comm->courseUid());
            } else if (field == KItemChangeHistory) {
                builder->addValue(KFieldChangeHistory, comm->changeHistory());
            } else if (field == KItemPerson) {
                builder->addValue(KFieldPersonUid, comm->personUid());
            } else if (field == KItemCommunityDept) {
                builder->addValue(KFieldCommDeptUid, comm->commDeptUid());
            } else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->modelStatus());
            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());
            } else if (field == KItemStartDate) {
                builder->addValue(KFieldStartDate, comm->startDate());
            } else {
                err = DbSqliteTbl::updateBuilderFieldFromModel(builder, field, item);
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
