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
    logd("Add tbl '%s', ver 0x%x", STR2CHA(tableName), version);
    if (!mListTableVersion.contains(tableName)) {
        logd("table not exist");
        mListTableVersion.insert(tableName, version);
    } else {
        logd("table existe, set new version");
        mListTableVersion[tableName] = version;
    }
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
        logd("table='%s', version=%d", STR2CHA(table), mListTableVersion[table]);
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
    logd("Save to file '%s'", STR2CHA(fpath));
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

ErrCode DbMetaInfo::fromJsonFile(const QString &fpath) const
{
    tracein;
    ErrCode err = ErrNone;
    logd("load metada from '%s'", STR2CHA(fpath));
    // TODO: implement it
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
