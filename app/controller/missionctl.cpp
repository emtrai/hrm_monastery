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
 * Filename: missionctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/15/2022
 * Brief:
 */
#include "missionctl.h"
#include "logger.h"
#include "mission.h"
#include "defs.h"
#include "dbctl.h"


MissionCtl* MissionCtl::gInstance = nullptr;


MissionCtl::MissionCtl()
{
    traced;
}

// Format: vn, Vietname,Asian, Asia,
DbModel *MissionCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Mission* item = new Mission();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setNameId(itemList->at(idx++));
    item->setName(itemList->at(idx++));
    tracede;
    return item;
}

QList<Mission *> MissionCtl::getCountryList()
{
    traced;
    // TODO: check if file update then reload??
    return mMissionList;
}


MissionCtl *MissionCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new MissionCtl();
    }
    return gInstance;
}

void MissionCtl::onLoad()
{

    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltMissionCSVFileName, KFileTypeCSV);

    QList items = DB->getModelHandler(KModelHdlMission)->getAll(&Mission::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        mMissionList.append((Mission*)model);
    }
}
