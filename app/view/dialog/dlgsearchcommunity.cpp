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
#include "communityctl.h"
#include "stringdefs.h"


DlgSearchCommunity *DlgSearchCommunity::build(QWidget *parent, bool isMulti)
{
    tracein;
    // TODO: create factory class and move this to factory???
    DlgSearchCommunity* ret = new DlgSearchCommunity(parent, isMulti);
    ret->setupUi();
    traceout;
    return ret;
}

DlgSearchCommunity::DlgSearchCommunity(QWidget *parent, bool isMulti):
    DlgSearch(parent, isMulti)
{
    traced;
}

DlgSearchCommunity::~DlgSearchCommunity()
{
    traced;
}

QString DlgSearchCommunity::getTitle()
{
    return STR_SEARCH_COMMUNITY;
}


ErrCode DlgSearchCommunity::doSearch(const QString &keyword, QList<DbModel*>& items)
{
    tracein;
    clearAll();
    logi("Start search community keyword '%s'", STR2CHA(keyword));
    ErrCode err = COMMUNITYCTL->search(keyword, &items);
    logd("search err=%d, cnt=%lld", err, items.count());
    traceout;
    return err;
}

ErrCode DlgSearchCommunity::doGetAll(QList<DbModel*>& items)
{
    tracein;
    ErrCode err = ErrNone;
    items = COMMUNITYCTL->getAllItemsFromDb();
    logd("get all cnt=%lld", items.count());
    traceout;
    return err;

}
