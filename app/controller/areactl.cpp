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

GET_INSTANCE_IMPL(AreaCtl)

AreaCtl::AreaCtl()
{
    traced;
}

// Format: Country ID, ID, Name, remark
DbModel *AreaCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Area* item = new Area();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setCountryUid(itemList->at(idx++));
    QString nameid = itemList->at(idx++);
    item->setNameId(nameid);
    item->setName(itemList->at(idx++));
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}

const char *AreaCtl::getPrebuiltFileName()
{
    return KPrebuiltAreaCSVFileName;
}

const char *AreaCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

QList<DbModel *> AreaCtl::getItemFromDb()
{
    traced;
    return DB->getModelHandler(KModelHdlArea)->getAll(&Area::builder);
}


