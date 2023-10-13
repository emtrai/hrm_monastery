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
#include <QObject>
#include <QAtomicInt>

#define BACKUP (BackupCtl::getInstance())

class QTimer;
class QThread;
class BackupCtl;
class BackupCtl : public QObject, public Controller
{
    Q_OBJECT
    GET_INSTANCE_DECL(BackupCtl);
public:
    BackupCtl();
    virtual ~BackupCtl();

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

    /**
     * @brief Do backup data
     * @param fpath backup file. Metadata will be "<fpath>.json"
     * @param dataOnly true if to backup database data only, false to backup al
     *                 i.e include images
     * @param force true to force to overide existing backup file
     *              false to return error if file existed
     * @return
     */
    ErrCode backup(const QString& fpath, bool dataOnly = false, bool force = true);

    /**
     * @brief do auto backup
     * @param fdir folder to store auto backup data
     * @return error code
     */
    ErrCode autoBackup(const QString& fdir);
private:
    void startAutoBackup();
    void stopAutoBackup();
public slots:
    void autoBackupTimerCallback();
    void initThread();
private:
    QTimer* mAutoBackupTimer;
    QThread mAutoBackupThread;
    QAtomicInt mAutoBackupThreadStop;
};

#endif // BACKUPCTL_H
