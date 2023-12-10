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
 * Filename: provincectl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "provincectl.h"
#include "logger.h"
#include "province.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"
#include "modeldefs.h"

GET_INSTANCE_CONTROLLER_IMPL(ProvinceCtl)

ProvinceCtl::ProvinceCtl():
    ModelController(KControllerProvince, KModelHdlProvince)
{
    tracein;
}

// Format: Country name id, Name id, Province name[, <parent province if any>]
DbModel *ProvinceCtl::buildModel(void *items, const QString &fmt)
{
    tracein;
    Province* item = new Province();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
//    QString countryId = itemList->at(idx++).trimmed();
//    item->setCountryUid(countryId); // TODO: check if country id is valid or not
//    QString nameid = itemList->at(idx++).trimmed();
//    QString name = itemList->at(idx++).trimmed();
//    if (nameid.isEmpty()){
//        nameid = Utils::UidFromName(name, UidNameConvertType::NO_VN_MARK_UPPER);
//    }
//    // TODO: check/validate if name/nameid is valid
//    item->setNameId(NAMEID(item->countryUid(), nameid));
    QString countryNameId = itemList->at(idx++).trimmed();
    QString nameId = itemList->at(idx++).trimmed();
    QString name = itemList->at(idx++).trimmed();
    item->setNameId(name);
    // TODO: search country name id then search country uid???
//    item->setName(name);
    item->setName(name);
    if (sz > idx) {
        // TODO: search parent name id the put parent uid???
        QString parentNameId = itemList->at(idx++).trimmed();
//        if (!parentid.isEmpty())
//            item->setParentUid(NAMEID(item->countryUid(), parentid));
    }
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    traceout;
    return item;
}

const char *ProvinceCtl::getPrebuiltFileName()
{
    return KPrebuiltProvinceCSVFileName;
}


DbModelBuilder ProvinceCtl::getMainBuilder()
{
    return &Province::build;
}


const QList<Province *> *ProvinceCtl::getProvinceList(const QString &country)
{
    tracein;
    // TODO: implement it?
//    if (mProvinceList.contains(country))
//        return mProvinceList[country];
    return nullptr;
}

