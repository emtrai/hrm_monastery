/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: uicommunitymgrpeoplelistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#include "uicommunitymgrlistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "communityctl.h"
#include "utils.h"
#include "dialog/dlgcommmgr.h"
#include "community.h"
#include "errreporterctl.h"
#include "stringdefs.h"
#include "dialogutils.h"
#include "datetimeutils.h"
#include "communitymanager.h"

UICommunityMgrListView::UICommunityMgrListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}


UICommunityMgrListView::~UICommunityMgrListView()
{
    tracein;
    COMMUNITYCTL->delListener(this);
    traceout;
}

QString UICommunityMgrListView::getName()
{
    return "UICommunityMgrListView";
}

Community *UICommunityMgrListView::community() const
{
    return (Community*)parentModel();
}

ErrCode UICommunityMgrListView::setCommunity(const Community *newcomm)
{
    traced;
    // parent model is comm, as this listview will show all contact people of an comm
    return setParentModel(newcomm);
}

void UICommunityMgrListView::setupUI()
{
    tracein;
    UICommonListView::setupUI();

    // register listener for any change in comm
    COMMUNITYCTL->addListener(this);
    traceout;
}

QString UICommunityMgrListView::getMainModelName()
{
    return KModelNameCommManager; // this listview lists up list of comm mgr model
}

void UICommunityMgrListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_PERSON_NAMEID);
    mHeader.append(STR_FULLNAME);
    mHeader.append(STR_ROLE);
    mHeader.append(STR_TERM);
    mHeader.append(STR_MODELSTATUS);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_NOTE);
    traceout;
}

ImportTarget UICommunityMgrListView::getImportTarget()
{
    return IMPORT_TARGET_AREA_CONTACT;
}

QString UICommunityMgrListView::getTitle()
{
    Community* comm = community();
    return QString(tr("Danh sách quản lý của cộng đoàn: %1"))
        .arg(comm?comm->name():STR_UNKNOWN);
}

void UICommunityMgrListView::initFilterFields()
{
    tracein;
    // filter by model status
    appendFilterField(FILTER_FIELD_MODEL_STATUS, STR_MODELSTATUS);
    // filter by person full name
    appendFilterField(FILTER_FIELD_FULL_NAME, STR_FULLNAME);
    // TODO: support start date & end date and/term (course)
    traceout;
}

ModelController *UICommunityMgrListView::getController()
{
    return COMMUNITYCTL; // comm mgr people is controlled by comm controller
}

ErrCode UICommunityMgrListView::fillValueTableRowItem(DbModel *item,
                                                           UITableItem *tblItem, int idx)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        logd("updateItem '%s'", MODELSTR2CHA(item));
        if (item->modelName() == KModelNameCommManager) {
            CommunityManager* per = (CommunityManager*) item;
            tblItem->addValue(per->nameId());
            tblItem->addValue(per->personNameId());
            tblItem->addValue(per->personName());
            tblItem->addValue(per->roleName());
            tblItem->addValue(per->courseName());
            tblItem->addValue(per->modelStatusName());
            tblItem->addValue(DatetimeUtils::date2String(per->startDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->endDate()));
            tblItem->addValue(per->personTel());
            tblItem->addValue(per->personEmail());
            tblItem->addValue(per->remark());
        } else {
            loge("No item found, or not expected model '%s'",
                 MODELSTR2CHA(item));
        }
    }
    traceret(err);
    return err;
}

QList<DbModel *> UICommunityMgrListView::getListDbModels()
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> items;
    QString uid;
    Community* comm = community();
    if (!comm) {
        loge("No comm, nothing to load");
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
        dbg(LOG_DEBUG, "Load person list of comm '%s'", MODELSTR2CHA(comm));
        err = COMMUNITYCTL->getManagersList(uid, items);
        if (err != ErrNone) {
            loge("Failed to get contact people list for comm '%s', err=%d",
                 MODELSTR2CHA(comm), err);
        }
    }

    if (err != ErrNone) {
        loge("Faild to get list item, err=%d", err);
        REPORTERRCTL->reportErr(STR_ERR_QUERY_COMMUNITY_MANAGER, err);
    }

    traceout;
    return items;
}

ErrCode UICommunityMgrListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DlgCommMgr* dlg = nullptr;
    Community* comm = community();
    UNUSED(item);

    if (err == ErrNone && !comm) {
        loge("Invalid comm, null value");
        err = ErrInvalidData;
    }
    if (err == ErrNone) {
        dlg = DlgCommMgr::build(
            this, true, KModelNameCommManager, nullptr, this);
        if (!dlg) {
            loge("failed to build DlgCommMgr, no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        logd("Set comm '%s' for person", MODELSTR2CHA(comm));
        err = dlg->setCommunity(comm);
    }

    if (err == ErrNone) {
        dlg->exec();
    } else {
        DialogUtils::showErrorBox(err, tr("Thêm mới thất bại"));
    }

    FREE_PTR(dlg);
    traceret(err);
    return err;
}

ErrCode UICommunityMgrListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DlgCommMgr* dlg = nullptr;
    const DbModel* model = nullptr;
    Community* comm = community();
    if (!item) {
        loge("Invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && !comm) {
        loge("Invalid comm, null value");
        err = ErrInvalidData;
    }
    if (err == ErrNone && ((model = item->itemData()) == nullptr)) {
        loge("Invalid comm data, null item data");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        logd("edit model '%s'", MODELSTR2CHA(model));
        dlg = DlgCommMgr::build(this, true,
                                   KModelNameCommManager,
                                   model, this);
        if (!dlg) {
            loge("failed to build DlgCommMgr, no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        logd("Edit comm mgr for comm '%s'", MODELSTR2CHA(comm));
        err = dlg->setCommunity(comm);
    }

    if (err == ErrNone) {
        dlg->exec();
    } else {
        DialogUtils::showErrorBox(err, tr("Chỉnh sửa thất bại"));
    }

    FREE_PTR(dlg);
    traceret(err);
    return err;
}


