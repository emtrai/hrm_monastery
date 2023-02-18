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
 * Filename: countryctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/13/2022
 * Brief:
 */
#include "countryctl.h"
#include "logger.h"
#include "country.h"
#include "defs.h"
#include "dbctl.h"

GET_INSTANCE_IMPL(CountryCtl)

CountryCtl::CountryCtl():CommonCtl(KModelHdlCountry)
{
    traced;
}

CountryCtl::~CountryCtl()
{
    traced;
}

DbModelHandler *CountryCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlCountry);
}
// Format: vn, Vietname,Asian, Asia,
DbModel *CountryCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Country* item = new Country();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setShortName(itemList->at(idx++));
    item->setNameId(item->shortName());
    item->setName(itemList->at(idx++));
    item->setRegion(itemList->at(idx++));
    item->setContinent(itemList->at(idx++));

    return item;
}

const char *CountryCtl::getPrebuiltFileName()
{
    return KPrebuiltCountryCSVFileName;
}

const char *CountryCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

QList<DbModel *> CountryCtl::getItemFromDb()
{
    return getModelHandler()->getAll(&Country::build);
}

