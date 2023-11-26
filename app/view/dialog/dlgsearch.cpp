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
 * Filename: DlgSearch.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/20/2022
 * Brief:
 */
#include "dlgsearch.h"
#include "ui_dlgsearch.h"
#include "logger.h"
#include "dbmodel.h"
#include "dialogutils.h"
#include "stringdefs.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include "viewutils.h"

#define ITEMS_PER_PAGE (10)

DlgSearch::DlgSearch(QWidget *parent, bool isMulti) :
    QDialog(parent),
    ui(new Ui::DlgSearch),
    mIsMultiSelection(isMulti)
{
    tracein;
    ui->setupUi(this);
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    if (mIsMultiSelection)
        ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
    else
        ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    ui->tblList->setShowGrid(true);
    //    ui->tblCommunityList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500); // TODO: define default fix?

    ui->btnAll->setVisible(false);
    ui->btnManagers->setVisible(false);

}


DlgSearch::~DlgSearch()
{
    tracein;
    clearAll();
    delete ui;
    traceout;
}


void DlgSearch::on_btnSearch_clicked()
{
    QString keyword = ui->txtName->text().trimmed();
    tracein;
    if (!keyword.isEmpty()) {
        logd("search with keyword '%s'", keyword.toStdString().c_str());
        onSearch(keyword);
        updatePage(1);
    } else {
        loge("Nothing to search");
        DialogUtils::showErrorBox(tr("Nhập tên để tìm")); // TODO: translation
    }
    traceout;
}

const QList<DbModel *> &DlgSearch::selectedItems() const
{
    return mSelectedItems;
}

const DbModel *DlgSearch::selectedItem() const
{
    DbModel* ret = nullptr;
    tracein;
    logd("no. selected item %lld", mSelectedItems.count());
    if (mSelectedItems.count() > 0) {
        ret = mSelectedItems[0];
    }
    traceout;
    return ret;
}

bool DlgSearch::getIsMultiSelection() const
{
    return mIsMultiSelection;
}

void DlgSearch::setIsMultiSelection(bool newIsMultiSelection)
{
    mIsMultiSelection = newIsMultiSelection;
    if (mIsMultiSelection)
        ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
    else
        ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
}

void DlgSearch::enableGetAllSupport()
{
    traced;
    ui->btnAll->setVisible(true);
}

void DlgSearch::enableGetManagerSupport()
{
    traced;
    ui->btnManagers->setVisible(true);

}

void DlgSearch::setupUi()
{
    tracein;
    initHeader();
    ui->tblList->setColumnCount(mHeader.count());
    ui->tblList->setHorizontalHeaderLabels(mHeader);
    ui->lblTitle->setText(getTitle());
    if (ui->buttonBox->button(QDialogButtonBox::Cancel))
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_CANCEL);
    if (ui->buttonBox->button(QDialogButtonBox::Ok))
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText(STR_OK);

    traceout;
}

void DlgSearch::clearAll()
{
    tracein;
    RELEASE_LIST_DBMODEL(mListItems);
    mSelectedItems.clear(); // item in selected item is just pointer to mListItems
    traceout;
}


void DlgSearch::accept()
{
    tracein;
    // TODO: validate data
    QTableWidget* tbl = ui->tblList;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    logd("Selection count %d", (int)selection.count());
    // only select one
    if (selection.count() > 0) {
        for(int i=0; i< selection.count(); i++)
        {
            QModelIndex index = selection.at(i);
            // TODO: validate value
            qint32 idx = getItemIdxFromRowIdx(index.row());
            if (idx >= 0) {
                DbModel* item = getItemAtIdx(idx);
                if (item) {
                    mSelectedItems.append(item);
                } else {
                    loge("Null item at index %d row %d", idx, index.row());
                }
            } else {
                loge("invalid idx");
                break;
            }
        }
    } else {
        loge("Nothing to select");
        // TODO:show warning dialog???
    }
    QDialog::accept();

}

QString DlgSearch::getTitle()
{
    return STR_LIST;
}

void DlgSearch::initHeader()
{
    traced;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
}

ErrCode DlgSearch::onSearch(const QString &keyword)
{
    tracein;
    clearAll();
    ErrCode err = MainWindow::showProcessingDialog(
        STR_QUERYING, nullptr,
        [this, keyword](ErrCode* err, void* data, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            ErrCode ret = this->doSearch(keyword, this->mListItems);
            if (err) *err = ret;
            return nullptr;//nothing to return
        },
        [keyword](ErrCode err, void* data, void* result, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            UNUSED(result);
            dbgd("Search result %d", err);
            MAINWIN->showIfErrorBox(QString(tr("Tìm kiếm '%1' lỗi")).arg(keyword), err);
            return err;
        });
    traceout;
    return err;
}

ErrCode DlgSearch::onGetAll()
{
    tracein;
    clearAll();
    ErrCode err = MainWindow::showProcessingDialog(
        STR_QUERYING, nullptr,
        [this](ErrCode* err, void* data, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            ErrCode ret = this->doGetAll(this->mListItems);
            if (err) *err = ret;
            return nullptr;//nothing to return
        },
        [](ErrCode err, void* data, void* result, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            UNUSED(result);
            dbgd("Get all result %d", err);
            MAINWIN->showIfErrorBox(QString(tr("Truy vấn thông tin lỗi")), err);
            return err;
        });
    traceout;
    return err;
}

ErrCode DlgSearch::onGetManagers()
{
    tracein;
    clearAll();
    ErrCode err = MainWindow::showProcessingDialog(
        STR_QUERYING, nullptr,
        [this](ErrCode* err, void* data, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            ErrCode ret = this->doGetManager(this->mListItems);
            if (err) *err = ret;
            return nullptr;//nothing to return
        },
        [](ErrCode err, void* data, void* result, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            UNUSED(result);
            dbgd("Get manager result %d", err);

            MAINWIN->showIfErrorBox(QString(tr("Truy vấn người quản lý lỗi")), err);
            return err;
        });
    traceout;
    return err;
}

ErrCode DlgSearch::doGetManager(QList<DbModel*>& items)
{
    tracein;
    UNUSED(items);
    loge("MUST BE CALLED BY DERIVED CLASS, NOT CALL HERE");
    traceout;
    return ErrNotImpl;
}

QString DlgSearch::getValueOfItemAt(int idx, int col, DbModel *item)
{
    tracein;
    QString val;
    logd("idx = %d, col = %d", idx, col);
    if (item != nullptr) {
        logd("item is nullptr, get from index");
        item = getItemAtIdx(idx);
    }
    if (item != nullptr){
        val = getValueOfItemCol(col, item);
    }
    traceout;
    return val;
}

QString DlgSearch::getValueOfItemCol(int col, const DbModel *item)
{
    tracein;
    QString val;
    switch (col) {
    case 0:
        val = item->nameId();
        break;
    case 1:
        val = item->name();
        break;
    default:
        loge("invalid col %d", col);
        break;
    }
    logd("value of col %d is '%s'", col, STR2CHA(val));
    traceout;
    return val;
}

void DlgSearch::updatePage(qint32 page, bool updatePage)
{
    tracein;
    QTableWidget* tbl = ui->tblList;
    cleanupTableItems();
    quint32 cnt = mListItems.size();

    logd("Found %d item", cnt);
    if (cnt > 0 && page > 0) {

        qint32 idx = tbl->rowCount();
        qint64 startIdx = (page-1)*ITEMS_PER_PAGE;
        qint64 endIdx = startIdx + ITEMS_PER_PAGE;
        if (endIdx >= mListItems.size()) {
            endIdx = mListItems.size();
        }
        for(; startIdx < endIdx; startIdx++){
            DbModel* item = getItemAtIdx(startIdx);
            logd("idx=%d", idx);
            if (item) {
                tbl->insertRow(idx);
                for (int col = 0; col < mHeader.count(); col++) {
                    tbl->setItem(idx, col,
                                 new QTableWidgetItem(getValueOfItemAt(startIdx, col, item)));
                }
                idx++;
            } else {
                logw("item at idex %lld is null", startIdx);
            }
        }
        if (updatePage) {
            updatePageInfo(page, cnt);
        }
    } else {
        logi("no data cnt %d page %d", cnt, page);
        updatePageInfo(0);
    }
    traceout;
}

void DlgSearch::updatePageInfo(qint32 page, qint32 totalItems)
{
    tracein;
    ViewUtils::updatePageInfo(ui->cbPage, ui->lblPage,
                              page, totalItems, ITEMS_PER_PAGE);
    traceout;
}

void DlgSearch::cleanupTableItems()
{
    tracein;

    QTableWidget* tbl = ui->tblList;
    int rowCnt = tbl->rowCount();
    int colCnt = tbl->columnCount();
    logd("rowCnt %d colCnt %d", rowCnt, colCnt);
    for (int i = 0; i < rowCnt; i++) {
        for (int j = 0; j < colCnt; j++) {
            QTableWidgetItem* item = tbl->takeItem(i, j);
            if (item) {
                delete item;
            }
        }
    }
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    traceout;
}

qint32 DlgSearch::getItemIdxFromRowIdx(qint32 rowIdx)
{
    tracein;
    qint32 idx = -1;
    logd("rowIdx=%d", rowIdx);
    qint32 page = getCurrentSelectedPage();
    if (page > 0) {
        idx = (page-1)*ITEMS_PER_PAGE + rowIdx;
        logd("idx = %d", idx);
    } else {
        loge("invalid page");
    }
    traceret(idx);
    return idx;
}

qint32 DlgSearch::getCurrentSelectedPage(qint32 index)
{
    tracein;
    int page = 0;
    logd("index = %d", index);
    if (index < 0)
        index = ui->cbPage->currentIndex();
    logd("idx %d", index);
    if (index >= 0) {
        QVariant value = ui->cbPage->itemData(index);
        if (value.isValid()) {
            bool ok = false;
            page = value.toInt(&ok);
            if (!ok) {
                loge("invalid value");
                page = 0;
            }
        } else {
            loge("valid is invalid");
        }
    }
    traceret(page);
    return page;
}



void DlgSearch::on_btnAll_clicked()
{
    tracein;
    onGetAll();
    updatePage(1);
    traceout;
}


void DlgSearch::on_btnManagers_clicked()
{
    tracein;
    onGetManagers();
    updatePage(1);
    traceout;

}


DbModel *DlgSearch::getItemAtIdx(int idx)
{
    tracein;
    DbModel* ret = nullptr;
    logd("get item at idx=%d", idx);
    if (idx >= 0 && idx < mListItems.count()) {
        ret = (DbModel*)mListItems[idx];
    } else {
        loge("invalid idx %d", idx);
    }
    traceout;
    return ret;
}

void DlgSearch::on_cbPage_currentIndexChanged(int index)
{
    tracein;
    int page = getCurrentSelectedPage(index);
    if (page > 0) {
        logd("update page %d", page);
        updatePage(page);
    }
    traceout;
}

