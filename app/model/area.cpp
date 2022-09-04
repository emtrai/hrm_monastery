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


Area::Area()
{

}

DbModel *Area::builder()
{
    return new Area();
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

const QString &Area::remark() const
{
    return mRemark;
}

void Area::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}


DbModelHandler *Area::getDbModelHandler()
{

    return DB->getModelHandler(KModelHdlProvince);
}

const QString &Area::countryUid() const
{
    return mCountryUid;
}

void Area::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
}
