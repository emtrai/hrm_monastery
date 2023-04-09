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
 * Filename: uicommonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uicommonlistview.h"
#include "logger.h"
#include "saintctl.h"
#include "communityctl.h"
#include "specialistctl.h"
#include "coursectl.h"
#include "workctl.h"
#include "eductl.h"
#include "filter.h"
#include "mainwindow.h"

UICommonListView::UICommonListView(QWidget *parent):
    UITableView(parent)
{
}

UICommonListView::~UICommonListView()
{
    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    traceout;
}

QList<UITableItem *> UICommonListView::getListItem(qint32 page, qint32 perPage, qint32 totalPages)
{
    QList<UITableItem *> items;
    tracein;
    int idx = 0;
    foreach (DbModel* item, mItemList) {
        UITableItem* tblItem = UITableItem::build(item->clone());
        updateItem(item, tblItem, ++idx);
        items.append(tblItem);
    }
    return items;
}

void UICommonListView::initHeader()
{
    tracein;
    mHeader.append(tr("STT"));
    mHeader.append(tr("Tên định danh"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Ghi chú"));
    traceout;
}
void UICommonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    tblItem->addValue(QString("%1").arg(idx));
    tblItem->addValue(item->nameId());
    tblItem->addValue(item->name());
    tblItem->addValue(item->remark());
    traceout;
}

qint32 UICommonListView::getTotalItems()
{
    tracein;
    return mItemList.count();
}

QList<DbModel *> UICommonListView::getListItem()
{
    return getController()->getAllItemsFromDb();
}

ModelController *UICommonListView::getController()
{
    return nullptr;
}

ErrCode UICommonListView::onLoad()
{
    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    mItemList = getListItem();
    clearFilter();
    traceout;
    return ErrNone;
}

ErrCode UICommonListView::onReload()
{
    tracein;
    onLoad();
    traceout;
    return ErrNone;
}

void UICommonListView::initFilterFields()
{
    tracein;
    appendFilterField(FILTER_FIELD_NAME, tr("Tên"));
    traceout;
}


QHash<QString, QString> UICommonListView::getFilterKeywords(int fieldId, const QString &fieldText)
{
    tracein;
    logd("Query search keywords form db, field %d, %s", fieldId, fieldText.toStdString().c_str());
    QHash<QString, QString> keywords;
    QList<DbModel*> modelList;
    switch (fieldId) {
    case FILTER_FIELD_HOLLY_NAME:
        modelList = SAINTCTL->getAllItems();
        break;
    case FILTER_FIELD_COMMUNITY:
        modelList = COMMUNITYCTL->getAllItems();
        break;
    case FILTER_FIELD_SPECIALIST:
        modelList = SPECIALISTCTL->getAllItems();
        break;
    case FILTER_FIELD_COURSE:
        modelList = COURSECTL->getAllItems();
        break;
    case FILTER_FIELD_WORK:
        modelList = WORKCTL->getAllItems();
        break;
    case FILTER_FIELD_EDUCATION:
        modelList = EDUCTL->getAllItems();
        break;
    default:
        loge("Field %d not supported", fieldId);
    };
    if (modelList.count() > 0) {
        foreach (DbModel* item, modelList) {
            keywords.insert(item->uid(), item->name());
        }
    }
    logd("found %d keywords", (int)keywords.count());
    traceout;
    return keywords;
}


void UICommonListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showOnHtmlViewer(comm, getTitle());
    } else {
        loge("Comm obj is null");
        Utils::showErrorBox("Không có thông tin để xem");
    }
    traceout;
}

void UICommonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    // TODO: handle it
    MainWindow::showAddEditCommonModel(true, nullptr, this);
    traceout;
}

void UICommonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showAddEditCommonModel(true, comm, this);
    } else {
        loge("Model obj is null");
        Utils::showErrorBox("Không có thông tin để chỉnh sửa");
    }
    traceout;
}

void UICommonListView::onDbModelReady(ErrCode ret, DbModel *model, DlgCommonEditModel *dlg)
{
    tracein;
    if (ret == ErrNone) {
        if (model){
            model->dump();
        }
        onReload();
    }

    traceout;
}

DbModel *UICommonListView::onNewModel()
{
    tracein;
    return nullptr;
}

QString UICommonListView::getName()
{
    return "UICommonListView";
}

void UICommonListView::onModelControllerDataUpdated()
{
    tracein;
    if (!mSuspendReloadOnDbUpdate) {
        reload();
    } else {
        logw("Suspend reload on data update");
    }
    traceout;
}
