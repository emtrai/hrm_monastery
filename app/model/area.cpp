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
 * Filename: area.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#include "area.h"

#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"


Area::Area():DbModel(),
      mCountryDbId(0)
    , mPersonDbId(0)
{
    traced;
}

DbModel *Area::build()
{
    Area* item = new Area();
    item->init();
    return item;
}

QString Area::modelName() const
{
    return KModelNameArea;
}

Country *Area::getCountry() const
{
    return country;
}

void Area::setCountry(Country *newCountry)
{
    country = newCountry;
}

qint64 Area::countryDbId() const
{
    return mCountryDbId;
}

void Area::setCountryDbId(qint64 newCountryDbId)
{
    mCountryDbId = newCountryDbId;
}

qint64 Area::personDbId() const
{
    return mPersonDbId;
}

void Area::setPersonDbId(qint64 newPersonDbId)
{
    mPersonDbId = newPersonDbId;
}

DbModelHandler *Area::getDbModelHandler()
{

    return DB->getModelHandler(KModelHdlArea);
}

const QString &Area::areaCode() const
{
    return mAreaCode;
}

void Area::setAreaCode(const QString &newAreaCode)
{
    mAreaCode = newAreaCode;
}

QString Area::personName() const
{
    return mPersonName;
}

void Area::setPersonName(QString newPersonName)
{
    mPersonName = newPersonName;
}

QString Area::personUid() const
{
    return mPersonUid;
}

void Area::setPersonUid(QString newPersonUid)
{
    mPersonUid = newPersonUid;
}

QString Area::countryName() const
{
    return mCountryName;
}

void Area::setCountryName(QString newCountryName)
{
    mCountryName = newCountryName;
}

const QString &Area::countryUid() const
{
    return mCountryUid;
}

void Area::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
}

// <country name>,<area name>
QString Area::getFullName()
{
    // TODO: no country name???
    return QString("%s,%s").arg(countryName()).arg(name());
}
