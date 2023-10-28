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
 * Filename: dbmetainfo.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/5/2023
 * Brief:
 */
#include "dbmetainfo.h"

#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "jsondefs.h"

DbMetaInfo::DbMetaInfo()
{
    traced;
}

void DbMetaInfo::setAppVersion(qint64 version)
{
    mAppVersion = version;
}

void DbMetaInfo::setDbVersion(qint64 version)
{
    mDbVersion = version;
}

void DbMetaInfo::addTableVersion(const QString &tableName, qint64 version)
{
    tracein;
    dbgtrace;
    dbgv("Add tbl '%s', ver 0x%llx", STR2CHA(tableName), version);
    if (!mListTableVersion.contains(tableName)) {
        logd("table not exist");
        mListTableVersion.insert(tableName, version);
    } else {
        logd("table existed, set new version");
        mListTableVersion[tableName] = version;
    }
    traceout;
}

qint64 DbMetaInfo::tableVersion(const QString &tableName, bool *ok)
{
    tracein;
    qint64 ver = 0;
    dbgtrace;
    dbgd("get table version for table '%s'", STR2CHA(tableName));
    if (mListTableVersion.contains(tableName)) {
        ver = mListTableVersion[tableName];
        logd("Tbl '%s', ver 0x%llx", STR2CHA(tableName), ver);
        if (ok) *ok = true;
    } else {
        logw("table '%s' not exist", STR2CHA(tableName));
        if (ok) *ok = false;
    }
    dbgd("version 0x%llx", ver);
    traceout;
    return ver;
}

QString DbMetaInfo::toJson(bool *ok) const
{
    QJsonObject jRoot;
    tracein;
    jRoot.insert("appversion", mAppVersion);
    jRoot.insert("appversionstr", mAppVersionString);
    jRoot.insert("dbversion", mDbVersion);
    QJsonArray jListTables;
    foreach (QString table, mListTableVersion.keys()) {
        logd("table='%s', version=%lld", STR2CHA(table), mListTableVersion[table]);
        QJsonObject jtable;
        jtable.insert("table", table);
        jtable.insert("version", mListTableVersion[table]);
        jListTables.push_back(QJsonValue(jtable));
    }

    jRoot.insert("tables", jListTables);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jRoot);
    QString jstr = jsonDoc.toJson();
    logd("json string: \n%s", STR2CHA(jstr));
    if (ok) *ok = true;
    traceout;
    return jstr;
}

ErrCode DbMetaInfo::saveJson(const QString &fpath) const
{
    tracein;
    ErrCode err = ErrNone;
    bool ok = false;
    QString jstr = toJson(&ok);
    QFile file(fpath);
    logi("Save metainfo to file '%s'", STR2CHA(fpath));
    if (!ok) {
        loge("conver json string failed");
        err = ErrFailedConvert;
    }
    if (err == ErrNone) {
        if (!file.open(QIODevice::WriteOnly)) {
            loge("failed to open save file '%s'", STR2CHA(fpath));
            err = ErrFileOpen;
        }
    }
    if (err == ErrNone) {
        file.write(jstr.toLocal8Bit());
    }
    file.close();
    traceret(err);
    return err;
}

ErrCode DbMetaInfo::fromJsonFile(const QString &fpath)
{
    tracein;
    ErrCode err = ErrNone;
    logi("Load meta json file %s", STR2CHA(fpath));
    QFile loadFile(fpath);
    QByteArray importData;

    if (!loadFile.open(QIODevice::ReadOnly)) {
        loge("Couldn't open file %s", STR2CHA(fpath));
        err = ErrFileRead;
    }

    if (err == ErrNone){
        logd("Parse json");
        importData = loadFile.readAll();
        logd("importData length %d", (int)importData.length());
        // TODO: too big data????
        if (importData.size() == 0) {
            err = ErrNoData;
            loge("No data to parse");
        }
    }

    if (err == ErrNone) {
        QJsonDocument loadDoc = QJsonDocument::fromJson(importData);

        logd("loadDoc isEmpty %d", loadDoc.isEmpty());
        QJsonObject jRootObj = loadDoc.object();
        JSON_GET_INT(jRootObj, JSON_APPVER, mAppVersion, 0);
        JSON_GET_INT(jRootObj, JSON_DBVER, mDbVersion, 0);
        JSON_GET_STR(jRootObj, JSON_APPVERSTR, mAppVersionString);

        if (jRootObj.contains(JSON_TABLES) && jRootObj[JSON_TABLES].isArray()) {
            QJsonArray jlist = jRootObj[JSON_TABLES].toArray();
            for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                logd("Table idx=%d", levelIndex);
                QJsonObject jObj = jlist[levelIndex].toObject();
                QString tblName;
                qint64 version = 0;
                JSON_GET_STR(jObj, JSON_TABLE, tblName);
                JSON_GET_INT(jObj, JSON_VERSION, version, 0);
                if (!tblName.isEmpty()) {
                    mListTableVersion[tblName] = version;
                }
            }
        } else {
            loge("Invalid json data, not found %s", JSON_TABLES);
            err = ErrInvalidData;
        }
    }
    loadFile.close();

    traceret(err);
    return err;
}

qint64 DbMetaInfo::appVersion() const
{
    return mAppVersion;
}

qint64 DbMetaInfo::dbVersion() const
{
    return mDbVersion;
}

const QString &DbMetaInfo::appVersionString() const
{
    return mAppVersionString;
}

void DbMetaInfo::setAppVersionString(const QString &newAppVersionString)
{
    mAppVersionString = newAppVersionString;
}
