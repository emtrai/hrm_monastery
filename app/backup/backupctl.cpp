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
#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"
#include "defs.h"
#include <QFile>
#include <QFileInfoList>
#include <QTemporaryDir>
#include <QDir>
#include "filectl.h"
#include "backupmetainfo.h"
#include "controllerdefs.h"

#define META_FILE_NAME "backup.json"

GET_INSTANCE_CONTROLLER_IMPL(BackupCtl)

BackupCtl::BackupCtl()
{
    traced;
}

void BackupCtl::init()
{
    traced;
}

ErrCode BackupCtl::onLoad()
{
    return ErrNone;
}

void BackupCtl::onUnload()
{
    traced;
}

QString BackupCtl::getName()
{
    return KControllerBackup;
}

ErrCode BackupCtl::backup(const QString &fpath)
{
    tracein;
    ErrCode err = ErrNone;
    QString workingDataDirFpath = FileCtl::getAppWorkingDataDir();
    QDir workingDataDir(workingDataDirFpath);
    QFileInfoList fileList = workingDataDir.entryInfoList();
    QTemporaryDir tmpDir;
    BackupMetaInfo meta;

    if (!tmpDir.isValid()) {
        err = ErrFailed;
        loge("Failed to create temp dir for backup");
    }

    if (err == ErrNone) {
        logd("workingDataDirFpath=%s, no file: %d", STR2CHA(workingDataDirFpath), fileList.size());
        if(fileList.size() <= 0){
            loge("There is no file to backup");
            err = ErrNoData;
        }
    }

    if (err == ErrNone) {
        if (fpath.isEmpty()) {
            loge("invalid arg");
            err = ErrInvalidArg;
        }
    }

    if (err == ErrNone) {
        logi("tmpdir=%s", STR2CHA(tmpDir.path()));
        logi("Backup to file '%s'", STR2CHA(fpath));

        logi("Prepare file to backup from tmp file from '%s' to '%s'",
             STR2CHA(workingDataDirFpath), STR2CHA(tmpDir.path()));
        meta.setDateTime();
        foreach (QFileInfo finfo, fileList) {
            // TODO:
            if (!finfo.isFile()) {
                logd("'%s' not a file, skip", STR2CHA(finfo.absoluteFilePath()));
                continue;
            }

            QString fullPath = finfo.absoluteFilePath();
            QString filename = finfo.fileName();

            QFile file(fullPath);
            if(file.exists()){
                logi("File to backup '%s', filename '%s'", STR2CHA(fullPath), STR2CHA(filename));
                file.copy(tmpDir.filePath(finfo.fileName()));
                meta.addFile(finfo.path());
            }
            else{
                loge("file '%s' not exist:", STR2CHA(fullPath));
                err = ErrNotExist;
                break;
            }
        }
    }
    if (err == ErrNone) {
        logi("Zip tmp file from '%s' to '%s'", STR2CHA(tmpDir.path()), STR2CHA(fpath));
        QString metafpath = tmpDir.filePath(META_FILE_NAME);
        logd("metafile=%s", STR2CHA(metafpath));
        meta.saveJson(metafpath);
        QDir tmpFileDir(tmpDir.path());
        QFileInfoList tmpFileList = tmpFileDir.entryInfoList();
        QZipWriter writer(fpath);
        foreach (QFileInfo finfo, tmpFileList) {
            // TODO:
            if (!finfo.isFile()) {
                logd("'%s' not a file, skip", STR2CHA(finfo.absoluteFilePath()));
                continue;
            }

            QString fullPath = finfo.absoluteFilePath();
            QString filename = finfo.fileName();

            QFile file(fullPath);
            if(file.exists()){
                logi("Add file to zip '%s', filename '%s'", STR2CHA(fullPath), STR2CHA(filename));
                file.open(QIODevice::ReadOnly);
                writer.addFile(filename, file.readAll());
                file.close();
            }
            else{
                loge("file '%s' not exist:", STR2CHA(fullPath));
                err = ErrNotExist;
                break;
            }
        }
        writer.close();
    }
    traceret(err);
    return err;

}
