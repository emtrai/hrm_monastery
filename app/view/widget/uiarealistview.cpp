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
 * Filename: uiarealistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#include "uiarealistview.h"

#include "areactl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "area.h"
#include "utils.h"
#include "mainwindow.h"
#include "uidepartmentlistview.h"
#include "uitableviewfactory.h"

UIAreaListView::UIAreaListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIAreaListView::~UIAreaListView()
{
    traced;
}

void UIAreaListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
    mHeader.append(tr("Quốc gia"));
    mHeader.append(tr("Quản lý vùng"));
}

void UIAreaListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;

    UICommonListView::updateItem(item, tblItem);
    Area* model = (Area*) item;
    tblItem->addValue(model->countryName());
    tblItem->addValue(model->personName());
}


ErrCode UIAreaListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UIAreaListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}


void UIAreaListView::onViewItem(UITableWidgetItem *item)
{
    traced;

}

Controller *UIAreaListView::getController()
{
    traced;
    return AREACTL;
}

QList<DbModel *> UIAreaListView::getListItem()
{
    traced;
    return AREACTL->getAllItems();
}


QList<UITableMenuAction *> UIAreaListView::getMenuMultiItemActions(const QMenu *menu,
                                                                   const QList<UITableItem *>& items)
{
    traced;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);

    return actionList;

}

