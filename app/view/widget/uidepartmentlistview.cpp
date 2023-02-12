/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: uidepartmentlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/12/2023
 * Brief:
 */
#include "uidepartmentlistview.h"

#include "areactl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "department.h"
#include "utils.h"
#include "mainwindow.h"
#include "uicommdeptlistview.h"
#include "uitableviewfactory.h"
#include "departctl.h"

UIDepartmentListView::UIDepartmentListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIDepartmentListView::~UIDepartmentListView()
{
    traced;
}

void UIDepartmentListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
}

void UIDepartmentListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;

    UICommonListView::updateItem(item, tblItem);
}


ErrCode UIDepartmentListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UIDepartmentListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}


void UIDepartmentListView::onViewItem(UITableWidgetItem *item)
{
    traced;

}

Controller *UIDepartmentListView::getController()
{
    traced;
    return DEPART;
}

QList<DbModel *> UIDepartmentListView::getListItem()
{
    traced;
    return DEPART->getAllDepartment();
}


QList<UITableMenuAction *> UIDepartmentListView::getMenuMultiItemActions(const QMenu *menu,
                                                                   const QList<UITableItem *>& items)
{
    traced;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);

    return actionList;

}

