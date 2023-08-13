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
#include "uitableviewfactory.h"
#include "coursectl.h"
#include "dialogutils.h"

UICourseListView::UICourseListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
}

UICourseListView::~UICourseListView()
{
    tracein;
}

void UICourseListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    COURSECTL->addListener(this);
    traceout;
}

QString UICourseListView::getTitle()
{
    return tr("Khóa/Nhiệm Kỳ/Lớp Khấn");
}

DbModel *UICourseListView::onCreateDbModelObj(const QString& modelName)
{
    return Course::build();
}

ErrCode UICourseListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    MainWindow::showAddEditCourse(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UICourseListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item) {
        DbModel* course = item->itemData();
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

ErrCode UICourseListView::onDeleteItem(const QList<UITableItem *> &selectedItems)
{
    tracein;
    UNDER_DEV(tr("Xóa dữ liệu..."));
    traceout;
    return ErrNotImpl;
}

ErrCode UICourseListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showOnHtmlViewer(comm, tr("Khóa/Nhiệm Kỳ/Lớp Khấn"));
    } else {
        loge("Model obj is null");
        DialogUtils::showErrorBox("Không có thông tin để xem");
    }
    traceout;
    return ErrNone;//TODO: check return value
}

QList<DbModel *> UICourseListView::getListItem()
{
    return COURSECTL->getAllItems();;
}

void UICourseListView::initHeader()
{
    tracein;
    mHeader.append(tr("Tên định danh"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Loại"));
    mHeader.append(tr("Khóa"));
    mHeader.append(tr("Ngày bắt đầu"));
    mHeader.append(tr("Ngày kết thúc đầu"));
    mHeader.append(tr("Ghi chú"));
    traceout;
}

void UICourseListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
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
    }
    traceout;
}
