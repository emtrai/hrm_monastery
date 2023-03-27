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
 * Filename: uicourselistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/9/2023
 * Brief:
 */
#include "uicourselistview.h"


#include "coursectl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "course.h"
#include "utils.h"
#include "mainwindow.h"
#include "uitableviewfactory.h"

UICourseListView::UICourseListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
}

UICourseListView::~UICourseListView()
{
    tracein;
}

QString UICourseListView::getTitle()
{
    return tr("Khóa");
}

DbModel *UICourseListView::onNewModel()
{
    return Course::build();
}

QList<DbModel *> UICourseListView::getListItem()
{
    return COURSECTL->getAllItems();;
}

void UICourseListView::initHeader()
{
    tracein;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Tên định danh"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Loại"));
    mHeader.append(tr("Ghi chú"));
    traceout;
}

void UICourseListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    Course* course = (Course*)item;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(course->nameId());
    tblItem->addValue(course->name());
    tblItem->addValue(course->courseTypeName());
    tblItem->addValue(course->remark());
    traceout;
}
