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
#include "filter.h"
#include "mainwindow.h"

UICommonListView::UICommonListView(QWidget *parent):
    UITableView(parent)
{
}

UICommonListView::~UICommonListView()
{
    traced;
}

QList<UITableItem *> UICommonListView::getListItem(qint32 page, qint32 perPage, qint32 totalPages)
{
    QList<UITableItem *> items;
    traced;
    foreach (DbModel* item, mItemList) {
        UITableItem* tblItem = UITableItem::build(item);
        updateItem(item, tblItem);
        items.append(tblItem);
    }
    return items;
}

void UICommonListView::initHeader()
{
    traced;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Tên định danh"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Ghi chú"));
    tracede;
}
void UICommonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(item->nameId());
    tblItem->addValue(item->name());
    tblItem->addValue(item->remark());
    tracede;
}

qint32 UICommonListView::getTotalItems()
{
    traced;
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
    traced;
    mItemList.clear(); // TODO: clean up item data???
    mItemList = getListItem();
    clearFilter();
    tracede;
    return ErrNone;
}

ErrCode UICommonListView::onReload()
{
    traced;
    onLoad();
    tracede;
    return ErrNone;
}

void UICommonListView::initFilterFields()
{
    traced;
    appendFilterField(FILTER_FIELD_NAME, tr("Tên"));
    tracede;
}


QHash<QString, QString> UICommonListView::getFilterKeywords(int fieldId, const QString &fieldText)
{
    traced;
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
    default:
        loge("Field %d not supported", fieldId);
    };
    if (modelList.count() > 0) {
        foreach (DbModel* item, modelList) {
            keywords.insert(item->uid(), item->name());
        }
    }
    logd("found %d keywords", (int)keywords.count());
    tracede;
    return keywords;
}


void UICommonListView::onViewItem(UITableWidgetItem *item)
{
    traced;
    int idx = item->idx();
    DbModel* comm = item->itemData();
    if (comm) {
        MainWindow::showOnHtmlViewer(comm, getTitle());
    } else {
        loge("Comm obj is null");
        Utils::showErrorBox("Không có thông tin để xem");
    }
    tracede;
}

void UICommonListView::onAddItem(UITableWidgetItem *item)
{
    traced;
    // TODO: handle it
    MainWindow::showAddEditCommonModel(true, nullptr, this);
    tracede;
}

void UICommonListView::onDbModelReady(ErrCode ret, DbModel *model, DlgCommonEditModel *dlg)
{
    traced;
    if (ret == ErrNone) {
        if (model){
            model->dump();
        }
        onReload();
    }

    tracede;
}

DbModel *UICommonListView::onNewModel()
{
    traced;
    return nullptr;
}
