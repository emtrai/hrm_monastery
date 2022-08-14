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
 * Filename: uicommunitylistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uicommunitylistview.h"
#include "communityctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "community.h"
#include "utils.h"


UICommunityListView::UICommunityListView(QWidget *parent):
    UICommonListView(parent)
{


}

UICommunityListView::~UICommunityListView()
{
    traced;
}

ErrCode UICommunityListView::onLoad()
{
    QList<Community*> items = CommunityCtl::getInstance()->getAll();
    traced;
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (Community* item, items) {
        mItemList.append(static_cast<DbModel*>(item));
    }
    return ErrNone;
}
