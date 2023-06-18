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
#include "logger.h"
#include "errcode.h"
#include <QCoreApplication>

#include <QStandardPaths>
#include <QRandomGenerator>
#include <QDir>
#include "dbctl.h"
#include "utils.h"

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
    tracein;
    return getInstance()->doInit();
}


QString Config::getNextPersonalCode(qint64* code)
{
    tracein;
    return getInstance()->doGetNextPersonalCode(code);
}

QString Config::doGetNextPersonalCode(qint64* code)
{
    tracein;
    qint64 seq = 0;
    if (!code) {
        logd("get from db");
        bool ok = false;
        seq = DB->getCurrentPersonCodeNumber(&ok);
        if (!ok) {
            loge("Get sequence number failed, restart from 0");
            seq = 1;
        } else {
            logd("Increase '%d' by one", seq);
            seq++;
        }
    } else {
        logd("get from input");
        seq = *code;
    }

    // TODO: this is just dummy persone code, IMPLEMENT IT AGAIN
//    int id = DatetimeUtils::currentTimeMs(qint64 code);
    logd("seq %d", seq);
//    QString code = "MS" + QString::number(id).rightJustified(8, '0');
    QString id = QString("%1%2").arg(mConfigKeyValue.value("codeprefix"),
                                       QString::number(seq));
    logd("MS id %s", STR2CHA(id));
    traceout;
    return id;


}



ErrCode Config::loadConfig()
{
    mConfigKeyValue["codeprefix"] = "NUTU_";
    dumpConfig();
    return ErrNone;
}

void Config::dumpConfig()
{
    tracein;
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
    tracein;

    loadConfig();



    return ErrNone;
}
