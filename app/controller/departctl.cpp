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
 * Filename: departctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "departctl.h"

#include "logger.h"
#include "department.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"

GET_INSTANCE_IMPL(gInstance,DepartCtl)

DepartCtl::DepartCtl()
{
    traced;
}

// Format: Country short name, Province name[, <parent province if any>]
DbModel *DepartCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Department* item = new Department();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setShortName(itemList->at(idx++));
    QString nameid = itemList->at(idx++);
    item->setNameId(nameid + item->shortName());
    item->setName(itemList->at(idx++));
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}


const QList<Department *> *DepartCtl::getDeptList()
{
    traced;

    return &mDeptList;
}

void DepartCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltDeptJsonFileName, KFileTypeJson);
    // TODO: should do lazy load??? load all consume much memory
    QList items = DB->getModelHandler(KModelHdlDept)->getAll(&Department::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Department* item = (Department*)model;
        mDeptList.append(item);
    }
    tracede;
}
