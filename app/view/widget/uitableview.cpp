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

UITableView::UITableView(QWidget *parent) :
    QFrame(parent),
    BaseView(),
    ui(new Ui::UITableView),
    mItemPerPage(0),
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
                 SIGNAL(signalDeleteDone(ErrCode,QString)),
                 SLOT(onHandleSignalDeleteDone(ErrCode,QString)))) {
        loge("Failed to connect signalDeleteDone to onHandleSignalDeleteDone");
    }

    if (!connect(this,
                 SIGNAL(signalRequestReload()),
                 SLOT(onRequestReload()))) {
        loge("Failed to connect signalRequestReload to onRequestReload");
    }
//    QObject::connect(ui->cbKeyword, SIGNAL(returnPressed()), this, SLOT(on_cbKeyword_returnPressed()));
}

UITableView::~UITableView()
{
    tracein;
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

void UITableView::onUpdatePage(qint32 page)
{
    QTableWidget* tbl = ui->tblList;
    qint32 totalPages = 0;
    ErrCode err = ErrNone;
    tracein;

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
        totalPages = total/perPage;
        logd("total %d perpage %d totaPage %d", total, perPage, totalPages);
        QList<UITableItem*> items;
        err = getListTableRowItems(page, perPage, totalPages, items);
        int idx = tbl->rowCount();
        foreach (UITableItem* item, items){
            tbl->insertRow(idx);
            QStringList values = item->valueList();
            for (int i = 0; i < values.count(); ++i) {
                tbl->setItem(idx, i, UITableCellWidgetItem::build(values.value(i), i, idx, item));

            }

            idx ++;
        }
    }
    onUpdatePageDone(total, totalPages, total);
    traceout;
}

void UITableView::onUpdatePageDone(qint32 page, qint32 totalpages, qint32 totalItems)
{
    tracein;
    logd("page %d, totalPages %d, totalItems %d", page, totalpages, totalItems);
    traceout;
}

QList<UITableItem *> UITableView::getListAllTableRowItems()
{
    tracein;
    QList<UITableItem*> items;
    ErrCode err = ErrNone;
    // TODO: fix me please!!!
    qint32 total = getTotalItems();
    logd("total item %d", total);
    if (total > 0){
        // TODO: remove paging, as it not need????
        qint32 perPage = ((mItemPerPage != 0) && (mItemPerPage < total))?mItemPerPage:total;
        qint32 totalPages = total/perPage;
        logd("total %d perpage %d totaPage %d", total, perPage, totalPages);

        err = getListTableRowItems(0, perPage, totalPages, items);
    } else {
        logd("Not item to load");
    }
    tracein;
    return items;
}

qint32 UITableView::getTotalItems()
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
    tracein;
}

ErrCode UITableView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    logd("parent class, nothing to do");
    traceout;
    return ErrNoData;
}

ErrCode UITableView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("parent class, nothing to do");
    traceret(err);
    return err;

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
                            DbModel* model = item->data();
//                            dlg->setMessage(QString(tr("Xóa %1")).arg(model->name()));
                            *err = model->remove(true, &msg);
                            if (*err == ErrNone) {
                                cnt++;
                            } else {
                                loge("Delete '%s' err = %d", STR2CHA(model->toString()), err);
                                break;
                            }
                            if (cnt % 4 == 0) {
                                dlg->setMessage(QString(tr("Đã xóa %1 / %2")).arg(cnt, total));
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
//            logd("delete result %d", err);
//            if (err != ErrNone) {
//                loge("failed to delete item, err=%d", err);
////                DialogUtils::showErrorBox(QString(tr("Lỗi, mã lỗi: %1")).arg(err));
//            } else {
//                logi("Deleted %d item", cnt);
//                DialogUtils::showMsgBox(QString(tr("Đã xóa '%1' mục")).arg(cnt));
//                mSuspendReloadOnDbUpdate = false;
//                reload();
//            }
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


QMenu* UITableView::buildPopupMenu(UITableCellWidgetItem* wgitem, const QList<UITableItem*>& items)
{
    tracein;
    if (mMenu == nullptr) {
        logd("build menu actions");
        mMenu = new QMenu(this);
    }
    mMenu->clear();

    QList<UITableMenuAction*> actions = getMenuCommonActions(mMenu);
    // TODO: cache actions for later use, to improve performance

    if (items.count() > 1){
        actions.append(UITableMenuAction::buildSeparateAction());
        QList<UITableMenuAction*> itemActions = getMenuMultiSelectedItemActions(mMenu, items);
        if (!itemActions.empty()) {
            actions.append(itemActions);
        }
    } else {
        if (wgitem != nullptr){

            actions.append(UITableMenuAction::buildSeparateAction());
            QList<UITableMenuAction*> itemActions = getMenuSingleSelectedItemActions(mMenu, wgitem);
            // add one more callback/lister to call to derived class, if item action should be in disable state or not???
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
//        DialogUtils::showErrorBox(err, tr("Lỗi xóa dữ liệu"));
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
    reload();
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

void UITableView::onHandleSignalDeleteDone(ErrCode err, QString msg)
{
    if (err != ErrNone) {
        loge("failed to delete item, err=%d", err);
        DialogUtils::showErrorBox(QString(tr("Lỗi, mã lỗi: %1")).arg(err));
    } else {
        DialogUtils::showMsgBox(msg);
        mSuspendReloadOnDbUpdate = false;
        reload();
    }
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

UITableItem::~UITableItem()
{
    tracein;
    if (mData) delete mData;
    traceout;
}

UITableItem *UITableItem::build(const DbModel *data)
{
    return new UITableItem(data->clone());
}

UITableItem *UITableItem::addValue(const QString &val)
{
    mValueList.append(val);
    return this;
}

UITableItem::UITableItem(DbModel* data):
    mData(data)
{
    tracein;
}

DbModel *UITableItem::data() const
{
    return mData;
}

UITableItem *UITableItem::clone()
{
    tracein;
    UITableItem* item = new UITableItem();
    if (item) {
        item->clone(this);
    }
    traceout;
    return item;
}

void UITableItem::clone(const UITableItem *item)
{
    tracein;
    if (item) {
        if (mData) {
            delete mData;
            mData = nullptr;
        }
        mData = item->mData->clone();
        mValueList = item->mValueList;
    }
    traceout;
}

UITableItem::UITableItem():mData(nullptr)
{
    tracein;
}

UITableItem::UITableItem(const UITableItem &item):UITableItem()
{
    tracein;
    mData = item.mData->clone();
    mValueList = item.mValueList;
    traceout;
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
    if (select->hasSelection()) {
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
    menu->popup(tbl->viewport()->mapToGlobal(pos));
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
    // TODO: should has ID for menu/action? so that other can judge state of action/menu, i.e. should in disable state or not
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    if (item)
        menu->addItemList(item->item());
    else
        logd("item is null");
    menu->setTblCellItem(item);
    menu->setIsMultiSelectedItem(false);
    traceout;
    return menu;
}

UITableMenuAction *UITableMenuAction::buildMultiItem(const QString &text, QObject *parent,
                                                     const QList<UITableItem *> *items, qint32 idx)
{
    tracein;
    UITableMenuAction* menu = new UITableMenuAction(text, parent);
    if (items != nullptr && !items->empty()) {
        foreach (UITableItem* item, *items) {
            menu->addItemList(item);
        }
    } else {
        logd("Not UITableItem to add");
    }
    menu->setIsMultiSelectedItem(true);
    traceout;
    return menu;
}

UITableMenuAction *UITableMenuAction::buildSeparateAction()
{
    tracein;
    UITableMenuAction* act = new UITableMenuAction();
    act->setMenuType(MENU_ACTION_SEPARATE);
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
                outList.append(item->data()->clone());
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

DbModel *UITableMenuAction::getData()
{
    tracein;
    if (mTblCellItem != nullptr) {
        UITableItem* item = mTblCellItem->item();
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

UITableMenuAction *UITableMenuAction::addItemList(UITableItem *newItemList)
{
    tracein;
    if (newItemList != nullptr) {
        mItemList.append(newItemList->clone());
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

void UITableView::on_btnAdd_clicked()
{
    tracein;
    logi("Add from parent, do nothing");
}

QMenu *UITableView::menu() const
{
    return mMenu;
}


UITableCellWidgetItem::UITableCellWidgetItem(const QString &text):
    QTableWidgetItem(text),
    mItem(nullptr),
    mItemIdx(0),
    mIdx(0)
{
//    tracein;
}

UITableCellWidgetItem *UITableCellWidgetItem::build(const QString &txt, qint32 itemIdx, qint32 idx, UITableItem *item)
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

DbModel *UITableCellWidgetItem::itemData() const
{
    tracein;
    DbModel* model = nullptr;
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

