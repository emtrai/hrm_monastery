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
 * Filename: uimissionlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/12/2023
 * Brief:
 */
#include "uimissionlistview.h"


#include "missionctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "mission.h"
#include "utils.h"
#include "stringdefs.h"
#include "modeldefs.h"

UIMissionListView::UIMissionListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}

UIMissionListView::~UIMissionListView()
{
    traced;
}

QString UIMissionListView::getName()
{
    return "UIMissionListView";
}

ModelController *UIMissionListView::getController()
{
    return MISSIONCTL;
}


QString UIMissionListView::getTitle()
{
    return STR_CONG_TAC_XA_HOI;
}

DbModel *UIMissionListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Mission::build();
}

QString UIMissionListView::getMainModelName()
{
    return KModelNameMission;
}

void UIMissionListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}


