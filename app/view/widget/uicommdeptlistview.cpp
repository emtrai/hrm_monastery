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

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "community.h"
#include "utils.h"
#include "mainwindow.h"
#include "uidepartmentpersonlistview.h"
#include "communitydeptctl.h"
#include "communitydept.h"
#include "stringdefs.h"
#include "errreporterctl.h"

UICommDeptListView::UICommDeptListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    // there is not much items, so skipping import
    // TODO: support to import?
    mHasImportMenu = false;
    mHasExportMenu = true;
    traceout;
}

UICommDeptListView::~UICommDeptListView()
{
    tracein;
    traceout;
}
QString UICommDeptListView::getTitle()
{
    traced;
    QString title;
    title = QString(tr("Phòng ban của cộng đoàn: %1"))
                .arg(community()?community()->name():STR_UNKNOWN);

    return title;
}

void UICommDeptListView::initHeader()
{
    tracein;
    UICommonListView::initHeader();
    mHeader.append(STR_DEPARTMENT);
    // TODO: show director of dept
    mHeader.append(STR_MODELSTATUS);
    mHeader.append(STR_ESTABLISHDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_TEL);
    mHeader.append(STR_ADDR);
    traceout;
}

ErrCode UICommDeptListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        err = UICommonListView::fillValueTableRowItem(item, tblItem, idx);
    }
    if (err == ErrNone) {
        if (IS_MODEL_NAME(item, KModelNameCommDept)) {
            CommunityDept* dept = (CommunityDept*)item;
            tblItem->addValue(dept->departmentName());
            // TODO: show director of dept
            tblItem->addValue(DbModel::modelStatus2Name((DbModelStatus)dept->modelStatus()));
            tblItem->addValue(DatetimeUtils::date2String(dept->establishDate()));
            tblItem->addValue(DatetimeUtils::date2String(dept->closedDate()));
            tblItem->addValue(dept->email());
            tblItem->addValue(dept->tel());
            tblItem->addValue(dept->addr());
        } else {
            loge("Update table failed, unsupported model '%s'", MODELSTR2CHA(item));
            err = ErrInvalidModel;
        }
    }
    traceret(err);
    return err;
}

DbModel *UICommDeptListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return CommunityDept::build();
}

ErrCode UICommDeptListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    MainWindow::showAddEditCommDept(true, community(), nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UICommDeptListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        DbModel* comm = item->itemData();
        if (comm) {
            MainWindow::showAddEditCommDept(true, community(), comm, this);
        } else {
            loge("Edit failed, null model");
            err = ErrNoData;
        }
    } else {
        loge("Edit failed, null item");
        err = ErrNoData;
    }
    traceret(err);
    return err;
}

ErrCode UICommDeptListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = ErrNone;
    CommunityDept* item = nullptr;
    UIDepartmentPersonListView* view = nullptr;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid argument, no act");
    }
    if (ret == ErrNone) {
        item = dynamic_cast<CommunityDept*>(act->getData());
        if (!item) {
            loge("no department info");
            ret = ErrNoData;
        }
    }

    if (ret == ErrNone) {
        view = (UIDepartmentPersonListView*)
            MAINWIN->getView(ViewType::VIEW_DEPARTMENT_PERSON);
        if (!view) {
            loge("no view info");
            ret = ErrNotFound;
        }
    }

    // switch to new view
    if (ret == ErrNone) {
        view->setCommDept(item);;

        view->setTitle(QString(tr("Thành viên phòng ban %1 cộng đoàn %2"))
                           .arg(item->departmentName(), item->communityName()));
        MAINWIN->switchView(view);
    }

    traceret(ret);
    return ret;

}

QList<UITableMenuAction *> UICommDeptListView::getMenuSingleSelectedItemActions(
    const QMenu* menu,
    UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList =
        UITableView::getMenuSingleSelectedItemActions(menu, item);

    actionList.append(UITableMenuAction::build(tr("Danh sách thành viên"), this, item)
                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionListPerson(m, a);
                                           }));
    return actionList;

}

QList<DbModel *> UICommDeptListView::getListDbModels()
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> items;
    QString uid;
    Community* comm = community();
    if (!comm) {
        loge("No community, nothing to load");
        err = ErrNoData;
    }

    if (err == ErrNone) {
        uid = comm->uid();
        if (uid.isEmpty()) {
            err = ErrInvalidData;
            loge("Invalid comm info, uid is null, comm '%s'", MODELSTR2CHA(comm));
        }
    }
    if (err == ErrNone) {
        bool ok = false;
        logd("Load dept list of comm '%s'", MODELSTR2CHA(comm));
        items = COMMUNITYDEPTCTL->getListDept(community()->uid(), &ok);
        if (!ok) {
            err = ErrLoadFailed;
            loge("Failed to get dept list for comm '%s', err=%d",
                 MODELSTR2CHA(comm), err);
        }
    }

    if (err != ErrNone) {
        loge("Faild to get list item, err=%d", err);
        REPORTERRCTL->reportErr(
            tr("Lỗi truy vấn thông tin phòng ban"), err);
    }

    traceout;
    return items;
}


Community *UICommDeptListView::community() const
{
    return (Community*)parentModel();
}

ErrCode UICommDeptListView::setCommunity(const Community *newCommunity)
{
    traced;
    // parent model is community, as this listview will show all dept of community
    return setParentModel(newCommunity);
}

QString UICommDeptListView::getMainModelName()
{
    return KModelNameCommDept;
}

ModelController *UICommDeptListView::getController()
{
    return COMMUNITYDEPTCTL;
}


void UICommDeptListView::initFilterFields()
{
    tracein;
    // filter by model status
    appendFilterField(FILTER_FIELD_MODEL_STATUS, STR_MODELSTATUS);
    // filter by name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    // TODO: support start date & end date and/term (course)
    traceout;
}
