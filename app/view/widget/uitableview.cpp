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

}

void UITableView::reload()
{
    traced;
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
                QTableWidgetItem* wgitem = new QTableWidgetItem(values.value(i));
                wgitem->setData(Qt::UserRole, idx);
                tbl->setItem(idx, i, wgitem);
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

void UITableView::onViewItem(qint32 idx)
{
    traced;
    logd("parent class, nothing to do");
}

QMenu* UITableView::buildPopupMenu()
{
    traced;
    if (mMenu == nullptr) {
        logd("build menu actions");
        mMenu = new QMenu(this);
        QList<QAction*> actions = getMenuActions();
        foreach (QAction* act, actions) {
            connect(act, &QAction::triggered, [this, act](){
                        logd("lambda trigger call");
                        onMenuActionTrigger(this->mMenu, act);
                        // TODO: handle return???
                    });
            mMenu->addAction(act);
        }
    }
    return mMenu;
}

QList<QAction *> UITableView::getMenuActions()
{
    traced;
    QList<QAction*> actionList;
    QAction* action = new QAction("New", this);
    action->setData(MenuAction::ACTION_NEW);
    actionList.append(action);

    action = new QAction("Delete", this);
    action->setData(MenuAction::ACTION_DELETE);
    actionList.append(action);

    return actionList;
}

ErrCode UITableView::onMenuActionTrigger(QMenu* menu, QAction *action)
{
    traced;
    if (action == nullptr)
        return ErrInvalidArg;
    bool isOk = false;
    ErrCode err = ErrNone;
    logd("action %s", action->text().toStdString().c_str());
    MenuAction act = static_cast<MenuAction>(action->data().toInt(&isOk));
    if (isOk) {
        logd("action id %d", act);
        // TODO: what should we do now??
        switch (act) {
        case MenuAction::ACTION_NEW:
            err = onMenuAddAction(menu, action);
            break;
        case MenuAction::ACTION_DELETE:
            err = onMenuDeleteAction(menu, action);
            break;
        default:
            loge("Unknow action id %d", act);
            err = ErrNotSupport;
            break;
        }
    } else {
        err = ErrUnknown;
        logd("not defined menu action, what should we do now???");
        // TODO: what should we do now??
    }
    tracedr(err);
    return err;
}

ErrCode UITableView::onMenuAddAction(QMenu *menu, QAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UITableView::onMenuDeleteAction(QMenu *menu, QAction *act)
{
    traced;
    // TODO: handle it
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
    qint32 idx = 0;
    bool ok = false;
    // TODO: handle view only and edit mode
    idx = item->data(Qt::UserRole).toInt(&ok);
    if (ok){
        logd("view item %d", idx);
        onViewItem(idx);
    } else {
        loge("Cannot get data from widget item");
    }
}



void UITableView::customMenuRequested(QPoint pos)
{
    QModelIndex index=ui->tblList->indexAt(pos);
    // TODO: hande it
    QMenu* menu = buildPopupMenu();
    menu->popup(ui->tblList->viewport()->mapToGlobal(pos));
}


void UITableView::on_btnFilter_clicked()
{
    traced;
    onFilter(ui->cbCategory->currentText(), 0, ui->cbKeyword->currentText());
}

