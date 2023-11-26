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
    CHECK_MODIFIED_THEN_SET(mFeastDay, newFeastDay, KItemFeastDay);
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
    CHECK_MODIFIED_THEN_SET(mGender, newGender, KItemGender);
}

void Saint::setGender(const QString &gender)
{
    setGender(Utils::genderFromString(gender));
}

const QString &Saint::country() const
{
    return mCountry;
}

void Saint::setCountry(const QString &newCountry)
{
    CHECK_MODIFIED_THEN_SET(mCountry, newCountry, KItemCountry);
}

QString Saint::fullName() const
{
    return mFullName;
}

void Saint::setFullName(const QString &newFullName)
{
    CHECK_MODIFIED_THEN_SET(mFullName, newFullName, KItemFullName);
}

void Saint::dump() const
{
    tracein;
    DbModel::dump();
#ifdef DEBUG_TRACE
    logd("- feastday: %d", (int)feastDay());
    logd("- gender: %d", (int)gender());
    logd("- Full Name %s", fullName().toStdString().c_str());
    logd("- Country dbId %s", countryUid().toStdString().c_str());
#endif //DEBUG_TRACE
    traceout;
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
    CHECK_MODIFIED_THEN_SET(mOriginName, newOriginName, KItemOriginName);
}

const QString &Saint::countryUid() const
{
    return mCountryUid;
}

void Saint::setCountryUid(const QString &newCountryUid)
{
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountryUid);
}

