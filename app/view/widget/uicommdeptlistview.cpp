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
    tracein;
}

UICommDeptListView::~UICommDeptListView()
{
    tracein;
    if (mCommunity) delete mCommunity;

    mCommunity = nullptr;
}
QString UICommDeptListView::getTitle()
{
    tracein;
    QString title;
    title = QString(tr("Phòng ban của cộng đoàn: %1")).arg(mCommunity?mCommunity->name():tr("Không rõ"));

    return title;
}

void UICommDeptListView::initHeader()
{
    tracein;
    UICommonListView::initHeader();
//    mHeader.append(tr("Cộng đoàn"));
    traceout;
}

void UICommDeptListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UICommonListView::updateItem(item, tblItem, idx);
//    CommunityDept* dept = (CommunityDept*)item;

//    tblItem->addValue(dept->communityUid());
    traceout;
}

DbModel *UICommDeptListView::onNewModel()
{
    return CommunityDept::build();
}

ErrCode UICommDeptListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
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

    traceret(ret);
    return ret;

}

ErrCode UICommDeptListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    DbModel* data = (DbModel*)(act->getData());
    if (data != nullptr) {
        data->dump();
    }

    return ret;
}

QList<UITableMenuAction *> UICommDeptListView::getMenuSingleSelectedItemActions(const QMenu* menu,
                                            UITableCellWidgetItem *item)
{
    tracein;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);

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
    tracein;
    if (mCommunity) {
        setTitle(getTitle());
        logd("get list of dept of community '%s'", STR2CHA(mCommunity->toString()));
        items = COMMUNITYDEPTCTL->getListDept(mCommunity->uid());
        RELEASE_LIST_DBMODEL(mItemList);
        mItemList.clear(); // TODO: clean up item data
        // TODO: loop to much, redundant, do something better?
        logd("found %ld dept", items.size());
        foreach (DbModel* item, items) {
            mItemList.append(static_cast<DbModel*>(item));
    //        mItemList.append(std::shared_ptr<DbModel>(item));
        }
    }
    traceret(ret);
    return ret;
}

Community *UICommDeptListView::community() const
{
    return mCommunity;
}

void UICommDeptListView::setCommunity(Community *newCommunity)
{
    tracein;
    if (mCommunity) {
        delete mCommunity;
        mCommunity = nullptr;
    }
    if (newCommunity) {
        mCommunity = (Community*)((DbModel*)newCommunity)->clone();
    } else {
        logw("No community to set");
    }
    traceout;
}
