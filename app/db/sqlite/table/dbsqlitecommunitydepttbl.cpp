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
 * Filename: dbsqlitecommunitydeptmaptbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitecommunitydepttbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "defs.h"
#include "dbsqlitetablebuilder.h"
#include "communitydept.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include "logger.h"
#include "errcode.h"
#include "dbpersonmodelhandler.h"
#include "dbsqlite.h"
#include "dbsqliteperson.h"
#include "dbctl.h"
#include "dbmodelutils.h"
#include "dbsqliteupdatebuilder.h"

const qint32 DbSqliteCommunityDeptTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityDeptTbl::DbSqliteCommunityDeptTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommDept, KTableCommDept, KVersionCode)
{
    tracein;
}

QList<DbModel *> DbSqliteCommunityDeptTbl::getListDept(const QString &commUid, int status)
{
    tracein;
    QList<DbModel *> olist;
    ErrCode ret = ErrNone;
    QHash<QString, FieldValue> fields;
    fields.insert(KFieldCommunityUid, FieldValue(commUid));
    fields.insert(KFieldModelStatus, FieldValue(status));
    // TODO: check status???
    logd("Start search commUid %s, status=0x%x", commUid.toStdString().c_str(), status);
    ret = search(fields, true, nullptr, &CommunityDept::build, &olist, true);
    logd("ret=%d", ret);
    traceout;
    return olist;
}


ErrCode DbSqliteCommunityDeptTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    CommunityDept* cmm = (CommunityDept*) item;
    builder->addValue(KFieldCommunityUid, cmm->communityUid());
    builder->addValue(KFieldCommunityDbId, cmm->communityDbId());
    builder->addValue(KFieldDepartmentUid, cmm->departmentUid());
    builder->addValue(KFieldDepartmentDbId, cmm->departmentDbId());
    builder->addValue(KFieldCreateDate, cmm->establishDate());
    builder->addValue(KFieldCloseDate, cmm->closedDate());
    builder->addValue(KFieldAddr, cmm->addr());
    builder->addValue(KFieldTel, cmm->tel());
    builder->addValue(KFieldEmail, cmm->email());
    builder->addValue(KFieldModelStatus, cmm->modelStatus());
    builder->addValue(KFieldBrief, cmm->brief());
    traceout;
    return ErrNone;
}

ErrCode DbSqliteCommunityDeptTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    CommunityDept* cmm = (CommunityDept*) item;
    cmm->setCommunityUid(qry.value(KFieldCommunityUid).toString());
    if (!cmm->communityUid().isEmpty()) {
        DbModel* model = DB->getCommunityModelHandler()->getByUid(cmm->communityUid());
        if (model) {
            cmm->setCommunityDbId(model->dbId());
            cmm->setCommunityName(model->name());
            cmm->setCommunityNameId(model->nameId());
            delete model;
        }
    }

    cmm->setDepartmentUid(qry.value(KFieldDepartmentUid).toString());
    if (!cmm->departmentUid().isEmpty()) {
        DbModel* model = DB->getDepartmentModelHandler()->getByUid(cmm->departmentUid());
        if (model) {
            cmm->setDepartmentDbId(model->dbId());
            cmm->setDepartmentNameId(model->nameId());
            cmm->setDepartmentName(model->name());
            cmm->setName(model->name());
            delete model;
        }
    }
    // TODO: load information about current director?

    cmm->setEstablishDate(qry.value(KFieldCreateDate).toInt());
    cmm->setClosedDate(qry.value(KFieldCloseDate).toInt());
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setModelStatus(qry.value(KFieldModelStatus).toInt());
    cmm->setBrief(qry.value(KFieldBrief).toString());
    traceout;
    return err;
}

//QString DbSqliteCommunityDeptTbl::getSearchQueryStringWithTag(const QString &cond, const QString& condTag)
//{
//    tracein;
//    // TODO: check with condTag????
//    QString queryString = QString("SELECT *, %2.%4 AS %3, %2.%5 AS %6, %2.%7 AS %8 FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
//                              .arg(name(), KTableDepartment) // 1 & 2
//                              .arg(KFieldDepartmentUid, KFieldUid) // 3 & 4
//                              .arg(KFieldName, KFieldDepartmentName) // 5 & 6
//                              .arg(KFieldNameId, KFieldDepartmentNameId) // 7, 8
//        ;
//    if (!cond.isEmpty()) {
//        queryString += QString(" WHERE %1").arg(cond);
//    }
//    logd("queryString: %s", queryString.toStdString().c_str());
//    return queryString;
//}

DbModelBuilder DbSqliteCommunityDeptTbl::mainModelBuilder()
{
    return &CommunityDept::build;
}

ErrCode DbSqliteCommunityDeptTbl::updateBuilderFieldFromModel(
    DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{

    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNameCommDept) {
            CommunityDept* comm = (CommunityDept*) item;
            if (field == KItemEstablishDate) {
                builder->addValue(KFieldCreateDate, comm->establishDate());

            } else if (field == KItemEmail) {
                builder->addValue(KFieldEmail, comm->email());

            } else if (field == KItemAddress) {
                builder->addValue(KFieldAddr, comm->addr());

            } else if (field == KItemTel) {
                builder->addValue(KFieldTel, comm->tel());

            } else if (field == KItemBrief) {
                builder->addValue(KFieldBrief, comm->brief());

            } else if (field == KItemStatus) {
                builder->addValue(KFieldModelStatus, comm->modelStatus());

            } else if (field == KItemCloseDate) {
                builder->addValue(KFieldCloseDate, comm->closedDate());

            } else if (field == KItemCommunityDbId) {
                builder->addValue(KFieldCommunityDbId, comm->communityDbId());

            } else if (field == KItemDepartmentDbId) {
                builder->addValue(KFieldDepartmentDbId, comm->departmentDbId());

            } else if (field == KItemDepartment) {
                builder->addValue(KFieldDepartmentUid, comm->departmentUid());

            } else if (field == KItemCommunity) {
                builder->addValue(KFieldCommunityUid, comm->communityUid());
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

void DbSqliteCommunityDeptTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldCommunityUid, TEXT);
    builder->addField(KFieldCommunityDbId, INT64);
    builder->addField(KFieldDepartmentUid, TEXT);
    builder->addField(KFieldDepartmentDbId, INT64);
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldCloseDate, INT64);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldBrief, TEXT);
    builder->addField(KFieldModelStatus, INT64);
    traceout;
}
