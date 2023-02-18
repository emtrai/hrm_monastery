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
 * Filename: uieducationlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/13/2023
 * Brief:
 */
#include "uieducationlistview.h"
#include "eductl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "education.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"

UIEducationListView::UIEducationListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIEducationListView::~UIEducationListView()
{
    traced;
}

Controller *UIEducationListView::getController()
{
    return EDUCTL;
}

QString UIEducationListView::getTitle()
{
    return tr("Trình độ học vấn");
}

DbModel *UIEducationListView::onNewModel()
{
    return Education::build();
}

