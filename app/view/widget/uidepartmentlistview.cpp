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
 * Filename: uidepartmentlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/12/2023
 * Brief:
 */
#include "uidepartmentlistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "department.h"
#include "utils.h"
#include "departctl.h"
#include "stringdefs.h"
#include "modeldefs.h"

UIDepartmentListView::UIDepartmentListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIDepartmentListView::~UIDepartmentListView()
{
    traced;
}

QString UIDepartmentListView::getName()
{
    return "UIDepartmentListView";
}
QString UIDepartmentListView::getTitle()
{
    return STR_DEPARTMENT;
}

ModelController *UIDepartmentListView::getController()
{
    traced;
    return DEPART;
}

QList<DbModel *> UIDepartmentListView::getListDbModels()
{
    traced;
    return DEPART->getAllItems(true);
}

DbModel *UIDepartmentListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Department::build();
}

QString UIDepartmentListView::getMainModelName()
{
    return KModelNameDepartment;
}

void UIDepartmentListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;

}

