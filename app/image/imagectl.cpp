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
    return ErrNone;
}

QString ImageCtl::getName()
{
    return IMG_CTL_NAME;
}

void ImageCtl::onUnload()
{
    traced;
}

ErrCode ImageCtl::createThumbImage(const QString &fullPath, const QString &thumbPath,
                                  int height, int width)
{
    tracein;
    ErrCode err = ErrNone;
    QString path = thumbPath;
    QImage img;
    QImage thumbImg;
    if (fullPath.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid fullpath");
    } else if (!QFile::exists(fullPath)) {
        err = ErrNotExist;
        loge("File '%s' not exist", STR2CHA(fullPath));
    }
    if (err == ErrNone) {
        if (thumbPath.isEmpty()) {
            path = QString("%1.thumb").arg(fullPath);
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
        if (!thumbImg.save(path)) {
            loge("Save thumb img '%s' failed", STR2CHA(path));
            err = ErrSaveFailed;
        }
    }
    traceret(err);
    return err;
}

