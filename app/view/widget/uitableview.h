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
 * Filename: uitableview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UITABLEVIEW_H
#define UITABLEVIEW_H

#include <QFrame>
#include <QStringList>
#include <QTableWidgetItem>
#include <QAction>
#include "errcode.h"

class UITableItem;
class QMenu;

typedef QList<UITableItem*> (*onRequestData)(qint32 page, qint32 perPage, qint32 totalPages);
typedef qint32 (*onRequestTotalData)();

namespace Ui {
class UITableView;
}

//enum MenuAction {
//    ACTION_NEW,
//    ACTION_DELETE,
//    ACTION_MAX
//};


class UITableItem
{
public:
    static UITableItem* build(void* data);
    UITableItem* addValue(const QString& val);

    const QStringList &valueList() const;
    void setValueList(const QStringList &newValueList);

    void *data() const;

private:
    UITableItem();
    UITableItem(void* data);
private:
    QStringList mValueList;
    void* mData;
};
class UITableWidgetItem: public QTableWidgetItem
{
public:
    UITableWidgetItem(const QString &text);
    static UITableWidgetItem* build(const QString& txt, qint32 itemIdx = 0, qint32 idx = 0, UITableItem* mItem = nullptr);
    UITableItem *item() const;
    void setItem(UITableItem *newItem);
    qint32 itemIdx() const;
    void setItemIdx(qint32 newItemIdx);

    qint32 idx() const;
    void setIdx(qint32 newIdx);

private:
    UITableItem* mItem;
    qint32 mItemIdx; // idx of field in a row
    qint32 mIdx; // idx of items (row id)
};

class UITableMenuAction: public QAction {
public:
    UITableMenuAction(const QString &text, QObject *parent = nullptr);
    UITableWidgetItem *tblItem() const;
    UITableMenuAction* setTblItem(UITableWidgetItem *newTblItem);

    static UITableMenuAction* build(const QString &text,
                                    QObject *parent = nullptr,
                                    UITableWidgetItem* item = nullptr,
                                    qint32 idx = 0);


    const std::function<ErrCode (QMenu *, UITableMenuAction *)> &callback() const;
    UITableMenuAction* setCallback(const std::function<ErrCode (QMenu *, UITableMenuAction *)> &newCallback);
    void* getData();
private:
    UITableWidgetItem* mTblItem;
    std::function<ErrCode(QMenu* menu, UITableMenuAction* act)> mCallback;

};

class UITableView : public QFrame
{
    Q_OBJECT

public:
    explicit UITableView(QWidget *parent = nullptr);
    virtual ~UITableView();
    void setHeader(const QStringList &newHeader);


    void setFpDataReq(onRequestData newFpDataReq);

    void setFpTotalDataReq(onRequestTotalData newFpTotalDataReq);

    qint32 itemPerPage() const;
    void setItemPerPage(qint32 newItemPerPage);

    virtual void initHeader();
    virtual void setupUI();
    virtual void reload();
    quint32 currentPage() const;
    void setCurrentPage(quint32 newCurrentPage);

    quint32 totalPages() const;
    void setTotalPages(quint32 newTotalPages);
    void setTitle(const QString& title);

    QMenu *menu() const;

protected:
    virtual QString getTitle();
    virtual QStringList getHeader();
    virtual void showEvent(QShowEvent *ev);
    virtual void onUpdatePage(qint32 page);
    virtual QList<UITableItem*> getListItem(qint32 page, qint32 perPage, qint32 totalPages);
    virtual qint32 getTotalItems();
    virtual ErrCode onLoad();
    virtual void importRequested(const QString& fpath);
    virtual void onViewItem(UITableWidgetItem *item);

    // MENU
    virtual QMenu* buildPopupMenu(UITableWidgetItem* item);
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    virtual QList<UITableMenuAction*> getMenuItemActions(const QMenu* menu, UITableWidgetItem* item);
//    virtual ErrCode onMenuActionTrigger(QMenu* menu, UITableMenuAction*);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionDelete(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionEdit(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);

    virtual void onFilter(const QString& catetory, qint64 opFlags, const QString& keywords);

private slots:
    void on_btnImport_clicked();

    void on_tblList_itemDoubleClicked(QTableWidgetItem *item);
    void customMenuRequested(QPoint pos);
    void on_btnFilter_clicked();
//    void on_cbKeyword_returnPressed();

    void on_btnAdd_clicked();

protected:
    QStringList mHeader;
    Ui::UITableView *ui;
    onRequestData mFpDataReq;
    onRequestTotalData mFpTotalDataReq;
    qint32 mItemPerPage;
    quint32 mCurrentPage;
    quint32 mTotalPages;
    QMenu* mMenu;
};

#endif // UITABLEVIEW_H
