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
#include "uidepartmentlistview.h"

#include "departctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "community.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"
#include "uidepartmentpersonlistview.h"

UIDepartmentListView::UIDepartmentListView(QWidget *parent):
    UICommonListView(parent),
    mCommunity(nullptr)
{
    traced;
}

UIDepartmentListView::~UIDepartmentListView()
{
    traced;
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
//    int idx = item->idx();
//    logd("idx=%d",idx);
//    if (idx < mItemList.length()){
//        Community* model = (Community*)mItemList.value(idx);
//        if (model == nullptr) {
//            loge("no data");
//            return;
//        }
//        model->dump();
//        QString uid = model->uid();
//        if (uid.isEmpty()) {
//            loge("no uid");
//            return;
//        }
//        UICommunityPersonListView* view = (UICommunityPersonListView*)UITableViewFactory::getView(ViewType::COMMUNITY_PERSON);

//        logd("community uid %s", uid.toStdString().c_str());
////        view->setCommunityUid(uid);
//        view->setCommunity(model);
//        view->setTitle(model->name());
//        MainWindow::getInstance()->switchView(view);
//    } else {
//        loge("Invalid idx");
//        // TODO: popup message???
    //    }
}

QString UIDepartmentListView::getTitle()
{
    traced;
    QString title;
    if (mCommunity == nullptr)
        title = tr("Tất cả phòng ban");
    else
        title = QString(tr("Phòng ban của cộng đoàn: %1")).arg(mCommunity->name());

    return title;
}

void UIDepartmentListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
    mHeader.append(tr("Cộng đoàn"));
    tracede;
}

void UIDepartmentListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    UICommonListView::updateItem(item, tblItem);
    Department* dept = (Department*)item;
    tblItem->addValue(dept->communityName());
    tracede;
}



ErrCode UIDepartmentListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    Department* item = dynamic_cast<Department*>(act->getData());
    if (item != nullptr) {
        item->dump();
        UIDepartmentPersonListView* view = (UIDepartmentPersonListView*)
            UITableViewFactory::getView(ViewType::VIEW_DEPARTMENT_PERSON);

        view->setDepartment(item);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no department info");
        ret = ErrNoData;
    }

    tracedr(ret);
    return ret;

}

ErrCode UIDepartmentListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* data = (DbModel*)(act->getData());
    if (data != nullptr) {
        data->dump();
    }

    return ret;
}

QList<UITableMenuAction *> UIDepartmentListView::getMenuItemActions(const QMenu* menu,
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

ErrCode UIDepartmentListView::onLoad()
{
    QList<Department*> items;
    ErrCode ret = ErrNone;
    traced;
    if (mCommunity == nullptr)
        items = DEPART->getDeptList();
    else
        items = DEPART->getDeptList(mCommunity->uid());
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (Department* item, items) {
        mItemList.append(static_cast<DbModel*>(item));
    }
    tracedr(ret);
    return ret;
}

Community *UIDepartmentListView::community() const
{
    return mCommunity;
}

void UIDepartmentListView::setCommunity(Community *newCommunity)
{
    mCommunity = newCommunity;
}
