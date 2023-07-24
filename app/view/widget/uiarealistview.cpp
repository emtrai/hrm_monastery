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
#include "uitableviewfactory.h"
#include "stringdefs.h"
#include "errreporterctl.h"
#include "dialogutils.h"

UIAreaListView::UIAreaListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
}

UIAreaListView::~UIAreaListView()
{
    tracein;
    AREACTL->delListener(this);
}

void UIAreaListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    AREACTL->addListener(this);
    traceout;
}

ImportTarget UIAreaListView::getImportTarget()
{
    return IMPORT_TARGET_AREA;
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
}

void UIAreaListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    if (item) {
        UICommonListView::updateItem(item, tblItem, idx);
        if (item->modelName() == KModelNameArea) {
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

ModelController *UIAreaListView::getController()
{
    tracein;
    return AREACTL;
}


DbModel *UIAreaListView::onNewModel(const QString& modelName)
{
    return Area::build();
}

ErrCode UIAreaListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
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
        if (area) {
            MainWindow::showAddEditArea(true, area, this);
        } else {
            loge("Edit failed, null area");
        }
    } else {
        loge("Edit failed, null item");
    }
    traceret(err);
    return err;
}

ErrCode UIAreaListView::onDeleteItem(const QList<UITableItem *> &selectedItems)
{
    UNDER_DEV("Xóa dữ liệu");
    return ErrNotImpl;
}

QList<UITableMenuAction *> UIAreaListView::getMenuSingleSelectedItemActions(const QMenu *menu, UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);
    actionList.append(UITableMenuAction::build(tr("Xem danh sách liên lạc"), this, item)
                                               ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                   return this->onMenuActionViewContactPeople(m, a);
                                               }));
    // TODO: export data
    // TODO: import data
    return actionList;
}

ErrCode UIAreaListView::onMenuActionViewContactPeople(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Area* area = dynamic_cast<Area*>(act->getData());
    if (area != nullptr) {
        UIAreaContactPeopleListView* view =
                (UIAreaContactPeopleListView*)UITableViewFactory::getView(ViewType::VIEW_AREA_PERSON);

        logd("area to view person %s", STR2CHA(area->toString()));
        view->setArea(area);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no area info");
        ret = ErrNoData;
        DialogUtils::showErrorBox(tr("Vui lòng chọn khu vực cần xem"));
    }

    traceret(ret);
    return ret;
}

