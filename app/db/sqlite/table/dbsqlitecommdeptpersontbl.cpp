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
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "persondept.h"
#include "dbctl.h"
#include "role.h"
#include "course.h"
#include "department.h"
#include "dbsqlite.h"

const qint32 DbSqliteCommDeptPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommDeptPersonTbl::DbSqliteCommDeptPersonTbl(DbSqlite *db):
    DbSqliteTbl(db, KTableCommDepartPerson, KTableCommDepartPerson, KVersionCode)
{
    tracein;


}


//QList<DbModel *> DbSqliteCommDeptPersonTbl::getListPerson(const QString &commDeptUid, int status)
//{
//    tracein;
////    DB->openDb();
//    QSqlQuery qry(SQLITE->currentDb());
//    qint32 cnt = 0;
//    if (commDeptUid.isEmpty()){
//        loge("Invalid departUid uid");
//        return QList<DbModel*>();
//    }
//    logi("commDeptUid '%s'", commDeptUid.toStdString().c_str());
//    QString queryString = QString("SELECT * FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4 WHERE %1.%5 = :uid")
//                              .arg(KTableCommDepartPerson, KTablePerson) // 1 & 2
//                              .arg(KFieldPersonUid, KFieldUid, KFieldDepartmentUid) // 3 & 4 & 5

//                                ;

//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());

//    // TODO: check sql injection issue
//    qry.bindValue( ":uid", commDeptUid);
//    // TODO: add query status
//    //    qry.bindValue( ":status", status); // TODO: support multiple status???
//    QList<DbModel *> outList;
//    cnt = runQuery(qry, &PersonDept::build, &outList);

//    logi("Found %d", cnt);

//    return outList;
//}
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

QString DbSqliteCommDeptPersonTbl::getSearchQueryStringWithTag(const QString &cond, const QString &condTag)
{
    tracein;
    // TODO: check with condTag????
    QString queryString = QString("SELECT * FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(KTableCommDepartPerson, KTablePerson) // 1 & 2
                              .arg(KFieldPersonUid, KFieldUid) // 3 & 4
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
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
        loge("Invali model name '%s'", modelName.toStdString().c_str());
    }
    traceret(ret);
    return ret;
}

ErrCode DbSqliteCommDeptPersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePerson )
    {
        logd("update for person model");
        DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
        tbl->updateModelFromQuery(item, qry);
    } else if (modelName == KModelNamePersonDept) {

        PersonDept* model = (PersonDept*) item;

        model->setCommDeptUid(qry.value(KFieldCommDeptUid).toString());
        model->setRoleUid(qry.value(KFieldRoleUid).toString());
        model->setPersonUid(qry.value(KFieldPersonUid).toString());
        model->setPersonName(FULLNAME(qry.value(KFieldFirstName).toString(),
                                      qry.value(KFieldLastName).toString()));
        DbModelHandler* hdl = DB->getModelHandler(KModelHdlRole);
        if (hdl != nullptr) {
            DbModel* tmp = hdl->getItem(model->roleUid(), &Role::build);
            if (tmp != nullptr) {
                model->setRoleName(tmp->name());
            } else {
                logi("Not found role uid");
            }
        } else {
            loge("not found role handler");
        }

        model->setCourseUid(qry.value(KFieldCourseUid).toString());
        // TODO: make below code be re-used????
        hdl = DB->getModelHandler(KModelHdlCourse);
        if (hdl != nullptr) {
            DbModel* tmp = hdl->getItem(model->courseUid(), &Course::build);
            if (tmp != nullptr) {
                model->setCourseName(tmp->name());
            } else {
                logi("Not found course uid");
            }
        } else {
            loge("not found course handler");
        }


        model->setModelStatus(qry.value(KFieldModelStatus).toInt());
        model->setStartDate(qry.value(KFieldStartDate).toInt());
        model->setEndDate(qry.value(KFieldEndDate).toInt());
        model->setChangeHistory(qry.value(KFieldChangeHistory).toString());

    } else {
        loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
    }
    traceout;
    return err;
}
