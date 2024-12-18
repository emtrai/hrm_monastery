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
#include <QTemporaryDir>

#define DEFAULT_MAX_DEPTH_SEARCH_FILE (10)

class FileCtl:public Controller
{
public:
    virtual ~FileCtl();
    static FileCtl* getInstance();
    virtual void init();


    /**
     * @brief Get subdir in app data dir
     * @param dirName
     * @param subDir
     * @return
     */
    static QString getAppDataSubdir(const QString& dirName, const QString& subDir);

    /**
     * @brief working data subdir in app data dir
     * @param subDir
     * @return
     */
    static QString getAppWorkingDataDir(const QString& subDir);
    static QString getAppWorkingDataDir();

    /**
     * @brief backup subdir in app data dir
     * @param subDir
     * @return
     */
    static QString getAppBackupDataDir(const QString& subDir);
    static QString getAppBackupDataDir();

    static ErrCode copyFile(const QString& src, const QString& dest, bool force = false);
    /**
     * @brief remove file
     *        if markRemove is true, file is not removed, from FS, but change file name
     * @param fpath
     * @param markRemove false to remove file, true not to remove file, just change name
     * @return
     */
    static ErrCode removeFile(const QString& fpath, bool markRemove = false);

    static QString getAppImageDataRootDir();
    static QString getAppImageDataDir(const QString& subDir = nullptr);
    static QString getAppPeopleImageDataDir();

    static QString getAppLocalDataDir(const QString& subDir);
    static QString getAppDataDir(const QString& subDir);
    static QString getAppDataDir();
    static QString getSystemAppDataDir(const QString& subDir);

    QString tmpDataDir(const QString& subDir);
    QString tmpDataFile(const QString& fname);
    static QString getTmpDataDir(const QString& subDir = nullptr);
    static QString getTmpDataFile(const QString& fname);

    static QString getAppInstallDir(const QString& subDir = nullptr);

    static QString getOrCreatePrebuiltDataDir();
    static QString getPrebuiltDataFile(const QString& fname);
    static QString getPrebuiltDataFilePath(const QString& fname);
    static QString getPrebuiltDataFileHashPath(const QString& fname);

    // Check if prebuilt data file match with hash file
    static bool checkPrebuiltDataFileHash(const QString& fname);
    static ErrCode readPrebuiltDataFileHash(const QString& fname, QString* hashOut);
    static ErrCode readFileString(const QString& fpath, QString& out);
    static ErrCode updatePrebuiltDataFileHash(const QString& fname);
    static QString getFullFilePath(const QString &fileName);

    static ErrCode writeStringToFile(const QString& content, const QString&fpath);
    static ErrCode checkAndUpdatePrebuiltFile(const QString& fname, bool backup);
    static QString getUpdatePrebuiltDataFilePath(const QString name, bool lang = true);

    const QString &tmpDirPath() const;

    static void cleanUpData();
    virtual QString getName() const;
    virtual ErrCode onLoad();
    virtual void onUnload();

    static ErrCode getListFilesRecursive(QDir dir, QList<QFileInfo>& fileInfos,
                                         int depth = 0, int maxdept = DEFAULT_MAX_DEPTH_SEARCH_FILE);

private:
    FileCtl();

private:
    static FileCtl* gInstance;
    QString mTmpDirPath;
    QTemporaryDir mTmpDir;
};

#endif // FILECTL_H
