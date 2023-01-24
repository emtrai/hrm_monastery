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
 * Filename: uicommunitylistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uicommunitylistview.h"
#include "communityctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "community.h"
#include "utils.h"
#include "mainwindow.h"
#include "uicommunitypersonlistview.h"
#include "uidepartmentlistview.h"
#include "uitableviewfactory.h"
#include "uipersonlistview.h"
#include "uitableview.h"

UICommunityListView::UICommunityListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UICommunityListView::~UICommunityListView()
{
    traced;
}

void UICommunityListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
    mHeader.append(tr("Trạng thái hoạt động"));
    mHeader.append(tr("Vùng"));
    mHeader.append(tr("Địa chỉ"));
    mHeader.append(tr("Tổng phụ trách"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
}

void UICommunityListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;

    UICommonListView::updateItem(item, tblItem);
    Community* model = (Community*) item;
    tblItem->addValue(COMMUNITYCTL->status2Name(model->getStatus()));
    tblItem->addValue(model->areaName());
    tblItem->addValue(model->addr());
    tblItem->addValue(model->currentCEO());
    tblItem->addValue(model->tel());
    tblItem->addValue(model->email());
}


ErrCode UICommunityListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    MainWindow::showAddEditCommunity(true, nullptr, this);
    return ErrNone;
}

ErrCode UICommunityListView::onMenuActionImport(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    MainWindow::showImportDlg(IMPORT_TARGET_COMMUNITY);
    tracedr(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    // TODO: don't delete root community
    return ErrNone;
}

ErrCode UICommunityListView::onMenuActionAddPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNotImpl;
    Utils::showDlgUnderDev();
    // TODO: implement it
    tracedr(err);
    return err;
}

ErrCode UICommunityListView::onMenuActionAddDepart(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNotImpl;
    // TODO: implement it
    Utils::showDlgUnderDev();
    tracedr(err);
    return err;
}

//ErrCode UICommunityListView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
//{
//    traced;
//    if (act == nullptr) {
//        loge("Invalid arg");
//        return ErrInvalidArg;
//    }

//    Community* model = (Community*)act->getData();
//    if (model == nullptr) {
//        loge("no data");
//        return ErrInvalidData;
//    }
//    model->dump();
//    QString uid = model->uid();
//    if (uid.isEmpty()) {
//        loge("no uid");
//        return ErrNoData;
//    }
//    UICommunityPersonListView* view = (UICommunityPersonListView*)UITableViewFactory::getView(ViewType::COMMUNITY_PERSON);

//    logd("community uid %s", uid.toStdString().c_str());
//    view->setCommunityUid(uid);
//    MainWindow::getInstance()->switchView(view);
//    // TODO: handle it
//    return ErrNone;
//}
void UICommunityListView::onViewItem(UITableWidgetItem *item)
{
//    traced;
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

void UICommunityListView::onEditItem(UITableWidgetItem *item)
{
    traced;
    DbModel* comm = item->itemData();
    MainWindow::showAddEditCommunity(false, dynamic_cast<Community*>(comm), this);
    tracede;
}

QString UICommunityListView::getTitle()
{
    return tr("Danh sách cộng đoàn");
}



ErrCode UICommunityListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    Community* community = dynamic_cast<Community*>(act->getData());
    if (community != nullptr) {
        community->dump();
        UITableView* view = (UITableView*)MAIN->getView(ViewType::PERSON);
        view->addFilter(KItemCommunity, QString(), QVariant(community->uid()));
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no community info");
        ret = ErrNoData;
    }

    tracedr(ret);
    return ret;

}

// Show lis of people in all period
ErrCode UICommunityListView::onMenuActionListPersonHistory(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNone;
    UNDER_DEV("Danh sách nữ tu ở tất cả thời kì");
    tracedr(err);
    return err;
}

ErrCode UICommunityListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    Community* community = dynamic_cast<Community*>(act->getData());
    if (community != nullptr) {
        community->dump();
        UIDepartmentListView* view = (UIDepartmentListView*)UITableViewFactory::getView(ViewType::VIEW_DEPARTMENT);

        view->setCommunity(community);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no community info");
        ret = ErrNoData;
    }

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
//        //        view->setCommunityUid(uid);
//        view->setCommunity(model);
//        view->setTitle(model->name());
//        MainWindow::getInstance()->switchView(view);
//    } else {
//        loge("Invalid idx");
//        // TODO: popup message???
//    }
    tracedr(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionListManagers(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNotImpl;
    Utils::showDlgUnderDev();
    // TODO: implement it
    tracedr(err);
    return err;
}

QList<UITableMenuAction *> UICommunityListView::getMenuItemActions(const QMenu* menu,
                                                                   UITableWidgetItem* item)
{
    traced;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuItemActions(menu, item);

    actionList.append(UITableMenuAction::build(tr("Xuất danh sách nữ tu hiện tại"), this, item)
                                                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                                                       return this->onMenuActionExportListPerson(m, a);
                                                                                   }));
    actionList.append(UITableMenuAction::build(tr("Danh sách nữ tu hiện tại"), this, item)
                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionListPerson(m, a);
                                           }));
    actionList.append(UITableMenuAction::build(tr("Lịch sử danh sách nữ tu"), this, item)
                                                                                ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                                                    return this->onMenuActionListPersonHistory(m, a);
                                                                                }));
    actionList.append(UITableMenuAction::build(tr("Danh sách ban"), this, item)
                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                              return this->onMenuActionListDepartment(m, a);
                                          }));
    actionList.append(UITableMenuAction::build(tr("Ban Quản Lý"), this, item)
                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                              return this->onMenuActionListManagers(m, a);
                                          }));
    actionList.append(UITableMenuAction::build(tr("Thêm nữ tu"), this, item)
                                                ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                    return this->onMenuActionListDepartment(m, a);
                                                }));
    actionList.append(UITableMenuAction::build(tr("Thêm ban"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionListDepartment(m, a);
                                                   }));
    return actionList;

}

QList<UITableMenuAction *> UICommunityListView::getMenuCommonActions(const QMenu *menu)
{
    traced;
    QList<UITableMenuAction*> actionList = UITableView::getMenuCommonActions(menu);
    actionList.append(UITableMenuAction::build(tr("Nhập từ tập tin"), this)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionImport(m, a);
                                                   }));
                      tracede;
                      return actionList;
}

ErrCode UICommunityListView::onMenuActionExportListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNotImpl;
    UNDER_DEV("Xuất danh sách thành viên hiện tại");
    tracedr(err);
    return err;
}

ErrCode UICommunityListView::onLoad()
{
//    QList<Community*> items = COMMUNITYCTL->getAllItems();
    traced;
    mItemList.clear(); // TODO: clean up item data
    mItemList = COMMUNITYCTL->getAllItems();
    // TODO: loop to much, redundant, do something better?
//    foreach (Community* item, items) {
//        mItemList.append(dynamic_cast<DbModel*>(item));
//    }
    tracede;
    return ErrNone;
}

ErrCode UICommunityListView::onReload()
{
    traced;
    COMMUNITYCTL->loadFromDb();
    onLoad();
    tracede;
    return ErrNone;
}

void UICommunityListView::onDbModelReady(ErrCode ret, DbModel *model, DlgCommonEditModel *dlg)
{
    traced;
    if (ret == ErrNone) {
        if (model){
            model->dump();
        }
        onReload();
    }

    tracede;
}
