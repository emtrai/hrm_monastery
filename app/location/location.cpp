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
 * Filename: location.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#include "location.h"

#include "logger.h"
#include "errcode.h"
#include "filectl.h"
#include "defs.h"
#include "utils.h"

Location* Location::gInstance = nullptr;

Location *Location::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new Location();
    }

    return gInstance;
}

Location::Location()
{

}

ErrCode Location::loadCountryFromFiles()
{
//    FileCtl::getPrebuiltDataFile(
//         Utils::getPrebuiltFileByLang(KPrebuiltSaintCSVFileName))

    traced;
QString path = FileCtl::getPrebuiltDataFilePath(Utils::getPrebuiltFileByLang(KPrebuiltCountryCSVFileName));
    logd("Country file path %s", path.toStdString().c_str());

    ErrCode ret = Utils::parseCSVFile(path, &Location::oneCSVItemCallback, this);
    logd("ret %d", ret);

    return ret;
}

QList<Country*> Location::getListCountry()
{
    return mListCountry.values();
}

#define MAX_SPLIT_ITEMS (3)
ErrCode Location::doOneCSVItemCallback(const QStringList &items, void *param)
{
    traced;
    ErrCode ret = ErrNone;
    (void) param;
    if (!items.empty() && items.length() >= MAX_SPLIT_ITEMS) {
        Country *country = new Country();

        qint32 idx = 0;
        //id
//        if (ret == ErrNone)
//            country->setId(items[idx++].simplified());

        // full name
        if (ret == ErrNone)
            country->setName(items[idx++].simplified());

        // TODO: Continent??

//        if (ret == ErrNone)
//        {
//            if (country->isValid()){
//                country->dump();
//                mListCountry.insert(country->id(), country);
//            }
//            else{
//                ret = ErrInvalidData;
//                loge("Country data is invalid");
//            }
//        }

    }
    else {
        loge("Invalid data");
        // TODO: should break or continue???
        ret = ErrInvalidData;
    }
    logd("ret %d", ret);
    return ret;
}

ErrCode Location::oneCSVItemCallback(const QStringList &items, void* caller, void *param)
{

    traced;
    return getInstance()->doOneCSVItemCallback(items, param);
}



void Location::addCountry(QString id, const Country &country)
{
    traced;
}

void Location::onLoad()
{
    traced;
    loadCountryFromFiles();
    foreach (Country* country, getListCountry()) {
        country->dump();
    }
}
