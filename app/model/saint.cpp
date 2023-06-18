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
#include "datetimeutils.h"
#include "crypto.h"

#include "dbctl.h"

#include "defs.h"


DbModel *Saint::build()
{
    Saint* model = new Saint();
    model->init();
    return model;
}

DbModelBuilder Saint::getBuilder() const
{
    return &Saint::build;
}

void Saint::clone(const DbModel *model)
{
    tracein;
    if (model) {
        DbModel::clone(model);
        copy(*(Saint*)model);
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

void Saint::init()
{
    tracein;
    initImportFields();
}


void Saint::initImportFields()
{
    tracein;
    // TODO: check fields like holly name, country, etc. and mark invalid field???

    mImportCallbacks.insert(KItemUid, [this](const QString& value){
        this->setUid(value);
        return ErrNone;
    });

    mImportCallbacks.insert(KItemName, [this](const QString& value){
        this->setName(value);
        return ErrNone;
    });

    mImportCallbacks.insert(KItemFullName, [this](const QString& value){
        this->setFullName(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemOriginName, [this](const QString& value){
        this->setOriginName(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemGender, [this](const QString& value){
        this->setGender(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemFeastDay, [this](const QString& value){
        this->setFeastDay(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemRemark, [this](const QString& value){
        this->setRemark(value);
        return ErrNone;
    });
}

Saint::Saint(): DbModel()

{
    mFeastDay = 0;
}

ErrCode Saint::onImportParseDataItem(const QString& importName, int importFileType,
                            const QString &keyword, const QString &value, quint32 idx,
                            QList<DbModel *>* outList)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("importFileType %d", importFileType);

    // TODO: raise exception when error occur???
    logd("keyword %s", keyword.toStdString().c_str());
    if (mImportCallbacks.contains(keyword)){
        ImportCallbackFunc func = mImportCallbacks.value(keyword);
        if (func != nullptr) ret = func(value);
    }

    traceret(ret);
    return ret;
}

QString Saint::modelName() const
{
    return KModelNameSaint;
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
    setFeastDay(DatetimeUtils::dateFromString(newFeastDay, f));
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
    tracein;
    DbModel::dump();
#ifdef DEBUG_TRACE
    logd("- feastday: %d", (int)feastDay());
    logd("- gender: %d", (int)gender());
    logd("- Full Name %s", fullName().toStdString().c_str());
    logd("- Country dbId %s", countryUid().toStdString().c_str());
#endif //DEBUG_TRACE
}


DbModelHandler *Saint::getDbModelHandler() const
{
    return DbCtl::getInstance()->getDb()->getSaintModelHandler();
}

void Saint::copy(const Saint &model)
{
    tracein;
    mImportCallbacks = model.mImportCallbacks;
    mFullName = model.mFullName;
    mOriginName = model.mOriginName;
    mGender = model.mGender;
    mFeastDay = model.mFeastDay;
    mCountry = model.mCountry;
    mCountryUid = model.mCountryUid;
    traceout;
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
//         DatetimeUtils::date2String(feastDay(), QString()).toStdString().c_str());
//#endif //DEBUG_TRACE

//}

//QString Saint::toString()
//{
//    return QString("%1,%2").arg(nameid(), name());
//}

//ErrCode Saint::save()
//{
//    tracein;
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

