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
#include "filectl.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dbcommunitymodelhandler.h"
#include "errreporterctl.h"
#include "dbmodel.h"
#include "areactl.h"
#include "countryctl.h"
#include "missionctl.h"

GET_INSTANCE_CONTROLLER_IMPL(CommunityCtl)

CommunityCtl::CommunityCtl():ModelController(KModelHdlCommunity)
{
    tracein;
}

CommunityCtl::~CommunityCtl()
{
    tracein;
}

DbModel* CommunityCtl::onJsonParseOneItem(const QJsonObject& jobj, bool* ok )
{
    // TODO: use import instead???
    tracein;
    ErrCode err = ErrNone;
    Community* ret = static_cast<Community*>(ModelController::onJsonParseOneItem(jobj, ok));
    if (!ret) {
        err = ErrNoMemory;
        loge("no memory");
    }
    if (err == ErrNone) {
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
            if (area) {
                ret->setAreaUid(area->uid());
                ret->setAreaName(area->name());
                delete area;
            } else {
                err = ErrNotFound;
                loge("Not found area '%s'", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_AREA_NAMEID);
        }
    }

    if (err == ErrNone && jobj.contains(JSON_PARENT_NAMEID)){
        QString tmp = jobj[JSON_PARENT_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("parent nameid '%s'", STR2CHA(tmp));
            Community* comm = static_cast<Community*>(getModelByNameId(tmp));
            if (comm) {
                ret->setParentUid(comm->uid());
                ret->setParentName(comm->name());
                ret->setLevel(comm->level() + 1);
                delete comm;
            } else {
                err = ErrNotFound;
                loge("Not found parent community '%s'", STR2CHA(tmp));
            }
        } else {
            logw("%s defined but no data", JSON_PARENT_NAMEID);
        }
    }


    if (err == ErrNone && jobj.contains(JSON_COUNTRY_NAMEID)){
        QString tmp = jobj[JSON_COUNTRY_NAMEID].toString().trimmed();
        if (!tmp.isEmpty()) {
            logd("country nameid '%s'", STR2CHA(tmp));
            DbModel* model = COUNTRYCTL->getModelByNameId(tmp);
            if (model) {
                ret->setCountryUid(model->uid());
                ret->setCountryName(model->name());
                delete model;
            } else {
                err = ErrNotFound;
                loge("Not found country '%s'", STR2CHA(tmp));
            }
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
                    if (model) {
                        ret->addMissionUid(model->uid());
                        ret->addMissionName(model->name());
                        delete model;
                    } else {
                        err = ErrNotFound;
                        loge("Not found mission '%s'", STR2CHA(missionNameId));
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
        delete ret;
        ret = nullptr;
    }
    traceout;
    return ret;

}

const QString CommunityCtl::exportListPrebuiltTemplateName() const
{
    return KPrebuiltCommunityExportTemplateName;
}

ErrCode CommunityCtl::onImportDataStart(const QString &importName, int importFileType, const QString &fname)
{
    tracein;
    logi("start import '%s', fname '%s'", STR2CHA(importName), STR2CHA(fname));
    mImportFields.clear();
    traceout;
    return ErrNone;
}

ErrCode CommunityCtl::getActivePersonList(const QString &communityUid, QList<DbModel*>& outList)
{
    tracein;
    ErrCode err = getPersonList(communityUid, outList, MODEL_STATUS_MAX);
    traceout;
    return err;
}

ErrCode CommunityCtl::getPersonList(const QString &communityUid, QList<DbModel *> &outList, qint64 modelStatus)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdl = nullptr;
    QList<DbModel *> items;
    logd("get list of person for community uid '%s', status 0x%x", STR2CHA(communityUid), modelStatus);
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
            outList.append(items);
        } else {
            logw("not found list person of communit uid '%s'", STR2CHA(communityUid));
        }
    }
    if (err != ErrNone) {
        loge("Get list of active person failed, err=%d", err);
        // we don't have error code return, so report error here.
        REPORTERRCTL->reportErr(QObject::tr("Lỗi truy vấn danh sách nữ tu của cộng đoàn"), err, true);
    } else {
        logd("Got %lld items", items.size());
    }

    traceout;
    return err;
}

ErrCode CommunityCtl::addPerson2Community(const Community *comm, const Person *per,
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
        err = hdl->addPerson2Community(comm, per, status, startdate, enddate, remark);
    }
    traceret(err);
    return err;
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

DbModelHandler *CommunityCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlCommunity);
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
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
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
                QString field = mImportFields[i++];
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

    logd("err = %d", err);
    return model;
}

DbModel *CommunityCtl::doImportOneItem(const QString& importName, int importFileType,
                                       const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode err = ErrNone;
    DbModel* model = nullptr;
    logd("idx = %d", idx);
    if (importName == KModelHdlCommunity)
        model = Community::build();
    else { // TODO: import person to community
        err = ErrNotSupport;
        loge("import '%s' not support", STR2CHA(importName));
    }
    if (err == ErrNone) {
        foreach (QString field, items.keys()) {
            QString value = items.value(field);
            logd("Import field %s", field.toStdString().c_str());
            logd("Import value %s", value.toStdString().c_str());
            err = model->onImportParseDataItem(importName, importFileType, field, value, idx);
            if (err != ErrNone) {
                loge("on import item failed, %d", err);
                break;
            }
        }
     }

    if (err != ErrNone) {
        if (model) {
            delete model;
            model = nullptr;
        }
    }
    traceout;
    return model;
}

