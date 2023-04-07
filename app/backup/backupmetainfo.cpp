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
 * Filename: backupmetainfo.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/1/2023
 * Brief:
 */
#include "backupmetainfo.h"
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "errcode.h"
#include "logger.h"
#include "utils.h"

BackupMetaInfo::BackupMetaInfo()
{
    mVersion = BACKUP_META_VERSION;
}

void BackupMetaInfo::setDateTime(const QString &time)
{
    if (time.isEmpty()) {
        mDateTime = QDateTime::currentDateTime().toString();
    } else {
        mDateTime = time;
    }
}

void BackupMetaInfo::addFile(const QString& rpath, const QString& hash)
{
    tracein;
    logd("add rfile='%s', hash='%s'", STR2CHA(rpath), STR2CHA(hash));
    mListFiles.insert(rpath, hash);
    traceout;
}

QString BackupMetaInfo::toJson(bool* ok) const
{
    QJsonObject jRoot;
    tracein;
    jRoot.insert("version", mVersion);
    jRoot.insert("datetime", mDateTime);
    jRoot.insert("encryptionalgo", "");
    jRoot.insert("signaturealgo", "sha256"); // just for reserve
    jRoot.insert("certhash", ""); // just for reserve
    jRoot.insert("keyhash", ""); // just for reserve
    jRoot.insert("keysalt", ""); // just for reserve
    QJsonArray jFiles;
    foreach (QString rpath, mListFiles.keys()) {
        logd("rfile='%s', hash='%s'", STR2CHA(rpath), STR2CHA(mListFiles[rpath]));
        QJsonObject jfile;
        jfile.insert("rpath", rpath);
        jfile.insert("hash", mListFiles[rpath]);
        jfile.insert("signature", "");
        jFiles.push_back(QJsonValue(jfile));
    }

    jRoot.insert("files", jFiles);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jRoot);
    QString jstr = jsonDoc.toJson();
    logd("json string: \n%s", STR2CHA(jstr));
    if (ok) *ok = true;
    traceout;
    return jstr;
}

ErrCode BackupMetaInfo::saveJson(const QString &fpath) const
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
