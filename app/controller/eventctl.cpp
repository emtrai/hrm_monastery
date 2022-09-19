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
 * Filename: eventctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/19/2022
 * Brief:
 */
#include "eventctl.h"

#include "logger.h"
#include "event.h"
#include "dbctl.h"
#include "utils.h"
#include "defs.h"


GET_INSTANCE_IMPL(EventCtl)

EventCtl::EventCtl()
{
    traced;
}


EventCtl::~EventCtl()
{
    traced;
}

// Format: id,name,remark
DbModel *EventCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Event* item = new Event();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    QString nameid = itemList->at(idx++).trimmed();
    QString name = itemList->at(idx++).trimmed();
    item->setName(name);
    if (nameid.isEmpty())
        nameid = Utils::UidFromName(name, UidNameConvertType::NO_VN_MARK_UPPER);
    item->setNameId(nameid);
    if (sz > idx) {
        QString remark; // last item is remark, so merge all items as remark
        for (; idx < sz; idx++){
            if (!remark.isEmpty()){
                remark += ",";
            }
            remark += itemList->at(idx);
        }

        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}


const QList<Event *> EventCtl::getEventList()
{
    traced;

    return mList;
}

ErrCode EventCtl::reloadDb()
{
    traced;
    mList.clear();
    // TODO: loop to delete each element????

    QList items = DB->getModelHandler(KModelHdlEvent)->getAll(&Event::build);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Event* item = (Event*)model;
        mList.append(item);
    }
    return ErrNone;
}

void EventCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltEventCSVFileName, KFileTypeCSV);
    // TODO: should do lazy load??? load all consume much memory
    reloadDb();
    tracede;
}
