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
#include "logger.h"
#include "errcode.h"
#include "defs.h"
#include "utils.h"
#include "crypto.h"
#include <QTemporaryDir>


FileCtl* FileCtl::gInstance = nullptr;

FileCtl::~FileCtl()
{
    tracein;
}

FileCtl *FileCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new FileCtl();
        gInstance->init();
    }
    return gInstance;

}

void FileCtl::init()
{
    tracein;
}

QString FileCtl::getAppWorkingDataDir(const QString &subDir)
{
    QString appPath = FileCtl::getAppDataDir(KWorkingDirName);
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

QString FileCtl::getAppWorkingDataDir()
{
    traced;
    return getAppWorkingDataDir(QString());
}

QString FileCtl::getAppImageDataDir(const QString &subDir)
{
    tracein;
    QString imageRootDir = getAppImageDataRootDir();
    QDir appDirPath(imageRootDir);
    logd("imageRootDir %s", STR2CHA(imageRootDir));
    logd("subDir %s", STR2CHA(subDir));
    QString imgDirPath;
    if (!appDirPath.exists()){
        logi("imageRootDir not exist, create new one '%s'", STR2CHA(imageRootDir));
        appDirPath.mkpath(appDirPath.absolutePath());
    }
    if (subDir.isEmpty()){
        imgDirPath = imageRootDir;
    } else {
        imgDirPath = appDirPath.filePath(subDir);
        QDir imgDir(imgDirPath);
        if (!imgDir.exists()){
            logi("imgDir not exist, create new one '%s'", STR2CHA(imgDirPath));
            imgDir.mkpath(imgDir.absolutePath());
        }
    }
    logd("imgDirPath '%s'", STR2CHA(imgDirPath));
    traceout;
    return imgDirPath;
}

QString FileCtl::getAppPeopleImageDataDir()
{
    traced;
    return getAppImageDataDir(KPeopleImageDirName);
}

QString FileCtl::getAppImageDataRootDir()
{
    return getAppDataDir(KImageDirName);
}

QString FileCtl::getAppDataDir(const QString &subDir)
{
    QString fdir = getAppInstallDir(KDataDirName);
    QDir appDirPath(fdir);
    if (!subDir.isEmpty()) {
        fdir = appDirPath.filePath(subDir);
    }
    logd("create app data dir '%s'", STR2CHA(fdir));
    return fdir;
}

QString FileCtl::getAppLocalDataDir(const QString& subDir)
{

    static QString appPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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


QString FileCtl::getAppDataDir()
{
    return getAppDataDir(QString());
}

QString FileCtl::tmpDataDir(const QString &subDir)
{
    tracein;
    QString fpath = mTmpDir.path();
    logd("create tmp data dir with subDir '%s'", STR2CHA(subDir));
    if (!subDir.isEmpty()) {
        fpath = mTmpDir.filePath(subDir);
    }
    QDir dir(fpath);
    if (!dir.exists()){
        logi("App path not exist, create new one");
        dir.mkpath(fpath);
    }
    logd("create tmp data dir '%s'", STR2CHA(fpath));
    traceout;
    return fpath;
}

QString FileCtl::tmpDataFile(const QString &fname)
{
    tracein;
    return mTmpDir.filePath(fname);
}

QString FileCtl::getTmpDataDir(const QString& subDir)
{
    tracein;
    // TODO: implement it, this is just termprary processing
    logd("subDir '%s'", STR2CHA(subDir));
    QString tmpDirpath = getInstance()->tmpDataDir(subDir);
    logd("tmpDirpath '%s'", STR2CHA(tmpDirpath));
    return tmpDirpath;
}

QString FileCtl::getTmpDataFile(const QString &fname)
{
    tracein;
    // TODO: implement it, this is just termprary processing
    logd("fname '%s'", STR2CHA(fname));
    QString tmpFpath = getInstance()->tmpDataFile(fname);
    logd("tmpFpath '%s'", STR2CHA(tmpFpath));
    return tmpFpath;
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
    QString prebuildDataDir = FileCtl::getAppWorkingDataDir(KPrebuiltDirName);
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

ErrCode FileCtl::writeStringToFile(const QString &content, const QString &fpath)
{
    tracein;
    ErrCode ret = ErrNone;
    QFile file(fpath);
    logd("Open file %s", fpath.toStdString().c_str());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        logd("Write content '%s'", content.toStdString().c_str());
        QTextStream stream(&file); // TODO: catch exception????
        stream << content;
        file.close();
        ret = ErrNone;
    }
    else {
        ret = ErrFileOpen;
        loge("Failed to open file %s", fpath.toStdString().c_str());
    }
    return ret;
}
QString FileCtl::getUpdatePrebuiltDataFilePath(const QString name, bool lang)
{
    tracein;
    QString fname = Utils::getPrebuiltFileByLang(name, lang);
    QString prebuiltDir = getOrCreatePrebuiltDataDir();
    QString newFpath = QDir(prebuiltDir).filePath(fname);
    return newFpath;
}
ErrCode FileCtl::checkAndUpdatePrebuiltFile(const QString &name, bool backup)
{
    ErrCode ret = ErrNone;
    UNUSED(backup);
    tracein;
    // TODO: file should be from installed dir, rather than embedded inside bin??
    QString fname = Utils::getPrebuiltFileByLang(name, false);
    QString fpath = getPrebuiltDataFilePath(fname);
    if (!QFile::exists(fpath)){
        ret = ErrNotExist;
        loge("File %s not exist", fname.toStdString().c_str());
    }
    if ((ret == ErrNone) && !FileCtl::checkPrebuiltDataFileHash(fname)){
        // TODO: do backup
        QString prebuiltDir = getOrCreatePrebuiltDataDir();
        QString newFpath = QDir(prebuiltDir).filePath(fname);
        logd("oldpath %s", fpath.toStdString().c_str());
        logd("newFpath %s", newFpath.toStdString().c_str());
        if (QFile::exists(newFpath)) {
            logd("newFpath existed, remove '%s'", STR2CHA(newFpath));
            QFile::remove(newFpath);
        }

        if (!QFile::copy(fpath, newFpath)){
            ret = ErrFileWrite;
            loge("copy file %s failed", fname.toStdString().c_str());
        }
    }

    if (ret == ErrNone){
        logi("Updaet file hash %s", fname.toStdString().c_str());
        ret = FileCtl::updatePrebuiltDataFileHash(fname);
    }

    logi("Check to update prebuilt file %s: %d", name.toStdString().c_str(), ret);

    return ret;
}


QString FileCtl::getPrebuiltDataFile(const QString& fname)
{
    QFile file(QString(":/data/%1").arg(fname));
    QString prebuiltDir = getOrCreatePrebuiltDataDir();
    QFile saintFile(QDir(prebuiltDir).filePath(fname));
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

QString FileCtl::getPrebuiltDataFilePath(const QString &fname)
{
    logd("get prefile data file for fname '%s'", STR2CHA(fname));
    QString fpathAtInstalLDir = getAppInstallDir(fname);
    QString fpath;
    logd("fpathAtInstalLDir '%s'", STR2CHA(fpathAtInstalLDir));
    if (QFile::exists(fpathAtInstalLDir)) fpath = fpathAtInstalLDir;
    else fpath = QString(":/data/%1").arg(fname);
    logd("prebuild file data file fpath'%s'", STR2CHA(fpath));
    return fpath;
}

QString FileCtl::getPrebuiltDataFileHashPath(const QString &fname)
{
    QString hashFname = QString("%1.sha256").arg(fname);
    QString prebuiltDir = getOrCreatePrebuiltDataDir();
    QString hashFile = QDir(prebuiltDir).filePath(hashFname);
    logd("Hash file path %s", hashFile.toStdString().c_str());
    return hashFile;
}

bool FileCtl::checkPrebuiltDataFileHash(const QString &fname)
{
    tracein;
    ErrCode ret = ErrNone;
    QString fileHash;
    bool match = false;
    ret = readPrebuiltDataFileHash(fname, &fileHash);
    if (ret == ErrNone){
        QString hash = Crypto::hashFile(getPrebuiltDataFilePath(fname));
        if (fileHash.compare(hash, Qt::CaseInsensitive) == 0){
            logi("Hash file match");
            match = true;
        }
        else{
            logi("Hash file not match");
        }
    }
    else {
        loge("Something stupid error here %d", ret);
    }

    logd("Match %d", match);
    return match;
}

ErrCode FileCtl::readPrebuiltDataFileHash(const QString &fname, QString* hashOut)
{
    tracein;
    ErrCode ret = ErrNone;
    QString hashFname = getPrebuiltDataFileHashPath(fname);
    QFile hashFile(hashFname);
    QString path;
    logd("hashFile file %s", hashFile.fileName().toStdString().c_str());
    if (hashFile.exists())
    {
        if (hashFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&hashFile);
            QString hashValue = stream.readAll();
            if (!hashValue.isEmpty()){
                logd("hashValue %s", hashValue.toStdString().c_str());
                if (hashOut != nullptr){
                    *hashOut = hashValue.simplified().toLower();
                }
            }
            ret = ErrNone; // TODO: should validate content?
            hashFile.close();
        }
        else{
            ret = ErrFileRead;
            loge("Read file '%s' failed", hashFile.fileName().toStdString().c_str());
        }
    }
    else{
        loge("Hash of %s file not exisst", fname.toStdString().c_str());
        ret = ErrNotExist;
    }
    return ret;
}

ErrCode FileCtl::updatePrebuiltDataFileHash(const QString &fname)
{
    tracein;

    QString fpath = getPrebuiltDataFilePath(fname);
    QString fHashpath = getPrebuiltDataFileHashPath(fname);
    QString hash = Crypto::hashFile(fpath);
    return writeStringToFile(hash, fHashpath);
}


FileCtl::FileCtl()
{

}

const QString &FileCtl::tmpDirPath() const
{
    return mTmpDirPath;
}

void FileCtl::cleanUpData()
{
    tracein;
    // TODO: implement it, clean up all data, i.e data in temp storage
}

QString FileCtl::getName()
{
    return "FileCtl";
}

ErrCode FileCtl::onLoad()
{
    tracein;
    // create folder if any;
    getAppDataDir();
    getAppWorkingDataDir();
    getAppPeopleImageDataDir();
    return ErrNone;
}

void FileCtl::onUnload()
{
    tracein;
    logi("remove temp dir '%s'", STR2CHA(mTmpDir.path()));
    if (!mTmpDir.remove()) {
        loge("remove tmp dir '%s' failed", STR2CHA(mTmpDir.path()));
    }
    traceout;
}
