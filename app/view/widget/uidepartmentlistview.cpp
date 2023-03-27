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
    tracein;
}

UIDepartmentListView::~UIDepartmentListView()
{
    tracein;
}

void UIDepartmentListView::initHeader()
{
    tracein;
    UICommonListView::initHeader();
}

void UIDepartmentListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;

    UICommonListView::updateItem(item, tblItem, idx);
}


ModelController *UIDepartmentListView::getController()
{
    tracein;
    return DEPART;
}

QList<DbModel *> UIDepartmentListView::getListItem()
{
    tracein;
    return DEPART->getAllItemsFromDb(); // TODO: getAllItem???
}

DbModel *UIDepartmentListView::onNewModel()
{
    return Department::build();
}

