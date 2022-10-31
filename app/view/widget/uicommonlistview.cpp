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

void UICommonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(item->uid());
    tblItem->addValue(item->name());
    tracede;
}

qint32 UICommonListView::getTotalItems()
{
    traced;
    return mItemList.count();
}

QList<DbModel *> UICommonListView::getListItem()
{
    return QList<DbModel*>();
}

Controller *UICommonListView::getController()
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
    Controller* ctl = getController();
    if (ctl != nullptr)
        ctl->loadFromDb();
    onLoad();
    tracede;
    return ErrNone;
}

void UICommonListView::initHeader()
{
    traced;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Mã định danh"));
    mHeader.append(tr("Tên"));
        tracede;
}

void UICommonListView::initFilterFields()
{
    traced;
    appendFilterField(FILTER_FIELD_NAME, tr("Tên"));
    tracede;
}

void UICommonListView::initFilterOperators()
{
    traced;
    appendFilterOperator(FILTER_OP_EQUAL, tr("Là"));
    appendFilterOperator(FILTER_OP_NOT_EQUAL, tr("Khác"));
    appendFilterOperator(FILTER_OP_CONTAIN, tr("Có"));
    appendFilterOperator(FILTER_OP_NOT_CONTAIN, tr("Không có"));
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
