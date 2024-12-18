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
 * Filename: uiworklistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/13/2023
 * Brief:
 */
#include "uiworklistview.h"
#include "workctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "work.h"
#include "utils.h"
#include "stringdefs.h"
#include "modeldefs.h"

UIWorkListView::UIWorkListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}

UIWorkListView::~UIWorkListView()
{
    traced;
}

QString UIWorkListView::getName()
{
    return "UIWorkListView";
}

ModelController *UIWorkListView::getController()
{
    return WORKCTL;
}

QString UIWorkListView::getTitle()
{
    return STR_WORK;
}

DbModel *UIWorkListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Work::build();
}

QString UIWorkListView::getMainModelName()
{
    return KModelNameWork;
}

void UIWorkListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}
