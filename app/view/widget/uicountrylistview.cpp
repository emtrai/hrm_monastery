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
 * Filename: uicountrylistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/14/2023
 * Brief:
 */
#include "uicountrylistview.h"

#include "countryctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"

UICountryListView::UICountryListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UICountryListView::~UICountryListView()
{
    traced;
}

void UICountryListView::onViewItem(UITableWidgetItem *item)
{

}

Controller *UICountryListView::getController()
{
    return COUNTRYCTL;
}

QString UICountryListView::getTitle()
{
    return tr("Quốc gia");
}
