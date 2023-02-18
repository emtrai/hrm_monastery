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
 * Filename: country.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#include "country.h"
#include "logger.h"
#include "errcode.h"
#include "province.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "provincectl.h"


Country::Country()
{

}

Country::Country(const Country &country):DbModel(country)
{
    setShortName(country.shortName());
    setRegion(country.region());
    setContinent(country.continent());
}


const QList<Province *>* Country::getListProvince()
{
    traced;
    return PROVINCE->getProvinceList(shortName());
}

DbModelHandler *Country::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlCountry);
}

const QString &Country::shortName() const
{
    return mShortName;
}

void Country::setShortName(const QString &newShortName)
{
    mShortName = newShortName;
}


const QString &Country::continent() const
{
    return mContinent;
}

void Country::setContinent(const QString &newContinent)
{
    mContinent = newContinent;
}

void Country::dump()
{
    traced;
    DbModel::dump();
#ifdef DEBUG_TRACE
    logd("- Short name %s", shortName().toStdString().c_str());
#endif //DEBUG_TRACE
}

DbModel *Country::build()
{
    Country* model = new Country();
    model->init();
    return model;
}

const QString &Country::region() const
{
    return mRegion;
}

void Country::setRegion(const QString &newRegion)
{
    mRegion = newRegion;
}

