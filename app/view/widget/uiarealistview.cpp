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
 * Filename: uiarealistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#include "uiarealistview.h"

#include "areactl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "area.h"
#include "utils.h"
#include "mainwindow.h"
#include "uiareacontactpeoplelistview.h"
#include "stringdefs.h"
#include "errreporterctl.h"
#include "dialogutils.h"

UIAreaListView::UIAreaListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    // there is not much area items, so skipping import area
    // TODO: support to import areas?
    mHasImportMenu = false;
    mHasExportMenu = true;

    traceout;
}

UIAreaListView::~UIAreaListView()
{
    tracein;
    AREACTL->delListener(this);
    traceout;
}

void UIAreaListView::setupUI()
{
    tracein;
    UICommonListView::setupUI();
    AREACTL->addListener(this);
    traceout;
}

QString UIAreaListView::getTitle()
{
    return STR_AREA;
}

void UIAreaListView::initHeader()
{
    tracein;
    UICommonListView::initHeader();
    mHeader.append(STR_MODELSTATUS);
    mHeader.append(STR_COUNTRY);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_ADDR);

    traceout;
}

void UIAreaListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(tblItem);
    UNUSED(idx);
    if (item) {
        if (item->modelName() == KModelNameArea) {
            UICommonListView::updateItem(item, tblItem, idx);
            Area* model = (Area*) item;
            tblItem->addValue(model->modelStatusName());
            tblItem->addValue(model->countryName());
            tblItem->addValue(model->tel());
            tblItem->addValue(model->email());
            tblItem->addValue(model->addr());
        } else {
            loge("Not area model name, it's '%s'", STR2CHA(item->modelName()));
            REPORTERRCTL->reportErr(tr("Dữ liệu không phù hợp"));
        }
    } else {
        loge("Invalid item to update");
        REPORTERRCTL->reportErr(tr("Không có dữ liệu để cập nhật"));
    }
    traceout;
}

void UIAreaListView::initFilterFields()
{
    tracein;
    // filter by model status
    appendFilterField(FILTER_FIELD_MODEL_STATUS, STR_MODELSTATUS);
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}

ModelController *UIAreaListView::getController()
{
    tracein;
    return AREACTL;
}

DbModel *UIAreaListView::onNewModel(const QString& modelName)
{
    tracein;
    DbModel* model = nullptr;
    logd("new model name '%s'", STR2CHA(modelName));
    if (modelName.isNull() || modelName.isEmpty() || modelName == KModelNameArea) {
        model = Area::build();
    } else {
        loge("cannot create model, invalid modelName '%s'", STR2CHA(modelName));
    }
    traceout;
    return model;
}

QString UIAreaListView::getMainModelName()
{
    return KModelNameArea;
}

ErrCode UIAreaListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    MainWindow::showAddEditArea(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UIAreaListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        DbModel* area = item->itemData();
        if (area && area->modelName() == KModelNameArea) {
            MainWindow::showAddEditArea(true, area, this);
        } else {
            loge("Edit failed, invalid area '%s'", MODELSTR2CHA(area));
            REPORTERRCTL->reportErr(STR_INVALID_DATA);
        }
    } else {
        loge("Edit failed, null item");
        REPORTERRCTL->reportErr(STR_NO_DATA);
    }
    traceret(err);
    return err;
}

QList<UITableMenuAction *> UIAreaListView::getMenuSingleSelectedItemActions(
    const QMenu *menu, UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList =
        UITableView::getMenuSingleSelectedItemActions(menu, item);
    actionList.append(UITableMenuAction::build(tr("Xem danh sách liên lạc"), this, item)
                                               ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                   return this->onMenuActionViewContactPeople(m, a);
                                               }));
    return actionList;
}

ErrCode UIAreaListView::onMenuActionViewContactPeople(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = ErrNone;
    Area* area = nullptr;
    UIAreaContactPeopleListView* view = nullptr;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }
    if (ret == ErrNone) {
        area = dynamic_cast<Area*>(act->getData());
        if (!area) {
            loge("No area data in menu item");
            ret = ErrNoData;
        }
    }
    if (ret == ErrNone) {
        view = (UIAreaContactPeopleListView*)
               MAINWIN->getView(ViewType::VIEW_AREA_PERSON);
        if (!view) {
            loge("no view found, no memory?");
            ret = ErrNotFound;
        }
    }
    if (ret == ErrNone) {
        logd("area to view contact list '%s'", MODELSTR2CHA(area));
        ret = view->setArea(area);
    }

    if (ret == ErrNone) {
        logd("Switch view");
        MAINWIN->switchView(view);
    }

    if (ret != ErrNone) {
        DialogUtils::showErrorBox(ret, tr("Xem danh sách liên lạc lỗi"));
    }

    traceret(ret);
    return ret;
}

