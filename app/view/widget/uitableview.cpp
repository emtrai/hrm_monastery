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
#include <QFileDialog>
#include <QMenu>
#include "filectl.h"

UITableView::UITableView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UITableView),
    mFpDataReq(nullptr),
    mFpTotalDataReq(nullptr),
    mItemPerPage(0),
    mMenu(nullptr)
{
    ui->setupUi(this);
    ui->tblList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tblList, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint)));
//    QObject::connect(ui->cbKeyword, SIGNAL(returnPressed()), this, SLOT(on_cbKeyword_returnPressed()));
}

UITableView::~UITableView()
{
    traced;
    if (mMenu != nullptr){
        // TODO: action may be allocated, need to check again if action is really clear???
        // see: https://doc.qt.io/qt-6/qmenu.html#clear
        mMenu->clear();
        delete mMenu;
    }
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

    setTitle(getTitle());
    tracede;
}

void UITableView::reload()
{
    traced;
    onLoad();
    onUpdatePage(1);
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

//    if (mFpTotalDataReq == nullptr || mFpDataReq == nullptr)
//        return;

    qint32 total = getTotalItems();
    if (total > 0){
        qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
        qint32 totalPages = total/perPage;
        logd("total %d perpage %d totaPage %d", total, perPage, totalPages);

        QList<UITableItem*> items = getListItem(page, perPage, totalPages);
        int idx = tbl->rowCount();
        foreach (UITableItem* item, items){
            tbl->insertRow(idx);
            QStringList values = item->valueList();
            for (int i = 0; i < values.count(); ++i) {
//                QTableWidgetItem* wgitem = new QTableWidgetItem(values.value(i));
//                wgitem->setData(Qt::UserRole, idx);
//                tbl->setItem(idx, i, wgitem);
                tbl->setItem(idx, i, UITableWidgetItem::build(values.value(i), i, idx, item));

            }

            idx ++;
        }
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

void UITableView::importRequested(const QString& fpath)
{
    traced;
}

void UITableView::onViewItem(UITableWidgetItem *item)
{
    traced;
    logd("parent class, nothing to do");
}

QMenu* UITableView::buildPopupMenu(UITableWidgetItem* item)
{
    traced;
    if (mMenu == nullptr) {
        logd("build menu actions");
        mMenu = new QMenu(this);
    }
    mMenu->clear();

    QList<UITableMenuAction*> actions = getMenuCommonActions(mMenu);


    if (item != nullptr){
//        QVariant data = item->data(Qt::UserRole);
//        bool ok = false;
//        int idx = data.toInt(&ok);
//        logd("item data ok %d idx %d", ok, idx);
//        if (!ok) {
//            idx = -1;
//        }

        QList<UITableMenuAction*> itemActions = getMenuItemActions(mMenu, item);
        if (!itemActions.empty()) {
            actions.append(itemActions);
        }
    }
//    mMenu->addSeparator();
    foreach (UITableMenuAction* act, actions) {
        connect(act, &QAction::triggered, [this, act](){
            logd("lambda trigger call");
            act->callback()(this->menu(), act);
//                    onMenuActionTrigger(this->mMenu, act);
//                    act->callback(this, act);
//                    this->onMenuAddAction(mN)
                    // TODO: handle return???
                });
        mMenu->addAction(act);
    }

    return mMenu;
}

QList<UITableMenuAction *> UITableView::getMenuCommonActions(const QMenu* menu)
{
    traced;
    QList<UITableMenuAction*> actionList;

    actionList.append(UITableMenuAction::build(tr("Thêm"), this)
                        ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                return this->onMenuActionAdd(m, a);
                          }));

    return actionList;
}

QList<UITableMenuAction *> UITableView::getMenuItemActions(const QMenu* menu,
                                                           UITableWidgetItem *item)
{
    traced;
    QList<UITableMenuAction*> actionList;
    actionList.append(UITableMenuAction::build(tr("Xem"), this, item)
                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                              return this->onMenuActionView(m, a);
                                          }));
    actionList.append(UITableMenuAction::build(tr("Xoá"), this, item)
                                           ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionDelete(m, a);
                                           }));
    actionList.append(UITableMenuAction::build(tr("Chỉnh sửa"), this, item)
                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                  return this->onMenuActionEdit(m, a);
                                              }));
    return actionList;
}


ErrCode UITableView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UITableView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UITableView::onMenuActionEdit(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UITableView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    traced;
    onViewItem((UITableWidgetItem*)act->tblItem());
    return ErrNone;

}

void UITableView::onFilter(const QString &catetory, qint64 opFlags, const QString &keywords)
{
    traced;
    logi("DEFAULT filter, should not caller here, DERIVED CLASS implement this");
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
    traced;
}

void *UITableItem::data() const
{
    return mData;
}

UITableItem::UITableItem():mData(nullptr)
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

void UITableView::on_btnImport_clicked()
{
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));

    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        importRequested(fname);
    }
}

quint32 UITableView::totalPages() const
{
    return mTotalPages;
}

void UITableView::setTotalPages(quint32 newTotalPages)
{
    mTotalPages = newTotalPages;
}

void UITableView::setTitle(const QString &title)
{
    traced;
    ui->lblTitle->setText(title);
    tracede;
}

QString UITableView::getTitle()
{
    return "";
}

quint32 UITableView::currentPage() const
{
    return mCurrentPage;
}

void UITableView::setCurrentPage(quint32 newCurrentPage)
{
    mCurrentPage = newCurrentPage;
}


void UITableView::on_tblList_itemDoubleClicked(QTableWidgetItem *item)
{
    traced;
    onViewItem((UITableWidgetItem*)item);
}



void UITableView::customMenuRequested(QPoint pos)
{
    traced;
//    QModelIndex index = ui->tblList->indexAt(pos);
    // TODO: hande it
    int row = ui->tblList->rowAt(pos.y());
    int col = ui->tblList->rowAt(pos.x());
    logd("menu at row=%d col=%d", row, col);
//    cell = ui->tblList->item(row, col);
    UITableWidgetItem *item = (UITableWidgetItem*)ui->tblList->itemAt(pos);
    QMenu* menu = buildPopupMenu(item);
    menu->popup(ui->tblList->viewport()->mapToGlobal(pos));
}


void UITableView::on_btnFilter_clicked()
{
    traced;
    onFilter(ui->cbCategory->currentText(), 0, ui->cbKeyword->currentText());
}

//void UITableView::on_cbKeyword_returnPressed()
//{
//    traced;
//    on_btnFilter_clicked();
//}


UITableMenuAction::UITableMenuAction(const QString &text, QObject *parent):
    QAction(text, parent),
    mTblItem(nullptr)
{
    traced;
}

UITableWidgetItem *UITableMenuAction::tblItem() const
{
    return mTblItem;
}

UITableMenuAction* UITableMenuAction::setTblItem(UITableWidgetItem *newTblItem)
{
    mTblItem = newTblItem;
    return this;
}

UITableMenuAction *UITableMenuAction::build(const QString &text, QObject *parent,
                                            UITableWidgetItem *item, qint32 idx)
{
    traced;
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    menu->setTblItem(item);
    return menu;
}

const std::function<ErrCode (QMenu *, UITableMenuAction *)> &UITableMenuAction::callback() const
{
    return mCallback;
}

UITableMenuAction* UITableMenuAction::setCallback(const std::function<ErrCode (QMenu *, UITableMenuAction *)> &newCallback)
{
    mCallback = newCallback;
    return this;
}

void *UITableMenuAction::getData()
{
    traced;
    if (mTblItem != nullptr) {
        UITableItem* item = mTblItem->item();
        if (item != nullptr) {
            return item->data();
        } else {
            loge("invalid tbl item");
        }
    } else {
        loge("invaid widget item");
    }
    logi("invalid data");
    return nullptr;
}

void UITableView::on_btnAdd_clicked()
{
    traced;
    logi("Add from parent, do nothing");
}

QMenu *UITableView::menu() const
{
    return mMenu;
}


UITableWidgetItem::UITableWidgetItem(const QString &text):
    QTableWidgetItem(text),
    mItem(nullptr),
    mItemIdx(0),
    mIdx(0)
{
    traced;
}

UITableWidgetItem *UITableWidgetItem::build(const QString &txt, qint32 itemIdx, qint32 idx, UITableItem *item)
{
    UITableWidgetItem* wg = new UITableWidgetItem(txt);
    wg->setItem(item);
    wg->setItemIdx(itemIdx);
    wg->setIdx(idx);
    return wg; // TODO: when this item is deleted??? check carefuly please
}

UITableItem *UITableWidgetItem::item() const
{
    return mItem;
}

void UITableWidgetItem::setItem(UITableItem *newItem)
{
    mItem = newItem;
}

qint32 UITableWidgetItem::itemIdx() const
{
    return mItemIdx;
}

void UITableWidgetItem::setItemIdx(qint32 newItemIdx)
{
    mItemIdx = newItemIdx;
}

qint32 UITableWidgetItem::idx() const
{
    return mIdx;
}

void UITableWidgetItem::setIdx(qint32 newIdx)
{
    mIdx = newIdx;
}
