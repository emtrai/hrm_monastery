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
#include "stringdefs.h"
#include "utils.h"
#include "datetimeutils.h"
#include "mainwindow.h"
#include "uipeopleincommunitylistview.h"
#include "uicommdeptlistview.h"
#include "uitableview.h"
#include <QFileDialog>
#include "personctl.h"
#include "dialogutils.h"
#include "viewutils.h"

UICommunityListView::UICommunityListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = true;
    mHasExportMenu = true;
    traceout;
}

UICommunityListView::~UICommunityListView()
{
    traced;
}

QString UICommunityListView::getName()
{
    return "UICommunityListView";
}

void UICommunityListView::setupUI()
{
    tracein;
    UICommonListView::setupUI();
    MainWindow::addMainWindownImportListener(this);
    traceout;
}

void UICommunityListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    mHeader.append(STR_NOTE);
    mHeader.append(STR_MODELSTATUS);
    mHeader.append(STR_NGAY_BON_MANG);
    mHeader.append(STR_NHIEM_VU_XA_HOI);
    mHeader.append(STR_AREA);
    mHeader.append(STR_TONG_PHU_TRACH);
    mHeader.append(STR_CHURCH);
    mHeader.append(STR_ADDR);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_COUNTRY);
    mHeader.append(STR_PARENT_COMMUNITY);
    traceout;
}


ErrCode UICommunityListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(idx);
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCommunity)) {
        err = ErrInvalidModel;
        loge("invalid model, not community model");
    }
    if (err == ErrNone) {
        Community* model = (Community*) item;
        tblItem->addValue(model->nameId());
        tblItem->addValue(model->name());
        tblItem->addValue(model->remark());
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
    traceret(err);
    return err;
}

ModelController *UICommunityListView::getController()
{
    return COMMUNITYCTL;
}

ImportTarget UICommunityListView::getImportTarget()
{
    return IMPORT_TARGET_COMMUNITY;
}

ErrCode UICommunityListView::onAddItem(UITableCellWidgetItem *item)
{

    tracein;
    UNUSED(item);
    MainWindow::showAddEditCommunity(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UICommunityListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    const DbModel* comm = nullptr;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        comm = item->itemData();
        if (!comm || !IS_MODEL_NAME(comm, KModelNameCommunity)) {
            err = ErrInvalidModel;
            loge("no model info '%s'", MODELSTR2CHA(comm));
        }
    }

    if (err == ErrNone) {
        logd("Show edit for model '%s'", MODELSTR2CHA(comm));
        MainWindow::showAddEditCommunity(true, dynamic_cast<const Community*>(comm), this);
    }
    traceret(err);
    return err;
}

QString UICommunityListView::getTitle()
{
    return STR_COMMUNITY;
}

void UICommunityListView::onMainWindownImportStart(ImportTarget target)
{
    tracein;
    logd("target %d", target);
    if (target == getImportTarget()) {
        if (ready2FetchData()) {
            logd("suspend reload on db update");
            mSuspendReloadOnDbUpdate = true;
        } else {
            logd("not ready to feth data");
        }
    } else {
        logd("not expected import target %d", target);
        // this may be called multiple times when other do import,
        // so consider carrefully when using logi/loge, etc.
    }
    traceout;
}

void UICommunityListView::onMainWindownImportEnd(ImportTarget target, ErrCode err, void *importData)
{
    tracein;
    UNUSED(importData);
    logd("target %d, err %d", target, err);
    if (target == getImportTarget()) {
        logd("resume & reload on db update");
        // not check ready2FetchData, because I'm worry that it may cause inconsistent state
        // due to unknown issue, and data cannot be updated properly
        mSuspendReloadOnDbUpdate = false;
        if (err == ErrNone && ready2FetchData()) {
            logi("reload data after import for target %d", target);
            reload();
        } else {
            loge("Error %d so no update data, or not ready to fetch data", err);
        }
    } else {
        logd("not expected import target %d", target);
    }
    traceout;
}

ErrCode UICommunityListView::showListPeople(UITableMenuAction *act, bool activeOnly)
{
    tracein;
    ErrCode ret = ErrNone;
    const Community* community = nullptr;
    UIPeopleInCommunityListView* view = nullptr;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }
    if (ret == ErrNone) {
        community = dynamic_cast<const Community*>(act->getData());
        if (!community || !IS_MODEL_NAME(community, KModelNameCommunity)
            || community->uid().isEmpty()) {
            ret = ErrInvalidModel;
            loge("invalid community '%s'", MODELSTR2CHA(community));
        }
    }

    if (ret == ErrNone) {
        logi("view list of person for community '%s', active only %d",
             MODELSTR2CHA(community), activeOnly);
        view = (UIPeopleInCommunityListView*)MAINWIN->getView(ViewType::VIEW_PEOPLE_IN_COMMUNITY_LIST);
        if (!view) {
            ret = ErrInvalidView;
            loge("invalid view");
        }
    }
    if (ret == ErrNone) {
        logd("community to view person '%s'", MODELSTR2CHA(community));
        view->setCommunity(community);
        if (activeOnly) {
            view->setModelStatus(MODEL_STATUS_ACTIVE);
        } else {
            view->clearModelStatus();
        }
        MAINWIN->switchView(view);
    }

    if (ret != ErrNone) {
        DialogUtils::showErrorBox(ret, tr("Lỗi xem danh sách Nữ tu trong cộng đoàn"));
    }

    traceret(ret);
    return ret;
}

ErrCode UICommunityListView::exportListPeople(UITableMenuAction *act, bool activeOnly)
{
    tracein;
    ErrCode err = ErrNone;
    const Community* community = nullptr;
    QString uid;
    QString fpath;
    QList<DbModel*> items;
    if (!act){
        err = ErrInvalidArg;
        loge("export list person failed, no menu action");
    }
    if (err == ErrNone && !act->getData()) {
        err = ErrInvalidArg;
        loge("export list person failed, action no data");
    }
    if (err == ErrNone) {
        community = dynamic_cast<const Community*>(act->getData());
        if (community->uid().isEmpty()) {
            err = ErrInvalidData;
            loge("comunity '%s' has no uid", STR2CHA(community->toString()));
        } else {
            uid = community->uid();
        }
    }
    if (err == ErrNone) {
        logi("export people in community uid '%s', activeOnly=%d",
             STR2CHA(uid), activeOnly);
        err = COMMUNITYCTL->getListCommunityPerson(uid, items,
                                                   activeOnly?MODEL_STATUS_ACTIVE:MODEL_STATUS_MAX);

        if (err == ErrNone && items.empty()) {
            logw("nothing to export");
            err = ErrNoData;
        }
    }
    if (err == ErrNone) {
        logd("do export %lld items", items.size());
        err = MainWindow::exportListItems(&items, KModelNameCommPerson,
                                          COMMUNITYCTL,
                                          STR_XUAT_DANH_SACH_NU_TU, EXPORT_XLSX, &fpath);
    }

    // TODO: below process not work as expected
    // dialog is shown then exist intermedately, check it returns 0, not standard button
    // don't know why, need to investigate more...
    if (err == ErrNone) {
        logi("export data done");
        // TODO: look like this message box not shown correctly (auto close)
        // check it
        // FIXME: fix this, this dialog looks like close intermediately
        // cause by menu is closed?
//        DialogUtils::showMsgBox(QString(STR_XUAT_DU_LIEU_THANH_CONG_FILE).arg(fpath), this);
    } else {
        loge("export list person failed, error code %d",err);
//        DialogUtils::showErrorBox(err, QString("Xuất dữ liệu danh sách nữ tu trong cộng đoàn lỗi"));
    }

    RELEASE_LIST_DBMODEL(items);
    traceret(err);
    return err;
}


ErrCode UICommunityListView::onMenuActionListActivePerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = showListPeople(act, true);
    traceret(ret);
    return ret;

}

// Show list of people in all period
ErrCode UICommunityListView::onMenuActionListAllPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = showListPeople(act, false);
    traceret(ret);
    return ret;
}

ErrCode UICommunityListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = ErrNone;
    const Community* community = nullptr;
    UICommDeptListView* view  = nullptr;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }
    if (ret == ErrNone) {
        community = dynamic_cast<const Community*>(act->getData());
        if (!community || !IS_MODEL_NAME(community, KModelNameCommunity)) {
            loge("invalid model '%s'", MODELSTR2CHA(community));
            ret = ErrInvalidModel;
        }
    }
    if (ret == ErrNone) {
        view = (UICommDeptListView*)MAINWIN->getView(ViewType::VIEW_COMMUNITY_DEPT);
        if (!view) {
            ret = ErrInvalidView;
            loge("not found view community dept");
        }
    }

    if (ret == ErrNone) {
        ret = view->setCommunity(community);
    }

    if (ret == ErrNone) {
        logd("switch to view VIEW_COMMUNITY_DEPT, community '%s'",
             MODELSTR2CHA(community));
        MAINWIN->switchView(view);
    }
    traceret(ret);
    return ret;
}

QList<UITableMenuAction *> UICommunityListView::getMenuSingleSelectedItemActions(const QMenu* menu,
                                                                   UITableCellWidgetItem* item)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);
    actionList.append(BUILD_MENU_ACTION_IMPL(tr("Xem danh sách Nữ tu hiện tại"),
                                             item,
                                             onMenuActionListActivePerson));
    actionList.append(BUILD_MENU_ACTION_IMPL(tr("Xem danh sách tất cả Nữ tu"),
                                             item,
                                             onMenuActionListAllPerson));

    actionList.append(BUILD_MENU_SEPARATE);

    actionList.append(BUILD_MENU_ACTION_IMPL(tr("Danh sách phòng ban"),
                                             item,
                                             onMenuActionListDepartment));

    actionList.append(BUILD_MENU_SEPARATE);

    actionList.append(BUILD_MENU_ACTION_IMPL(tr("Xuất danh sách Nữ tu hiện tại"),
                                             item,
                                             onMenuActionExportListActivePeople));
    actionList.append(BUILD_MENU_ACTION_IMPL(tr("Xuất danh sách tất cả Nữ tu"),
                                             item,
                                             onMenuActionExportListAllPeople));

    return actionList;
}

ErrCode UICommunityListView::onMenuActionExportListAllPeople(QMenu *menu, UITableMenuAction *act)
{
    traced;
    UNUSED(menu);
    return exportListPeople(act, false);
}

ErrCode UICommunityListView::onMenuActionExportListActivePeople(QMenu *menu, UITableMenuAction *act)
{
    traced;
    UNUSED(menu);
    return exportListPeople(act, true);
}

DbModel *UICommunityListView::onCreateDbModelObj(const QString& modelName)
{
    tracein;
    DbModel* ret = nullptr;
    logd("model name '%s'", STR2CHA(modelName));
    if (modelName == KModelNameCommunity) {
        ret = Community::build();
    } else {
        loge("invalid model name '%s'", STR2CHA(modelName));
        ret = nullptr;
    }
    traceout;
    return ret;
}

QString UICommunityListView::getMainModelName()
{
    return KModelNameCommunity;
}

QList<DbModel *> UICommunityListView::getListDbModels()
{
    return COMMUNITYCTL->getAllItems(true);
}
