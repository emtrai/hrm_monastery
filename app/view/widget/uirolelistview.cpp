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
 * Filename: uirolelistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#include "uirolelistview.h"

#include "rolectl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "role.h"
#include "rolectl.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"
#include "stringdefs.h"

UIRoleListView::UIRoleListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}

UIRoleListView::~UIRoleListView()
{
    traced;
}

QString UIRoleListView::getName()
{
    return "UIRoleListView";
}

ModelController *UIRoleListView::getController()
{
    return ROLECTL;
}

QString UIRoleListView::getTitle()
{
    return STR_ROLE;
}

DbModel *UIRoleListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Role::build();
}

QString UIRoleListView::getMainModelName()
{
    return KModelNameRole;
}

void UIRoleListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}

