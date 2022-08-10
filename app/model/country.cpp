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

Country::Country():
    mProvinceLoaded(false)
{

}

Country::Country(const Country &country)
{
    setId(country.id());
    setName(country.name());
}

const QString &Country::id() const
{
    return mId;
}

void Country::setId(const QString &newId)
{
    mId = newId;
}

const QString &Country::name() const
{
    return mName;
}

void Country::setName(const QString &newName)
{
    mName = newName;
}

bool Country::isValid()
{
    return !(name().isEmpty() || id().isEmpty());
}

void Country::dump()
{
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE

    logd("DUMP COUNTRY:");
    logd("- id %s", id().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- Province List:");

    foreach (Province* item, getListProvince()) {
        item->dump();
    }
    // TODO: province???
#endif //DEBUG_TRACE
}

QList<Province *> Country::getListProvince()
{
    traced;
    if (!mProvinceLoaded){
        loadProvinceFromFiles();
    }
    return mListProvince.values();
}

ErrCode Country::loadProvinceFromFiles()
{
    traced;
    QString path = FileCtl::getPrebuiltDataFilePath(Utils::getPrebuiltFileByLang(id()));
    logd("Province file path %s", path.toStdString().c_str());

    ErrCode ret = Utils::parseCSVFile(path, &Country::oneCSVItemCallback, this);
    logd("ret %d", ret);
    if (ret == ErrNone){
        mProvinceLoaded = true; // TODO: should mark only err none, or even in error case??
    }
    return ret;
}


#define MAX_SPLIT_ITEMS (3)
ErrCode Country::doOneCSVItemCallback(const QStringList &items, void *param)
{
    traced;
    ErrCode ret = ErrNone;
    (void) param;
    if (!items.empty() && items.length() >= MAX_SPLIT_ITEMS) {
        Province *province = new Province();

        qint32 idx = 0;
        //id
        if (ret == ErrNone)
            province->setId(items[idx++].simplified());

        // full name
        if (ret == ErrNone)
            province->setName(items[idx++].simplified());

        // TODO: Continent??

        if (ret == ErrNone)
        {
            if (province->isValid()){
                province->dump();
                mListProvince.insert(province->id(), province);
            }
            else{
                ret = ErrInvalidData;
                loge("Province data is invalid");
            }
        }

    }
    else {
        loge("Invalid data");
        // TODO: should break or continue???
        ret = ErrInvalidData;
    }
    logd("ret %d", ret);
    return ret;
}

ErrCode Country::oneCSVItemCallback(const QStringList &items, void *param)
{
    traced;
    if (param != nullptr)
        return ((Country*)param)->doOneCSVItemCallback(items, param);
    else
    {
        loge("Unknown caller");
        return ErrUnknown;
    }
}
