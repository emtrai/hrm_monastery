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
#include "dbmodel.h"
#include "mainwindow.h"
#include "dialogutils.h"
#include "viewutils.h"
#include "stringdefs.h"

#define ITEMS_PER_PAGE (50)

/****************************************************************************
 * UITableItem
 ****************************************************************************/

UITableItem::UITableItem(const DbModel* data):UITableItem()
{
    tracein;
    mData = CLONE_DBMODEL(data);
    traceout;
}

UITableItem::UITableItem():mData(nullptr)
{
    traced;
}

UITableItem::UITableItem(const UITableItem &item):UITableItem()
{
    tracein;
    clone(&item);
    traceout;
}

UITableItem::~UITableItem()
{
    tracein;
    FREE_PTR(mData);
    traceout;
}

UITableItem *UITableItem::build(const DbModel *data)
{
    return new UITableItem(data);
}

UITableItem *UITableItem::addValue(const QString &val)
{
    // accept null/empty value as it's empty value
    mValueList.append(val);
    return this;
}

const DbModel *UITableItem::data() const
{
    return mData;
}

UITableItem *UITableItem::clone()
{
    tracein;
    UITableItem* item = new UITableItem();
    if (item) {
        item->clone(this);
    } else {
        loge("failed to clone, no memory to allocate new UITableItem?");
    }
    traceout;
    return item;
}

void UITableItem::clone(const UITableItem *item)
{
    tracein;
    if (item) {
        FREE_PTR(mData);
        mData = CLONE_DBMODEL(item->mData);
        mValueList = item->mValueList;
    } else {
        loge("failed to clone, invalid value");
    }
    traceout;
}


const QStringList &UITableItem::valueList() const
{
    return mValueList;
}


/****************************************************************************
 * UITableCellWidgetItem
 ****************************************************************************/


UITableCellWidgetItem::UITableCellWidgetItem(const QString &text):
    QTableWidgetItem(text),
    mItem(nullptr),
    mItemIdx(0),
    mIdx(0)
{}

UITableCellWidgetItem *UITableCellWidgetItem::build(const QString &txt,
                                                    qint32 itemIdx,
                                                    qint32 idx,
                                                    UITableItem *item)
{
    UITableCellWidgetItem* wg = new UITableCellWidgetItem(txt);
    wg->setItem(item);
    wg->setItemIdx(itemIdx);
    wg->setIdx(idx);
    return wg; // TODO: when this item is deleted??? check carefuly please
}

UITableItem *UITableCellWidgetItem::item() const
{
    return mItem;
}

const DbModel *UITableCellWidgetItem::itemData() const
{
    tracein;
    const DbModel* model = nullptr;
    if (mItem != nullptr) {
        model = mItem->data();
    } else {
        logd("mItem is nullptr");
    }
    traceout;
    return model;
}

void UITableCellWidgetItem::setItem(UITableItem *newItem)
{
    mItem = newItem;
}

qint32 UITableCellWidgetItem::itemIdx() const
{
    return mItemIdx;
}

void UITableCellWidgetItem::setItemIdx(qint32 newItemIdx)
{
    mItemIdx = newItemIdx;
}

qint32 UITableCellWidgetItem::idx() const
{
    return mIdx;
}

void UITableCellWidgetItem::setIdx(qint32 newIdx)
{
    mIdx = newIdx;
}


/****************************************************************************
 * UITableMenuAction
 ****************************************************************************/

UITableMenuAction::UITableMenuAction(QObject *parent):
    QAction(parent),
    mTblCellItem(nullptr),
    mMenuType(MENU_ACTION_NORMAL),
    isMultiSelectedItem(false)
{

}

UITableMenuAction::~UITableMenuAction()
{
    tracein;
    RELEASE_LIST(mItemList, UITableItem);
    traceout;
}

UITableMenuAction::UITableMenuAction(const QString &text, QObject *parent):
    QAction(text, parent),
    mTblCellItem(nullptr),
    mMenuType(MENU_ACTION_NORMAL)
{
    tracein;
}

UITableCellWidgetItem *UITableMenuAction::tblItem() const
{
    return mTblCellItem;
}

UITableMenuAction* UITableMenuAction::setTblCellItem(UITableCellWidgetItem *newTblItem)
{
    mTblCellItem = newTblItem;
    return this;
}

UITableMenuAction *UITableMenuAction::build(const QString &text, QObject *parent,
                                            UITableCellWidgetItem *item, qint32 idx)
{
    tracein;
    UNUSED(idx);
    // TODO: should has ID for menu/action? so that other can judge state of action/menu, i.e. should in disable state or not
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    if (menu) {
        if (item) {
            menu->addItemList(item->item());
            menu->setTblCellItem(item);
        } else {
            logd("item is null");
        }
        menu->setIsMultiSelectedItem(false);
    } else {
        loge("No memory to allocate UITableMenuAction");
    }
    traceout;
    return menu;
}

UITableMenuAction *UITableMenuAction::buildMultiItem(const QString &text, QObject *parent,
                                                     const QList<UITableItem *> *items, qint32 idx)
{
    tracein;
    UNUSED(idx);
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    if (menu) {
        if (items != nullptr && !items->empty()) {
            foreach (UITableItem* item, *items) {
                menu->addItemList(item);
            }
        } else {
            logd("Not UITableItem to add");
        }
        menu->setIsMultiSelectedItem(true);
    } else {
        loge("no memory to build UITableMenuAction");
    }
    traceout;
    return menu;
}

UITableMenuAction *UITableMenuAction::buildSeparateAction()
{
    tracein;
    UITableMenuAction* act = new UITableMenuAction();
    if (act) {
        act->setMenuType(MENU_ACTION_SEPARATE);
    } else {
        loge("no memory to allocate UITableMenuAction");
    }
    traceout;
    return act;
}

const QList<UITableItem *> &UITableMenuAction::itemList() const
{
    return mItemList;
}

/**
 * @brief Get item data (model).
 *        NOTE: Data is cloned, so caller MUST FREE after use (i.e. use RELEASE_LIST_DBMODEL)
 * @param outList
 * @return the number of item
 */
int UITableMenuAction::itemListData(QList<DbModel *>&outList)
{
    tracein;
    if (mItemList.count() > 0) {
        logd("selected %lld items", mItemList.count());
        foreach (UITableItem* item, mItemList) {
            if (item->data() != nullptr) {
                DbModel* model = CLONE_DBMODEL(item->data());
                if (model) {
                    outList.append(model);
                } else {
                    logw("failed to clone data, no memory??");
                }
            }
        }
    } else {
        logi("no selected item for menu");
    }
    traceret(outList.count());
    return outList.count();
}

bool UITableMenuAction::getIsMultiSelectedItem() const
{
    return isMultiSelectedItem;
}

void UITableMenuAction::setIsMultiSelectedItem(bool newIsMultiSelectedItem)
{
    isMultiSelectedItem = newIsMultiSelectedItem;
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

const DbModel *UITableMenuAction::getData()
{
    const DbModel* ret = nullptr;
    if (mTblCellItem != nullptr) {
        UITableItem* item = mTblCellItem->item();
        if (item != nullptr) {
            ret = item->data();
        } else {
            loge("invalid UITableItem, null?");
        }
    } else {
        loge("invaid mTblCellItem item");
    }
    return ret;
}

UITableMenuAction *UITableMenuAction::addItemList(UITableItem *newItemList)
{
    tracein;
    if (newItemList != nullptr) {
        mItemList.insert(mItemList.size(), newItemList->clone());
    }
    traceout;
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

/****************************************************************************
 * UITableView
 ****************************************************************************/


UITableView::UITableView(QWidget *parent) :
    QFrame(parent),
    BaseView(),
    ui(new Ui::UITableView),
    mItemPerPage(ITEMS_PER_PAGE),
    mMenu(nullptr),
    mSuspendReloadOnDbUpdate(false)
{
    ui->setupUi(this);
    // TODO: temporary disable/hide toolbox, till we have time to do
    ui->frameToolbox->setVisible(false);
    ui->tblList->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tblList->verticalHeader()->setVisible(false);
    ui->tblList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tblList->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tblList, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint)));
    if (!connect(this,
                 &UITableView::signalDeleteDone,
                 this, &UITableView::onHandleSignalDeleteDone,
                 Qt::QueuedConnection)) {
        loge("Failed to connect signalDeleteDone to onHandleSignalDeleteDone");
    }

    if (!connect(this,
                 &UITableView::signalRequestReload,
                 this,
                 &UITableView::onRequestReload, Qt::QueuedConnection)) {
        loge("Failed to connect signalRequestReload to onRequestReload");
    }
}

UITableView::~UITableView()
{
    tracein;
    // TODO: action may be allocated, need to check again if action is really clear???
    // see: https://doc.qt.io/qt-6/qmenu.html#clear
    cleanupMenuActions();
    if (mMenu) mMenu->clear();
    FREE_PTR(mMenu);
    RELEASE_LIST(mFilterList, FilterItem);
    cleanupTableItems();
    delete ui;
    traceout;
}

QStringList UITableView::getHeader()
{
    tracein;
    return mHeader;
}

void UITableView::setupUI()
{
    tracein;
    BaseView::setupUI();
    if (mHeader.empty())
        initHeader();
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(getHeader().count());
    ui->tblList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500);

    ui->tblList->setHorizontalHeaderLabels(getHeader());
    ui->tblList->verticalHeader()->setVisible(true);

    setTitle(getTitle());

    if (hasFilters()) {
        logd("has filters, load filters");
        loadFilterFields();
        int key = ui->cbCategory->currentData().toInt();
        loadFilterOperators(key);
    } else {
        ui->wgFilter->setVisible(false);
        logd("no filter for listview");
    }
    traceout;
}

void UITableView::sort(int column)
{
    logd("sort column %d", column);
    ui->tblList->sortByColumn(column, Qt::AscendingOrder);
}

void UITableView::reload()
{
    tracein;
    if (!mSuspendReloadOnDbUpdate) {
        onReload();
        onUpdatePage(1);
    } else {
        logi("reload is temporary suspended");
    }
    traceout;
}

void UITableView::refesh()
{
    tracein;
    onUpdatePage(1);
    traceout;
}

void UITableView::showEvent(QShowEvent *ev)
{
    tracein;
    QFrame::showEvent(ev);

    onLoad();

    onUpdatePage(1);
    traceout;
}

ErrCode UITableView::onUpdatePage(qint32 page, bool updatePage)
{
    QTableWidget* tbl = ui->tblList;
    qint32 totalPages = 0;
    ErrCode err = ErrNone;
    tracein;

    // TODO: is it really remove all data?
    // Is there any risk of leakage memory here??
    logd("Clear all");
    cleanupTableItems();

    qint32 total = getTotalModelItems();
    dbgd("update page %d, total item %d", page, total);
    if (total > 0){
        // TODO: remove paging, as it not need????
        qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
        totalPages = total/perPage + ((total%perPage == 0)?0:1);
        logi("total %d perpage %d totaPage %d", total, perPage, totalPages);
        err = getListTableRowItems(page, perPage, totalPages, mItemList);
        if (err == ErrNone) {
            int idx = tbl->rowCount();
            foreach (UITableItem* item, mItemList){
                if (!item) continue;
                tbl->insertRow(idx);
                QStringList values = item->valueList();
                logd("add item '%s'", MODELSTR2CHA(item->data()));
                for (int i = 0; i < values.count(); ++i) {
                    UITableCellWidgetItem* cell =
                        UITableCellWidgetItem::build(values.value(i), i, idx, item);
                    if (!cell) {
                        loge("No memory to allocate cell");
                        err = ErrNoMemory;
                        break;
                    }
                    tbl->setItem(idx, i, cell);
                }
                if (err != ErrNone) {
                    break;
                }
                idx ++;
            }
        } else {
            loge("failed to get list of table row items, err %d", err);
            RELEASE_LIST(mItemList, UITableItem);
        }

        if (err == ErrNone && updatePage) {
            updatePageInfo(page, total);
        }
    } else {
        updatePageInfo(0);
    }
    onUpdatePageDone(err, total, totalPages, total);
    traceout;
    return err;
}

void UITableView::onUpdatePageDone(ErrCode err, qint32 page, qint32 totalpages, qint32 totalItems)
{
    tracein;
    UNUSED(err);
    UNUSED(page);
    UNUSED(totalpages);
    UNUSED(totalItems);
    logd("page %d, totalPages %d, totalItems %d", page, totalpages, totalItems);
    traceout;
}

QList<UITableItem *> UITableView::getListAllTableRowItems()
{
    return mItemList;
}

qint32 UITableView::getTotalModelItems()
{
    return 0;
}

ErrCode UITableView::onLoad()
{
    tracein;
    return ErrNone;
}

ErrCode UITableView::onReload()
{
    tracein;
    return onLoad();
}

void UITableView::importRequested(const QString& fpath)
{
    UNUSED(fpath);
}

ErrCode UITableView::onViewItem(UITableCellWidgetItem *item)
{
    UNUSED(item);
    logd("parent class, nothing to do");
    return ErrNoData;
}

ErrCode UITableView::onEditItem(UITableCellWidgetItem *item)
{
    UNUSED(item);
    logd("parent class, nothing to do");
    return ErrNone;

}

ErrCode UITableView::onDeleteItem(const QList<UITableItem *>& selectedItems)
{
    tracein;
    ErrCode err = ErrNone;
    mSuspendReloadOnDbUpdate = true;
    if (selectedItems.size() > 0) {
        bool accept = DialogUtils::showConfirmDialog(this,
                             tr("Xoá"), QString(tr("Bạn có muốn xóa '%1' mục trong '%2'? (Tất cả dữ liệu liên quan cũng sẽ bị xóa)")
                                                           .arg(selectedItems.size())
                                                           .arg(getTitle())
                                                       ), nullptr);
        if (accept) {
            QString msg;
            int cnt = 0;
            err = MainWindow::showProcessingDialog(tr("Lưu dữ liệu"), nullptr,
               [ &cnt, &msg, selectedItems](ErrCode* err, void* data, DlgWait* dlg) {
                    tracein;
                    int total = selectedItems.size();
                    foreach (UITableItem* item, selectedItems) {
                        if (item && item->data()) {
                            DbModel* model = CLONE_DBMODEL(item->data());
                            if (model) {
                                *err = model->remove(true, &msg);
                                if (*err == ErrNone) {
                                    cnt++;
                                } else {
                                    loge("Delete '%s' err = %d", STR2CHA(model->toString()), *err);
                                    break;
                                }
                                if (cnt % 4 == 0) {
                                    dlg->setMessage(QString(tr("Đã xóa %1 / %2"))
                                                        .arg(QString::number(cnt), QString::number(total)));
                                }
                                FREE_PTR(model);
                            } else {
                                loge("failed to delete '%s', no memory?",
                                     MODELSTR2CHA(item->data()));
                            }
                        }
                    }
                    traceout;
                   return nullptr;
               },
                [this, &cnt](ErrCode err, void* data, void* result, DlgWait* dlg) {
                    traced;
                    logd("delete result %d", err);
                    QString errMsg;
                    if (err != ErrNone) {
                        loge("failed to delete item, err=%d", err);
                        errMsg = QString(tr("Lỗi, mã lỗi: %1")).arg(err);
                    } else {
                        logi("Deleted %d item", cnt);
                        errMsg = QString(tr("Đã xóa '%1' mục")).arg(cnt);
                    }

                    emit this->signalDeleteDone(err, errMsg);
                    return err;
                });
        }
    }

    traceout;
    return err;
}

ErrCode UITableView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    logd("parent class, nothing to do");
    return ErrNone;
}

ErrCode UITableView::addMenuActionCallback(QList<UITableMenuAction *> &actionList)
{
    logd("Add callback for menu action, no. item %lld", actionList.size());
    foreach (UITableMenuAction* act, actionList) {
        if (act && act->menuType() != MENU_ACTION_SEPARATE) {
            logd("connect for act '%s'", STR2CHA(act->text()));
//            disconnect(SIGNAL(QAction::triggered(bool)), this);
            connect(act, &QAction::triggered, this, [this, act](){
                QAction *sentAct = qobject_cast<QAction *>(sender());
                logd("lambda trigger call for menu sentAct '%s'", STR2CHA(sentAct->text()));
                logd("lambda trigger call for menu '%s'", STR2CHA(act->text()));
                act->callback()(this->menu(), act);
                // TODO: handle return???
            });
        } else {
            logd("action menuType separation, skip it");
        }
    }
    return ErrNone;
}


QMenu* UITableView::buildPopupMenu(UITableCellWidgetItem* wgitem, const QList<UITableItem*>& items)
{
    tracein;
    if (mMenu == nullptr) {
        logd("build menu actions");
        mMenu = new QMenu(this);
    }
    mMenu->clear(); // just clear menu, not actions, as they're reused
    cleanupMenuActions();
    // TODO: cache menu action?
    // a little bit stupid that menu item store information of model in menu action
    // so keep/caching menu action causes data is incorrect
    // so we need to re-create menu action with new items
    // stupid, how can we cache????
    if (mCommonMenuActionList.size() == 0) {
        mCommonMenuActionList = getMenuCommonActions(mMenu);
        addMenuActionCallback(mCommonMenuActionList);
    }
    mMenuActions.append(mCommonMenuActionList);
    if (items.count() > 1) { // more than one items are selected
        if (mMultiSelectedMenuActionList.size() == 0) {
            mMultiSelectedMenuActionList.append(UITableMenuAction::buildSeparateAction());
            mMultiSelectedMenuActionList = getMenuMultiSelectedItemActions(mMenu, items);
            addMenuActionCallback(mMultiSelectedMenuActionList);
        }
        mMenuActions.append(mMultiSelectedMenuActionList);
    } else {
        if (wgitem != nullptr){
            if (mSingleSelectedMenuActionList.size() == 0) {
                mSingleSelectedMenuActionList.append(UITableMenuAction::buildSeparateAction());
                mSingleSelectedMenuActionList = getMenuSingleSelectedItemActions(mMenu, wgitem);
                addMenuActionCallback(mSingleSelectedMenuActionList);
                // add one more callback/lister to call to derived class, if item action should be in disable state or not???
            }
            mMenuActions.append(mSingleSelectedMenuActionList);
        }
    }
    foreach (UITableMenuAction* act, mMenuActions) {
        if (act->menuType() == MENU_ACTION_SEPARATE) {
            mMenu->addSeparator();
        } else {
            mMenu->addAction(act);
        }
    }

    return mMenu;
}

/**
 * @brief Common menu action
 * @param menu
 * @return
 */
QList<UITableMenuAction *> UITableView::getMenuCommonActions(const QMenu* menu)
{
    tracein;
    ErrCode err = ErrNone;
    QList<UITableMenuAction*> actionList;
    if (!menu) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    if (err == ErrNone) {
        actionList.append(UITableMenuAction::build(tr("Thêm"), this)
                            ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                    return this->onMenuActionAdd(m, a);
                              }));

        actionList.append(UITableMenuAction::build(tr("Cập nhật thông tin (refresh)"), this)
                                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                                       return this->onMenuActionReload(m, a);
                                                                   }));
        actionList.append(BUILD_MENU_SEPARATE);
    }
    return actionList;
}

QList<UITableMenuAction *> UITableView::getMenuSingleSelectedItemActions(const QMenu* menu,
                                                           UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList;

    // TODO: some item not allow some action, show consider this?
    // i.e. add listener/callback to judge menu item exist, but in disable state
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

QList<UITableMenuAction *> UITableView::getMenuMultiSelectedItemActions(const QMenu *menu, const QList<UITableItem *>& items)
{
    tracein;
    QList<UITableMenuAction*> actionList;
    actionList.append(UITableMenuAction::buildMultiItem(tr("Xoá"), this, &items)
                                           ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                               return this->onMenuActionDelete(m, a);
                                           }));
    traceout;
    return actionList;

}


ErrCode UITableView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    if (!act || !menu) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = onAddItem((UITableCellWidgetItem*)act->tblItem());
    }
    traceret(err);
    return err;
}

ErrCode UITableView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;

    if (!act) {
        loge("No menu action for delete!");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && act->itemList().size() == 0) {
        loge("No cell item data to delete");
        err = ErrInvalidData;
    }

    if (err == ErrNone) {
        err = onDeleteItem(act->itemList());
    }

    if (err != ErrNone) {
        loge("delete model err %d", err);
    }
    traceret(err);
    return err;
}


ErrCode UITableView::onMenuActionEdit(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(menu);
    if (act) {
        err = onEditItem((UITableCellWidgetItem*)act->tblItem());
    } else {
        loge("Menu action is null!!!");
        err = ErrInvalidData;
    }
    if (err != ErrNone) {
        DialogUtils::showErrorBox(err, STR_ERR_EDIT_DATA);
    }
    // TODO: report error, show error dialog???
    return err;
}

ErrCode UITableView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    if (!act) {
        loge("Menu action is null!!!");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && !act->tblItem()) {
        loge("No cell item data");
        err = ErrInvalidData;
    }

    if (err == ErrNone) {
        err = onViewItem((UITableCellWidgetItem*)act->tblItem());
    }

    if (err != ErrNone) {
        loge("view err %d", err);
        DialogUtils::showErrorBox(err, tr("Không hiển thị được thông tin"));
    }
    traceout;
    return err;

}

ErrCode UITableView::onMenuActionReload(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    requestReload();
    traceout;
    return ErrNone;
}

int UITableView::onFilter(int catetoryid, const QString &catetory, qint64 opFlags, const QString &keywords, const QVariant *value)
{
    tracein;
    logi("DEFAULT filter, should not called here, DERIVED CLASS implement this");
    logd("category id %d", catetoryid);
    logd("category '%s'", catetory.toStdString().c_str());
    logd("opFlags %d", opFlags);
    logd("keywords %s", keywords.toStdString().c_str());
    traceout;
    return -1;
}

int UITableView::onFilter(const QList<FilterKeyworkItem *> &filters)
{
    tracein;
    logi("DEFAULT filter, should not called here, DERIVED CLASS implement this");
    traceout;
    return -1;
}

ErrCode UITableView::setFilter(const QString &item, const QString &keywords, const QVariant *value)
{
    tracein;
    ErrCode err = ErrNotSupport;
    loge("WARNING, filter not supported default one, MUST BE CALLED BY DERIVED CLASS");
    // TODO: should popup, crash app, to notice developer????
    // just print log silently here may be forgotten
    traceout;
    return err;
}

QHash<int, QString> UITableView::getFilterFields()
{
    tracein;
    return mFilterFields;
}

void UITableView::appendFilterField(int id, const QString &txt)
{
    tracein;
    logd("Add filter field %d, %s", id, txt.toStdString().c_str());
    if (!mFilterFields.contains(id))
        mFilterFields.insert(id, txt);
    else
        loge("Filter field id %d already exist", id);
    traceout;
}

void UITableView::initFilterFields()
{
    logi("Default init filter field, do nothing");
}

void UITableView::loadFilterFields()
{
    tracein;
    initFilterFields();
    QHash<int, QString> fields = getFilterFields();
    foreach(int key, fields.keys()) {
        ui->cbCategory->addItem(fields[key], key);
    }
    traceout;
}

void UITableView::reloadFilterFields()
{
    tracein;
    ui->cbCategory->clear();
    mFilterFields.clear();
    loadFilterFields();
    traceout;
}

void UITableView::onFilterFieldChange(int fieldId, const QString &fieldText)
{
    tracein;
    logd("Filter fieldId %d, text '%s'", fieldId, fieldText.toStdString().c_str());
    ui->cbKeyword->clear();
    ui->cbKeyword->clearEditText();
    QHash<QString, QString> fields = getFilterKeywords(fieldId, fieldText);
    foreach(QString key, fields.keys()) {
        ui->cbKeyword->addItem(fields[key], key);
    }
    traceout;
}

void UITableView::clearFilter()
{
    tracein;
    ui->cbKeyword->clearEditText();
    RELEASE_LIST(mFilterList, FilterItem);
//    mFilterList.clear(); // TODO: if clear here, so, how "back" function work????
    traceout;
}

void UITableView::loadFilterOperators(int fieldId)
{
    tracein;
    logd("fieldid %d", fieldId);
    ui->cbSearchOp->clear();
    QHash<int, QString> fields = getFilterOpsList(fieldId);
    foreach(int key, fields.keys()) {
        ui->cbSearchOp->addItem(fields[key], key);
    }
    traceout;
}

QHash<QString, QString> UITableView::getFilterKeywords(int fieldId, const QString &fieldText)
{
    tracein;
    logi("Default one, nothing to do");
    return QHash<QString, QString>();
}

void UITableView::updatePageInfo(qint32 page, qint32 total)
{
    tracein;
    ViewUtils::updatePageInfo(ui->cbPage, ui->lblPage, page, total, mItemPerPage);
    traceout;
}


ErrCode UITableView::doFilter(int field, int op, const QVariant& keyword)
{
    tracein;
    ErrCode err = ErrNone;
    logd("do filter field '%d', op '%d'", field, op);
    logd("do filter keyword '%s'", STR2CHA(keyword.toString()));
    Utils::setSelectItemComboxByData(ui->cbCategory, field);
    Utils::setSelectItemComboxByData(ui->cbSearchOp, op);
    Utils::setSelectItemComboxByData(ui->cbKeyword, keyword);
    on_btnFilter_clicked();
    traceret(err);
    return err;
}

void UITableView::requestReload()
{
    tracein;
    logd("emilt signalRequestReload");
    emit signalRequestReload();
    traceout;
}

void UITableView::cleanupTableItems()
{
    tracein;

    QTableWidget* tbl = ui->tblList;
    int rowCnt = tbl->rowCount();
    int colCnt = tbl->columnCount();
    logd("rowCnt %d colCnt %d", rowCnt, colCnt);
    for (int i = 0; i < rowCnt; i++) {
        for (int j = 0; j < colCnt; j++) {
            QTableWidgetItem* item =tbl->takeItem(i, j);
            if (item) {
                delete item;
            }
        }
    }
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    RELEASE_LIST(mItemList, UITableItem);
    traceout;
}

void UITableView::cleanupMenuActions()
{
    tracein;
    RELEASE_LIST(mCommonMenuActionList, UITableMenuAction);
    RELEASE_LIST(mSingleSelectedMenuActionList, UITableMenuAction);
    RELEASE_LIST(mMultiSelectedMenuActionList, UITableMenuAction);
    mMenuActions.clear();
    traceout;
}

void UITableView::onHandleSignalDeleteDone(ErrCode err, QString msg)
{
    tracein;
    mSuspendReloadOnDbUpdate = false;
    if (err != ErrNone) {
        loge("failed to delete item, err=%d", err);
        DialogUtils::showErrorBox(err, QString(tr("Lỗi xóa dữ liệu. %1")).arg(msg));
    } else {
        DialogUtils::showMsgBox(msg);
        requestReload();
    }
    traceout;
}

ErrCode UITableView::addFilter(const QString &filterItem, const QString &keyword, const QVariant &value)
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: check duplicate???
    FilterItem* item = nullptr;
    logd("Add filter for item %s", STR2CHA(filterItem));
    if (!filterItem.isEmpty()) {
        logd("keyword %s", STR2CHA(keyword));
        item = new FilterItem(filterItem, keyword, value);
        // TODO: check keyword or value exist???

        if (item) {
            mFilterList.append(item);
        } else {
            err = ErrNoMemory;
            loge("no memory for new filter");
        }
    } else {
        err = ErrInvalidArg;
        loge("no data/info for filter");
    }
    traceret(err);
    return err;
}

QWidget *UITableView::getWidget()
{
    return this;
}

bool UITableView::hasFilters()
{
    return true;
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
    tracein;
}

void UITableView::setHeader(const QStringList &newHeader)
{
    mHeader = newHeader;
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
    tracein;
    ui->lblTitle->setText(title);
    traceout;
}

QString UITableView::getTitle()
{
    return tr("Danh sách");
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
    tracein;
    ErrCode err = onViewItem((UITableCellWidgetItem*)item);

    if (err != ErrNone) {
        loge("double click to view err %d", err);
        DialogUtils::showErrorBox(err, tr("Không hiển thị được thông tin"));
    }
    traceout;
}



void UITableView::customMenuRequested(QPoint pos)
{
    tracein;

    QTableWidget *tbl = ui->tblList;
    QItemSelectionModel *select = tbl->selectionModel();
    QList<UITableItem*> selectedItem;
    if (select && select->hasSelection()) {
        QModelIndexList selected = select->selectedRows();
        logd("Selected %d rows", selected.count());
        QList<UITableItem*> items = getListAllTableRowItems();
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
    UITableCellWidgetItem *item = (UITableCellWidgetItem*)tbl->itemAt(pos);
    QMenu* menu = buildPopupMenu(item, selectedItem);
    if (menu) {
        menu->popup(tbl->viewport()->mapToGlobal(pos));
    }
}


void UITableView::on_btnFilter_clicked()
{
    tracein;
    int categoryId = -1;
    int opFlags = -1;
    bool isOk = false;
    QString categoryTxt = ui->cbCategory->currentText().trimmed();
    GET_VAL_INT_FROM_CB(ui->cbCategory, categoryId);
    GET_VAL_INT_FROM_CB(ui->cbSearchOp, opFlags);
    QString keyword = ui->cbKeyword->currentText().trimmed();
    QString value = Utils::getCurrentComboxDataString(ui->cbKeyword, &isOk);
    QVariant val;
    if (isOk) val.setValue(value);
    // TODO: val is local variable, is it safe? in case onFilter is called on thread context
    logd("call on filter id %d, opFlags %d, keyword %s", categoryId, opFlags, keyword.toStdString().c_str());
    int ret = onFilter(categoryId, categoryTxt, opFlags, keyword, isOk?&val:nullptr);
    logd("onFilter result %d", ret);
    if (ret >= 0) {
        refesh();
    }
    traceout;
}

//void UITableView::on_cbKeyword_returnPressed()
//{
//    tracein;
//    on_btnFilter_clicked();
//}



void UITableView::on_btnAdd_clicked()
{
    tracein;
    logi("Add from parent, do nothing");
}

QMenu *UITableView::menu() const
{
    return mMenu;
}

void UITableView::on_cbCategory_currentIndexChanged(int index)
{
    tracein;
    int categoryId = -1;
    QString categoryTxt = ui->cbCategory->currentText().trimmed();
    GET_VAL_INT_FROM_CB(ui->cbCategory, categoryId);

    onFilterFieldChange(categoryId, categoryTxt);
    loadFilterOperators(categoryId);
    traceout;
}

void UITableView::onRequestReload()
{
    tracein;
    reload();
    traceout;
}


void UITableView::on_cbPage_currentIndexChanged(int index)
{
    tracein;
    int page = 0;
    logd("idx %d", index);
    QVariant value = ui->cbPage->itemData(index);
    if (value.isValid()) {
        bool ok = false;
        page = value.toInt(&ok);
        if (!ok) {
            loge("invalid value");
            page = 0;
        }
    }
    if (page > 0) {
        logd("update page %d", page);
        onUpdatePage(page);
    }
    traceout;
}

