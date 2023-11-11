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
 * Filename: imagectl.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/27/2023
 * Brief:
 */

#include "imagectl.h"
#include "logger.h"
#include "utils.h"
#include <QFile>
#include <QImage>
#include "filectl.h"
#include <QFileInfo>

GET_INSTANCE_CONTROLLER_IMPL(ImageCtl);

ImageCtl::ImageCtl()
{
    traced;
}

ImageCtl::~ImageCtl()
{
    traced;
}

void ImageCtl::init()
{
    traced;
}

ErrCode ImageCtl::onLoad()
{
    traced;
    dbgtrace;
    return ErrNone;
}

QString ImageCtl::getName() const
{
    return IMG_CTL_NAME;
}

void ImageCtl::onUnload()
{
    traced;
    dbgtrace;
}

QString ImageCtl::getImageDirPath(const QString &subDir)
{
    logd("subDir '%s'", STR2CHA(subDir));
    return FileCtl::getAppImageDataDir(subDir);
}

ErrCode ImageCtl::createThumbImage(const QString &fullPath, QString &thumbPath,
                                int height, int width, const QString& format)
{
    tracein;
    ErrCode err = ErrNone;
    QString path = thumbPath;
    QImage img;
    QImage thumbImg;
    QFileInfo fileInfo;
    logi("create thumb image fullpath '%s', thumbPath '%s'",
         STR2CHA(fullPath), STR2CHA(thumbPath));
    if (fullPath.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid fullpath");
    } else if (!QFile::exists(fullPath)) {
        err = ErrNotExist;
        loge("File '%s' not exist", STR2CHA(fullPath));
    }
    if (err == ErrNone) {
        fileInfo.setFile(fullPath);
        if (thumbPath.isEmpty()) {
            path = THUMB_IMG_PATH(fullPath);
        }
        logd("fullPath '%s'", STR2CHA(fullPath));
        logd("path '%s'", STR2CHA(path));
    }

    if (err == ErrNone && !img.load(fullPath)) {
        err = ErrLoadFailed;
        loge("load image '%s' failed", STR2CHA(fullPath));
    }

    if (err == ErrNone) {
        logd("width %d height %d", width, height);
        logd("Scale image '%s' to '%s'", STR2CHA(fullPath), STR2CHA(path));
        thumbImg = img.scaled(width, height, Qt::KeepAspectRatio);
        if (thumbImg.isNull()) {
            err = ErrOperatorFailed;
            loge("Scaled image '%s' to '%s' failed", STR2CHA(fullPath), STR2CHA(path));
        }
    }

    if (err == ErrNone) {
        logi("Save thumb image to '%s'", STR2CHA(path));
        if (!thumbImg.save(path, STR2CHA(format))) {
            loge("Save thumb img '%s' failed", STR2CHA(path));
            err = ErrSaveFailed;
        }
    }
    if (err == ErrNone) {
        thumbPath = path;
    }
    traceret(err);
    return err;
}

ErrCode ImageCtl::convertImage(const QString &fullPath, const QString &finalPath,
                               const QString &format)
{
    tracein;
    ErrCode err = ErrNone;
    QString path = finalPath;
    QImage img;
    logi("convertImage fullPath '%s', finalPath '%s', format '%s'",
         STR2CHA(fullPath), STR2CHA(finalPath), STR2CHA(format));
    if (fullPath.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid fullpath");
    } else if (!QFile::exists(fullPath)) {
        err = ErrNotExist;
        loge("File '%s' not exist", STR2CHA(fullPath));
    }
    if (err == ErrNone) {
        if (finalPath.isEmpty()) {
            path = QString("%1.%2").arg(fullPath, format);
        }
        logd("fullPath '%s'", STR2CHA(fullPath));
        logd("path '%s'", STR2CHA(path));
    }
    if (err == ErrNone && (fullPath == path)) {
        err = ErrExisted;
        loge("same file, nothing to do");
    }

    if (err == ErrNone && !img.load(fullPath)) {
        err = ErrLoadFailed;
        loge("load image '%s' failed", STR2CHA(fullPath));
    }

    if (err == ErrNone) {
        logi("Save '%s' image to '%s'", STR2CHA(format), STR2CHA(path));
        if (!img.save(path, format.toStdString().c_str())) {
            loge("Save '%s' img '%s' failed", STR2CHA(format), STR2CHA(path));
            err = ErrSaveFailed;
        }
    }
    traceret(err);
    return err;
}

