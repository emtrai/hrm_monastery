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
 * Filename: dbmetainfo.h
 * Author: Anh, Ngo Huy
 * Created date:4/5/2023
 * Brief:
 */
#ifndef DBMETAINFO_H
#define DBMETAINFO_H

#include <QString>
#include <QHash>
#include "errcode.h"

class DbMetaInfo
{
public:
    DbMetaInfo();
    void setAppVersion(qint64 version);
    void setDbVersion(qint64 version);
    void addTableVersion(const QString& tableName, qint64 version);
    qint64 tableVersion(const QString& tableName, bool* ok = nullptr);
    QString toJson(bool* ok = nullptr) const;
    ErrCode saveJson(const QString& fpath) const;
    ErrCode fromJsonFile(const QString& fpath);
    qint64 appVersion() const;

    qint64 dbVersion() const;

    const QString &appVersionString() const;
    void setAppVersionString(const QString &newAppVersionString);

private:
    QString mAppVersionString;
    qint64 mAppVersion;
    qint64 mDbVersion;
    QHash<QString, qint64> mListTableVersion;
};

#endif // DBMETAINFO_H
