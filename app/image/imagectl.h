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
 * Filename: imagectl.h
 * Author: Anh, Ngo Huy
 * Created date:4/27/2023
 * Brief:
 */

#ifndef IMAGECTL_H
#define IMAGECTL_H

#include <controller.h>
#include "errcode.h"
#include "utils.h"
#include "imagedefs.h"

#define IMGCTL ImageCtl::getInstance()

class ImageCtl : public Controller
{
    GET_INSTANCE_DECL(ImageCtl);
private:
    ImageCtl();
    virtual ~ImageCtl();
public:
    virtual void init();
    /**
     * @brief on load controller
     * @return
     */
    virtual ErrCode onLoad();

    /**
     * @brief Name of controller
     * @return
     */
    virtual QString getName() const;

    virtual void onUnload();
    QString getImageDirPath(const QString& subDir = nullptr);
public:
    ErrCode createThumbImage(const QString& fullPath, QString& thumbPath,
                            int height = IMG_THUMB_H, int width = IMG_THUMB_W,
                         const QString& format = DEFAULT_IMAGE_FORMAT);
    ErrCode convertImage(const QString& fullPath, const QString& finalPath = nullptr,
                         const QString& format = DEFAULT_IMAGE_FORMAT);
};

#endif // IMAGECTL_H
