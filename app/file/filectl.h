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
 * Filename: filectl.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef FILECTL_H
#define FILECTL_H

#include "controller.h"
#include <QFile>
#include "errcode.h"
class FileCtl:public Controller
{
public:
    static FileCtl* getInstance();

    static QString getAppDataDir(const QString& subDir);
    static QString getAppDataDir(const char* subDir = nullptr);
    static QString getAppInstallDir(const QString& subDir);

    static QString getOrCreatePrebuiltDataDir();
    static QString getPrebuiltDataFile(const QString& fname);
    static QString getPrebuiltDataFilePath(const QString& fname);
    static QString getPrebuiltDataFileHashPath(const QString& fname);

    // Check if prebuilt data file match with hash file
    static bool checkPrebuiltDataFileHash(const QString& fname);
    static ErrCode readPrebuiltDataFileHash(const QString& fname, QString* hashOut);
    static ErrCode updatePrebuiltDataFileHash(const QString& fname);
    static QString getFullFilePath(const QString &fileName);

    static ErrCode writeStringToFile(const QString& content, const QString&fpath);

private:
    FileCtl();

private:
    static FileCtl* gInstance;
};

#endif // FILECTL_H
