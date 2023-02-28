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
 * Filename: uirolelistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#include "uirolelistview.h"

#include "rolectl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "role.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"

UIRoleListView::UIRoleListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIRoleListView::~UIRoleListView()
{
    traced;
}

QString UIRoleListView::getTitle()
{
    return tr("Vai trò/vị trí");
}

DbModel *UIRoleListView::onNewModel()
{
    return Role::build();
}

ErrCode UIRoleListView::onLoad()
{
    QList<DbModel*> items = ROLECTL->getAllItems();
    traced;
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (DbModel* item, items) {
        mItemList.append(dynamic_cast<DbModel*>(item));
    }
    return ErrNone;
}

