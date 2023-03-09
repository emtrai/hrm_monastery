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
#include "person.h"
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
#include "dbcommdepatmodelhandler.h"
#include "persondept.h"
#include "communityctl.h"
#include "departctl.h"

GET_INSTANCE_IMPL(CommunityDeptCtl)

CommunityDeptCtl::CommunityDeptCtl():ModelController(KModelHdlCommDept)
{
    traced;
}

CommunityDeptCtl::~CommunityDeptCtl()
{
    traced;
}

CommunityDept* CommunityDeptCtl::onJsonParseOneItem(const QJsonObject& jobj, bool* ok )
{
    traced;
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
        JSON_GET_TO_SET_STR(jobj, JSON_ESTABLISH, ret->setEstablishDateFromString);
        JSON_GET_TO_SET_STR(jobj, JSON_CLOSEDATE, ret->setClosedDateFromString);
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
                ret->setStatus(status);
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

    return ret;

}

ErrCode CommunityDeptCtl::onImportStart(const QString &importName, int importFileType, const QString &fname)
{
    traced;
    logi("start import '%s', fname '%s'", STR2CHA(importName), STR2CHA(fname));
    mImportFields.clear();
    tracede;
    return ErrNone;
}

ErrCode CommunityDeptCtl::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    logi("Parse prebuilt file for community dept, fpath '%s', ftype '%s'",
         STR2CHA(fpath), STR2CHA(ftype));

    if (fpath.isEmpty() || ftype.isEmpty()) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }
    if (ret == ErrNone) {
        if (ftype == KFileTypeJson) {

            logd("Load file %s", fpath.toStdString().c_str());
            QFile loadFile(fpath);
            QByteArray importData;

            if (!loadFile.open(QIODevice::ReadOnly)) {
                loge("Couldn't open file %s", fpath.toStdString().c_str());
                ret = ErrFileRead;
            }

            if (ret == ErrNone){
                logd("Parse json");
                importData = loadFile.readAll();
                logd("importData length %d", (int)importData.length());
                if (importData.size() == 0) {
                    ret = ErrNoData;
                    loge("not json data to parse");
                }
            }

            if (ret == ErrNone) {
                QJsonDocument loadDoc = QJsonDocument::fromJson(importData);

                logd("loadDoc isEmpty %d", loadDoc.isEmpty());
                QJsonObject jRootObj = loadDoc.object();
                if (jRootObj.contains(JSON_DEPARTMENTS) && jRootObj[JSON_DEPARTMENTS].isArray()) {
                    QJsonArray jlist = jRootObj[JSON_DEPARTMENTS].toArray();
                    for (int levelIndex = 0; levelIndex < jlist.size() && (ret == ErrNone); ++levelIndex) {
                        QJsonObject jObj = jlist[levelIndex].toObject();
                        bool ok = false;
                        CommunityDept* dept = onJsonParseOneItem(jObj, &ok);
                        if (dept) {
                            if (ok && dept->isValid()) {
                                ret = dept->save();
                                if (ret == ErrExisted) { // already exist, mean ok
                                    ret = ErrNone;
                                    logw("'%s' already exist", STR2CHA(dept->toString()));
                                }
                            }

                            delete dept;

                        } else {
                            ret = ErrFailed;
                            loge("parse json one item failed");
                        }
                    }
                } else {
                    loge("Invalid data, not found %s", JSON_DEPARTMENTS);
                    ret = ErrInvalidData;
                }
            }

        } else {
            ret = ErrNotSupport;
            loge("ftype '%s' is not support", STR2CHA(ftype));
        }
    }
    tracedr(ret);
    return ret;
}

QList<DbModel *> CommunityDeptCtl::getListDept(const QString &communityUid, bool* ok)
{
    traced;
    QList<DbModel *> items;
    logd("get dept for community uid='%s'", STR2CHA(communityUid));
    if (communityUid.isEmpty()) {
        DbCommDeptModelHandler* modelHdl =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
        if (modelHdl) {
            items = modelHdl->getListDept(communityUid, MODEL_ACTIVE, ok);
        } else {
            if (ok) *ok = false;
            loge("not found commdept handler");
        }
    } else {
        if (ok) *ok = false;
        loge("Invalid community uid");
    }
    logd("items cnt=%lld", items.size());
    tracede;
    return items;
}

const QList<DbModel *> CommunityDeptCtl::getListPerson(const QString &commDeptUid, bool* ok)
{
    traced;
    QList<DbModel *> items;
    logd("get people for commDeptUid uid='%s'", STR2CHA(commDeptUid));
    if (commDeptUid.isEmpty()) {
        DbCommDeptModelHandler* modelHdl =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
        if (modelHdl) {
            items = modelHdl->getListPerson(commDeptUid, MODEL_ACTIVE, ok);
        } else {
            if (ok) *ok = false;
            loge("not found commdept handler");
        }
    } else {
        if (ok) *ok = false;
        loge("Invalid commDeptUid uid");
    }
    logd("items cnt=%lld", items.size());
    tracede;
    return items;
}

const char *CommunityDeptCtl::getPrebuiltFileName()
{
    return KPrebuiltCommunityDeptJsonFileName;
}

const char *CommunityDeptCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
}

DbModelHandler *CommunityDeptCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlCommDept);
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
                ret = model->onImportItem(importName, importFileType, field, item, idx);
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

    tracedr(ret);
    return model;
}

DbModel *CommunityDeptCtl::doImportOneItem(const QString& importName, int importFileType,
                                           const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    CommunityDept* model = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    if (importName == KModelHdlCommDept)
        model = (CommunityDept*)CommunityDept::build();
    else { // TODO: import person to community
        ret = ErrNotSupport;
        loge("import '%s' not support", STR2CHA(importName));
    }

    if (ret == ErrNone) {
        foreach (QString field, items.keys()) {
            QString value = items.value(field);
            logd("Import field %s", field.toStdString().c_str());
            logd("Import value %s", value.toStdString().c_str());
            ret = model->onImportItem(importName, importFileType, field, value, idx);
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

    tracedr(ret);
    return model;
}

