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
#include "utils.h"
#include "filter.h"

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

    loadFilterFields();
    int key = ui->cbCategory->currentData().toInt();
    loadFilterOperators(key);
    tracede;
}

void UITableView::reload()
{
    traced;
    onReload();
    onUpdatePage(1);
    tracede;
}

void UITableView::refesh()
{
    traced;
    onUpdatePage(1);
    tracede;
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
        // TODO: remove paging, as it not need????
        qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
        qint32 totalPages = total/perPage;
        logd("total %d perpage %d totaPage %d", total, perPage, totalPages);

        QList<UITableItem*> items = getListItem(page, perPage, totalPages);
        int idx = tbl->rowCount();
        foreach (UITableItem* item, items){
            tbl->insertRow(idx);
            QStringList values = item->valueList();
            for (int i = 0; i < values.count(); ++i) {
                tbl->setItem(idx, i, UITableWidgetItem::build(values.value(i), i, idx, item));

            }

            idx ++;
        }
    }
    tracede;
}

QList<UITableItem *> UITableView::getListItem(qint32 page, qint32 perPage, qint32 totalPages)
{
    return (mFpDataReq != nullptr)?mFpDataReq(page, perPage, totalPages):QList<UITableItem *>();
}

QList<UITableItem *> UITableView::getListAllItem()
{
    traced;
    QList<UITableItem*> items;
    // TODO: fix me please!!!
    qint32 total = getTotalItems();
    logd("total item %d", total);
    if (total > 0){
        // TODO: remove paging, as it not need????
        qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
        qint32 totalPages = total/perPage;
        logd("total %d perpage %d totaPage %d", total, perPage, totalPages);

        items = getListItem(0, perPage, totalPages);
    } else {
        logd("Not item to load");
    }
    traced;
    return items;
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

ErrCode UITableView::onReload()
{
    traced;
    return onLoad();
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

void UITableView::onEditItem(UITableWidgetItem *item)
{
    traced;
    logd("parent class, nothing to do");

}


QMenu* UITableView::buildPopupMenu(UITableWidgetItem* item, const QList<UITableItem*>& items)
{
    traced;
    if (mMenu == nullptr) {
        logd("build menu actions");
        mMenu = new QMenu(this);
    }
    mMenu->clear();

    QList<UITableMenuAction*> actions = getMenuCommonActions(mMenu);

    if (items.count() > 1){
        actions.append(UITableMenuAction::buildSeparateAction());
        QList<UITableMenuAction*> itemActions = getMenuMultiItemActions(mMenu, items);
        if (!itemActions.empty()) {
            actions.append(itemActions);
        }
    } else {
        if (item != nullptr){

            actions.append(UITableMenuAction::buildSeparateAction());
            QList<UITableMenuAction*> itemActions = getMenuItemActions(mMenu, item);
            if (!itemActions.empty()) {
                actions.append(itemActions);
            }
        }
    }
//    mMenu->addSeparator();
    // TODO: free memory allocated for actions!!!!
    foreach (UITableMenuAction* act, actions) {
        if (act->menuType() == MENU_ACTION_SEPARATE) {
            mMenu->addSeparator();
        } else {
            connect(act, &QAction::triggered, [this, act](){
                logd("lambda trigger call");
                act->callback()(this->menu(), act);
                        // TODO: handle return???
                });
            mMenu->addAction(act);
        }
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

    actionList.append(UITableMenuAction::build(tr("Tải lại"), this)
                                                               ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                                   return this->onMenuActionReload(m, a);
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

QList<UITableMenuAction *> UITableView::getMenuMultiItemActions(const QMenu *menu, const QList<UITableItem *>& items)
{
    traced;
    traced;
    QList<UITableMenuAction*> actionList;
    actionList.append(UITableMenuAction::buildMultiItem(tr("Xoá"), this, &items)
                                           ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionMultiDelete(m, a);
                                           }));
    tracede;
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

ErrCode UITableView::onMenuActionMultiDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UITableView::onMenuActionEdit(QMenu *menu, UITableMenuAction *act)
{
    traced;
    onEditItem((UITableWidgetItem*)act->tblItem());
    return ErrNone;
}

ErrCode UITableView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    traced;
    onViewItem((UITableWidgetItem*)act->tblItem());
    return ErrNone;

}

ErrCode UITableView::onMenuActionReload(QMenu *menu, UITableMenuAction *act)
{
    traced;
    reload();
    tracede;
}

int UITableView::onFilter(int catetoryid, const QString &catetory, qint64 opFlags, const QString &keywords)
{
    traced;
    logi("DEFAULT filter, should not caller here, DERIVED CLASS implement this");
    logd("category id %d", catetoryid);
    logd("category '%s'", catetory.toStdString().c_str());
    logd("opFlags %d", opFlags);
    logd("keywords %s", keywords.toStdString().c_str());
    tracede;
    return -1;
}

QHash<int, QString> UITableView::getFilterFields()
{
    traced;
    return mFilterFields;
}

void UITableView::appendFilterField(int id, const QString &txt)
{
    traced;
    logd("Add filter field %d, %s", id, txt.toStdString().c_str());
    if (!mFilterFields.contains(id))
        mFilterFields.insert(id, txt);
    else
        loge("Filter field id %d already exist", id);
    tracede;
}

void UITableView::initFilterFields()
{
    traced;
    logi("Default init filter field, do nothing");
}

void UITableView::loadFilterFields()
{
    traced;
    initFilterFields();
    QHash<int, QString> fields = getFilterFields();
    foreach(int key, fields.keys()) {
        ui->cbCategory->addItem(fields[key], key);
    }
    tracede;
}

void UITableView::onFilterFieldChange(int fieldId, const QString &fieldText)
{
    traced;
    logd("Filter fieldId %d, text '%s'", fieldId, fieldText.toStdString().c_str());
    ui->cbKeyword->clear();
    ui->cbKeyword->clearEditText();
    QHash<QString, QString> fields = getFilterKeywords(fieldId, fieldText);
    foreach(QString key, fields.keys()) {
        ui->cbKeyword->addItem(fields[key], key);
    }
    tracede;
}

void UITableView::clearFilter()
{
    traced;
    ui->cbKeyword->clearEditText();
    tracede;
}

void UITableView::loadFilterOperators(int fieldId)
{
    traced;
    logd("fieldid %d", fieldId);
    ui->cbSearchOp->clear();
    QHash<int, QString> fields = getFilterOpsList(fieldId);
    foreach(int key, fields.keys()) {
        ui->cbSearchOp->addItem(fields[key], key);
    }
    tracede;
}

QHash<QString, QString> UITableView::getFilterKeywords(int fieldId, const QString &fieldText)
{
    traced;
    logi("Default one, nothing to do");
    return QHash<QString, QString>();
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

UITableItem *UITableItem::build(DbModel *data)
{
    return new UITableItem(data);
}

UITableItem *UITableItem::addValue(const QString &val)
{
    mValueList.append(val);
    return this;
}

UITableItem::UITableItem(DbModel* data):
    mData(data)
{
    traced;
}

DbModel *UITableItem::data() const
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

    QTableWidget *tbl = ui->tblList;
    QItemSelectionModel *select = tbl->selectionModel();
    QList<UITableItem*> selectedItem;
    if (select->hasSelection()) {
        QModelIndexList selected = select->selectedRows();
        logd("Selected %d rows", selected.count());
        QList<UITableItem*> items = getListAllItem();
        for(int i=0; i< selected.count(); i++)
        {
            QModelIndex index = selected.at(i);
            logd("Selected row idx %d", index.row());
            if (index.row() < items.count())
                selectedItem.append(items.at(index.row()));

        }
    } else {
        logd("not row selected");
    }

//    QModelIndex index = ui->tblList->indexAt(pos);
    // TODO: hande it
    int row = tbl->rowAt(pos.y());
    int col = tbl->rowAt(pos.x());
    logd("menu at row=%d col=%d", row, col);
//    cell = ui->tblList->item(row, col);
    UITableWidgetItem *item = (UITableWidgetItem*)tbl->itemAt(pos);
    QMenu* menu = buildPopupMenu(item, selectedItem);
    menu->popup(tbl->viewport()->mapToGlobal(pos));
}


void UITableView::on_btnFilter_clicked()
{
    traced;
    int categoryId = -1;
    int opFlags = -1;
    QString categoryTxt = ui->cbCategory->currentText().trimmed();
    GET_VAL_INT_FROM_CB(ui->cbCategory, categoryId);
    GET_VAL_INT_FROM_CB(ui->cbSearchOp, opFlags);
    QString keyword = ui->cbKeyword->currentText().trimmed();
    logd("call on filter id %d, opFlags %d, keyword %s", categoryId, opFlags, keyword.toStdString().c_str());
    int ret = onFilter(categoryId, categoryTxt, opFlags, keyword);
    logd("onFilter result %d", ret);
    if (ret >= 0) {
        refesh();
    }
    tracede;
}

//void UITableView::on_cbKeyword_returnPressed()
//{
//    traced;
//    on_btnFilter_clicked();
//}


UITableMenuAction::UITableMenuAction(QObject *parent):
    QAction(parent),
    mTblItem(nullptr),
    mMenuType(MENU_ACTION_NORMAL)
{

}

UITableMenuAction::UITableMenuAction(const QString &text, QObject *parent):
    QAction(text, parent),
    mTblItem(nullptr),
    mMenuType(MENU_ACTION_NORMAL)
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

UITableMenuAction *UITableMenuAction::buildMultiItem(const QString &text, QObject *parent, const QList<UITableItem *> *items, qint32 idx)
{
    traced;
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    if (items != nullptr && !items->empty()) {
        foreach (UITableItem* item, *items) {
            menu->addItemList(item);
        }
    } else {
        logd("Not UITableItem to add");
    }
    return menu;
}

UITableMenuAction *UITableMenuAction::buildSeparateAction()
{
    traced;
    UITableMenuAction* act = new UITableMenuAction();
    act->setMenuType(MENU_ACTION_SEPARATE);
    tracede;
    return act;
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

DbModel *UITableMenuAction::getData()
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

UITableMenuAction* UITableMenuAction::setItemList(const QList<UITableItem *> &newItemList)
{
    traced;
    mItemList = newItemList;
    return this;
}

UITableMenuAction *UITableMenuAction::addItemList(UITableItem *newItemList)
{
    traced;
    if (newItemList != nullptr) {
        mItemList.append(newItemList);
    }
    return this;
}

UITableMenuActionType UITableMenuAction::menuType() const
{
    return mMenuType;
}

void UITableMenuAction::setMenuType(UITableMenuActionType newMenuType)
{
    mMenuType = newMenuType;
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

DbModel *UITableWidgetItem::itemData() const
{
    traced;
    DbModel* model = nullptr;
    if (mItem != nullptr) {
        model = mItem->data();
    } else {
        logd("mItem is nullptr");
    }
    tracede;
    return model;
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

void UITableView::on_cbCategory_currentIndexChanged(int index)
{
    traced;
    int categoryId = -1;
    QString categoryTxt = ui->cbCategory->currentText().trimmed();
    GET_VAL_INT_FROM_CB(ui->cbCategory, categoryId);

    onFilterFieldChange(categoryId, categoryTxt);
    loadFilterOperators(categoryId);
    tracede;
}

