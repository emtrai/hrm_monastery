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
 * Filename: uiethniclistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/14/2023
 * Brief:
 */
#include "uiethniclistview.h"
#include "ethnicctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "utils.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include "modeldefs.h"
#include "filter.h"

UIEthnicListView::UIEthnicListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIEthnicListView::~UIEthnicListView()
{
    traced;
}


QString UIEthnicListView::getName()
{
    return "UIEthnicListView";
}

ModelController *UIEthnicListView::getController()
{
    return ETHNIC;
}

QString UIEthnicListView::getTitle()
{
    return STR_ETHNIC;
}

DbModel *UIEthnicListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Ethnic::build();
}

QString UIEthnicListView::getMainModelName()
{
    return KModelNameEthnic;
}

void UIEthnicListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}

void UIEthnicListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    mHeader.append(STR_COUNTRY);
    mHeader.append(STR_NOTE);
    traceout;
}

ErrCode UIEthnicListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameEthnic)) {
        loge("Invalid model '%s'", MODELSTR2CHA(item));
        err = ErrInvalidModel;
    }
    if (err == ErrNone) {
        Ethnic* model = (Ethnic*)item;
        tblItem->addValue(model->nameId());
        tblItem->addValue(model->name());
        tblItem->addValue(model->countryName());
        tblItem->addValue(model->remark());
    }
    traceret(err);
    return err;
}

ErrCode UIEthnicListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    MainWindow::showAddEditEthnic(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UIEthnicListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        DbModel* ethnic = item->itemData();
        if (ethnic) {
            MainWindow::showAddEditEthnic(true, ethnic, this);
        } else {
            loge("Edit failed, null ethnic");
            err = ErrInvalidModel;
        }
    } else {
        loge("Edit failed, null item");
        err = ErrInvalidArg;
    }
    traceret(err);
    return err;
}
