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
#include "dbsqlite.h"
#include "dbctl.h"
#include "dbsqliteupdatebuilder.h"

const qint32 DbSqliteCommunityDeptTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityDeptTbl::DbSqliteCommunityDeptTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommDept, KTableCommDept, KVersionCode,
                  KModelNameCommDept)
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
    dbg(LOG_DEBUG, "getListDept commUid %s, status=0x%x",
                                        STR2CHA(commUid), status);
    ret = search(fields, true, nullptr, &CommunityDept::build, &olist, true);
    logd("ret=%d", ret);
    if (ret != ErrNone) {
        RELEASE_LIST_DBMODEL(olist);
        loge("search list dept failed err=%d", ret);
    }
    traceout;
    return olist;
}


ErrCode DbSqliteCommunityDeptTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !builder) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCommDept)) {
        err = ErrInvalidModel;
        loge("invalid model '%s'", MODELSTR2CHA(item));
    }
    if (err == ErrNone) {
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
    }
    traceout;
    return ErrNone;
}

ErrCode DbSqliteCommunityDeptTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    dbg(LOG_DEBUG, "update model '%s' from data query", MODELSTR2CHA(item));
    if (!item) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    }

    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCommDept)) {
        err = ErrInvalidModel;
        loge("invalid model '%s'", MODELSTR2CHA(item));
    }

    if (err == ErrNone) {
        CommunityDept* cmm = (CommunityDept*) item;
        cmm->setCommunityUid(qry.value(KFieldCommunityUid).toString());
        dbg(LOG_DEBUG, "update communityUid '%s'", STR2CHA(cmm->communityUid()));
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
        dbg(LOG_DEBUG, "update departmentUid '%s'", STR2CHA(cmm->departmentUid()));
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
    }
    logife(err, "update db model from query failed");
    traceout;
    return err;
}

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

ErrCode DbSqliteCommunityDeptTbl::filterFieldCond(int fieldId, int operatorId, QString fieldValueName, const DbModel *parentModel, QString &cond, int &dataType, bool &isExact)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::filterFieldCond(fieldId, operatorId, fieldValueName,
                                       parentModel, cond, dataType, isExact);
    if (err == ErrNone && parentModel) {
        if (IS_MODEL_NAME(parentModel, KModelNameCommunity)) {
            logd("Append uid '%s'", STR2CHA(parentModel->uid()));
            cond = QString("((%1) AND (%2 = '%3'))").arg(cond, KFieldCommunityUid, parentModel->uid());
        } else {
            err = ErrInvalidData;
            loge("invalid parentModel '%s'", MODELSTR2CHA(parentModel));
        }
    }
    logd("cond: '%s'", STR2CHA(cond));
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
