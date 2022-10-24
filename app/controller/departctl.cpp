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
 * Filename: departctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "departctl.h"

#include "logger.h"
#include "department.h"
#include "dbdepartmentmodelhandler.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"
#include <QFile>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GET_INSTANCE_IMPL(DepartCtl)

DepartCtl::DepartCtl()
{
    traced;
}

// Format: Country short name, Province name[, <parent province if any>]
DbModel *DepartCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Department* item = new Department();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setShortName(itemList->at(idx++));
    QString nameid = itemList->at(idx++);
    item->setNameId(nameid + item->shortName());
    item->setName(itemList->at(idx++));
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}


const QList<Department *> DepartCtl::getDeptList(const QString& communityUid)
{
    traced;
    if (communityUid.isEmpty()) {
        logd("empty community uid, return all");
        return mDeptList;
    } else {
        logd("community uid '%s'", communityUid.toStdString().c_str());
        QList<Department*> dept;
        foreach(Department* item, mDeptList) {
            if (item->communityUid() == communityUid) {
                dept.append(item);
            }
        }
        logd("found %d", dept.count());
        return dept;
    }
}

const QList<DbModel *> DepartCtl::getPersonList(const QString &deptUid)
{
    traced;
    DbDepartmentModelHandler* model =  dynamic_cast<DbDepartmentModelHandler*>(DB->getModelHandler(KModelHdlDept));
    QList<DbModel *> items = model->getListPerson(deptUid);

    tracede;
    return items;
}

Department* DepartCtl::parseOneItem(const QJsonObject& jobj)
{
    Department* ret = new Department();
    traced;
    if (jobj.contains(JSON_ID)){
        QString tmp = jobj[JSON_ID].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setUid(tmp);
    }

    if (jobj.contains(JSON_NAME)){
        QString tmp = jobj[JSON_NAME].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setName(tmp);

    }

    if (jobj.contains(JSON_COMMUNITY_UID)){
        QString tmp = jobj[JSON_COMMUNITY_UID].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setCommunityUid(tmp);
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

    if (jobj.contains(JSON_EMAIL)){
        QString tmp = jobj[JSON_EMAIL].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setBrief(tmp);

    }

    if (jobj.contains(JSON_REMARK)){
        QString tmp = jobj[JSON_REMARK].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setRemark(tmp);
    }
    // TODO: set status

    if (jobj.contains(JSON_ESTABLISH)){
        QString tmp = jobj[JSON_ESTABLISH].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setEstablishDateFromString(tmp);
    }

    return ret;

}

ErrCode DepartCtl::parsePrebuiltFile(const QString &fpath, const QString &ftype)
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
                    Department* depart = parseOneItem(jObj);
                    if (depart->isValid()) {
                        logd("Save %s", depart->name().toStdString().c_str());
                        depart->save();
                    }

                    delete depart;

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

void DepartCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltDeptJsonFileName, KFileTypeJson);
    // TODO: should do lazy load??? load all consume much memory
    QList items = DB->getModelHandler(KModelHdlDept)->getAll(&Department::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Department* item = (Department*)model;
        mDeptList.append(item);
    }
    tracede;
}
