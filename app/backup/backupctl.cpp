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
 * Filename: backctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/1/2023
 * Brief:
 */
#include "backupctl.h"
#include "defs.h"
#include <QFile>
#include <QFileInfoList>
#include <QTemporaryDir>
#include <QDir>
#define QTGUI QtGui
#define WRITER QtGui/private/qzipwriter_p.h
// ~~ #include "QtGui/6.4.0/QtGui/private/qzipwriter_p.h"
#include STRINGIFY(QTGUI/QTSDK_VER/WRITER)
#include "filectl.h"
#include "backupmetainfo.h"
#include "controllerdefs.h"
#include "datetimeutils.h"
#include "config.h"
#include <QTimer>
#include <QThread>
#include <QObject>
#define META_FILE_NAME "backup.json"
#define AUTO_BACKUP_FNAME "autobackup"
#define AUTO_BACKUP_LAST_ID_FNAME "lastid"
#define AUTO_BACKUP_LAST_UPDATE_FNAME "lastupdate"
#define AUTO_BACKUP_MAX_ITEM (20) // max backup data, oldest one will be deleted.
#define AUTO_BACKUP_MAX_DELTA_TIME_HOUR (12)
#define AUTO_BACKUP_MAX_DELTA_TIME_MIN (AUTO_BACKUP_MAX_DELTA_TIME_HOUR * 60)
#define AUTO_BACKUP_MAX_DELTA_TIME_SEC (AUTO_BACKUP_MAX_DELTA_TIME_MIN * 60)
//#define AUTO_BACKUP_MAX_DELTA_TIME_SEC 2

#ifdef AUTO_BACKUP_TIME_SEC
#undef AUTO_BACKUP_MAX_DELTA_TIME_SEC
#define AUTO_BACKUP_MAX_DELTA_TIME_SEC AUTO_BACKUP_TIME_SEC
#endif

#define AUTO_BACKUP_MAX_DELTA_TIME_MS (AUTO_BACKUP_MAX_DELTA_TIME_SEC * 1000)


GET_INSTANCE_CONTROLLER_IMPL(BackupCtl)

BackupCtl::BackupCtl()
{
    traced;
    mAutoBackupTimer = nullptr;
    mAutoBackupThreadStop = 0;
}

BackupCtl::~BackupCtl()
{
    tracein;
    stopAutoBackup();
    FREE_PTR(mAutoBackupTimer);
    traceout;
}

void BackupCtl::init()
{
    traced;
}

ErrCode BackupCtl::onLoad()
{
    tracein;
    if (CONFIG->autoBackupEnable()) {
        logi("Check & do auto backup");
        startAutoBackup();
    } else {
        logi("Skip auto backup");
    }
    traceout;
    return ErrNone;
}

void BackupCtl::onUnload()
{
    tracein;
    stopAutoBackup();
    traceout;
}

QString BackupCtl::getName() const
{
    return KControllerBackup;
}

ErrCode BackupCtl::backup(const QString &fpath, bool dataOnly, bool force)
{
    tracein;
    ErrCode err = ErrNone;
    QStringList backupdirList;
    BackupMetaInfo meta;
    QString metafpath = fpath + ".json";

    backupdirList.append(FileCtl::getAppWorkingDataDir());
    if (!dataOnly) {
        backupdirList.append(FileCtl::getAppImageDataDir());
    }

    if (err == ErrNone && fpath.isEmpty()) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    if (err == ErrNone && QFile::exists(fpath)) {
        logd("bk file '%s' exist", STR2CHA(fpath));
        if (force) {
            logd("try to remove bk file '%s'", STR2CHA(fpath));
            if (!QFile::remove(fpath)) {
                err = ErrFileOp;
                loge("failed to remove file '%s'", STR2CHA(fpath));
            }
        } else {
            err = ErrExisted;
            loge("file '%s' existed", STR2CHA(fpath));
        }
    }
    if (err == ErrNone && QFile::exists(metafpath)) {
        logd("backup meta file '%s' exist", STR2CHA(metafpath));
        if (force) {
            logd("try to remove backup meta file '%s'", STR2CHA(metafpath));
            if (!QFile::remove(metafpath)) {
                err = ErrFileOp;
                loge("failed to remove backup meta file '%s'", STR2CHA(metafpath));
            }
        } else {
            err = ErrExisted;
            loge("backup meta file '%s' existed", STR2CHA(metafpath));
        }
    }

    if (err == ErrNone) {
        meta.setDateTime();
        logi("Backup to file '%s'", STR2CHA(fpath));
        logi("Backup meta file '%s'", STR2CHA(metafpath));

        QZipWriter writer(fpath);
        foreach (QString fdir, backupdirList) {
            logi("Scan folder to backup '%s'", STR2CHA(fdir));
            QDir dir(fdir);
            if (dir.exists()) {
                QList<QFileInfo> fileInfos;
                FileCtl::getListFilesRecursive(dir, fileInfos);
                foreach (QFileInfo finfo, fileInfos) {
                    if (finfo.isFile()) {
                        QString fin = dir.dirName() + finfo.absoluteFilePath().mid(fdir.length());
                        QFile file(finfo.absoluteFilePath());
                        file.open(QIODevice::ReadOnly);
                        logd("add to backup file '%s'", STR2CHA(fin));
                        writer.addFile(fin, file.readAll());
                        file.close();
                        logd("add to meta '%s'", STR2CHA(fin));
                        meta.addFile(fin);
                    } else {
                        logi("'%s' not a file", STR2CHA(finfo.absoluteFilePath()));
                    }
                }
            } else {
                logw("dir '%s' not exist", STR2CHA(dir.absolutePath()));
            }

        }
        logi("Save backup data '%s'", STR2CHA(metafpath));
        meta.saveJson(metafpath);
        writer.close();
    }
    traceret(err);
    return err;

}

ErrCode BackupCtl::autoBackup(const QString &fdir)
{
    tracein;
    ErrCode err = ErrNone;
    ErrCode tmperr = ErrNone;
    QDir bkDirRoot(fdir);
    QDir bkDir;
    QString bkDirFpath;
    QString lastIdPath = bkDirRoot.filePath(AUTO_BACKUP_LAST_ID_FNAME);
    QString lastUpdatePath = bkDirRoot.filePath(AUTO_BACKUP_LAST_UPDATE_FNAME);
    QString lastIdStr;
    QString lastUpdateStr;
    qint64 lastId = 0;
    qint64 nextId = 0;
    qint64 lastUpdate = 0;
    qint64 delta = 0;
    bool ok = false;
    qint64 currTime = DatetimeUtils::currentTimeMs();
    bool doBackup = true;

    /* Each backup data will be stored in each folder, folder name is 0, 1, 2... max value
     * last id will be stored in lastid file
     * last update/backup time is stored in lastupdate file
     * each time app is open, it'll check if last update time is larger than max detal time or not
     * if larger, do autobackup
     */

    if (fdir.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    // TODO: validate fdir, if it's valid path, and valid folder, i.e. not root folder
    // TODO: check read/write permission of fdir
    logd("root auto backup dir '%s'", STR2CHA(fdir));
    if (err == ErrNone && !bkDirRoot.mkpath(fdir)) {
        err = ErrCreateDirFailed;
        loge("failed to create root backup dir '%s'", STR2CHA(bkDirRoot.absolutePath()));
    }

    logd("lastUpdatePath '%s'", STR2CHA(lastUpdatePath));
    if (err == ErrNone && QFileInfo::exists(lastUpdatePath)) {
        tmperr = FileCtl::readFileString(lastUpdatePath, lastUpdateStr);
        if (tmperr == ErrNone) {
            logd("lastUpdateStr '%s'", STR2CHA(lastUpdateStr));
            lastUpdate = lastUpdateStr.toLong(&ok);
            if (!ok) {
                loge("convert '%s' to int failed", STR2CHA(lastUpdateStr));
                lastUpdate = 0;
            }
        } else {
            logw("Failed to read lastUpdatePath '%s', err=%d",
                 STR2CHA(lastUpdatePath), tmperr);
            // skip error, continue, as we can update last update later
        }
    } else {
        logi("lastUpdatePath '%s' not exist", STR2CHA(lastUpdatePath));
    }
    delta = currTime - lastUpdate;
    if (err != ErrNone ||
        (lastUpdate > 0 && (currTime > lastUpdate)  && (delta < AUTO_BACKUP_MAX_DELTA_TIME_MS))) {
        doBackup = false;
        logi("not do autobackup, due to: err=%d, currTime %lld ms, lastUpdate %lld ms, delta %lld ms, max %d ms",
             err, currTime, lastUpdate, delta, AUTO_BACKUP_MAX_DELTA_TIME_MS);
    } else {
        logi("Do autobackup, due to: currTime %lld ms, lastUpdate %lld ms, delta %lld ms, max %d ms",
             currTime, lastUpdate, delta, AUTO_BACKUP_MAX_DELTA_TIME_MS);
    }

    logd("lastIdPath '%s'", STR2CHA(lastIdPath));
    if (err == ErrNone && doBackup && QFileInfo::exists(lastIdPath)) {
        tmperr = FileCtl::readFileString(lastIdPath, lastIdStr);
        logd("lastIdStr '%s'", STR2CHA(lastIdStr));
        if (tmperr == ErrNone) {
            lastId = lastIdStr.toInt(&ok);
            if (ok) {
                nextId = ++lastId;
                if (nextId >= AUTO_BACKUP_MAX_ITEM) {
                    nextId = 0;
                    logi("read max %d, reset nextId to 0", AUTO_BACKUP_MAX_ITEM);
                }
            } else {
                loge("convert '%s' to int failed", STR2CHA(lastIdStr));
                lastId = 0;
                nextId = 0;
            }
        } else {
            logw("Failed to read lastIdPath '%s', err=%d",
                 STR2CHA(lastIdPath), tmperr);
            // skip error, continue, as we can update last id later
        }
    }

    if (err == ErrNone && doBackup) {
        bkDirFpath = bkDirRoot.filePath(QString::number(nextId));
        logd("bkDirFpath '%s'", STR2CHA(bkDirFpath));
        if (!bkDirRoot.exists(bkDirFpath)) {
            logi("bk dir '%s' not exist, create new one", STR2CHA(bkDirFpath));
            if (bkDirRoot.mkpath(bkDirFpath)) {
                bkDir.setPath(bkDirFpath);
            } else {
                err = ErrCreateDirFailed;
                loge("faild to create backup dir '%s'", STR2CHA(bkDirFpath));
            }
        } else {
            bkDir.setPath(bkDirFpath);
        }
    }

    if (err == ErrNone && doBackup) {
        QString bkFpath = bkDir.absoluteFilePath(AUTO_BACKUP_FNAME);
        logd("do backup with bkFpath '%s'", STR2CHA(bkFpath));
        err = backup(bkFpath, true, true);
    }

    if (err == ErrNone && doBackup) {
        logi("Write lastIdPath '%s', nextId %lld", STR2CHA(lastIdPath), nextId);
        tmperr = FileCtl::writeStringToFile(QString::number(nextId), lastIdPath);
        if (tmperr != ErrNone) {
            loge("Write lastIdPath '%s', nextId %lld failed, err=%d",
                 STR2CHA(lastIdPath), nextId, tmperr);
        }

        logi("Write lastUpdatePath '%s', currTime %lld", STR2CHA(lastUpdatePath), currTime);
        tmperr = FileCtl::writeStringToFile(QString::number(currTime), lastUpdatePath);
        if (tmperr != ErrNone) {
            loge("Write lastUpdatePath '%s', currTime %lld failed, err=%d",
                 STR2CHA(lastIdPath), currTime, tmperr);
        }
    }

    logife(err, "Auto backup failed");//just logging, continue even auto backup failed

    traceret(err);
    return err;
}

void BackupCtl::startAutoBackup()
{
    tracein;
    autoBackup(CONFIG->getAutoBackupDirPath());

    // Start timer to do auto backup
    dbg(LOG_INFO, "Start timer to check & do auto backup, time %d ms",
        AUTO_BACKUP_MAX_DELTA_TIME_MS);

    this->moveToThread(&mAutoBackupThread);
    connect(&mAutoBackupThread, SIGNAL(started()), this, SLOT(autobackupThreadRun()));
    QObject::connect(&mAutoBackupThread, &QThread::finished, this, &QObject::deleteLater);
    mAutoBackupThread.start();

    traceout;
}

void BackupCtl::stopAutoBackup()
{
    tracein;
    logi("stop auto backup");
    mAutoBackupThreadStop = 1;
    if (mAutoBackupTimer) {
        mAutoBackupTimer->stop();
        if (mAutoBackupThread.isRunning()) {
            logi("Stop autobackup thread");
            mAutoBackupThread.quit();
            mAutoBackupThread.requestInterruption();
            mAutoBackupThread.terminate();
        }
    }
    traceout;
}

void BackupCtl::autoBackupTimerCallback()
{
    tracein;
    dbg(LOG_INFO, "Autobackup time interval, do check autobackup");
    if (!mAutoBackupThreadStop) {
        autoBackup(CONFIG->getAutoBackupDirPath());
    } else {
        logi("Exit autobackup");
        mAutoBackupThread.exit(0);
    }
    traceout;
}

void BackupCtl::autobackupThreadRun()
{
    tracein;
    dbg(LOG_DEBUG,"in autobackup thread");
    mAutoBackupTimer = new QTimer(this);
    if (mAutoBackupTimer) {
        mAutoBackupTimer->moveToThread(&mAutoBackupThread);
        QObject::connect(mAutoBackupTimer, &QTimer::timeout, this,
                         &BackupCtl::autoBackupTimerCallback);

        dbg(LOG_VERBOSE, "start timer %d ms", AUTO_BACKUP_MAX_DELTA_TIME_MS);
        mAutoBackupTimer->start(AUTO_BACKUP_MAX_DELTA_TIME_MS);
    } else {
        loge("failed to create mAutoBackupTimer, no memory?");
    }
    traceout;
}
