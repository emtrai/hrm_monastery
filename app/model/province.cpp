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
 * Filename: province.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#include "province.h"
#include "logger.h"
#include "errcode.h"
#include "province.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

Province::Province()
{
    traced;
}

DbModel *Province::builder()
{
    return new Province();
}

QString Province::nameid() const
{
    QString hash;
    if (parentUid().isEmpty())
        hash = Utils::UidFromName(name()+countryShortName());
    else
        hash = Utils::UidFromName(name()+countryShortName()+parentUid());
    logd("province uuid %s", hash.toStdString().c_str());
    return hash;
}

qint64 Province::parentDbId() const
{
    return mParentDbId;
}

void Province::setParentDbId(qint64 newParentDbId)
{
    mParentDbId = newParentDbId;
}

qint64 Province::countryDbId() const
{
    return mCountryDbId;
}

void Province::setCountryDbId(qint64 newCountryDbId)
{
    mCountryDbId = newCountryDbId;
}

const QString &Province::remark() const
{
    return mRemark;
}

void Province::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}


DbModelHandler *Province::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlProvince);
}

const QString &Province::parentUid() const
{
    return mParentUid;
}

void Province::setParentUid(const QString &newParentUid)
{
    mParentUid = newParentUid;
}


const QString &Province::countryShortName() const
{
    return mCountryShortName;
}

void Province::setCountryShortName(const QString &newCountryShortName)
{
    mCountryShortName = newCountryShortName;
}

