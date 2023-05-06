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
 * Filename: image.h
 * Author: Anh, Ngo Huy
 * Created date:4/30/2023
 * Brief:
 */
#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include "errcode.h"

class Image
{
public:
    Image();
    QString uid() const;

    ErrCode loadImage(const QString& fullPath, const QString& tag = nullptr);
    ErrCode loadImageFromUid(const QString& uid, const QString& tag = nullptr);
    QString fullImgPath() const;
    void setFullImgPath(const QString &newFullImgPath);

    QString thumbImgPath() const;
    void setThumbImgPath(const QString &newThumbImgPath);

    /**
     * @brief Image will be stored in temporary folder first, then need to call
     * save to store officially
     * @return
     */
    ErrCode save();
    void remove(bool markRemove = false);
    QString tag() const;
    void setTag(const QString &newTags);

    QString baseDir() const;
    void setBaseDir(const QString &newBaseDir);
    void copy(const Image& rhs);

private:
    QString buildUid(const QString fpath = nullptr, const QString& seed = nullptr);
    QString pathFromUid(const QString& uid, const QString& tag = nullptr, bool* ok = nullptr);
    void removeTmp();
private:
    QString mUid;
    QString mFullImgTmpPath;
    QString mFullImgPath;
    QString mThumbImgPath;
    QString mThumbImgTmpPath;
    QString mTag;
    QString mBaseDir; // path to base dir to store images
};

#endif // IMAGE_H
