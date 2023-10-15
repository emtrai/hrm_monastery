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
 * Filename: community.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "communityctl.h"
#include "logger.h"
#include "errcode.h"
#include "community.h"
#include "utils.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dbcommunitymodelhandler.h"
#include "dbmodel.h"
#include "areactl.h"
#include "countryctl.h"
#include "missionctl.h"
#include "jsondefs.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(CommunityCtl)

CommunityCtl::CommunityCtl():ModelController(KControllerCommunity, KModelHdlCommunity)
{
    traced;
}

CommunityCtl::~CommunityCtl()
{
    traced;
}

DbModel* CommunityCtl::onJsonParseOneItem(const QJsonObject& jobj, bool* ok )
{
    // TODO: use import instead???
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = ModelController::onJsonParseOneItem(jobj, ok);
    Community* ret = nullptr;
    if (!model) {
        err = ErrNoMemory;
        loge("no memory");
    }
    if (err == ErrNone && !IS_MODEL_NAME(model, KModelNameCommunity)) {
        err = ErrInvalidModel;
        loge("model '%s' is not community model", MODELSTR2CHA(model));
    }
    if (err == ErrNone) {
        ret = static_cast<Community*>(model);
        logd("parse model '%s'", MODELSTR2CHA(model));
        JSON_GET_TO_SET_STR(jobj, JSON_BRIEF, ret->setBrief);
        JSON_GET_TO_SET_STR(jobj, JSON_CHURCH, ret->setChurch);
        JSON_GET_TO_SET_STR(jobj, JSON_ADDR, ret->setAddr);
        JSON_GET_TO_SET_STR(jobj, JSON_TEL, ret->setTel);
        JSON_GET_TO_SET_STR(jobj, JSON_EMAIL, ret->setEmail);
        JSON_GET_TO_SET_STR(jobj, JSON_FEASTDAY, ret->setFeastDateFromString);
        JSON_GET_TO_SET_STR(jobj, JSON_ESTABLISH, ret->setCreateDateFromString);
        JSON_GET_TO_SET_STR(jobj, JSON_CLOSEDATE, ret->setCloseDateFromString);
    }

    if (err == ErrNone && jobj.contains(JSON_AREA_NAMEID)){
        QString tmp = jobj[JSON_AREA_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("area nameid '%s'", STR2CHA(tmp));
            DbModel* area = AREACTL->getModelByNameId(tmp);
            if (IS_MODEL_NAME(area, KModelNameArea)) {
                ret->setAreaUid(area->uid());
                ret->setAreaName(area->name());
            } else {
                err = ErrInvalidModel;
                loge("Not found uid '%s' or invalid model area '%s'",
                     STR2CHA(tmp), MODELSTR2CHA(area));
            }
            FREE_PTR(area);
        } else {
            logw("%s defined but no data", JSON_AREA_NAMEID);
        }
    }

    if (err == ErrNone && jobj.contains(JSON_PARENT_NAMEID)){
        QString tmp = jobj[JSON_PARENT_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("parent nameid '%s'", STR2CHA(tmp));
            Community* comm = static_cast<Community*>(getModelByNameId(tmp));
            if (IS_MODEL_NAME(comm, KModelNameCommunity)) {
                ret->setParentUid(comm->uid());
                ret->setParentName(comm->name());
                ret->setLevel(comm->level() + 1);
            } else {
                err = ErrNotFound;
                loge("Not found uid '%s' or invalid model comm '%s'",
                     STR2CHA(tmp), MODELSTR2CHA(comm));
            }
            FREE_PTR(comm);
        } else {
            logw("%s defined but no data", JSON_PARENT_NAMEID);
        }
    }


    if (err == ErrNone && jobj.contains(JSON_COUNTRY_NAMEID)){
        QString tmp = jobj[JSON_COUNTRY_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("country nameid '%s'", STR2CHA(tmp));
            DbModel* model = COUNTRYCTL->getModelByNameId(tmp);
            if (IS_MODEL_NAME(model, KModelNameCountry)) {
                ret->setCountryUid(model->uid());
                ret->setCountryName(model->name());
            } else {
                err = ErrNotFound;
                loge("Not found uid '%s' or invalid model '%s'",
                     STR2CHA(tmp), MODELSTR2CHA(model));
            }
            FREE_PTR(model);
        } else {
            logw("%s defined but no data", JSON_COUNTRY_NAMEID);
        }
    }


    if (err == ErrNone && jobj.contains(JSON_MISSION_NAMEID)){
        QString tmp = jobj[JSON_MISSION_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("mission nameid '%s'", STR2CHA(tmp));
            QStringList missionList = tmp.split(MISSION_DELIM);
            logd("mission name id cnt = %lld", missionList.size());
            if (missionList.size() > 0) {
                foreach (QString missionNameId, missionList) {
                    logd("missionNameId '%s'", STR2CHA(missionNameId));
                    DbModel* model = MISSIONCTL->getModelByNameId(missionNameId);
                    if (IS_MODEL_NAME(model, KModelNameMission)) {
                        ret->addMissionUid(model->uid());
                        ret->addMissionName(model->name());
                    } else {
                        err = ErrNotFound;
                        loge("Not found missionNameId uid '%s' or invalid model '%s'",
                             STR2CHA(missionNameId), MODELSTR2CHA(model));
                    }
                    FREE_PTR(model);
                    if (err != ErrNone) {
                        break;
                    }
                }
            } else {
                logw("%s defined but no data", JSON_MISSION_NAMEID);
            }
        } else {
            logw("%s defined but no data", JSON_MISSION_NAMEID);
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
    if (err != ErrNone) {
        FREE_PTR(ret);
    }
    traceout;
    return ret;

}

const QString CommunityCtl::exportListPrebuiltTemplateName(const QString& modelName) const
{
    tracein;
    QString tmplate;
    logd("export template for model '%s'", STR2CHA(modelName));
    if (modelName == KModelNameCommPerson) {
        tmplate = KPrebuiltCommunityPersonExportTemplateName;
    } else {
        tmplate = KPrebuiltCommunityExportTemplateName;
    }
    logd("template '%s'", STR2CHA(tmplate));
    traceout;
    return tmplate;
}

ErrCode CommunityCtl::onImportDataStart(const QString &importName,
                                        int importFileType,
                                        const QString &fname)
{
    tracein;
    UNUSED(importFileType);
    logi("start import '%s', fname '%s'", STR2CHA(importName), STR2CHA(fname));
    mImportHeaderFields.clear();
    traceout;
    return ErrNone;
}

ErrCode CommunityCtl::getActivePersonList(const QString &communityUid, QList<Person*>& outList)
{
    tracein;
    ErrCode err = getPersonList(communityUid, outList, MODEL_STATUS_MAX);
    logife(err, "get active person list failed");
    traceret(err);
    return err;
}

ErrCode CommunityCtl::getPersonList(const QString &communityUid,
                                    QList<Person *> &outList, qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdl = nullptr;
    QList<Person *> items;
    logd("get list of person for community uid '%s', status 0x%llx",
         STR2CHA(communityUid), modelStatus);
    if (communityUid.isEmpty()) {
        err = ErrInvalidArg;
        loge("Get person failed invalid args");
    }

    if (err == ErrNone) {
        hdl = dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
        if (!hdl) {
            err = ErrInvalidData;
            loge("not found handler, something was wrong");
        }
    }

    if (err == ErrNone) {
        items = hdl->getListPerson(communityUid, modelStatus);
        if (items.size() > 0) {
            logd("got %lld item", items.size());
            outList.append(items);
        } else {
            logw("not found list person of communit uid '%s'", STR2CHA(communityUid));
        }
    }
    logife(err, "Get list of active person failed");

    traceout;
    return err;
}

ErrCode CommunityCtl::getListCommunityPerson(const QString &communityUid,
                                               QList<DbModel *> &outList,
                                               qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdl = nullptr;
    QList<DbModel *> items;
    logd("get list of person for community uid '%s', status 0x%llx",
         STR2CHA(communityUid), modelStatus);
    if (communityUid.isEmpty()) {
        err = ErrInvalidArg;
        loge("Get person failed invalid args");
    }

    if (err == ErrNone) {
        hdl = dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
        if (!hdl) {
            err = ErrInvalidData;
            loge("not found handler, something was wrong");
        }
    }

    if (err == ErrNone) {
        items = hdl->getListCommunityPerson(communityUid, modelStatus);
        if (items.size() > 0) {
            logd("Got %lld items", items.size());
            outList.append(items);
        } else {
            logw("not found list person of communit uid '%s'", STR2CHA(communityUid));
        }
    }

    logife(err, "Get list of active person failed");

    traceout;
    return err;

}

ErrCode CommunityCtl::getListActiveCommunityPersonOfPerson(const QString &perUid,
                                                           QList<DbModel *> &outList)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdl = nullptr;
    QList<DbModel *> items;
    logd("get list of comm per of perUid '%s'", STR2CHA(perUid));
    if (perUid.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid args");
    }

    if (err == ErrNone) {
        hdl = dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
        if (!hdl) {
            err = ErrInvalidData;
            loge("not found handler, something was wrong");
        }
    }

    if (err == ErrNone) {
        items = hdl->getListActiveCommunityPersonOfPerson(perUid);
        if (items.size() > 0) {
            logd("Got %lld items", items.size());
            outList.append(items);
        } else {
            logw("not found list comm per for perUid '%s'", STR2CHA(perUid));
        }
    }
    logife(err, "Get list of comm per failed");

    traceout;
    return err;

}

ErrCode CommunityCtl::addPerson2Community(const Community *comm, const Person *per,
                                          bool updateCommPer, // update mapping comm & per
                                          int status,
                                          qint64 startdate,
                                          qint64 enddate,
                                          const QString &remark)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdl = nullptr;
    if (!comm || !per) {
        err = ErrInvalidArg;
        loge("invalid argment");
    }
    if (err == ErrNone) {
        hdl = dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
        if (!hdl) {
            err = ErrInvalidData;
            loge("not found handler, something was wrong");
        }
    }
    if (err == ErrNone) {
        logd("add person to community");
        err = hdl->addPerson2Community(comm, per, updateCommPer, status, startdate, enddate, remark);
    }
    logife(err, "Add person to community failed");
    traceret(err);
    return err;
}

const Community *CommunityCtl::getRootCommunity()
{
    DbCommunityModelHandler* hdl =
        dynamic_cast<DbCommunityModelHandler*>(DB->getCommunityModelHandler());
    ASSERT((hdl != nullptr), "Not found community model handler");
    return hdl->getRootCommunity();
}

const char *CommunityCtl::getPrebuiltFileName()
{
    // TODO: use import instead???
    return KPrebuiltCommunityJsonFileName;
}

const char *CommunityCtl::getPrebuiltFileType()
{
    // TODO: use import instead???
    return KFileTypeJson;
}

DbModelBuilder CommunityCtl::getMainBuilder()
{
    return &Community::build;
}


DbModel *CommunityCtl::doImportOneItem(const QString& importName, int importFileType,
                                       const QStringList &items, quint32 idx)
{
    ErrCode err = ErrNone;
    DbModel* model = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    logd("importName '%s'", STR2CHA(importName));
    if (idx == 0) { // first item is header
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportHeaderFields.append(item.trimmed());
        }
    } else {
        if (importName == KModelHdlCommunity)
            model = Community::build();
        else { // TODO: import person to community
            err = ErrNotSupport;
            loge("import '%s' not support", STR2CHA(importName));
        }

        if (err == ErrNone) {
            foreach (QString item, items) { // items must be in order with fields
                // TODO: this is not safe, should not be used???
                QString field = mImportHeaderFields[i++];
                logd("Import field %s", field.toStdString().c_str());
                err = model->onImportParseDataItem(importName, importFileType, field, item, idx);
                if (err != ErrNone) {
                    loge("on import item failed, %d", err);
                    break;
                }
            }
        }
    }


    if (err != ErrNone) {
        if (model) {
            delete model;
            model = nullptr;
        }
    }

    traceret(err);
    return model;
}


