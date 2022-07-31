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
 * Filename: filectl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "filectl.h"
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include "std.h"
#include "defs.h"
FileCtl* FileCtl::gInstance = nullptr;

FileCtl *FileCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new FileCtl();
    }
    return gInstance;

}

QString FileCtl::getAppDataDir(const QString& subDir)
{
    QString appPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDirPath(appPath);
    logd("App path %s", appPath.toStdString().c_str());

    if (!appDirPath.exists()){
        logi("App path not exist, create new one");
        appDirPath.mkpath(appDirPath.absolutePath());
    }
    if (subDir.isEmpty()){
        return appPath;
    }
    else{
        return appDirPath.filePath(subDir);
    }
}

QString FileCtl::getAppDataDir(const char *subDir)
{
    if (subDir == nullptr){
        return getAppDataDir(QString());
    }
    else{
        return getAppDataDir(QString(subDir));
    }
}

QString FileCtl::getAppInstallDir(const QString& subDir)
{
    QString dir = QCoreApplication::applicationDirPath();
    if (subDir.isEmpty())
        return dir;
    else
        return QDir(dir).filePath(subDir);
}


QString FileCtl::getOrCreatePrebuiltDataDir()
{
    QString prebuildDataDir = FileCtl::getAppDataDir(KPrebuiltDirName);
    QDir dir(prebuildDataDir);
    if (!dir.exists()){
        logi("App path not exist, create new one");
        dir.mkpath(prebuildDataDir);
    }
    logd("Prebuilt Data Dir %s", prebuildDataDir.toStdString().c_str());
    return prebuildDataDir;
}
QString FileCtl::getFullFilePath(const QString &fileName)
{
    return QDir(getAppDataDir()).filePath(fileName);
}


QString FileCtl::getPrebuiltDataFile(const QString& fname)
{
    QFile file(QString(":/data/%1").arg(fname));
    QString prebuiltDir = getOrCreatePrebuiltDataDir();
    QFile saintFile = QFile(QDir(prebuiltDir).filePath(fname));
    QString path;
    logd("Resource prebuilt file %s", file.fileName().toStdString().c_str());
    if (file.exists())
    {
        if (!saintFile.exists()){
            logd("%s file not exist, do copy to %s",
                 fname.toStdString().c_str(),
                 saintFile.fileName().toStdString().c_str());
            file.copy(saintFile.fileName());
        }
        else{
            logd("%s file already exist", fname.toStdString().c_str());
            // TODO: should notifify or do update silently????
        }
        path = saintFile.fileName();
    }
    else{
        loge("Prebuit %s file not exisst", fname.toStdString().c_str());
    }
    return path;
}

FileCtl::FileCtl()
{

}
