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
#include "mainwindow.h"
#include "dialogutils.h"
#include "stringdefs.h"

#define ITEM_CHECK (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable)
#define ITEM_NONE_CHECK (Qt::NoItemFlags)

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

void *UIImportItem::data() const
{
    return mData;
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
    ui(new Ui::DlgImportListResult),
    mTargetModel(nullptr)
{
    tracein;
    ui->setupUi(this);
    traceout;
//    DIALOG_SIZE_SHOW(this);
}

DlgImportListResult::~DlgImportListResult()
{
    RELEASE_LIST_DBMODEL(mList);
    FREE_PTR(mTargetModel);
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

ErrCode DlgImportListResult::setup(const QList<DbModel *> &newList)
{
    ErrCode err = ErrNone;
    tracein;
    setupUI();
    mList = newList;
    traceret(err);
    return err;
}

void DlgImportListResult::setupUI()
{
    tracein;

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

    if (mTargetModel) {
        QString title = QString(tr("Nhập dữ liệu cho: %1")).arg(mTargetModel->name());
        setTitle(title);
    }
    traceout;
}

void DlgImportListResult::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    traceout;
}

QStringList DlgImportListResult::getFinalHeader()
{
    tracein;
    QStringList hdrs;
    hdrs.append(tr("Chọn"));
//    hdrs.append(tr(""));
    hdrs.append(getHeader());
    traceout;
    return hdrs;
}

QStringList DlgImportListResult::getHeader()
{
    return mHeader;
}

QList<UIImportItem *> DlgImportListResult::getItems(bool* ok)
{
    tracein;
    QList<UIImportItem *> items;
    ErrCode err = ErrNone;
    int cnt = 0;
    if (mList.size() > 0) {
        foreach (DbModel* item, mList) {
            if (item) {
                UIImportItem* wgitem = UIImportItem::build(item);
                if (wgitem) {
                    setWidgetItem(wgitem, item);
                    // TODO: add more information??
                    items.append(wgitem);
                } else {
                    loge("no memory?");
                    err = ErrNoMemory;
                    break;
                }
            } else {
                loge("Something wrong, data at %d in list but null", cnt);
                err = ErrInvalidData;
            }
            cnt++;
        }
    } else {
        logw("No import data to show");
    }
    if (err != ErrNone) {
        loge("invalid data for import err=%d", err);
        RELEASE_LIST(items, UIImportItem);
    }
    if (ok) *ok = (err == ErrNone);
    traceout;
    return items;
}

ErrCode DlgImportListResult::setWidgetItem(UIImportItem *wgitem, DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!wgitem || !item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        wgitem->addValue(item->nameId());
        wgitem->addValue(item->name());
    }
    traceret(err);
    return err;
}

ErrCode DlgImportListResult::onLoad()
{
    tracein;
    QTableWidget* tbl = ui->tblList;
    bool ok = false;
    ErrCode err = ErrNone;

    // TODO: is it really remove all data?
    // Is there any risk of leakage memory here??
    logd("Clear all");
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    // TODO: clear all data in UIImportItem???
    // TODO: check carefully to avoid memory leak, stupid C/C++

    QList<UIImportItem*> items = getItems(&ok);
    if (!ok) {
        err = ErrInvalidData;
        loge("no item data to get");
    }
    if (err == ErrNone && !items.empty()) {
        int idx = tbl->rowCount();
        QIcon icOk (QString::fromUtf8(":/icon/icon/icons8-ok-48"));
        QIcon icNok (QString::fromUtf8(":/icon/icon/icons8-error-cloud-96"));
        QIcon icDup (QString::fromUtf8(":/icon/icon/icons8-copy-64.png"));
        int itemIdx = 0;
        foreach (UIImportItem* item, items){
            tbl->insertRow(idx);
            QStringList values = item->valueList();

            QTableWidgetItem *widgetItem = new QTableWidgetItem();

//            tbl->setItem(idx, 0, widgetItem);

            DbModel* val = (DbModel* )item->data();
            if (val != nullptr) {
                bool isExist = val->isExist();
                if (isExist) {
                    widgetItem->setFlags(ITEM_NONE_CHECK);
                    widgetItem->setIcon(icDup);
                    widgetItem->setToolTip(STR_DATA_EXISTED);
                } else {
                    ErrCode valRes = val->validateAllFields();
                    if (valRes == ErrNone) {
                        widgetItem->setFlags(ITEM_CHECK);
                        widgetItem->setCheckState(Qt::CheckState::Unchecked);
                    } else {
                        widgetItem->setFlags(ITEM_NONE_CHECK);
                        widgetItem->setIcon(icNok);
                        widgetItem->setToolTip(STR_DATA_ERROR);
                    }
                }

            } else {
                widgetItem->setFlags(ITEM_NONE_CHECK);
                widgetItem->setIcon(icNok);
                widgetItem->setToolTip(STR_DATA_ERROR);
            }
            tbl->setItem(idx, 0, widgetItem);
            widgetItem->setData(Qt::UserRole, itemIdx);

            for (int i = 0; i < values.count(); ++i) {
                tbl->setItem(idx, i+1, new QTableWidgetItem(values.value(i)));
            }
            idx ++;
            itemIdx++;
        }
        RELEASE_LIST(items, UIImportItem);
    } else {
        loge("Nothing to show");
    }
    traceret(err);
    return err;

}

void DlgImportListResult::showEvent(QShowEvent *event)
{
    tracein;
    QWidget::showEvent( event );
    onLoad();
    traceout;
}

ErrCode DlgImportListResult::saveItems(const QList<DbModel *> &list)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("save %d items", list.size());
    ret = MainWindow::showProcessingDialog(tr("Lưu dữ liệu"), nullptr,
        [this, list](ErrCode* err, void* data, DlgWait* dlg) {
            int total = list.size();
            logd("saving %ld item", list.size());
            int cnt = 0;
            foreach (DbModel* item, list) {
//                logd("Save %s", item->name().toStdString().c_str());
                item->dump();
                *err = item->save();
                logi("save item result %d", *err);
                cnt++;
                if (cnt % 3 == 0){
                    dlg->setMessage(QString(tr("Đã lưu %1 / %2")).arg(cnt, total));
                }
            }
            return nullptr;
        },
        [this](ErrCode err, void* data, void* result, DlgWait* dlg) {
            logd("Save result %d", err);
            return err;
        });
    // TODO: review this again
//    foreach (DbModel* item, list) {
//        logd("Save %s", item->name().toStdString().c_str());
//        item->dump();
//        ret = item->save();
//        logi("save item result %d", ret);
//    }
    traceout;
    return ErrNone; // TODO: handle error case???
}

void DlgImportListResult::accept()
{
    tracein;
    ErrCode ret = ErrNone;
    // TODO: show dialog to confirm when dialog is closed/saved???
    if (mList.count() > 0) {
        QTableWidget* tbl = ui->tblList;
        QList<DbModel *> selectedItem;
        int cnt = tbl->rowCount();
        for (int idx = 0; idx < cnt; idx++){
            QTableWidgetItem *widgetItem = tbl->item(idx, 0);
            if (widgetItem->checkState() == Qt::CheckState::Checked) {
                int itemIdx = widgetItem->data(Qt::UserRole).toInt();
                logd("selected item idx %d", itemIdx);
                selectedItem.append(mList[itemIdx]);
            }
        }
        if (selectedItem.size() > 0) {
            ret = saveItems(selectedItem);
            logi("Save %ld items, ret %d", selectedItem.size(), ret);
            if (ret == ErrNone)
                QDialog::accept();
            else
                DialogUtils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));
        } else {
            loge("no selected item");
            DialogUtils::showErrorBox(tr("Vui lòng chọn mục để lưu"));
        }
    } else {
        logi("Nothing to save");
        QDialog::accept();
    }
    traceout;
}

void DlgImportListResult::setTargetModel(const DbModel *newTargetModel)
{
    tracein;
    FREE_PTR(mTargetModel);
    if (newTargetModel) {
        logd("clone model '%s'", MODELSTR2CHA(newTargetModel));
        mTargetModel = CLONE_MODEL(newTargetModel, DbModel);
    } else {
        logw("null input value");
    }
    traceout;
}

void DlgImportListResult::setTitle(const QString &title)
{
    if (!title.isEmpty()) {
        ui->txtSubject->setText(title);
    }
}


void DlgImportListResult::on_chkSelect_stateChanged(int arg1)
{
    tracein;
    logd("state %d", arg1);
    bool isChecked = (arg1 == Qt::Checked);
    QTableWidget* tbl = ui->tblList;
    QList<DbModel *> selectedItem;
    int cnt = tbl->rowCount();
    for (int idx = 0; idx < cnt; idx++){
        QTableWidgetItem *widgetItem = tbl->item(idx, 0);
        logd("item flag %d", widgetItem->flags());
        if (widgetItem->flags() == ITEM_CHECK) {
            widgetItem->setCheckState(isChecked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked );
        }
    }
    // TODO: allow to edit item before saving????
    traceout;
}

