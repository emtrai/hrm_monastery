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
#include "controllerdefs.h"

/*
 * log level:
 * 0: Error
 * 1: Informative
 * 2: Verbose
 * 3: Debug
 */
#define DEFAULT_LOGLEVEL (LOG_INFO)

Config* Config::gInstance = nullptr;

Config *Config::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new Config();
    }
    return gInstance;
}

ErrCode Config::initConfig()
{
    traced;
    return getInstance()->doInit();
}


QString Config::getNextPersonalCode(qint64* code, ErrCode *outErr)
{
    traced;
    return getInstance()->doGetNextPersonalCode(code, outErr);
}

int Config::getLogLevel()
{
    traced;
#ifdef DEBUG_LOG
    return LOG_DEBUG;
#else //!DEBUG_LOG
    return CONFIG->getValueInt(CONFIG_LOG_LEVEL, DEFAULT_LOGLEVEL);
#endif //DEBUG_LOG
}

quint64 Config::getMaxPersonCodeValue()
{
    return getInstance()->doGetMaxPersonCodeValue();
}

ErrCode Config::onLoad()
{
    // do nothing
    return ErrNone;
}

QString Config::getName() const
{
    return KControllerConfig;
}

void Config::onUnload()
{
    if (mConfigUpdated) {
        logi("Config is updated, store it");
        saveConfig();
    }
}

ErrCode Config::initDefaultConfig()
{
    tracein;
    ErrCode err = ErrNone;
    mConfigKeyValue[CONFIG_PREFIX] = DEFAULT_PREFIX_PERSON;
    mConfigKeyValue[CONFIG_CODE_LENGTH] = QString::number(DEFAULT_CODE_LENGTH);
    mConfigKeyValue[CONFIG_LOG_LEVEL] = QString::number(DEFAULT_LOGLEVEL);
    mConfigKeyValue[CONFIG_PERSON_CODE_VAL_MAX] = QString::number(0);
    traceret(err);
    return err;
}

QString Config::doGetNextPersonalCode(qint64* code, ErrCode *outErr)
{
    tracein;
    ulong seq = 0;
    ErrCode err = ErrNone;
    int maxSeqChar = getValueInt(CONFIG_CODE_LENGTH, DEFAULT_CODE_LENGTH);
    QString prefix = mConfigKeyValue.value(CONFIG_PREFIX);
    if (!code) {
        logd("get from db");
        bool ok = false;
        seq = DB->getCurrentPersonCodeNumber(&ok);
        if (!ok) {
            loge("Get sequence number failed, restart from 0");
            seq = 1;
        } else {
            logd("Increase '%lu' by one", seq);
            seq++;
        }
    } else {
        logd("get from input");
        seq = *code;
    }

    logd("seq %ld", seq);
    QString id = prefix + QString("%1").arg((ulong)seq, maxSeqChar, 10, QChar('0'));
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
    logd("get config key '%s'", STR2CHA(key));
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

quint64 Config::getValueInt(QString key, quint64 defaultValue, bool *ok)
{
    QString value;
    quint64 valueInt = defaultValue;
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

    logd("Key '%s':%llu", STR2CHA(key), valueInt);

    return valueInt;
}

ErrCode Config::setValueInt(QString key, quint64 value)
{
    dbgd("set config value key '%s' = %lld", STR2CHA(key), value);
    mConfigKeyValue[key] = QString::number(value);
    mConfigUpdated = true;
    return ErrNone;
}

quint64 Config::doGetMaxPersonCodeValue()
{
    return getValueInt(CONFIG_PERSON_CODE_VAL_MAX, 0);
}


ErrCode Config::loadConfig()
{
    tracein;
    ErrCode err = ErrNone;
    QDir dataDir(FileCtl::getAppWorkingDataDir());
    mConfigPath = dataDir.absoluteFilePath(CONFIG_FNAME);
    QMap<QString, QString> config;
    logi("Parse config file '%s'", STR2CHA(mConfigPath));
    err = parseConfig(mConfigPath, config);
    if (err == ErrNone && config.size() > 0) {
        logd("Assign to %lld item to main config", config.size());
        foreach(QString key, config.keys()) {
            mConfigKeyValue[key] = config[key];
        }
    } else if (err == ErrNotExist) {
        logi("config file '%s' not exist, store default one", STR2CHA(mConfigPath));
        err = saveConfig(mConfigPath);
    }
    Logger::setLogLevel(getLogLevel());
    dumpConfig();
    traceret(err);
    return err;
}

#define COMMENT '#'
#define SPLIT ':'
#define NEXT_LINE '\\'
ErrCode Config::parseConfig(const QString &fpath, QMap<QString, QString> &config)
{
    tracein;
    ErrCode ret = ErrNone;
    if (fpath.isEmpty()){
        loge("invalid argument");
        ret = ErrInvalidArg;
    }
    if (ret == ErrNone && !QFileInfo::exists(fpath)) {
        ret = ErrNotExist;
        loge("File %s not exist", STR2CHA(fpath));
    }
    if (ret == ErrNone) {
        QFile file(fpath);
        logi("Read file '%s' then parse", STR2CHA(fpath));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            // TODO: catch exception????
            while(!stream.atEnd()) {
                QString line = stream.readLine();
                logd("> Line '%s'", line.toStdString().c_str());
                if (!line.isEmpty()) line = line.simplified();

                if (!line.isEmpty() && !line.startsWith(COMMENT)) {
                    QStringList items = line.split(SPLIT);
                    logd("split int to %lld items", items.length());
                    QString key;
                    QString value;
                    if (!items.empty()) {
                        key = items[0];
                        items.removeFirst();
                    }
                    if (!items.empty()) {
                        value = items.join(SPLIT);
                    }
                    if (!key.isEmpty()) {
                        config.insert(key, value);
                    }
                } // ignore line start with # or empty
            }
            file.close();
        } else {
            ret = ErrFileRead;
            loge("Read file '%s' failed", fpath.toStdString().c_str());
        }
    }
    logd("found %lld items", config.size());
    logife(ret, "Parse config failed");
    traceret(ret);
    return ret;
}

ErrCode Config::saveConfig(const QString &fpath, const QMap<QString, QString> &config)
{
    tracein;
    ErrCode err = ErrNone;
    if (fpath.isEmpty()){
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        QFile file(fpath);
        logi("Write %lld item to config file '%s'", config.size(), STR2CHA(fpath));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file); // TODO: catch exception????
            foreach (QString key, config.keys()) {
                QString item = QString("%1%2%3\n").arg(key).arg(SPLIT).arg(config[key]);
                dbgd("Write to config file '%s'", STR2CHA(item));
                stream << item;
            }
            file.close();
        }
    }
    logife(err, "Write config failed");
    traceret(err);
    return err;
}

ErrCode Config::saveConfig(const QString &fpath)
{
    traced;
    return saveConfig(fpath, mConfigKeyValue);
}

ErrCode Config::saveConfig()
{
    ErrCode err = ErrNone;
    traceout;
    if (!mConfigPath.isEmpty()) {
        err = saveConfig(mConfigPath);
    } else {
        logd("no config path to save");
    }
    traceret(err);
    return err;
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

ErrCode Config::updatePersonCode(const QString &personCode)
{
    ErrCode err = ErrNone;
    tracein;
    if (personCode.isEmpty()) {
        err = ErrInvalidArg;
        loge("invald value, empty person code");
    }
    if (err == ErrNone) {
        logd("Update person code '%s'", STR2CHA(personCode));
        QString prefix = getPersonCodePrefix();
        bool ok = false;
        quint64 currMaxCode = doGetMaxPersonCodeValue();
        logd("perfix '%s', currMaxCode %lld", STR2CHA(prefix), currMaxCode);
        quint64 code = personCode.mid(prefix.length()).toULong(&ok);
        logd("code %lld", code);
        if (ok) {
            if (code > currMaxCode) {
                dbgi("Update max person code %lld", code);
                err = setValueInt(CONFIG_PERSON_CODE_VAL_MAX, code);
            } else {
                logd("new code %lld is less than max %lld", code, currMaxCode);
            }
        } else {
            err = ErrInvalidArg;
            loge("failed to parse person code '%s'", STR2CHA(personCode));
        }
    }
    traceret(err);
    return err;
}

QString Config::getPersonCodePrefix()
{
    QString prefix;
    bool ok = false;
    prefix = getValue(CONFIG_PREFIX, NULL, &ok);
    return prefix;
}


Config::Config():mConfigUpdated(false)
{
    traced;
}

ErrCode Config::doInit()
{
    tracein;
    initDefaultConfig();
    loadConfig();

    traceout;
    return ErrNone;
}
