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
 * Filename: dlgimportlistresult.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#include "dlgimportlistresult.h"
#include "ui_dlgimportlistresult.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"

UIImportItem *UIImportItem::build(void *data)
{
    return new UIImportItem(data);
}

UIImportItem *UIImportItem::addValue(const QString &val)
{
    mValueList.append(val);
    return this;
}

UIImportItem::UIImportItem(void* data):
    mData(data)
{

}

const QStringList &UIImportItem::valueList() const
{
    return mValueList;
}

void UIImportItem::setValueList(const QStringList &newValueList)
{
    mValueList = newValueList;
}

//==================================================================

DlgImportListResult::DlgImportListResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportListResult)
{
    traced;
    ui->setupUi(this);
    DIALOG_SIZE_SHOW(this);
}

DlgImportListResult::~DlgImportListResult()
{
    delete ui;
}

void DlgImportListResult::setHeader(const QStringList &newHeader)
{
    mHeader = newHeader;
}
const QList<DbModel *> &DlgImportListResult::list() const
{
    return mList;
}

void DlgImportListResult::setup(const QList<DbModel *> &newList)
{
    setupUI();
    mList = newList;
}

void DlgImportListResult::setupUI()
{
    traced;

    initHeader();
    QStringList hdrs = getFinalHeader();
    logd("hdrs cnt %d", hdrs.count());
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(hdrs.count());
    ui->tblList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500);

    ui->tblList->setHorizontalHeaderLabels(hdrs);
}

void DlgImportListResult::initHeader()
{
    traced;
}

QStringList DlgImportListResult::getFinalHeader()
{
    traced;
    QStringList hdrs;
    hdrs.append(tr("Chọn"));
    hdrs.append(getHeader());
    return hdrs;
}

QStringList DlgImportListResult::getHeader()
{
    return mHeader;
}

QList<UIImportItem *>* DlgImportListResult::getItems()
{
    return nullptr;
}

ErrCode DlgImportListResult::onLoad()
{
    traced;
    QTableWidget* tbl = ui->tblList;
    traced;

    // TODO: is it really remove all data?
    // Is there any risk of leakage memory here??
    logd("Clear all");
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    // TODO: clear all data in UIImportItem???
    // TODO: check carefully to avoid memory leak, stupid C/C++

    QList<UIImportItem*>* items = getItems();
    if (items && !items->empty()) {
        int idx = tbl->rowCount();
        foreach (UIImportItem* item, *items){
            tbl->insertRow(idx);
            QStringList values = item->valueList();

            QTableWidgetItem *widgetItem = new QTableWidgetItem();
            widgetItem->setCheckState(Qt::CheckState::Unchecked);
            tbl->setItem(idx, 0, widgetItem);
            for (int i = 0; i < values.count(); ++i) {
                tbl->setItem(idx, i+1, new QTableWidgetItem(values.value(i)));
            }
            idx ++;
        }
        delete items;
    } else {
        loge("Nothing to show");
    }
    return ErrNone;

}

void DlgImportListResult::showEvent(QShowEvent *event)
{
    traced;
    QWidget::showEvent( event );
    onLoad();
}

ErrCode DlgImportListResult::saveItems(const QList<DbModel *> &list)
{
    traced;
    ErrCode ret = ErrNone;
    // TODO: review this again
    foreach (DbModel* item, list) {
        logd("Save %d", item->name().toStdString().c_str());
        ret = item->save();
        logi("save item result %d", ret);
    }
    return ErrNone; // TODO: handle error case???
}

void DlgImportListResult::accept()
{
    traced;
    ErrCode ret = ErrNone;

    if (mList.count() > 0) {
        ret = saveItems(mList);
    } else {
        logi("Nothing to save");
    }
    logi("Add Country, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));
}
