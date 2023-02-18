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

CommunityDeptCtl* CommunityDeptCtl::gInstance = nullptr;

CommunityDeptCtl::CommunityDeptCtl():CommonCtl(KModelHdlCommDept)
{
    traced;
}

CommunityDeptCtl *CommunityDeptCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new CommunityDeptCtl;
    }
    return gInstance;
}

CommunityDept* CommunityDeptCtl::parseOneItem(const QJsonObject& jobj)
{
    CommunityDept* ret = new CommunityDept();
    traced;
    if (jobj.contains(JSON_CODE)){
        QString tmp = jobj[JSON_CODE].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setCode(tmp);
    }

    if (jobj.contains(JSON_NAME)){
        QString tmp = jobj[JSON_NAME].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setName(tmp);

    }

    if (jobj.contains(JSON_ADDR)){
        QString tmp = jobj[JSON_ADDR].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setAddr(tmp);
    }

    if (jobj.contains(JSON_TEL)){
        QString tmp = jobj[JSON_TEL].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setTel(tmp);
    }

    if (jobj.contains(JSON_EMAIL)){
        QString tmp = jobj[JSON_EMAIL].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setEmail(tmp);

    }

    if (jobj.contains(JSON_ESTABLISH)){
        QString tmp = jobj[JSON_ESTABLISH].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setEstablishDate(tmp.toInt());
    }

    if (jobj.contains(JSON_STATUS)){
        QString tmp = jobj[JSON_STATUS].toString().trimmed();
        if (!tmp.isEmpty()) {
            int status = QString(tmp).toInt();
            // TODO: handle return value
            ret->setStatus(status);
        }
    }

    return ret;

}
ErrCode CommunityDeptCtl::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    if (ftype == KFileTypeJson) {

        logd("Load file %s", fpath.toStdString().c_str());
        QFile loadFile(fpath);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            loge("Couldn't open file %s", fpath.toStdString().c_str());
            ret = ErrFileRead;
        }

        if (ret == ErrNone){
            logd("Parse json");
            QByteArray saveData = loadFile.readAll();
            logd("saveData length %d", (int)saveData.length());

            QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);

            logd("loadDoc isEmpty %d", loadDoc.isEmpty());
            QJsonObject jRootObj = loadDoc.object();
            qWarning() << jRootObj;
            if (jRootObj.contains(JSON_DEPARTMENTS) && jRootObj[JSON_DEPARTMENTS].isArray()) {
                QJsonArray jlist = jRootObj[JSON_DEPARTMENTS].toArray();
                for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                    QJsonObject jObj = jlist[levelIndex].toObject();
                    CommunityDept* dept = parseOneItem(jObj);
                    if (dept->isValid())
                        dept->save();

                    delete dept;

                }
            } else {
                loge("Invalid data, not found %s", JSON_DEPARTMENTS);
                ret = ErrInvalidData;
            }
        }

    } else {
        ret = ErrNotSupport;
    }
    tracedr(ret);
    return ret;
}
ErrCode CommunityDeptCtl::addNew(DbModel *model)
{
    traced;
    // TODO: auto add management board department, to contain list of managmenet member and role
    // at least, we need to have CEO for community
    return Controller::addNew(model);
}

ErrCode CommunityDeptCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}

QList<DbModel *> CommunityDeptCtl::getDepartList(const QString &communityUid)
{
    traced;
    DbCommDeptModelHandler* model =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
    QList<DbModel *> items = model->getListDept(communityUid);
    return items;
}

ErrCode CommunityDeptCtl::addPerson2CommunityDept(PersonDept* perdept)
{
    traced;
    DbCommDeptModelHandler* model =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
    return model->addPerson2Department(perdept);
}

const char *CommunityDeptCtl::getPrebuiltFileName()
{
    return KPrebuiltCommunityDeptJsonFileName;
}

const char *CommunityDeptCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
}

QList<DbModel *> CommunityDeptCtl::getItemFromDb()
{
    return getModelHandler()->getAll(&CommunityDept::build);
}

DbModelHandler *CommunityDeptCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlCommDept);
}

ErrCode CommunityDeptCtl::markModelDelete(DbModel *model)
{
    traced;
    // TODO: implement delete
    // TODO: don't delete root community
    ASSERT(false, "implement markModelDelete");
    return ErrNotImpl;
}

ErrCode CommunityDeptCtl::deleteModel(DbModel *model)
{
    traced;
    // TODO: implement delete
    // TODO: don't delete root community, judge basing on level
    ASSERT(false, "implement deleteModel");
    return ErrNotImpl;
}

DbModel *CommunityDeptCtl::doImportOneItem(int importFileType, const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    CommunityDept* comm = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
        }
    } else {

        comm = (CommunityDept*)CommunityDept::build();
        foreach (QString item, items) {
            QString field = mImportFields[i++];
            logd("Import field %s", field.toStdString().c_str());
            ret = comm->onImportItem(importFileType, field, item, idx);
        }
    }

    tracedr(ret);
    return comm;
}

DbModel *CommunityDeptCtl::doImportOneItem(int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    CommunityDept* comm = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    comm = (CommunityDept*)CommunityDept::build();
    foreach (QString field, items.keys()) {
        QString value = items.value(field);
        logd("Import field %s", field.toStdString().c_str());
        logd("Import value %s", value.toStdString().c_str());
        ret = comm->onImportItem(importFileType, field, value, idx);
    }

    tracedr(ret);
    return comm;
}



const QList<DbModel *> CommunityDeptCtl::getDeptList(const QString& communityUid)
{
    traced;
    logd("community uid '%s'", communityUid.toStdString().c_str());
    QList<DbModel*> dept;
    DbCommDeptModelHandler* model =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
    dept = model->getListDept(communityUid);
    logd("found %d", dept.count());
    return dept;
}

const QList<DbModel *> CommunityDeptCtl::getPersonList(const QString &deptUid)
{
    traced;
    DbCommDeptModelHandler* model =  dynamic_cast<DbCommDeptModelHandler*>(DB->getModelHandler(KModelHdlCommDept));
    QList<DbModel *> items = model->getListPerson(deptUid);

    tracede;
    return items;
}
