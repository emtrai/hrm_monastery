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
 * Filename: image.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/30/2023
 * Brief:
 */
#include "image.h"
#include "logger.h"
#include "imagectl.h"
#include <QUuid>
#include "utils.h"
#include "crypto.h"
#include <QFile>
#include <QDir>
#include "filectl.h"

#define MAX_LOOP_FIND_UID 100
Image::Image()
{
    traced;
}

QString Image::uid() const
{
    return mUid;
}


ErrCode Image::loadImage(const QString &fullPath, const QString& tag)
{
    tracein;
    ErrCode err = ErrNone;
    QDir imgDir;
    QString imgDirPath;
    QString finalFilePath;
    QString tmpFilePath;
    QString thumbImg;
    QString uid;
    removeTmp();
    if (fullPath.isEmpty() || !QFile::exists(fullPath)) {
        loge("full path is empty or not exist '%s'", STR2CHA(fullPath));
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        imgDirPath = IMGCTL->getImageDirPath(tag);
        logd("imgDirPath '%s'", STR2CHA(imgDirPath));
        if (!imgDirPath.isEmpty()) {
            imgDir.setPath(imgDirPath);
        } else {
            loge("img dir empty");
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        logd("fullPath '%s'", STR2CHA(fullPath));
        QFile file(fullPath);
        QString fname;
        int cnt = 0;
        bool found = false;
        do {
            QString seed = QString("%1").arg(cnt++);
            uid = buildUid(fullPath, seed);
            if (!uid.isEmpty()) {
                fname = QString("%1.%2").arg(uid, DEFAULT_IMAGE_FORMAT);
                logd("fname '%s'", STR2CHA(fname));
                finalFilePath = imgDir.filePath(fname);
                tmpFilePath = FileCtl::getTmpDataFile(fname);
                logd("finalFilePath '%s'", STR2CHA(finalFilePath));
                logd("tmpFilePath '%s'", STR2CHA(tmpFilePath));
                if (!QFile::exists(finalFilePath)) {
                    logi("convert '%s' to '%s', format '%s'",
                         STR2CHA(fullPath), STR2CHA(tmpFilePath), DEFAULT_IMAGE_FORMAT);
                    err = IMGCTL->convertImage(fullPath, tmpFilePath, DEFAULT_IMAGE_FORMAT);
                    found = true;
                } else {
                    logw("file '%s' existed, find other again, loop %d", STR2CHA(finalFilePath), cnt);
                }
            } else {
                err = ErrInvalidData;
                loge("no uuid");
                break;
            }
        } while ((err == ErrNone )&& !found && (cnt < MAX_LOOP_FIND_UID));
        if (cnt >= MAX_LOOP_FIND_UID) {
            loge("Reach max %d to find uid", MAX_LOOP_FIND_UID);
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        logd("create thum image for '%s'", STR2CHA(tmpFilePath));
        err = IMGCTL->createThumbImage(tmpFilePath, thumbImg);
    }
    if (err == ErrNone) {
        mFullImgTmpPath = tmpFilePath;
        mThumbImgTmpPath = thumbImg;
        mTag = tag;
        mBaseDir = imgDirPath;
        if (mUid.isEmpty()) {
            mFullImgPath = finalFilePath;
            mThumbImgPath = THUMB_IMG_PATH(finalFilePath);
            mUid = uid;
        } else {
            logd("keep origin image/thumb img path");
        }
    }
    traceret(err);
    return err;
}

ErrCode Image::loadImageFromUid(const QString &uid, const QString& tag)
{
    tracein;
    QString fpath;
    ErrCode err = ErrNone;
    bool ok = false;
    QString thumbPath;
    logd("uid '%s', tag '%s'", STR2CHA(uid), STR2CHA(tag));
    fpath = pathFromUid(uid, tag, &ok);
    logd("fpath '%s'", STR2CHA(fpath));
    if (!ok) {
        loge("path from uid '%s' failed", STR2CHA(uid));
        err = ErrFailed;
    }
    if (err == ErrNone && !QFile::exists(fpath)) {
        err = ErrNotExist;
        loge("file '%s' not exist", STR2CHA(fpath));
    }
    if (err == ErrNone) {
        thumbPath = THUMB_IMG_PATH(fpath);
        logd("thumPath '%s'", STR2CHA(thumbPath));
        if (!QFile::exists(thumbPath)) {
            err = ErrNotExist;
            loge("file thump '%s' not exist", STR2CHA(thumbPath));
        }
    }
    if (err == ErrNone) {
        mFullImgPath = fpath;
        mThumbImgPath = thumbPath;
        mUid = uid;
        mTag = tag;
    }
    traceret(err);
    return err;
}

QString Image::fullImgPath() const
{
    return mFullImgPath;
}

void Image::setFullImgPath(const QString &newFullImgPath)
{
    mFullImgPath = newFullImgPath;
}

QString Image::thumbImgPath() const
{
    return mThumbImgPath;
}

void Image::setThumbImgPath(const QString &newThumbImgPath)
{
    mThumbImgPath = newThumbImgPath;
}

ErrCode Image::save()
{
    tracein;
    ErrCode err = ErrNone;
    logd("mFullImgTmpPath: '%s'", STR2CHA(mFullImgTmpPath));
    logd("mThumbImgTmpPath: '%s'", STR2CHA(mThumbImgTmpPath));
    if (mFullImgTmpPath.isEmpty() || !QFile::exists(mFullImgTmpPath)) {
        loge("full img tmp path is empty or not exist '%s'", STR2CHA(mFullImgTmpPath));
        err = ErrNotExist;
    }
    if (err == ErrNone && (mThumbImgTmpPath.isEmpty() || !QFile::exists(mThumbImgTmpPath))) {
        loge("thumb img tmp path is empty or not exist '%s'", STR2CHA(mThumbImgTmpPath));
        err = ErrNotExist;
    }

    logd("mFullImgPath: '%s'", STR2CHA(mFullImgPath));
    logd("mThumbImgPath: '%s'", STR2CHA(mThumbImgPath));
    if (mFullImgPath.isEmpty()) {
        loge("mFullImgPath is empty");
        err = ErrInvalidData;
    }
    if (mThumbImgPath.isEmpty()) {
        loge("mThumbImgPath is empty");
        err = ErrInvalidData;
    }
    if (err == ErrNone) {
        err = FileCtl::copyFile(mFullImgTmpPath, mFullImgPath, true);
    }
    if (err == ErrNone) {
        err = FileCtl::copyFile(mThumbImgTmpPath, mThumbImgPath, true);
    }
    traceret(err);
    return err;
}

void Image::remove()
{
    tracein;
    removeTmp();
    if (!mThumbImgPath.isEmpty()) {
        logi("remove mThumbImgPath '%s' ", STR2CHA(mThumbImgPath));
        if (!QFile::remove(mThumbImgPath)) {
            loge("remove mThumbImgPath '%s' failed", STR2CHA(mThumbImgPath));
        }
    }
    if (!mFullImgPath.isEmpty()) {
        logi("remove mFullImgPath '%s' ", STR2CHA(mFullImgPath));
        if (!QFile::remove(mFullImgPath)) {
            loge("remove mFullImgPath '%s' failed", STR2CHA(mFullImgPath));
        }
    }
    traceout;

}

QString Image::buildUid(const QString fpath, const QString &seed)
{
    tracein;
    QUuid uuid = QUuid::createUuid();
    QString uuidStr;
    QString finalUid;
    if (!uuid.isNull()) {
        logd("Add uuid '%s'", STR2CHA(uuid.toString()));
        uuidStr += uuid.toString();
    } else {
        logw("FAILED to get uuid from system");
    }
    if (!seed.isEmpty()) {
        logd("Add seed '%s'", STR2CHA(seed));
        uuidStr += "_" + seed;
    }
    if (!fpath.isEmpty()) {
        logd("Add mFullImgPath '%s'", STR2CHA(fpath));
        uuidStr += "_" + fpath;
    }
    logd("Add time");
    uuidStr += QString("_%1").arg(Utils::currentTimeMs());
    logd("uuidStr '%s'", STR2CHA(uuidStr));
    finalUid = Crypto::hashString(uuidStr);
    logd("finalUid '%s'", STR2CHA(finalUid));
    traceout;
    return finalUid;
}

QString Image::pathFromUid(const QString &uid, const QString &tag, bool* ok)
{
    tracein;
    QDir imgDir;
    QString imgDirPath;
    QString finalFilePath;
    ErrCode err = ErrNone;
    if (uid.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        imgDirPath = IMGCTL->getImageDirPath(tag);
        logd("imgDirPath '%s'", STR2CHA(imgDirPath));
        if (!imgDirPath.isEmpty()) {
            imgDir.setPath(imgDirPath);
        } else {
            loge("img dir empty");
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        finalFilePath = QString("%1.%2").arg(imgDir.filePath(uid), DEFAULT_IMAGE_FORMAT);
    }
    logd("finalFilePath '%s'", STR2CHA(finalFilePath));
    if (ok) *ok = (err == ErrNone);
    traceout;
    return finalFilePath;
}

void Image::removeTmp()
{
    tracein;
    if (!mThumbImgTmpPath.isEmpty()) {
        logi("remove mFullImgPath '%s' ", STR2CHA(mFullImgPath));
        if (!QFile::remove(mThumbImgTmpPath)) {
            loge("remove mThumbImgTmpPath '%s' failed", STR2CHA(mThumbImgTmpPath));
        }
    }
    if (!mFullImgTmpPath.isEmpty()) {
        logi("remove mFullImgTmpPath '%s' ", STR2CHA(mFullImgTmpPath));
        if (!QFile::remove(mFullImgTmpPath)) {
            loge("remove mFullImgTmpPath '%s' failed", STR2CHA(mFullImgTmpPath));
        }
    }
    traceout;
}

QString Image::baseDir() const
{
    return mBaseDir;
}

void Image::setBaseDir(const QString &newBaseDir)
{
    mBaseDir = newBaseDir;
}

void Image::copy(const Image &rhs)
{
    tracein;
    if (this != &rhs) {
        mUid = rhs.mUid;
        mFullImgTmpPath = rhs.mFullImgTmpPath;
        mFullImgPath = rhs.mFullImgPath;
        mThumbImgPath = rhs.mThumbImgPath;
        mThumbImgTmpPath = rhs.mThumbImgTmpPath;
        mTag = rhs.mTag;
        mBaseDir = rhs.mBaseDir;
    } else {
        logw("It's same, nothing to copy");
    }
    traceout;
}

QString Image::tag() const
{
    return mTag;
}

void Image::setTag(const QString &newTags)
{
    mTag = newTags;
}
