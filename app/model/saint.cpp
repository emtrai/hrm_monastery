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
 * Filename: saint.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "saint.h"
#include "logger.h"
#include "errcode.h"
#include <QString>
#include <QStringLiteral>
#include "utils.h"
#include "crypto.h"

#include "dbctl.h"

#include "defs.h"


DbModel *Saint::build()
{
    return new Saint();
}

void Saint::init()
{
    traced;
    initImportFields();
}


void Saint::initImportFields()
{
    traced;
    // TODO: check fields like holly name, country, etc. and mark invalid field???

    mImportFields.insert(KExportFieldUid, [this](const QString& value){
        this->setUid(value);
    });

    mImportFields.insert(KExportFieldName, [this](const QString& value){
        this->setName(value);
    });

    mImportFields.insert(KExportFieldFullName, [this](const QString& value){
        this->setFullName(value);
    });
    mImportFields.insert(KExportFieldOriginName, [this](const QString& value){
        this->setOriginName(value);
    });
    mImportFields.insert(KExportFieldGender, [this](const QString& value){
        this->setGender(value);
    });
    mImportFields.insert(KExportFieldFeastDay, [this](const QString& value){
        this->setFeastDay(value);
    });
    mImportFields.insert(KExportFieldRemark, [this](const QString& value){
        this->setRemark(value);
    });
}

Saint::Saint(): DbModel()

{
    mFeastDay = 0;
    init();
}

ErrCode Saint::onImportItem(int importFileType, const QString &keyword, const QString &value, quint32 idx, void *tag)
{
    traced;
    ErrCode ret = ErrNone;
    logd("importFileType %d", importFileType);

    // TODO: raise exception when error occur???
    logd("keyword %s", keyword.toStdString().c_str());
    if (mImportFields.contains(keyword)){
        std::function<void(const QString& value)> func = mImportFields.value(keyword);
        if (func != nullptr) func(value);
    }

    tracedr(ret);
    return ret;
}


qint64 Saint::feastDay() const
{
    return mFeastDay;
}

void Saint::setFeastDay(qint64 newFeastDay)
{
    mFeastDay = newFeastDay;
}

void Saint::setFeastDay(const QString &newFeastDay, const QString& f)
{
    setFeastDay(Utils::dateFromString(newFeastDay, f));
}

Gender Saint::gender() const
{
    return mGender;
}

void Saint::setGender(Gender newGender)
{
    mGender = newGender;
}

void Saint::setGender(const QString &gender)
{
    mGender = Utils::genderFromString(gender);
}

const QString &Saint::country() const
{
    return mCountry;
}

void Saint::setCountry(const QString &newCountry)
{
    mCountry = newCountry;
}

const QString &Saint::fullName() const
{
    return mFullName;
}

void Saint::setFullName(const QString &newFullName)
{
    mFullName = newFullName;
}

void Saint::dump()
{
    traced;
    DbModel::dump();
#ifdef DEBUG_TRACE
    logd("- feastday: %d", (int)feastDay());
    logd("- gender: %d", (int)gender());
    logd("- Full Name %s", fullName().toStdString().c_str());
    logd("- Country dbId %s", countryUid().toStdString().c_str());
#endif //DEBUG_TRACE
}

DbModel *Saint::builder()
{
    return new Saint();
}

DbModelHandler *Saint::getDbModelHandler()
{
    return DbCtl::getInstance()->getDb()->getSaintModelHandler();
}

const QString &Saint::remark() const
{
    return mRemark;
}

void Saint::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}

const QString &Saint::originName() const
{
    return mOriginName;
}

void Saint::setOriginName(const QString &newOriginName)
{
    mOriginName = newOriginName;
}

const QString &Saint::countryUid() const
{
    return mCountryUid;
}

void Saint::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
}

//bool Saint::isValid()
//{
//    // TODO: add more checking for valid info
//    return !name().isEmpty();
//}

//void Saint::dump()
//{
//    // TODO: dump to stdout, sdderr or file???
//#ifdef DEBUG_TRACE

//    logd("DUMP SAINT:");
//    logd("- NameId %s", nameid().toStdString().c_str());
//    logd("- Name %s", name().toStdString().c_str());
//    logd("- FullName %s", fullName().toStdString().c_str());
//    logd("- gender %d", gender());
//    logd("- feastDay %lld (%s)", feastDay(),
//         Utils::date2String(feastDay(), QString()).toStdString().c_str());
//#endif //DEBUG_TRACE

//}

//QString Saint::toString()
//{
//    return QString("%1,%2").arg(nameid(), name());
//}

//ErrCode Saint::save()
//{
//    traced;
//    ErrCode ret = ErrNone;
//    IDbSaint* dbSaint = DbCtl::getInstance()->dbSaint();
//    if (dbSaint != nullptr){
//        ret = dbSaint->addSaint(this);
//        if (ret == ErrExisted){ // alrady exist, judge as ok
//            ret = ErrNone;
//            logi("%s already exist", toString().toStdString().c_str());
//        }
//    }
//    else{
//        ret = ErrDbNotReady;
//        loge("DbSaint not ready");
//    }
//    return ret;
//}

