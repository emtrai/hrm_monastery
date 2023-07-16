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
#include "uitableviewfactory.h"
#include "stringdefs.h"
#include "modeldefs.h"

UIEthnicListView::UIEthnicListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
}

UIEthnicListView::~UIEthnicListView()
{
    tracein;
}


ModelController *UIEthnicListView::getController()
{
    return ETHNIC;
}

QString UIEthnicListView::getTitle()
{
    return tr("Dân tộc");
}

DbModel *UIEthnicListView::onNewModel(const QString& modelName)
{
    return Ethnic::build();
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

void UIEthnicListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    Ethnic* model = (Ethnic*)item;
    if (model && model->modelName() == KModelNameEthnic) {
        tblItem->addValue(model->nameId());
        tblItem->addValue(model->name());
        tblItem->addValue(model->countryName());
        tblItem->addValue(model->remark());
    } else {
        loge("Invalid model '%s'", model?STR2CHA(model->toString()):"null");
    }
    traceout;
}

ErrCode UIEthnicListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    MainWindow::showAddEditEthnic(true, nullptr, this);
    traceout;
    return ErrNone;
}

void UIEthnicListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    if (item) {
        DbModel* ethnic = item->itemData();
        if (ethnic) {
            MainWindow::showAddEditEthnic(true, ethnic, this);
        } else {
            loge("Edit failed, null ethnic");
        }
    } else {
        loge("Edit failed, null item");
    }
    traceout;
}
