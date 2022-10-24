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
 * Filename: rolectl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "rolectl.h"

#include "logger.h"
#include "role.h"
#include "dbctl.h"
#include "utils.h"
#include "defs.h"

GET_INSTANCE_IMPL(RoleCtl)

RoleCtl::RoleCtl()
{
    traced;
}

// Format: Name,period,startdate,enddate,remark
DbModel *RoleCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Role* item = new Role();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setUid(itemList->at(idx++));
    item->setName(itemList->at(idx++));

    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}


const QList<Role *> RoleCtl::getList()
{
    traced;

    return mList;
}

ErrCode RoleCtl::reloadDb()
{
    traced;
    mList.clear();
    // TODO: loop to delete each element????

    QList items = DB->getModelHandler(KModelHdlRole)->getAll(&Role::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Role* item = (Role*)model;
        mList.append(item);
    }
    return ErrNone;
}

void RoleCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltRoleCSVFileName, KFileTypeCSV);
    // TODO: should do lazy load??? load all consume much memory
    reloadDb();
    tracede;
}
