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
 * Filename: uipersonstatuslistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/4/2023
 * Brief:
 */
#include "uipersonstatuslistview.h"

#include "personstatusctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "personstatus.h"
#include "utils.h"
#include "stringdefs.h"
#include "modeldefs.h"

UIPersonStatusListView::UIPersonStatusListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}

UIPersonStatusListView::~UIPersonStatusListView()
{
    traced;
}

QString UIPersonStatusListView::getName()
{
    return "UIPersonStatusListView";
}

QString UIPersonStatusListView::getTitle()
{
    return STR_PERSON_STATUS;
}

ModelController *UIPersonStatusListView::getController()
{
    return PERSONSTATUSCTL;
}

DbModel *UIPersonStatusListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return PersonStatus::build();
}

QString UIPersonStatusListView::getMainModelName()
{
    return KModelNameSpecialist;
}

void UIPersonStatusListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}
