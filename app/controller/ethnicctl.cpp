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


GET_INSTANCE_IMPL(EthnicCtl)

EthnicCtl::EthnicCtl():CommonCtl(KModelHdlEthnic)
{
    traced;
}


// Format: country short name, name[, remark]
DbModel *EthnicCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Ethnic* item = new Ethnic();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setNameId(itemList->at(idx++));
    item->setCountryShortName(itemList->at(idx++));
    item->setName(itemList->at(idx++));
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}

const char *EthnicCtl::getPrebuiltFileName()
{
    return KPrebuiltEthnicCSVFileName;
}

const char *EthnicCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

DbModelBuilder EthnicCtl::getMainBuilder()
{
    return &Ethnic::build;
}


