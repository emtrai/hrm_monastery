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
 * Filename: uitableview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uitableview.h"
#include "ui_uitableview.h"
#include "logger.h"
#include <QStringList>



UITableView::UITableView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UITableView),
    mItemPerPage(0)
{
    ui->setupUi(this);


}

UITableView::~UITableView()
{
    traced;
    delete ui;
}

QStringList UITableView::getHeader()
{
    traced;
    return mHeader;
}

void UITableView::setupUI()
{
    traced;
    if (mHeader.empty())
        initHeader();
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(getHeader().count());
    ui->tblList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500);

    ui->tblList->setHorizontalHeaderLabels(getHeader());
}

void UITableView::showEvent(QShowEvent *ev)
{
    QFrame::showEvent(ev);
    traced;

    onLoad();

    onUpdatePage(1);
}

void UITableView::onUpdatePage(qint32 page)
{
    QTableWidget* tbl = ui->tblList;
    traced;

    // TODO: is it really remove all data?
    // Is there any risk of leakage memory here??
    logd("Clear all");
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    // TODO: clear saints???

    if (mFpTotalDataReq == nullptr || mFpDataReq == nullptr)
        return;

    qint32 total = getTotalItems();
    qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
    qint32 totalPages = total/perPage;
    logd("total %d perpage %d totaPage %d", total, perPage, totalPages);

    QList<UITableItem*> items = getListItem(page, perPage, totalPages);
    int idx = tbl->rowCount();
    foreach (UITableItem* item, items){
        tbl->insertRow(idx);
        QStringList values = item->valueList();
        for (int i = 0; i < values.count(); ++i) {
            tbl->setItem(idx, i, new QTableWidgetItem(values.value(i)));
        }

        idx ++;
    }
}

QList<UITableItem *> UITableView::getListItem(qint32 page, qint32 perPage, qint32 totalPages)
{
    return (mFpDataReq != nullptr)?mFpDataReq(page, perPage, totalPages):QList<UITableItem *>();
}

qint32 UITableView::getTotalItems()
{
    return (mFpTotalDataReq != nullptr)?mFpTotalDataReq():0;
}

ErrCode UITableView::onLoad()
{
    traced;
    return ErrNone;
}

qint32 UITableView::itemPerPage() const
{
    return mItemPerPage;
}

void UITableView::setItemPerPage(qint32 newItemPerPage)
{
    mItemPerPage = newItemPerPage;
}

void UITableView::initHeader()
{
    traced;
}

void UITableView::setFpTotalDataReq(onRequestTotalData newFpTotalDataReq)
{
    mFpTotalDataReq = newFpTotalDataReq;
}

void UITableView::setFpDataReq(onRequestData newFpDataReq)
{
    mFpDataReq = newFpDataReq;
}

void UITableView::setHeader(const QStringList &newHeader)
{
    mHeader = newHeader;
}

UITableItem *UITableItem::build(void *data)
{
    return new UITableItem(data);
}

UITableItem *UITableItem::addValue(const QString &val)
{
    mValueList.append(val);
    return this;
}

UITableItem::UITableItem(void* data):
    mData(data)
{

}

const QStringList &UITableItem::valueList() const
{
    return mValueList;
}

void UITableItem::setValueList(const QStringList &newValueList)
{
    mValueList = newValueList;
}
