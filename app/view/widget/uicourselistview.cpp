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
#include "datetimeutils.h"
#include "mainwindow.h"
#include "coursectl.h"
#include "stringdefs.h"
#include "modeldefs.h"


UICourseListView::UICourseListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}

UICourseListView::~UICourseListView()
{
    tracein;
}

QString UICourseListView::getName()
{
    return "UICourseListView";
}
QString UICourseListView::getTitle()
{
    return tr("Khóa/Nhiệm Kỳ/Lớp Khấn");
}

DbModel *UICourseListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Course::build();
}

QString UICourseListView::getMainModelName()
{
    return KModelNameCourse;
}

void UICourseListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}

ErrCode UICourseListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    MainWindow::showAddEditCourse(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UICourseListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        const DbModel* course = item->itemData();
        if (course) {
            MainWindow::showAddEditCourse(true, course, this);
        } else {
            loge("Edit failed, null course");
        }
    } else {
        loge("Edit failed, null item");
        err = ErrInvalidArg;
    }
    traceret(err);
    return err;
}

QList<DbModel *> UICourseListView::getListDbModels()
{
    return COURSECTL->getAllItems(true);
}

ModelController *UICourseListView::getController()
{
    return COURSECTL;
}

void UICourseListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    mHeader.append(STR_TYPE);
    mHeader.append(STR_COURSE);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_NOTE);
    traceout;
}

ErrCode UICourseListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        Course* course = (Course*)item;
        if (course && course->modelName() == KModelNameCourse) {
            tblItem->addValue(course->nameId());
            tblItem->addValue(course->name());
            tblItem->addValue(course->courseTypeName());
            tblItem->addValue(course->period());
            tblItem->addValue(DatetimeUtils::date2String(course->startDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(DatetimeUtils::date2String(course->endDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(course->remark());
        } else {
            loge("Invalid course '%s'", course?STR2CHA(course->toString()):"null");
            err = ErrInvalidData;
        }
    }
    traceret(err);
    return err;
}
