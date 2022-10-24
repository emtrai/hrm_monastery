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
 * Filename: dbsqlitedepartmentpersontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#include "dbsqlitedepartmentpersontbl.h"

#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/deptmgr.h"
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

const qint32 DbSqliteDepartmentPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteDepartmentPersonTbl::DbSqliteDepartmentPersonTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableDepartPerson, KTableDepartPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldDepartmentUid;
    mFieldNameDbId1 = KFieldDepartmentDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

DbSqliteDepartmentPersonTbl::DbSqliteDepartmentPersonTbl(DbSqlite *db,
                                                         const QString &baseName,
                                                         const QString &name,
                                                         qint32 versionCode)
    : DbSqliteMapTbl(db, baseName, name, versionCode)
{
    traced;
    mFieldNameUid1 = KFieldDepartmentUid;
    mFieldNameDbId1 = KFieldDepartmentDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<DbModel *> DbSqliteDepartmentPersonTbl::getListPerson(const QString &departUid, int status)
{
    traced;
    QSqlQuery qry;
    qint32 cnt = 0;
    if (departUid.isEmpty()){
        loge("Invalid departUid uid");
        return QList<DbModel*>();
    }
    logi("CommunityUid '%s'", departUid.toStdString().c_str());
    QString queryString = QString("SELECT * FROM %1 JOIN %2 ON %1.%3 = %2.%4 WHERE %1.%5 = :uid")
                              .arg(KTableDepartPerson, KTablePerson)
                              .arg(KFieldPersonUid, KFieldUid, KFieldDepartmentUid);

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", departUid);
    // TODO: add query status
    //    qry.bindValue( ":status", status); // TODO: support multiple status???
    QList<DbModel *> outList;
    cnt = runQuery(qry, &PersonDept::build, &outList);

    logi("Found %d", cnt);

    return outList;
}


void DbSqliteDepartmentPersonTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);
    builder->addField(KFieldCourseUid, TEXT);
}

ErrCode DbSqliteDepartmentPersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    ErrCode ret = ErrNone;
    QString modelName = item->modelName();
    logd("model name to insert '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePersonDept ) {
        DeptMgr* map = new DeptMgr();
        PersonDept* model = (PersonDept*) item;
        Department* dep = model->department();
        Person* per = model->person();

        if (dep != nullptr)
            map->setDbId1(dep->dbId());

        map->setUid1(model->departUid());

        if (map->uid1().isEmpty()) {
            ret = ErrInvalidData;
            loge("no depart uid info");
        }
        if (ret == ErrNone) {
            if (per != nullptr) {
                map->setDbId2(per->dbId());
                map->setUid2(per->uid());
            } else {
                ret = ErrInvalidData;
                loge("no person info");
            }
        }

        if (ret == ErrNone) {
            map->setStatus(model->status());
            map->setStartDate(model->startDate());
            map->setEndDate(model->endDate());
            map->setRemark(model->remark());
        }

        logd("Build map object");
        if (ret == ErrNone) {
            ret = DbSqliteMapTbl::insertTableField(builder, map);
            builder->addValue(KFieldRoleUid, model->roleUid());
            builder->addValue(KFieldCourseUid, model->courseUid());
        }

        delete map;
        map = nullptr;

    } else {
        ret = ErrInvalidArg;
        loge("Invali model name '%s'", modelName.toStdString().c_str());
    }
    tracedr(ret);
    return ret;
}

QHash<QString, QString> DbSqliteDepartmentPersonTbl::getFieldsCheckExists(const DbModel *item)
{
    traced;
    QHash<QString, QString> list;
    QString modelName = item->modelName();
    logd("check exist for map model '%s'", modelName.toStdString().c_str());
    if (item->modelName() == KModelNamePersonDept) {
        PersonDept* model = (PersonDept*)(item);
        bool isvalid = true;
        logd("check exist for KModelNamePersonDept model");
        model->dump();
        QString deptUid = model->departUid();
        Person* per = model->person();
        if (!deptUid.isEmpty())
            list[KFieldDepartmentUid] = model->departUid();
        else {
            isvalid = false;
            loge("invalid dept uid");
        }

        if (isvalid && per != nullptr)
            list[KFieldPersonUid] = per->uid();
        else {
            isvalid = false;
            loge("invalid person uid");
        }

        if (isvalid) {
            list[KFieldStartDate] = QString("%1").arg(model->startDate());
            list[KFieldEndDate] = QString("%1").arg(model->endDate());
            list[KFieldStatus] = QString("%1").arg(model->status());
        }

        logd("isvalid %d", isvalid);
    } else {
        list = DbSqliteMapTbl::getFieldsCheckExists(item);
    }

    tracedr(list.count());
    return list;
}


void DbSqliteDepartmentPersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteMapTbl::updateModelFromQuery(item, qry);
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePerson )
    {
        logd("update for person model");
        DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
        tbl->updateModelFromQuery(item, qry);
    } else if (modelName == KModelNamePersonDept) {

        DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));

        PersonDept* model = (PersonDept*) item;
        DbModel* per = Person::build();
        tbl->updateModelFromQuery(per, qry);
        model->setPerson((Person*)per);

        model->setRoleUid(qry.value(KFieldRoleUid).toString());
        DbModelHandler* hdl = DB->getModelHandler(KModelHdlRole);
        if (hdl != nullptr) {
            DbModel* tmp = hdl->getItem(model->roleUid(), &Role::builder);
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
            DbModel* tmp = hdl->getItem(model->courseUid(), &Course::builder);
            if (tmp != nullptr) {
                model->setCourseName(tmp->name());
            } else {
                logi("Not found course uid");
            }
        } else {
            loge("not found course handler");
        }

        model->setDepartUid(qry.value(KFieldDepartmentUid).toString());


    } else {
        loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
    }
}
