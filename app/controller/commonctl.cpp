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


QList<DbModel *> CommonCtl::getAllItemsFromDb(qint64 status, int from, int noItems, int* total)
{
    traced;
    DbModelHandler* hdl = getModelHandler();
    QList<DbModel *> list;
    if (hdl) {
        list = hdl->getAll(getMainBuilder(), status, nullptr, from, noItems, total);
    } else {
        loge("not found default handler");
    }
    logd("item counter = %d", list.size());
    return list;
}


void CommonCtl::clearCacheItemList()
{
    traced;
    logd("clear %ld item in cache", mCacheItemList.size());
    foreach(DbModel* item, mCacheItemList) {
        delete item;
    }
    mCacheItemList.clear();
    tracede;
}

const char *CommonCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
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


ErrCode CommonCtl::reloadDb()
{
    traced;
    ErrCode err = ErrNone;
    tracedr(err);
    return err;
}


QList<DbModel*> CommonCtl::getAllItems(bool reload, int from, int noItems, int* total)
{
    traced;
    logd("reload %d", reload);
    if (reload || mCacheItemList.empty()) {
        logd("reload from db");
        clearCacheItemList();
        QList<DbModel*> items = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total);
        mCacheItemList = items;
    }
    QList<DbModel*> list;
    foreach (DbModel* item, mCacheItemList) {
        list.append(item->clone());
    }
    tracede;
    return list;
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

//DbModelBuilder CommonCtl::getMainBuilder()
//{
//    FAIL("DEFAULT getMainBuilder, should not be called");
//    return nullptr;
//}

DbModel *CommonCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    ErrCode err = ErrNone;
    DbModel* item = nullptr;
    QStringList* itemList = nullptr;
    qint32 noItem = 0;
    qint32 idx = 0;
    QString nameId;
    QString name;
    QString remark;
    if (!items) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone && fmt != KDataFormatStringList) {
        err = ErrNotSupport;
        loge("invalid data format '%s", STR2CHA(fmt));
    } else {
        itemList = (QStringList*) items;
        noItem = itemList->length();
        logd("noItem %d", noItem);
    }

    if (err == ErrNone && (noItem < 2)) { // require name id, name
        err = ErrShortData;
        loge("Not enouth field, %d < 3", noItem);
    }

    if (err == ErrNone) {
        nameId = itemList->at(idx++);
        name = itemList->at(idx++);
        if (noItem > idx) {
            remark = itemList->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
        }

        logd("nameId '%s'", STR2CHA(nameId));
        logd("name '%s'", STR2CHA(name));
        if (nameId.isEmpty() || name.isEmpty()) {
            err = ErrInvalidData;
            loge("data is invalid, nameid/countrynameid or name is empty");
        }
    }
    if (err == ErrNone) {
        DbModelBuilder builder = getMainBuilder();
        if (builder) {
            item = builder();
            if (!item) {
                loge("No memory");
                err = ErrNoMemory;
            }
        } else {
            loge("No main builder");
            err = ErrNotAvail;
        }
    }

    if (err == ErrNone) {
        item->setNameId(nameId);
        item->setName(name);
        if (!remark.isEmpty()) {
            item->setRemark(remark);
        }
    } else {
        loge("failed to import/build model, err=%d", err);
        if (item) {
            delete item;
            item = nullptr;
        }
    }
    tracede;
    return item;
}
