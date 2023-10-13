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
#include "filectl.h"

#include "configdefs.h"

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


QString Config::getNextPersonalCode(qint64* code, ErrCode *outErr)
{
    tracein;
    return getInstance()->doGetNextPersonalCode(code, outErr);
}

QString Config::doGetNextPersonalCode(qint64* code, ErrCode *outErr)
{
    tracein;
    ulong seq = 0;
    ErrCode err = ErrNone;
    int maxSeqChar = getValue(CONFIG_CODE_LENGTH, DEFAULT_CODE_LENGTH);
    QString prefix = mConfigKeyValue.value(CONFIG_PREFIX);
    if (!code) {
        logd("get from db");
        bool ok = false;
        seq = DB->getCurrentPersonCodeNumber(&ok);
        if (!ok) {
            loge("Get sequence number failed, restart from 0");
            seq = 1;
        } else {
            logd("Increase '%lld' by one", seq);
            seq++;
        }
    } else {
        logd("get from input");
        seq = *code;
    }

    logd("seq %ld", seq);
    QString id = prefix + QString("%1").arg((ulong)seq, 8, 10, QChar('0'));
    logd("MS id %s", STR2CHA(id));

    if (err != ErrNone) {
        loge("Failed to get next personal code, err=%d", err);
    }
    if (outErr) {
        *outErr = err;
    }
    traceout;
    return id;


}

QString Config::getValue(QString key, QString* defaultValue, bool *ok)
{
    QString value;
    bool isOk = false;
    if (mConfigKeyValue.contains(key)) {
        value = mConfigKeyValue.value(key);
        isOk = true;
    } else {
        logw("Not found key, use default if any");
        if (defaultValue) value = *defaultValue;
    }
    logd("Key '%s':'%s'", STR2CHA(key), STR2CHA(value));
    if (ok) *ok = isOk;
    return value;
}

quint64 Config::getValue(QString key, quint64 defaultValue, bool *ok)
{
    QString value;
    quint64 valueInt;
    QString defaultValueString = QString::number(defaultValue);
    bool isOk = false;
    value = getValue(key, &defaultValueString, &isOk);
    if (!value.isEmpty()) {
        bool converok = false;
        valueInt = value.toULong(&converok);
        if (!converok) {
            loge("Convert from '%s' to int failed", STR2CHA(value));
        }
        if (isOk) isOk = converok; // only set to this value if previous call is ok
    } else {
        if (isOk) isOk = false; // only set to false if previous call is ok
    }
    if (ok) *ok = isOk;

    logd("Key '%s':%lu", STR2CHA(key), valueInt);

    return valueInt;
}



ErrCode Config::loadConfig()
{

    mConfigKeyValue[CONFIG_PREFIX] = DEFAULT_PREFIX_PERSON;
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

bool Config::autoBackupEnable()
{
    // TODO: implement this, read from file
    return true;
}

QString Config::getAutoBackupDirPath()
{
    return FileCtl::getAppBackupDataDir();
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
