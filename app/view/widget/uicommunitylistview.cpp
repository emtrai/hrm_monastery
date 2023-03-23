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
#include "uicommdeptlistview.h"
#include "uitableviewfactory.h"
#include "uipersonlistview.h"
#include "uitableview.h"
#include <QFileDialog>
#include "personctl.h"
#include "person.h"

UICommunityListView::UICommunityListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UICommunityListView::~UICommunityListView()
{
    traced;
}

void UICommunityListView::setupUI()
{
    traced;
    UITableView::setupUI();
    COMMUNITYCTL->addListener(this);
    tracede;
}

void UICommunityListView::initHeader()
{
    traced;
    UICommonListView::initHeader();
    mHeader.append(tr("Trạng thái hoạt động"));
    mHeader.append(tr("Nhiệm vụ xã hội"));
    mHeader.append(tr("Vùng"));
    mHeader.append(tr("Tổng phụ trách"));
    mHeader.append(tr("Giáo xứ"));
    mHeader.append(tr("Địa chỉ"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    mHeader.append(tr("Quốc gia"));
}

void UICommunityListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;

    UICommonListView::updateItem(item, tblItem);
    Community* model = (Community*) item;
    tblItem->addValue(DbModel::status2Name(model->getStatus()));
    tblItem->addValue(model->missionNameString());
    tblItem->addValue(model->areaName());
    tblItem->addValue(model->currentCEOName());
    tblItem->addValue(model->church());
    tblItem->addValue(model->addr());
    tblItem->addValue(model->tel());
    tblItem->addValue(model->email());
    tblItem->addValue(model->countryName());
}


ErrCode UICommunityListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
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

ErrCode UICommunityListView::onMenuActionAddPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
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
        Utils::showErrorBox(QString("Thêm nữ tu lỗi, mã lỗi: %1").arg(err));
    }
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

void UICommunityListView::onViewItem(UITableWidgetItem *item)
{
    traced;

    traced;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showOnHtmlViewer(comm, tr("Cộng đoàn"));
    } else {
        loge("Comm obj is null");
        Utils::showErrorBox("Không có thông tin để xem");
    }
    tracede;
}

void UICommunityListView::onDeleteItem(UITableWidgetItem *item)
{
    traced;
    ErrCode err = ErrNone;
    if (item) {
        DbModel* comm = item->itemData();
        bool accept = Utils::showConfirmDialog(this,
                             tr("Xoá"), QString(tr("Bạn có muốn xóa '%1'?").arg(comm->name())), nullptr);
        if (accept) {
            QString msg;
            err = comm->remove(false, &msg);
            if (err == ErrDependency) {
                accept = Utils::showConfirmDialog(this,
                                                  tr("Xoá"),
                                                  QString(tr("Lỗi:%1.Bạn có muốn tiếp tục xóa '%2'?").arg(msg, comm->name())), nullptr);
                if (accept) {
                    err =comm->remove(true, &msg);
                }
            }

            if (err != ErrNone) {
                Utils::showErrorBox(QString(tr("Lỗi, mã lỗi: %1")).arg(err));
            } else {
                Utils::showMsgBox(QString(tr("Đã xóa '%1'")).arg(comm->name()));
            }
        }
    } else {
        loge("Nothing to delete");
    }

    tracede;
}

void UICommunityListView::onEditItem(UITableWidgetItem *item)
{
    traced;
    DbModel* comm = item->itemData();
    MainWindow::showAddEditCommunity(true, dynamic_cast<Community*>(comm), this);
    tracede;
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
    traced;
    mSuspendReloadOnDbUpdate = true;
    logd("suspend reload on db update");
    tracede;
}

void UICommunityListView::onImportEnd(const QString &importName, ErrCode err, const QString &fpath, ImportType type)
{
    traced;
    mSuspendReloadOnDbUpdate = false;
    logd("resume reload on db update");
    reload();
    tracede;
}



ErrCode UICommunityListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;
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
//        UICommunityPersonListView* view = (UICommunityPersonListView*)UITableViewFactory::getView(ViewType::VIEW_COMMUNITY_PERSON);

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
    actionList.append(UITableMenuAction::build(tr("Xem danh sách nữ tu hiện tại"), this, item)
                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionListPerson(m, a);
                                           }));
    actionList.append(UITableMenuAction::build(tr("Xem danh sách tất cả nữ tu"), this, item)
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
                                                    return this->onMenuActionAddPerson(m, a);
                                                }));
    actionList.append(UITableMenuAction::build(tr("Thêm ban"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionAddDepart(m, a);
                                                   }));
    return actionList;

}

QList<UITableMenuAction *> UICommunityListView::getMenuMultiItemActions(const QMenu *menu, const QList<UITableItem *> &items)
{
    traced;
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);
    actionList.append(UITableMenuAction::buildMultiItem(tr("Xuất danh sách cộng đoàn"), this, &items)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionExportListCommunity(m, a);
                                                   }));
    tracede;
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
    ErrCode err = ErrNone;
    QString fpath;
    if (!act){
        err = ErrInvalidArg;
        loge("export list person failed, Empty menu action");
    }
    if (err == ErrNone && !act->getData()) {
        err = ErrInvalidArg;
        loge("export list person failed, action no data");
    }
    if (err == ErrNone) {
        // TODO: select export file type, i.e. csv, xlsx, etc.
        Community* community = dynamic_cast<Community*>(act->getData());
        logi("export people in community uid '%s'", STR2CHA(community->uid()));
        fpath = Utils::saveFileDialog(this, QString("Xuất danh sách nữ tu"),
                                              QString("dsach.csv"), QString("CSV (*.csv)"));
        if (!fpath.isEmpty()) {
            logi("export to file '%s'", STR2CHA(fpath));
            // TODO: show progress/run on separate thread?
            err = PERSONCTL->exportListPersonInCommunity(community->uid(), ExportType::EXPORT_CSV_LIST, fpath);
        } else {
            loge("no path is specified for export");
            err = ErrNoData;
        }
    }
    if (err == ErrNone){
        Utils::showMsgBox(QString("Lưu vào: %1").arg(fpath));
    } else {
        loge("export list person failed, error code %d",err);
        Utils::showErrorBox(QString("Xuất dữ liệu danh sách nữ tu trong cộng đoàn lỗi, mã lỗi: %1").arg(err));
    }
    tracedr(err);
    return err;
}

ErrCode UICommunityListView::onMenuActionExportListCommunity(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode err = ErrNotImpl;
    UNDER_DEV("Xuất danh sách cộng đoàn");
//    QList<DbModel*> items;
//    int cnt = act->itemListData(items);
//    logd("No. selected person %d", cnt);
//    foreach (DbModel* item, items){
//        logd("Add person to community");
//        item->dump();
//        COMMUNITYCTL->addPerson(comm, (Person*) item);
//    }
    tracedr(err);
    return err;
}

ErrCode UICommunityListView::onLoad()
{
//    QList<Community*> items = COMMUNITYCTL->getAllItems();
    traced;
    RELEASE_LIST_DBMODEL(mItemList);
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
    onLoad();
    tracede;
    return ErrNone;
}

DbModel *UICommunityListView::onNewModel()
{
    return Community::build();
}
