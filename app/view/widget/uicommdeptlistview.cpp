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
 * Filename: uidepartmentlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#include "uicommdeptlistview.h"

#include "departctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "community.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"
#include "uidepartmentpersonlistview.h"
#include "communitydeptctl.h"
#include "communitydept.h"

UICommDeptListView::UICommDeptListView(QWidget *parent):
    UICommonListView(parent),
    mCommunity(nullptr)
{
    traced;
}

UICommDeptListView::~UICommDeptListView()
{
    traced;
}
QString UICommDeptListView::getTitle()
{
    traced;
    QString title;
    title = QString(tr("Phòng ban của cộng đoàn: %1")).arg(mCommunity->name());

    return title;
}

void UICommDeptListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
//    mHeader.append(tr("Cộng đoàn"));
    tracede;
}

void UICommDeptListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    UICommonListView::updateItem(item, tblItem);
//    CommunityDept* dept = (CommunityDept*)item;

//    tblItem->addValue(dept->communityUid());
    tracede;
}

DbModel *UICommDeptListView::onNewModel()
{
    return CommunityDept::build();
}



ErrCode UICommDeptListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    CommunityDept* item = dynamic_cast<CommunityDept*>(act->getData());
    if (item != nullptr) {
        item->dump();
        UIDepartmentPersonListView* view = (UIDepartmentPersonListView*)
            UITableViewFactory::getView(ViewType::VIEW_DEPARTMENT_PERSON);

        view->setCommDept(item);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no department info");
        ret = ErrNoData;
    }

    tracedr(ret);
    return ret;

}

ErrCode UICommDeptListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* data = (DbModel*)(act->getData());
    if (data != nullptr) {
        data->dump();
    }

    return ret;
}

QList<UITableMenuAction *> UICommDeptListView::getMenuItemActions(const QMenu* menu,
                                            UITableWidgetItem *item)
{
    traced;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuItemActions(menu, item);

    actionList.append(UITableMenuAction::build(tr("Danh sách thành viên"), this, item)
                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionListPerson(m, a);
                                           }));
    return actionList;

}

ErrCode UICommDeptListView::onLoad()
{
    QList<DbModel*> items;
    ErrCode ret = ErrNone;
    traced;
    items = COMMUNITYDEPTCTL->getDepartList(mCommunity->uid());
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (DbModel* item, items) {
        mItemList.append(static_cast<DbModel*>(item));
//        mItemList.append(std::shared_ptr<DbModel>(item));
    }
    tracedr(ret);
    return ret;
}

Community *UICommDeptListView::community() const
{
    return mCommunity;
}

void UICommDeptListView::setCommunity(Community *newCommunity)
{
    mCommunity = newCommunity;
}
