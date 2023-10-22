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
#include "utils.h"
#include "dbmodel.h"
#include "dialogutils.h"

DlgSearch::DlgSearch(QWidget *parent, bool isMulti) :
    QDialog(parent),
    ui(new Ui::DlgSearch),
//    mPerson(nullptr),
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
//    tracein;
//    QTableWidget* tbl = ui->tblList;
    QString keyword = ui->txtName->text().trimmed();
////    mPerson = nullptr;
//    clearAll();
//    tbl->clearContents();
//    tbl->model()->removeRows(0, tbl->rowCount());

//    if (!keyword.isEmpty()) {
//        int cnt = onSearch(keyword);
//        logd("Found %d item", cnt);
//        if (cnt > 0) {

//            qint32 idx = tbl->rowCount();

//            for(int i = 0; i < cnt; i++){
//                DbModel* item = getItemAtIdx(i);
//                logd("idx=%d", idx);
//                item->dump();
//                tbl->insertRow(idx);
//                for (int col = 0; col < mHeader.count(); col++) {
//                    tbl->setItem(idx, col, new QTableWidgetItem(getValueOfItemAt(i, col, mHeader[col], item)));
//                }
//                idx++;
//            }
//        } else {
//            logi("No person");
//        }
//    } else {
//        loge("Nothing to search");
//        DialogUtils::showErrorBox(tr("Nhập tên để tìm")); // TODO: translation
//    }
    tracein;
    if (!keyword.isEmpty()) {
        logd("search with keyword '%s'", keyword.toStdString().c_str());
        query([this, keyword](){
            return this->onSearch(keyword);
        });
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
    logd("no. selected item %d", mSelectedItems.count());
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


    traceout;
}

void DlgSearch::clearAll()
{
    tracein;
    // TODO: free up data of each item? i.e. delete DbModel*
    clearSelectedItem();
    traceout;
}

void DlgSearch::clearSelectedItem()
{
    tracein;
    mSelectedItems.clear();
    // TODO: free up data of each item? i.e. delete DbModel*
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
            DbModel* item = getItemAtIdx(index.row());
            mSelectedItems.append(item);
        }
    } else {
        loge("Nothing to select");
        // TODO:show warning dialog???
    }
    QDialog::accept();

}

QString DlgSearch::getTitle()
{
    return tr("Danh sách");
}

void DlgSearch::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên"));
}

int DlgSearch::onGetAll()
{
    tracein;
    loge("MUST BE CALLED BY DERIVED CLASS, NOT CALL HERE");
    traceout;
    return 0;
}

int DlgSearch::onGetManagers()
{
    tracein;
    loge("MUST BE CALLED BY DERIVED CLASS, NOT CALL HERE");
    traceout;
    return 0;

}

QString DlgSearch::getValueOfItemAt(int idx, int col, QString header, DbModel *item)
{
    tracein;
    QString val;
    logd("idx = %d, col = %d", idx, col);
    if (item != nullptr) {
        logd("item is nullptr, get from index");
        item = getItemAtIdx(idx);
    }
    if (item != nullptr){

        // TODO: improve this, i.e. app callback to mHeader?
        switch (col) {
        case 0:
            val = item->uid();
            break;
        case 1:
            val = item->name();
            break;
        }
    }
    traceout;
    return val;
}

int DlgSearch::query(std::function<int ()> queryfunc)
{
    tracein;
    QTableWidget* tbl = ui->tblList;
    clearAll();
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    int cnt = queryfunc();

    logd("Found %d item", cnt);
    if (cnt > 0) {

        qint32 idx = tbl->rowCount();

        for(int i = 0; i < cnt; i++){
            DbModel* item = getItemAtIdx(i);
            logd("idx=%d", idx);
            if (item) {
                item->dump();
                tbl->insertRow(idx);
                for (int col = 0; col < mHeader.count(); col++) {
                    tbl->setItem(idx, col, new QTableWidgetItem(getValueOfItemAt(i, col, mHeader[col], item)));
                }
                idx++;
            } else {
                logw("item at idex %d is null", i);
            }
        }
    } else {
        logi("no data");
    }
    traceout;
    return cnt;
}



void DlgSearch::on_btnAll_clicked()
{
    tracein;
    query([this](){
        return this->onGetAll();
    });
    traceout;
}


void DlgSearch::on_btnManagers_clicked()
{
    tracein;
    query([this](){
        return this->onGetManagers();
    });
    traceout;

}

