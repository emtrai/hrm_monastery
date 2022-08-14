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
 * Filename: uisaintlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uisaintlistview.h"
#include "saintctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "saint.h"
#include "utils.h"

UISaintListView::UISaintListView(QWidget *parent):
    UICommonListView(parent)
{


}

UISaintListView::~UISaintListView()
{
    traced;
}

ErrCode UISaintListView::onLoad()
{
    QList<Saint*> items = SaintCtl::getInstance()->getListSaints();
    traced;
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (Saint* item, items) {
        mItemList.append(static_cast<DbModel*>(item));
    }
    return ErrNone;
}

void UISaintListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    UICommonListView::updateItem(item, tblItem);
    traced;
    tblItem->addValue(Utils::date2String(static_cast<Saint*>(item)->feastDay(), "M/D"));

}

void UISaintListView::initHeader()
{
    UICommonListView::initHeader();
    traced;
    mHeader.append("Memory Day");
}
