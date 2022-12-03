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

#define CONFIG Config::getInstance()

class Config:public QObject
{
    Q_OBJECT
        public:
            static Config* getInstance();
            static ErrCode init();


            static QString getNextPersonalCode();

            ErrCode loadConfig();
            void dumpConfig();

//            QString getFilePath()
        private:
            Config();
            ErrCode doInit();
            QString doGetNextPersonalCode();
        private:
            static Config* gInstance;
            QMap<QString, QString> mConfigKeyValue;
};

#endif // CONFIG_H
