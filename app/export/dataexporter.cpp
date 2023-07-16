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
 * Filename: idataexporter.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "dataexporter.h"
#include "logger.h"
#include "defs.h"
#include "exportfactory.h"
#include "utils.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "exporttype.h"
#include <QMap>
#include "jsondefs.h"

DataExporter::DataExporter()
{
    tracein;
}

ErrCode DataExporter::exportTemplatePath(FileExporter* exporter,
                                         const QString& name,
                                         QString& fpath,
                                         QString* ftype) const
{
    tracein;
    return ErrNone;
}

ErrCode DataExporter::getListTemplateExportKeywords(FileExporter *exporter,
                                                    const QString& datatype,
                                                    QList<QPair<QString,QString>>& outMap) const
{
    tracein;
    ErrCode err = ErrNone;

    QString fpath;
    QString ftype;

    if (exporter) {
        err = exportTemplatePath(exporter, datatype, fpath, &ftype);
    } else {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone && fpath.isEmpty()) {
        err = ErrInvalidArg;
        loge("not found template file");
    }
    logd("Load file '%s', ftype '%s'", STR2CHA(fpath), STR2CHA(ftype));

    if (err == ErrNone){
        if (ftype == KFileTypeJson) {
            err = parseJsonExportTemplate(fpath, outMap);
        } else if (ftype == KFileTypeCSV) {
            err = parseCsvExportTemplate(fpath, outMap);
        } else {
            err = ErrNotSupport;
            loge("Base class support json/csv format only");
        }
    }
    traceret(err);
    return err;
}

const QStringList DataExporter::getListExportKeyWord() const
{
    tracein;
    return QStringList();
}

ErrCode DataExporter::getExportDataString(const QString &item, QString* data) const
{
    tracein;
    loge("Not support here");
    return ErrNotSupport;
}

ErrCode DataExporter::getExportDataString(const QString &item, const DbModel *data, QString *exportData) const
{
    tracein;
    loge("Not support here");
    ASSERT(false, "CAN NOT GET EXPORT DATA HERE, MUST BE IMPLEMENTED BY DERIVED CLASS");
    return ErrNotSupport;
}

quint64 DataExporter::getExportTypeList()
{
    loge("get eport type list from base class, SHOULD be implemented in derived class");
    return ExportType::EXPORT_NONE;
}

ErrCode DataExporter::parseJsonExportTemplate(const QString& fpath, QList<QPair<QString,QString>> &outMap) const
{
    tracein;
    ErrCode err = ErrNone;

    QFile loadFile;
    QByteArray fileData;


    if (fpath.isEmpty()){
        err = ErrInvalidArg;
        loge("Invalid param");
    }

    if (err == ErrNone){

        logd("json file %s", STR2CHA(fpath));
        loadFile.setFileName(fpath);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            loge("Couldn't open file %s", fpath.toStdString().c_str());
            err = ErrFileRead;
        }
    }

    if (err == ErrNone){
        // TODO: ASSUME only JSON, how about other????
        logd("Parse json");
        fileData = loadFile.readAll();
        if (!fileData.isEmpty()) {
            logd("fileData length %d", (int)fileData.length());
        } else {
            err = ErrNoData;
            loge("file '%s' is empty?", STR2CHA(fpath));
        }
    }

    if (err == ErrNone) {
        QJsonDocument loadDoc = QJsonDocument::fromJson(fileData);

        logd("loadDoc isEmpty %d", loadDoc.isEmpty());
        QJsonObject jRootObj = loadDoc.object();
        if (jRootObj.contains(JSON_ITEMS) && jRootObj[JSON_ITEMS].isArray()) {
            QJsonArray jlist = jRootObj[JSON_ITEMS].toArray();
            for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                QJsonObject jObj = jlist[levelIndex].toObject();
                QString id;
                QString name;
                // id
                if (jObj.contains(JSON_ID)){
                    id = jObj[JSON_ID].toString().trimmed();
                } else {
                    loge("lack of id field %s", JSON_ID);
                }
                if (id.isEmpty()){
                    loge("invalid template, id not found/empty");
                    err = ErrInvalidData;
                    break;
                }
                //name
                if (jObj.contains(JSON_NAME)){
                    name = jObj[JSON_NAME].toString().trimmed();
                }
                // not found name, get default name???
                if (name.isEmpty()) {
                    name = exportItem2Name(id);
                }
                if (name.isEmpty()) {
                    name = id;
                }
                logd("id=%s, name=%s", STR2CHA(id), STR2CHA(name));
                if (!id.isEmpty() && !name.isEmpty()) {
                    outMap.append(qMakePair(id, name));
                } else {
                    err = ErrInvalidData;
                    loge("Export template lacks field of id or name");
                    break;
                }
            }
        } else {
            loge("Invalid data, not found %s", JSON_ITEMS);
            err = ErrInvalidData;
        }
    }
    loadFile.close();
    traceret(err);
    return err;
}

ErrCode DataExporter::parseCsvExportTemplate(const QString &fpath, QList<QPair<QString,QString>> &outMap) const
{
    tracein;
    ErrCode ret = ErrNone;
    qint32 cnt = 0;
    QStringList items;
    QString finalData;
    QHash<QString, QString> keywordMap;
    logd("templatePath %s", fpath.toStdString().c_str());

    if (fpath.isEmpty()) {
        ret = ErrInvalidData;
        loge("Export CSV failed, invalid data");
    }

    if (ret == ErrNone) {
        logd("parse csv file %s", fpath.toStdString().c_str());
        ret = Utils::parseCSVFile(fpath,
            [](const QStringList& items, void* caller, void* param, quint32 idx){
                tracein;
                logd("callback from lamda when parse csv file for export csv list, idx=%d", idx);
                return ErrCancelled; // we assume 1st line is headers, so don't need to read more
            },
            nullptr, nullptr, CSV_LIST_ITEM_SPLIT, &cnt, &items);
    }
    if (ret == ErrNone || ret == ErrCancelled) {
        if (items.length() == 0) {
            ret = ErrNoData;
            loge("No header found");
        } else {
            ret = ErrNone; // if ErrCancelled is intended, to stop reading, focus on 1st line only
        }
    }
    if (ret == ErrNone) {
        logd("get header");
        foreach (QString item, items) {
            QString id = item.trimmed();
            QString name = exportItem2Name(id);
            outMap.append(qMakePair(id, name));
        }
    }
    traceret(ret);
    return ret;
}
