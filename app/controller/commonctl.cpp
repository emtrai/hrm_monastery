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
 * Filename: commonctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#include "commonctl.h"

#include "community.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "dbmodel.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "filectl.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "exporttype.h"


QList<DbModel *> CommonCtl::getItemFromDb()
{
    traced;
    return QList<DbModel *>();
}


void CommonCtl::clearItemList(QList<DbModel *> *list)
{
    traced;
    if (list != nullptr) {
        list->clear(); // TODO: clear each item???
    }
    tracede;
}

ErrCode CommonCtl::getListExportKeywords(Exporter* exporter,
                                       QHash<QString, QString>& outMap) const
{
    traced;
    ErrCode err = ErrNone;

    QString fpath = exportTemplatePath(exporter);
    // TODO: ASSUME only JSON, how about other????

    logd("Load file %s", fpath.toStdString().c_str());
    QFile loadFile(fpath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        loge("Couldn't open file %s", fpath.toStdString().c_str());
        err = ErrFileRead;
    }

    if (err == ErrNone){
        logd("Parse json");
        QByteArray saveData = loadFile.readAll();
        logd("saveData length %d", (int)saveData.length());

        QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);

        logd("loadDoc isEmpty %d", loadDoc.isEmpty());
        QJsonObject jRootObj = loadDoc.object();
        qWarning() << jRootObj;
        if (jRootObj.contains(JSON_ITEMS) && jRootObj[JSON_ITEMS].isArray()) {
            QJsonArray jlist = jRootObj[JSON_ITEMS].toArray();
            for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                QJsonObject jObj = jlist[levelIndex].toObject();
                QString id;
                QString name;
                if (jObj.contains(JSON_ID)){
                    id = jObj[JSON_ID].toString().trimmed();
                }
                if (jObj.contains(JSON_NAME)){
                    name = jObj[JSON_NAME].toString().trimmed();
                }
                logd("id=%s, name=%s", STR2CHA(id), STR2CHA(name));
                if (!id.isEmpty() && !name.isEmpty()) {
                    outMap.insert(id, name);
                } else {
                    err = ErrInvalidData;
                    loge("Export template lacks field of id or name");
                }
            }
        } else {
            loge("Invalid data, not found %s", JSON_ITEMS);
            err = ErrInvalidData;
        }
    }
    tracede;
    return err;
}


ErrCode CommonCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}


ErrCode CommonCtl::loadFromDb()
{
    traced;
    ErrCode err = ErrNone;
    clearItemList(&mItemList);
    mItemList = getItemFromDb();
    foreach (DbModel* model, mItemList){
        model->dump();
    }
    logd("load %d item from db", mItemList.count());
    tracedr(err);
    return err;
}


const QList<DbModel *> CommonCtl::getAllItems(bool reload)
{
    traced;
    logd("reload %d", reload);
    if (reload) {
        loadFromDb();
    }
    return mItemList;
}

quint64 CommonCtl::getExportTypeList()
{
    return ExportType::EXPORT_CSV_LIST | ExportType::EXPORT_XLSX;
}

CommonCtl::CommonCtl()
{
    traced;
}

CommonCtl::CommonCtl(const QString &name):Controller(name)
{
    traced;
}
