/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: communitydeptctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/1/2023
 * Brief:
 */
#include "communitydeptctl.h"

#include "logger.h"
#include "errcode.h"
#include "communitydept.h"
#include "utils.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dbcommdepatmodelhandler.h"
#include "persondept.h"
#include "communityctl.h"
#include "departctl.h"
#include "jsondefs.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(CommunityDeptCtl)

CommunityDeptCtl::CommunityDeptCtl():ModelController(KControllerCommDept, KModelHdlCommDept)
{
    tracein;
}

CommunityDeptCtl::~CommunityDeptCtl()
{
    tracein;
}

CommunityDept* CommunityDeptCtl::onJsonParseOneItem(const QJsonObject& jobj, bool* ok )
{
    tracein;
    ErrCode err = ErrNone;
    CommunityDept* ret = static_cast<CommunityDept*>(CommunityDept::build());
    if (!ret) {
        err = ErrNoMemory;
        loge("no memory");
    }
    if (err == ErrNone) {
        JSON_GET_TO_SET_STR(jobj, JSON_NAMEID, ret->setNameId);
        JSON_GET_TO_SET_STR(jobj, JSON_NAME, ret->setName);
        JSON_GET_TO_SET_STR(jobj, JSON_ADDR, ret->setAddr);
        JSON_GET_TO_SET_STR(jobj, JSON_TEL, ret->setTel);
        JSON_GET_TO_SET_STR(jobj, JSON_EMAIL, ret->setEmail);
        JSON_GET_TO_SET_STR_RET(jobj, JSON_ESTABLISH, ret->setEstablishDateFromString, err);
        JSON_GET_TO_SET_STR_RET(jobj, JSON_CLOSEDATE, ret->setClosedDateFromString, err);
        JSON_GET_TO_SET_STR(jobj, JSON_BRIEF, ret->setBrief);
        JSON_GET_TO_SET_STR(jobj, JSON_REMARK, ret->setRemark);
    }

    if (err == ErrNone && jobj.contains(JSON_COMMUNITY_UID)){
        QString tmp = jobj[JSON_COMMUNITY_UID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("comm nameid '%s'", STR2CHA(tmp));
            DbModel* comm = COMMUNITYCTL->getModelByNameId(tmp);
            if (comm) {
                ret->setCommunityUid(comm->uid());
                ret->setCommunityName(comm->name());
                delete comm;
            } else {
                err = ErrNotFound;
                loge("Not found community '%s'", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_COMMUNITY_UID);
        }
    }
    if (err == ErrNone && jobj.contains(JSON_DEPARTMENT_NAMEID)){
        QString tmp = jobj[JSON_DEPARTMENT_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("dept nameid '%s'", STR2CHA(tmp));
            DbModel* dept = DEPART->getModelByNameId(tmp);
            if (dept) {
                ret->setDepartmentUid(dept->uid());
                ret->setDepartmentName(dept->name());
                delete dept;
            } else {
                err = ErrNotFound;
                loge("Not found dept '%s'", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_DEPARTMENT_NAMEID);
        }
    }
    if (err == ErrNone && jobj.contains(JSON_COMMUNITY_UID)){
        QString tmp = jobj[JSON_COMMUNITY_UID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("area nameid '%s'", STR2CHA(tmp));
            DbModel* comm = COMMUNITYCTL->getModelByNameId(tmp);
            if (comm) {
                ret->setCommunityUid(comm->uid());
                ret->setCommunityName(comm->name());
                delete comm;
            } else {
                err = ErrNotFound;
                loge("Not found community '%s'", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_COMMUNITY_UID);
        }
    }

    if (err == ErrNone && jobj.contains(JSON_STATUS)){
        QString tmp = jobj[JSON_STATUS].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("Status '%s'", STR2CHA(tmp));
            bool ok = false;
            int status = QString(tmp).toInt(&ok);
            logd("isOK=%d, status=0x%x", ok, status);
            if (ok && ((status & MODEL_STATUS_MAX) != 0)) {
                ret->setModelStatus(status);
            } else {
                err = ErrInvalidData;
                loge("invalid status data %s", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_STATUS);
        }
    }

    logd("Parse result %d", err);
    if (ok) *ok = (err == ErrNone);
    if (err != ErrNone && ret) {
        delete ret;
        ret = nullptr;
    }

    return ret;

}

ErrCode CommunityDeptCtl::onImportDataStart(const QString &importName,
                                            int importFileType, const QString &fname)
{
    tracein;
    UNUSED(importFileType);
    logi("start import '%s', fname '%s'", STR2CHA(importName), STR2CHA(fname));
    mImportFields.clear();
    traceout;
    return ErrNone;
}

QList<DbModel *> CommunityDeptCtl::getListDept(const QString &communityUid, bool* ok)
{
    tracein;
    QList<DbModel *> items;
    logd("get dept for community uid='%s'", STR2CHA(communityUid));
    if (!communityUid.isEmpty()) {
        DbCommDeptModelHandler* modelHdl =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
        if (modelHdl) {
            items = modelHdl->getListDept(communityUid, MODEL_STATUS_MAX, ok);
        } else {
            if (ok) *ok = false;
            loge("not found commdept handler");
        }
    } else {
        if (ok) *ok = false;
        loge("Invalid community uid");
    }
    logd("items cnt=%lld", items.size());
    traceout;
    return items;
}

const QList<DbModel *> CommunityDeptCtl::getListPerson(const QString &commDeptUid,
                                                       int modelStatus, bool* ok)
{
    tracein;
    QList<DbModel *> items;
    logd("get people for commDeptUid uid='%s'", STR2CHA(commDeptUid));
    if (!commDeptUid.isEmpty()) {
        DbCommDeptModelHandler* modelHdl =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
        if (modelHdl) {
            items = modelHdl->getListPerson(commDeptUid, modelStatus, ok);
        } else {
            if (ok) *ok = false;
            loge("not found commdept handler");
        }
    } else {
        if (ok) *ok = false;
        loge("Invalid commDeptUid uid");
    }
    logd("items cnt=%lld", items.size());
    traceout;
    return items;
}

const QList<DbModel *> CommunityDeptCtl::getListActivePeople(const QString &commDeptUid, bool *ok)
{
    return getListPerson(commDeptUid, MODEL_STATUS_ACTIVE, ok);
}

QString CommunityDeptCtl::getListActivePeopleInString(const QString &commDeptUid,
                                                          const QString &sep,
                                                          bool *ok)
{
    tracein;
    QStringList listString;
    QString ret;
    bool tmpok = false;
    QList<DbModel *> listPeople = getListActivePeople(commDeptUid, &tmpok);
    if (tmpok) {
        foreach (DbModel* model, listPeople) {
            if (model && IS_MODEL_NAME(model, KModelNamePersonDept)) {
                PersonDept* per = (PersonDept*) model;
                listString.append(QString("%1 : %2 %3").arg(per->roleName(),
                                                   per->personHollyName(),
                                                   per->personName()));
            } else {
                loge("invalid model '%s'", MODELSTR2CHA(model));
            }
        }
        if (listString.size() > 0) {
            logd("found %lld items", listString.size());
            ret = listString.join(sep);
        } else {
            logi("no data found");
        }
    } else {
        loge("get list active people failed");
    }
    if (ok) *ok = tmpok;
    traceout;
    return ret;
}

const char *CommunityDeptCtl::getPrebuiltFileName()
{
    return KPrebuiltCommunityDeptJsonFileName;
}

const char *CommunityDeptCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
}

const QString CommunityDeptCtl::exportListPrebuiltTemplateName(const QString &modelName) const
{
    QString ret;
    logd("modelName '%s'", STR2CHA(modelName));
    if (modelName == KModelNameCommDept) {
        ret = KPrebuiltCommunityDeptExportTemplateName;
    } else if (modelName == KModelNamePersonDept){
        ret = KPrebuiltCommunityDeptPersonExportTemplateName;
    } else {
        loge("unknown model name '%s'", STR2CHA(modelName));
    }
    return ret;
}

DbModelBuilder CommunityDeptCtl::getMainBuilder()
{
    return &CommunityDept::build;
}

DbModel *CommunityDeptCtl::doImportOneItem(const QString& importName, int importFileType,
                                           const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    CommunityDept* model = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    logd("importName '%s'", STR2CHA(importName));
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
        }
    } else {
        if (importName == KModelHdlCommDept)
            model = (CommunityDept*)CommunityDept::build();
        else { // TODO: import person to community
            ret = ErrNotSupport;
            loge("import '%s' not support", STR2CHA(importName));
        }
        if (ret == ErrNone) {
            foreach (QString item, items) {
                QString field = mImportFields[i++];
                logd("Import field %s", field.toStdString().c_str());
                ret = model->onImportParseDataItem(importName, importFileType, field, item, idx);
                if (ret != ErrNone) {
                    loge("on import item failed, %d", ret);
                    break;
                }
            }
        }
    }

    if (ret != ErrNone) {
        if (model) {
            delete model;
            model = nullptr;
        }
    }

    traceret(ret);
    return model;
}

DbModel *CommunityDeptCtl::doImportOneItem(const QString& importName, int importFileType,
                                           const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    CommunityDept* model = nullptr;
    logd("idx = %d", idx);
    if (importName == KModelHdlCommDept) {
        model = (CommunityDept*)CommunityDept::build();
    } else { // TODO: import person to community
        ret = ErrNotSupport;
        loge("import '%s' not support", STR2CHA(importName));
    }

    if (ret == ErrNone) {
        foreach (QString field, items.keys()) {
            QString value = items.value(field);
            logd("Import field %s", field.toStdString().c_str());
            logd("Import value %s", value.toStdString().c_str());
            ret = model->onImportParseDataItem(importName, importFileType, field, value, idx);
            if (ret != ErrNone) {
                loge("on import item failed, %d", ret);
                break;
            }
        }
    }
    if (ret != ErrNone) {
        if (model) {
            delete model;
            model = nullptr;
        }
    }

    traceret(ret);
    return model;
}

