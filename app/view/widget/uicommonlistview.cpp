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
 * Filename: uicommonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uicommonlistview.h"
#include "logger.h"


UICommonListView::UICommonListView(QWidget *parent):
    UITableView(parent)
{
}

UICommonListView::~UICommonListView()
{
    traced;
}

QList<UITableItem *> UICommonListView::getListItem(qint32 page, qint32 perPage, qint32 totalPages)
{
    QList<UITableItem *> items;
    traced;
    foreach (DbModel* item, mItemList) {
        UITableItem* tblItem = UITableItem::build(item);
        updateItem(item, tblItem);
        items.append(tblItem);
    }
    return items;
}

void UICommonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(item->uid());
    tblItem->addValue(item->name());
}

qint32 UICommonListView::getTotalItems()
{
    traced;
    return mItemList.count();
}

ErrCode UICommonListView::onLoad()
{
    traced;
    return ErrNone;
}

void UICommonListView::initHeader()
{
    traced;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Mã định danh"));
    mHeader.append(tr("Tên"));
}
