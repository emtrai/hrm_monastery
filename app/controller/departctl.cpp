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
#include "dbcommdepatmodelhandler.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"
#include <QFile>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GET_INSTANCE_IMPL(DepartCtl)

DepartCtl::DepartCtl():CommonCtl(KModelHdlDept)
{
    traced;
}

const QList<DbModel *> DepartCtl::getAllDepartment()
{
    traced;
    return DB->getModelHandler(KModelHdlDept)->getAll(&Department::build);
}


DbModelBuilder DepartCtl::getMainBuilder()
{
    return &Department::build;
}

const char *DepartCtl::getPrebuiltFileName()
{
    return KPrebuiltDeptJsonFileName;
}

const char *DepartCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
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
    if (jobj.contains(JSON_NAME_ID)){
        QString tmp = jobj[JSON_NAME_ID].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setNameId(tmp);
    }

    if (jobj.contains(JSON_NAME)){
        QString tmp = jobj[JSON_NAME].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setName(tmp);

    }

    if (jobj.contains(JSON_REMARK)){
        QString tmp = jobj[JSON_REMARK].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setRemark(tmp);
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
