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
 * Filename: dlgsearchcommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/12/2022
 * Brief:
 */
#include "dlgsearchcommunity.h"
#include "logger.h"
#include "community.h"
#include "communityctl.h"


DlgSearchCommunity::~DlgSearchCommunity()
{
    traced;
}

DlgSearchCommunity *DlgSearchCommunity::build(QWidget *parent, bool isMulti)
{
    traced;
    // TODO: create factory class and move this to factory???
    DlgSearchCommunity* ret = new DlgSearchCommunity(parent, isMulti);
    ret->setupUi();
    tracede;
    return ret;
}

DlgSearchCommunity::DlgSearchCommunity(QWidget *parent, bool isMulti):
    DlgSearch(parent, isMulti)
{
    traced;
}

QString DlgSearchCommunity::getTitle()
{
    return tr("Tìm kiếm Cộng đoàn");
}


int DlgSearchCommunity::onSearch(const QString &keyword)
{
    traced;
    clearAll();
    logd("Start search community %s", keyword.toStdString().c_str());
    int cnt = COMMUNITYCTL->search(keyword, &mListItems);
    logd("search cnt=%d", cnt);
    tracede;
    return mListItems.count();
}

int DlgSearchCommunity::onGetAll()
{
    traced;
    clearAll();
    mListItems = COMMUNITYCTL->getAllItemsFromDb();
    logd("get all cnt=%d", mListItems.count());
    tracede;
    return mListItems.count();

}

void DlgSearchCommunity::clearAll()
{
    traced;

    DlgSearch::clearAll();
    // TODO: clear each element of list????
    mListItems.clear();
    tracede;
}

DbModel *DlgSearchCommunity::getItemAtIdx(int idx)
{
    traced;
    DbModel* ret = nullptr;
    logd("get item at idx=%d", idx);
    if (idx >= 0 && idx < mListItems.count()) {
        ret = (DbModel*)mListItems[idx];
    } else {
        loge("invalid idx %d", idx);
    }
    tracede;
    return ret;
}
