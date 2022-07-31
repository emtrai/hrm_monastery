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
 * Filename: config.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/25/2022
 * Brief:
 */
#include "config.h"
#include "std.h"
#include <QCoreApplication>

#include <QStandardPaths>
#include <QRandomGenerator>
#include <QDir>
#include "filectl.h"
#include "defs.h"

Config* Config::gInstance = nullptr;

Config *Config::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new Config();

    }
    return gInstance;
}

ErrCode Config::init()
{
    traced;
    return getInstance()->doInit();
}


QString Config::getNextPersonalCode()
{
    return getInstance()->doGetNextPersonalCode();
}

QString Config::doGetNextPersonalCode()
{
    return QString("%1%2").arg(mConfigKeyValue.value("codeprefix"),
                               QString::number(QRandomGenerator::global()->bounded(1000)));
}



ErrCode Config::loadConfig()
{
    mConfigKeyValue["codeprefix"] = "MSNV";
    dumpConfig();
    return ErrNone;
}

void Config::dumpConfig()
{
    traced;
    foreach (QString key, mConfigKeyValue.keys()){
        logd("%s:%s", key.toStdString().c_str(),
             mConfigKeyValue[key].toStdString().c_str());
    }
}


Config::Config()
{

}

ErrCode Config::doInit()
{
    traced;

    loadConfig();



    return ErrNone;
}
