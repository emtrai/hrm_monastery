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
 * Filename: ethnicctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "ethnicctl.h"
#include "logger.h"
#include "ethnic.h"
#include "defs.h"
#include "dbctl.h"
#include "countryctl.h"


GET_INSTANCE_CONTROLLER_IMPL(EthnicCtl)

EthnicCtl::EthnicCtl():ModelController(KModelHdlEthnic)
{
    traced;
}

// Format: name id, country name id, name[, remark]
DbModel *EthnicCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    ErrCode err = ErrNone;
    Ethnic* item = nullptr;
    QStringList* itemList = nullptr;
    qint32 noItem = 0;
    qint32 idx = 0;
    QString nameId;
    QString countryNameId;
    QString name;
    QString remark;
    if (!items) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone && fmt != KDataFormatStringList) {
        err = ErrNotSupport;
        loge("invalid data format '%s", STR2CHA(fmt));
    }
    if (err == ErrNone) {
        itemList = (QStringList*) items;
        noItem = itemList->length();
        logd("noItem %d", noItem);
    }

    if (err == ErrNone && (noItem < 2)) { // require name id, name
        err = ErrShortData;
        loge("Not enouth field, %d < 3", noItem);
    }

    if (err == ErrNone) {
        nameId = itemList->at(idx++);
        countryNameId = itemList->at(idx++);
        name = itemList->at(idx++);
        if (noItem > idx) {
            remark = itemList->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
        }

        logd("nameId '%s'", STR2CHA(nameId));
        logd("countryNameId '%s'", STR2CHA(countryNameId));
        logd("name '%s'", STR2CHA(name));
        if (nameId.isEmpty() || name.isEmpty()) {
            err = ErrInvalidData;
            loge("data is invalid, nameid/countrynameid or name is empty");
        }
    }
    if (err == ErrNone) {
        DbModelBuilder builder = getMainBuilder();
        if (builder) {
            item = (Ethnic*)builder();
            if (!item) {
                loge("No memory");
                err = ErrNoMemory;
            }
        } else {
            loge("No main builder");
            err = ErrNotAvail;
        }
    }

    if (err == ErrNone) {
        item->setNameId(nameId);
    }

    if (err == ErrNone) {
        logd("country nameid '%s'", STR2CHA(countryNameId));
        DbModel* model = COUNTRYCTL->getModelByNameId(countryNameId);
        if (model) {
            item->setCountryUid(model->uid());
            item->setCountryName(model->name());
            item->setCountryDbId(model->dbId());
            delete model;
        } else {
            err = ErrNotFound;
            loge("Not found country '%s'", STR2CHA(countryNameId));
        }
    }

    if (err == ErrNone) {
        item->setName(name);
        if (!remark.isEmpty()) {
            item->setRemark(remark);
        }
    }

    if (err != ErrNone) {
        loge("failed to import/build model, err=%d", err);
        if (item) {
            delete item;
            item = nullptr;
        }
    }
    tracede;
    return item;
}

const char *EthnicCtl::getPrebuiltFileName()
{
    return KPrebuiltEthnicCSVFileName;
}

DbModelBuilder EthnicCtl::getMainBuilder()
{
    return &Ethnic::build;
}


