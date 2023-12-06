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
 * Filename: config.h
 * Author: Anh, Ngo Huy
 * Created date:7/25/2022
 * Brief:
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include "errcode.h"
#include <QString>
#include <QMap>
#include "controller.h"

#define CONFIG Config::getInstance()

class Config: public Controller
{
public:
    static Config* getInstance();
    static ErrCode initConfig();


    static QString getNextPersonalCode(qint64* code = nullptr, ErrCode *outErr = nullptr);
    static int getLogLevel();
    static quint64 getMaxPersonCodeValue();
public:
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

    bool autoBackupEnable();
    QString getAutoBackupDirPath();

    ErrCode updatePersonCode(const QString& personCode);
    QString getPersonCodePrefix();
private:
    Config();
    ErrCode doInit();
    QString doGetNextPersonalCode(qint64* code = nullptr, ErrCode *outErr = nullptr);
    QString getValue(QString key, QString* defaultValue = nullptr, bool* ok = nullptr);
    quint64 getValueInt(QString key, quint64 defaultValue, bool* ok = nullptr);
    ErrCode setValueInt(QString key, quint64 value);
    quint64 doGetMaxPersonCodeValue();
    ErrCode initDefaultConfig();
    ErrCode loadConfig();
    ErrCode parseConfig(const QString& fpath, QMap<QString, QString>& config);
    ErrCode saveConfig(const QString& fpath, const QMap<QString, QString>& config);
    ErrCode saveConfig(const QString& fpath);
    ErrCode saveConfig();
    void dumpConfig();
private:
    static Config* gInstance;
    QMap<QString, QString> mConfigKeyValue;
    QString mConfigPath;
    bool mConfigUpdated;
};

#endif // CONFIG_H
