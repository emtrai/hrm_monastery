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
 * Filename: uiareacontactpeople.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/16/2023
 * Brief:
 */
#include "uiareacontactpeoplelistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "areactl.h"
#include "utils.h"
#include "dialog/dlgareaperson.h"
#include "area.h"
#include "areaperson.h"
#include "errreporterctl.h"
#include "stringdefs.h"
#include "mainwindow.h"
#include "dialogutils.h"
#include "datetimeutils.h"

UIAreaContactPeopleListView::UIAreaContactPeopleListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = true;
    mHasExportMenu = true;
    traceout;
}


UIAreaContactPeopleListView::~UIAreaContactPeopleListView()
{
    tracein;
    AREACTL->delListener(this);
    traceout;
}

Area *UIAreaContactPeopleListView::area() const
{
    return (Area*)parentModel();
}

ErrCode UIAreaContactPeopleListView::setArea(const Area *newArea)
{
    ErrCode err = ErrNone;
    tracein;
    err = setParentModel(newArea);
    if (err == ErrNone) {
        setTitle(getTitle());
    }
    traceret(err);
    return err;
}

void UIAreaContactPeopleListView::setupUI()
{
    tracein;
    UITableView::setupUI();

    // register listener for any change in Area
    AREACTL->addListener(this);
    traceout;
}

QString UIAreaContactPeopleListView::getMainModelName()
{
    return KModelNameAreaPerson;
}

void UIAreaContactPeopleListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_PERSON_NAMEID);
    mHeader.append(STR_FULLNAME);
    mHeader.append(STR_ROLE);
    mHeader.append(STR_MODELSTATUS);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_NOTE);
    traceout;
}

ImportTarget UIAreaContactPeopleListView::getImportTarget()
{
    return IMPORT_TARGET_AREA_CONTACT;
}

QString UIAreaContactPeopleListView::getTitle()
{
    Area* ar = area();
    return QString(tr("Danh sách liên lạc của khu vực: %1"))
        .arg(ar?ar->name():STR_UNKNOWN);
}

void UIAreaContactPeopleListView::initFilterFields()
{
    tracein;
    appendFilterField(FILTER_FIELD_MODEL_STATUS, STR_MODELSTATUS);
    appendFilterField(FILTER_FIELD_FULL_NAME, STR_FULLNAME);
    // TODO: support start date & end date
    traceout;
}

ModelController *UIAreaContactPeopleListView::getController()
{
    return AREACTL;
}

void UIAreaContactPeopleListView::updateItem(DbModel *item,
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
        if (item->modelName() == KModelNameAreaPerson) {
            AreaPerson* per = (AreaPerson*) item;
            tblItem->addValue(per->nameId());
            tblItem->addValue(per->personNameId());
            tblItem->addValue(per->personName());
            tblItem->addValue(per->roleName());
            tblItem->addValue(per->modelStatusName());
            tblItem->addValue(DatetimeUtils::date2String(per->startDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->endDate()));
            tblItem->addValue(per->personTel());
            tblItem->addValue(per->personEmail());
            tblItem->addValue(per->remark());
        } else {
            loge("No item found, or not expected model '%s'",
                 STR2CHA(item->modelName()));
        }
    }
    traceout;
}

QList<DbModel *> UIAreaContactPeopleListView::getListItem()
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> items;
    QString uid;
    Area* ar = area();
    if (!ar) {
        loge("Nothing to load");
        err = ErrNoData;
    }

    if (err == ErrNone) {
        uid = ar->uid();
        if (uid.isEmpty()) {
            err = ErrInvalidData;
            loge("Invalud aream info, uid is null");
        }
    }
    if (err == ErrNone) {
        logd("Load person list of area '%s'", MODELSTR2CHA(ar));
        err = AREACTL->getContactPeopleList(uid, items);
        if (err != ErrNone) {
            loge("Failed to get contact people list for area '%s', err=%d",
                 MODELSTR2CHA(ar), err);
        }
    }

    if (err != ErrNone) {
        loge("Faild to get list item, err=%d", err);
        REPORTERRCTL->reportErr(
            tr("Lỗi truy vấn thông tin người liên lạc cho khu vực"), err);
    }

    traceout;
    return items;
}

ErrCode UIAreaContactPeopleListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DlgAreaPerson* dlg = nullptr;
    Area* ar = area();

    if (err == ErrNone && !ar) {
        loge("Invalid area, null value");
        err = ErrInvalidData;
    }
    if (err == ErrNone) {
        dlg = DlgAreaPerson::build(
                            this, true, KModelNameAreaPerson, nullptr, this);
        if (!dlg) {
            loge("failed to build DlgAreaPerson, no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        logd("Add area person for area '%s'", MODELSTR2CHA(ar));
        err = dlg->setArea(ar);
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

void UIAreaContactPeopleListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DlgAreaPerson* dlg = nullptr;
    DbModel* model = nullptr;
    Area* ar = area();
    if (!item) {
        loge("Invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && !ar) {
        loge("Invalid area, null value");
        err = ErrInvalidData;
    }
    if (err == ErrNone && ((model = item->itemData()) == nullptr)) {
        loge("Invalid area data, null item data");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        dlg = DlgAreaPerson::build(this, true,
                                   KModelNameAreaPerson,
                                   model, this);
        if (!dlg) {
            loge("failed to build DlgAreaPerson, no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        logd("Edit area person for area '%s'", MODELSTR2CHA(ar));
        err = dlg->setArea(ar);
    }

    if (err == ErrNone) {
        dlg->exec();
    } else {
        DialogUtils::showErrorBox(err, tr("Chỉnh sửa thất bại"));
    }

    FREE_PTR(dlg);
    traceout;
}
ErrCode UIAreaContactPeopleListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = nullptr;

    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone && ((model = item->itemData()) == nullptr)) {
        err = ErrInvalidData;
        loge("Invalid argument");
    }

    if (err == ErrNone) {
        err = MainWindow::showOnHtmlViewer(model, tr("Vùng"));
    }
    if (err != ErrNone) {
        loge("view item err %d", err);
        DialogUtils::showErrorBox(err, tr("Hiển thị thông tin lỗi"));
    }
    traceret(err);
    return err;
}

