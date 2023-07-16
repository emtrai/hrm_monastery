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
#include "datetimeutils.h"
#include "mainwindow.h"
#include "uipeopleincommunitylistview.h"
#include "uicommdeptlistview.h"
#include "uitableviewfactory.h"
#include "uipersonlistview.h"
#include "uitableview.h"
#include <QFileDialog>
#include "personctl.h"
#include "person.h"
#include "dialogutils.h"

UICommunityListView::UICommunityListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
}

UICommunityListView::~UICommunityListView()
{
    tracein;
}

void UICommunityListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    COMMUNITYCTL->addListener(this);
    traceout;
}

void UICommunityListView::initHeader()
{
    tracein;
    UICommonListView::initHeader();
    mHeader.append(tr("Trạng thái hoạt động"));
    mHeader.append(tr("Ngày bổn mạng"));
    mHeader.append(tr("Nhiệm vụ xã hội"));
    mHeader.append(tr("Vùng"));
    mHeader.append(tr("Tổng phụ trách"));
    mHeader.append(tr("Giáo xứ"));
    mHeader.append(tr("Địa chỉ"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    mHeader.append(tr("Quốc gia"));
    mHeader.append(tr("Cộng đoàn quản lý"));
}

void UICommunityListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;

    UICommonListView::updateItem(item, tblItem, idx);
    Community* model = (Community*) item;
    tblItem->addValue(DbModel::modelStatus2Name(model->getStatus()));
    tblItem->addValue(DatetimeUtils::date2String(model->feastDate(), DEFAULT_FORMAT_MD));
    tblItem->addValue(model->missionNameString());
    tblItem->addValue(model->areaName());
    tblItem->addValue(model->currentCEOName());
    tblItem->addValue(model->church());
    tblItem->addValue(model->addr());
    tblItem->addValue(model->tel());
    tblItem->addValue(model->email());
    tblItem->addValue(model->countryName());
    tblItem->addValue(model->parentName());
}


ErrCode UICommunityListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    MainWindow::showAddEditCommunity(true, nullptr, this);
    return ErrNone;
}

ErrCode UICommunityListView::onMenuActionImport(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    ret = MainWindow::showImportDlg(IMPORT_TARGET_COMMUNITY);
    traceret(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionAddPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QString fpath;
    if (!act){
        err = ErrInvalidArg;
        loge("add person failed, Empty menu action");
    }
    if (err == ErrNone && !act->getData()) {
        err = ErrInvalidArg;
        loge("add person failed, action no data");
    }
    if (err == ErrNone) {
        Community* community = dynamic_cast<Community*>(act->getData());
        community->dump();

        Person* per = (Person*) Person::build();
        per->setCommunityUid(community->uid());
        per->setCommunityName(community->name());
        logd("set community uid %s", STR2CHA(community->uid()));
        logd("set community name %s", STR2CHA(community->name()));
        MainWindow::showAddEditPerson(true, per, true);
    }
    if (err != ErrNone){
        loge("add person failed, error code %d",err);
        DialogUtils::showErrorBox(QString("Thêm nữ tu lỗi, mã lỗi: %1").arg(err));
    }
    traceret(err);
    return err;
}

ErrCode UICommunityListView::onMenuActionAddDepart(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNotImpl;
    // TODO: implement it
    UNDER_DEV();
    traceret(err);
    return err;
}

ErrCode UICommunityListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showOnHtmlViewer(comm, tr("Cộng đoàn"));
    } else {
        loge("Comm obj is null");
        DialogUtils::showErrorBox("Không có thông tin để xem");
    }
    traceout;
    return ErrNone;//TODO: check to return value
}


void UICommunityListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    DbModel* comm = item->itemData();
    MainWindow::showAddEditCommunity(true, dynamic_cast<Community*>(comm), this);
    traceout;
}

QString UICommunityListView::getTitle()
{
    return tr("Danh sách cộng đoàn");
}

QString UICommunityListView::getName()
{
    return "UICommunityListView";
}

void UICommunityListView::onImportStart(const QString &importName, const QString &fpath, ImportType type)
{
    tracein;
    mSuspendReloadOnDbUpdate = true;
    logd("suspend reload on db update");
    traceout;
}

void UICommunityListView::onImportEnd(const QString &importName, ErrCode err, const QString &fpath, ImportType type)
{
    tracein;
    mSuspendReloadOnDbUpdate = false;
    logd("resume reload on db update");
    reload();
    traceout;
}



ErrCode UICommunityListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Community* community = dynamic_cast<Community*>(act->getData());
    if (community != nullptr) {
        community->dump();
        UITableView* view = (UITableView*)MAIN->getView(ViewType::VIEW_PERSON);
        view->addFilter(KItemCommunity, QString(), QVariant(community->uid()));
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no community info");
        ret = ErrNoData;
    }

    traceret(ret);
    return ret;

}

// Show lis of people in all period
ErrCode UICommunityListView::onMenuActionListAllPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Community* community = dynamic_cast<Community*>(act->getData());
    if (community != nullptr) {
        UIPeopleInCommunityListView* view = (UIPeopleInCommunityListView*)UITableViewFactory::getView(ViewType::VIEW_PEOPLE_IN_COMMUNITY_LIST);

        logd("community to view person %s", STR2CHA(community->toString()));
        view->setCommunity(community);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no community info");
        ret = ErrNoData;
        DialogUtils::showErrorBox(tr("Vui lòng chọn cộng đoàn cần xem"));
    }

    traceret(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Community* community = dynamic_cast<Community*>(act->getData());
    if (community != nullptr) {
        community->dump();
        UICommDeptListView* view = (UICommDeptListView*)UITableViewFactory::getView(ViewType::VIEW_COMMUNITY_DEPT);

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
//        UICommunityPersonListView* view = (UICommunityPersonListView*)UITableViewFactory::getView(ViewType::VIEW_PEOPLE_IN_COMMUNITY);

//        logd("community uid %s", uid.toStdString().c_str());
//        //        view->setCommunityUid(uid);
//        view->setCommunity(model);
//        view->setTitle(model->name());
//        MainWindow::getInstance()->switchView(view);
//    } else {
//        loge("Invalid idx");
//        // TODO: popup message???
//    }
    traceret(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionListManagers(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNotImpl;
    UNDER_DEV();
    // TODO: implement it
    traceret(err);
    return err;
}

QList<UITableMenuAction *> UICommunityListView::getMenuSingleSelectedItemActions(const QMenu* menu,
                                                                   UITableCellWidgetItem* item)
{
    tracein;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);
    // TODO: show all current active people
    actionList.append(UITableMenuAction::build(tr("Xuất danh sách nữ tu hiện tại"), this, item)
                                               ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                   return this->onMenuActionExportListPerson(m, a);
                                               }));
    // TODO: show all current active people
//    actionList.append(UITableMenuAction::build(tr("Xem danh sách nữ tu hiện tại"), this, item)
//                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                               return this->onMenuActionListPerson(m, a);
//                                           }));
    actionList.append(UITableMenuAction::build(tr("Xem danh sách tất cả nữ tu"), this, item)
                                                                                ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                                                    return this->onMenuActionListAllPerson(m, a);
                                                                                }));
    actionList.append(UITableMenuAction::build(tr("Danh sách ban"), this, item)
                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                              return this->onMenuActionListDepartment(m, a);
                                          }));
    //TODO: show managememt board of community
//    actionList.append(UITableMenuAction::build(tr("Ban Quản Lý"), this, item)
//                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                              return this->onMenuActionListManagers(m, a);
//                                          }));
    // TODO: directly add persone to community
//    actionList.append(UITableMenuAction::build(tr("Thêm nữ tu"), this, item)
//                                                ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                                    return this->onMenuActionAddPerson(m, a);
//                                                }));
    //TODO: directly add department to community
//    actionList.append(UITableMenuAction::build(tr("Thêm ban"), this, item)
//                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                                       return this->onMenuActionAddDepart(m, a);
//                                                   }));
    return actionList;

}

QList<UITableMenuAction *> UICommunityListView::getMenuMultiSelectedItemActions(const QMenu *menu, const QList<UITableItem *> &items)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiSelectedItemActions(menu, items);
//    actionList.append(UITableMenuAction::buildMultiItem(tr("Xuất danh sách cộng đoàn"), this, &items)
//                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                                       return this->onMenuActionExportListCommunity(m, a);
//                                                   }));
    traceout;
    return actionList;
}

QList<UITableMenuAction *> UICommunityListView::getMenuCommonActions(const QMenu *menu)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuCommonActions(menu);
    actionList.append(UITableMenuAction::build(tr("Nhập từ tập tin"), this)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionImport(m, a);
                                                   }));
    actionList.append(UITableMenuAction::build(tr("Xuất danh sách cộng đoàn"), this)
                                                ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                    return this->onMenuActionExportListCommunity(m, a);
                                                }));
    traceout;
    return actionList;
}

ErrCode UICommunityListView::onMenuActionExportListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QString fpath;
    Community* community = nullptr;
    if (!act){
        err = ErrInvalidArg;
        loge("export list person failed, Empty menu action");
    }
    if (err == ErrNone && !act->getData()) {
        err = ErrInvalidArg;
        loge("export list person failed, action no data");
    }
    if (err == ErrNone) {
        community = dynamic_cast<Community*>(act->getData());
        if (community->uid().isEmpty()) {
            err = ErrInvalidData;
            loge("comunity '%s' has no uid", STR2CHA(community->toString()));
        }
    }
    if (err == ErrNone) {
        logi("export people in community uid '%s'", STR2CHA(community->uid()));
        QList<DbModel*> items;
        err = PERSONCTL->getListPersonInCommunity(community->uid(), items);
        if (!items.empty()) {
            err = MainWindow::exportListItems(&items, KModelNamePerson, PERSONCTL, tr("Xuất danh sách nữ tu"), EXPORT_XLSX);
            RELEASE_LIST_DBMODEL(items);
        } else {
            logw("nothing to export");
            err = ErrNoData;
        }
    }
    if (err != ErrNone){
        loge("export list person failed, error code %d",err);
        DialogUtils::showErrorBox(QString("Xuất dữ liệu danh sách nữ tu trong cộng đoàn lỗi, mã lỗi: %1").arg(err));
    }
    traceret(err);
    return err;
}

ErrCode UICommunityListView::onMenuActionExportListCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> list = COMMUNITYCTL->getAllItemsFromDb();
    if (!list.empty()) {
        err = MainWindow::exportListItems(&list, KModelNameCommunity, COMMUNITYCTL, tr("Xuất danh sách cộng đoàn"), EXPORT_XLSX);
    } else {
        logw("nothing to export");
    }
    RELEASE_LIST_DBMODEL(list);
//    UNDER_DEV("Xuất danh sách cộng đoàn");
//    QList<DbModel*> items;
//    int cnt = act->itemListData(items);
//    logd("No. selected person %d", cnt);
//    foreach (DbModel* item, items){
//        logd("Add person to community");
//        item->dump();
//        COMMUNITYCTL->addPerson(comm, (Person*) item);
//    }
    traceret(err);
    return err;
}

ErrCode UICommunityListView::onLoad()
{
//    QList<Community*> items = COMMUNITYCTL->getAllItems();
    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    mItemList = COMMUNITYCTL->getAllItems();
    // TODO: loop to much, redundant, do something better?
//    foreach (Community* item, items) {
//        mItemList.append(dynamic_cast<DbModel*>(item));
//    }
    traceout;
    return ErrNone;
}

ErrCode UICommunityListView::onReload()
{
    tracein;
    onLoad();
    traceout;
    return ErrNone;
}

DbModel *UICommunityListView::onNewModel(const QString& modelName)
{
    return Community::build();
}
