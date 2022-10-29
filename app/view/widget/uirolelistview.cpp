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
ErrCode UIRoleListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UIRoleListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

void UIRoleListView::onViewItem(UITableWidgetItem *item)
{
    traced;
    int idx = item->idx();
    logd("idx=%d",idx);
    if (idx < mItemList.length()){
        Role* model = (Role*)mItemList.value(idx);
        if (model == nullptr) {
            loge("no data");
            return;
        }
        model->dump();
        // TODO: show information
    } else {
        loge("Invalid idx");
        // TODO: popup message???
    }
}

QString UIRoleListView::getTitle()
{
    return tr("Vai trò/vị trí");
}

ErrCode UIRoleListView::onLoad()
{
    QList<Role*> items = ROLECTL->getList();
    traced;
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (Role* item, items) {
        mItemList.append(dynamic_cast<DbModel*>(item));
    }
    return ErrNone;
}

