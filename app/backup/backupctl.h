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
 * Filename: backctl.h
 * Author: Anh, Ngo Huy
 * Created date:4/1/2023
 * Brief:
 */
#ifndef BACKUPCTL_H
#define BACKUPCTL_H

#include <controller.h>
#include "utils.h"
#include "errcode.h"

#define BACKUP (BackupCtl::getInstance())

class BackupCtl : public Controller
{
    GET_INSTANCE_DECL(BackupCtl);
public:


public:
    BackupCtl();

    virtual void init();
    /**
     * @brief on load controller
     * @return
     */
    virtual ErrCode onLoad();
    virtual void onUnload();

    /**
     * @brief Name of controller
     * @return
     */
    virtual QString getName() const;

    ErrCode backup(const QString& fpath);
};

#endif // BACKUPCTL_H
