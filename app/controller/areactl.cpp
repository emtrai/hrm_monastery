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
 * Filename: areactl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "areactl.h"

#include "logger.h"
#include "area.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"
#include "countryctl.h"

GET_INSTANCE_CONTROLLER_IMPL(AreaCtl)

AreaCtl::AreaCtl():ModelController(KModelHdlArea)
{
    traced;
}

AreaCtl::~AreaCtl()
{
    traced;
}

// Format: Country name id, name id, Name, remark
DbModel* AreaCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    ErrCode err = ErrNone;
    Area* item = nullptr;
    QStringList* itemList = nullptr;
    qint32 noItem = 0;
    qint32 idx = 0;
    QString countryNameId;
    QString nameId;
    QString name;
    QString remark;
    if (!items) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone && fmt != KDataFormatStringList) {
        err = ErrNotSupport;
        loge("invalid data format '%s", STR2CHA(fmt));
    } else {
        itemList = (QStringList*) items;
        noItem = itemList->length();
        logd("noItem %d", noItem);
    }

    if (err == ErrNone && (noItem < 3)) { // require country name id, name id, name
        err = ErrShortData;
        loge("Not enouth field, %d < 3", noItem);
    }

    if (err == ErrNone) {
        countryNameId = itemList->at(idx++);
        nameId = itemList->at(idx++);
        name = itemList->at(idx++);
        if (noItem > idx) {
            remark = itemList->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
        }

        logd("countryNameId '%s'", STR2CHA(countryNameId));
        logd("nameId '%s'", STR2CHA(nameId));
        logd("name '%s'", STR2CHA(name));
        if (countryNameId.isEmpty() || nameId.isEmpty() || name.isEmpty()) {
            err = ErrInvalidData;
            loge("data is invalid, nameid/countrynameid or name is empty");
        }
    }
    if (err == ErrNone) {
        item = static_cast<Area*>(Area::build());
        if (!item) {
            loge("No memory");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        logd("search country with nameid '%s'", STR2CHA(countryNameId));
        DbModel* country = COUNTRYCTL->getModelByNameId(countryNameId);
        if (country) {
            item->setCountryUid(country->uid());
        } else {
            loge("Not found country with name id '%s'", STR2CHA(countryNameId));
            err = ErrNotFound;
        }
    }
    if (err == ErrNone) {
        item->setNameId(nameId);
        item->setName(name);
        if (!remark.isEmpty()) {
            item->setRemark(remark);
        }
    } else {
        loge("failed to import/build model for area, err=%d", err);
        if (item) {
            delete item;
            item = nullptr;
        }
    }
    tracede;
    return item;
}

const char *AreaCtl::getPrebuiltFileName()
{
    return KPrebuiltAreaCSVFileName;
}

DbModelBuilder AreaCtl::getMainBuilder()
{
    return &Area::build;
}

