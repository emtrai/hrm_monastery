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
 * Filename: ethnic.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "ethnic.h"
#include "logger.h"
#include "errcode.h"
#include "province.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"


Ethnic::Ethnic()
{
    traced;
}

DbModel *Ethnic::build()
{
    traced;
    Ethnic* model = new Ethnic();
    model->init();
    return model;
}

DbModelBuilder Ethnic::getBuilder()
{
    return &Ethnic::build;
}

qint64 Ethnic::countryDbId() const
{
    return mCountryDbId;
}

void Ethnic::setCountryDbId(qint64 newCountryDbId)
{
    mCountryDbId = newCountryDbId;
}

DbModelHandler *Ethnic::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlEthnic);
}

const QString &Ethnic::countryName() const
{
    return mCountryName;
}

void Ethnic::setCountryName(const QString &newCountryName)
{
    mCountryName = newCountryName;
}

const QString &Ethnic::countryUid() const
{
    return mCountryUid;
}

void Ethnic::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
}
