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
 * Filename: backupmetainfo.h
 * Author: Anh, Ngo Huy
 * Created date:4/1/2023
 * Brief:
 */
#ifndef BACKUPMETAINFO_H
#define BACKUPMETAINFO_H

#include <QString>
#include <QHash>
#include "errcode.h"

#define BACKUP_META_VERSION_MAJOR (0)
#define BACKUP_META_VERSION_MINOR (0)
#define BACKUP_META_VERSION_PATH (1)
#define BACKUP_META_VERSION ((BACKUP_META_VERSION_MAJOR << 24) | (BACKUP_META_VERSION_MINOR << 8) | (BACKUP_META_VERSION_PATH))


class BackupMetaInfo
{
public:
    BackupMetaInfo();
    void setDateTime(const QString& time = nullptr);
    void addFile(const QString& rpath, const QString& hash = nullptr);
    QString toJson(bool* ok = nullptr) const;
    ErrCode saveJson(const QString& fpath) const;
private:
    QString mDateTime;
    QHash<QString, QString> mListFiles;
    qint32 mVersion;
};

#endif // BACKUPMETAINFO_H
