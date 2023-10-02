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
#include "filter.h"
#include "baseview.h"

class UITableItem;
class QMenu;
class DbModel;
typedef QList<UITableItem*> (*onRequestData)(qint32 page, qint32 perPage, qint32 totalPages);
typedef qint32 (*onRequestTotalData)();

namespace Ui {
class UITableView;
}

/**
 * @brief A item (line) in table list view
 */
class UITableItem
{
public:
    virtual ~UITableItem();
public:
    static UITableItem* build(const DbModel* data);
    /**
     * @brief Add a value to display on list view
     *        Order of adding will be mapped to col of a line
     *        i.e 1st add, is 1st col, 2nd add, 2nd col and vice versa
     * @param val value to display on list
     * @return UITableItem itself to add new value
     */
    UITableItem* addValue(const QString& val);

    /**
     * @brief return list of value to display
     * @return
     */
    const QStringList &valueList() const;

    /**
     * @brief DbModel data which each line present to
     * @return
     */
    const DbModel *data() const;
    /**
     * @brief clone to new item, caller will take ownership of new item
     * @return
     */
    UITableItem* clone();

private:
    UITableItem();
    UITableItem(const UITableItem& item);
    UITableItem(const DbModel* data);
    void clone(const UITableItem* item);
private:
    QStringList mValueList;// list of display value of each cell
    DbModel* mData; // data ow by table item, will be released in destructor
};


/**
 * @brief Re-present for an item cell in table
 */
class UITableCellWidgetItem: public QTableWidgetItem
{
public:
    UITableCellWidgetItem(const QString &text);
    static UITableCellWidgetItem* build(const QString& txt, qint32 itemIdx = 0, qint32 idx = 0, UITableItem* mItem = nullptr);
    UITableItem *item() const;
    const DbModel *itemData() const;
    void setItem(UITableItem *newItem);
    qint32 itemIdx() const;
    void setItemIdx(qint32 newItemIdx);

    qint32 idx() const;
    void setIdx(qint32 newIdx);

private:
    UITableItem* mItem; // item value, reference only, will not be freed in this class
    qint32 mItemIdx; // idx of field in a row of table
    qint32 mIdx; // idx of items (row id)
};

enum UITableMenuActionType {
    MENU_ACTION_NORMAL = 0,
    MENU_ACTION_SEPARATE
};

class UITableMenuAction: public QAction {

public:
    UITableMenuAction(QObject *parent = nullptr);
    virtual ~UITableMenuAction();
    UITableMenuAction(const QString &text, QObject *parent = nullptr);
    UITableCellWidgetItem *tblItem() const;
    UITableMenuAction* setTblCellItem(UITableCellWidgetItem *newTblItem);


    const std::function<ErrCode (QMenu *, UITableMenuAction *)> &callback() const;
    UITableMenuAction* setCallback(const std::function<ErrCode (QMenu *, UITableMenuAction *)> &newCallback);
    const DbModel* getData();
    UITableMenuAction* addItemList(UITableItem* newItemList);

    UITableMenuActionType menuType() const;
    void setMenuType(UITableMenuActionType newMenuType);
public:
    static UITableMenuAction* build(const QString &text,
                                    QObject *parent = nullptr,
                                    UITableCellWidgetItem* item = nullptr,
                                    qint32 idx = 0);

    static UITableMenuAction* buildMultiItem(const QString &text,
                                             QObject *parent = nullptr,
                                             const QList<UITableItem*>* items = nullptr,
                                             qint32 idx = 0);

    static UITableMenuAction* buildSeparateAction();
    const QList<UITableItem *> &itemList() const;
    /**
     * @brief Get item data (model).
     *        NOTE: Data is cloned, so caller MUST FREE after use (i.e. use RELEASE_LIST_DBMODEL)
     * @param outList
     * @return the number of item
     */
    int itemListData(QList<DbModel *>&outList);
    bool getIsMultiSelectedItem() const;

protected:
    void setIsMultiSelectedItem(bool newIsMultiSelectedItem);

private:
    UITableCellWidgetItem* mTblCellItem;
    QList<UITableItem*> mItemList;
    std::function<ErrCode(QMenu* menu, UITableMenuAction* act)> mCallback;
    UITableMenuActionType mMenuType;
    bool isMultiSelectedItem;
};

class UITableView : public QFrame, public BaseView
{
    Q_OBJECT

public:
    explicit UITableView(QWidget *parent = nullptr);
    virtual ~UITableView();
    void setHeader(const QStringList &newHeader);


    qint32 itemPerPage() const;
    void setItemPerPage(qint32 newItemPerPage);

    virtual void initHeader();
    virtual void setupUI();
    virtual void reload();
    virtual void refesh();
    quint32 currentPage() const;
    void setCurrentPage(quint32 newCurrentPage);

    quint32 totalPages() const;
    void setTotalPages(quint32 newTotalPages);
    void setTitle(const QString& title);

    QMenu *menu() const;


    /**
     * @brief Add filters
     * @param filterItem filter item, such as KItemCommunity, etc.
     * @param keyword keyword for search, search with concept of "contain"
     * @param value value of search, search exact, normally it's uid.
     * @return
     */
    virtual ErrCode addFilter(const QString& filterItem, const QString& keyword, const QVariant& value);

    virtual QWidget* getWidget();
    ErrCode doFilter(int field, int op, const QVariant& keyword);

    void requestReload();
protected:
    virtual void cleanupTableItems();
    virtual void cleanupMenuActions();
    virtual void sort(int column);
    /**
     * @brief List view has filter or not
     * @return true if has filter
     */
    virtual bool hasFilters();
    virtual QString getTitle();
    virtual QStringList getHeader();
    virtual void showEvent(QShowEvent *ev);
    virtual ErrCode onUpdatePage(qint32 page);
    virtual void onUpdatePageDone(ErrCode err, qint32 page, qint32 totalpages, qint32 totalItems);
    virtual ErrCode getListTableRowItems(qint32 page, qint32 perPage, qint32 totalPages,
                                                      QList<UITableItem*>& items) = 0;
    virtual QList<UITableItem*> getListAllTableRowItems();
    virtual qint32 getTotalModelItems();
    virtual ErrCode onLoad();
    /**
     * @brief Reload data, may need to re-fetch data from db instead of cache
     * @return
     */
    virtual ErrCode onReload();
    virtual void importRequested(const QString& fpath);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
    virtual ErrCode onDeleteItem(const QList<UITableItem *>& selectedItems);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
//    virtual void onDeleteItem(UITableItem *item);

    // MENU
    virtual ErrCode addMenuActionCallback(QList<UITableMenuAction*> &actionList);
    virtual QMenu* buildPopupMenu(UITableCellWidgetItem* item, const QList<UITableItem*>& items);
    /**
     * @brief Common menu action, always show
     * @param menu
     * @return list of menu action
     */
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    /**
     * @brief Get menu actions list when an item is selected
     * @param menu
     * @param item
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu, UITableCellWidgetItem* item);
    /**
     * @brief get menu action list when multi item is selected
     * @param menu
     * @param items
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuMultiSelectedItemActions(const QMenu* menu, const QList<UITableItem *>& items);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionDelete(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionEdit(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionReload(QMenu* menu, UITableMenuAction* act);

    /**
     * @brief onFilter
     * @param catetoryid
     * @param catetory
     * @param opFlags
     * @param keywords
     * @return >= 0: the number of items, < 0: error
     */
    virtual int onFilter(int catetoryid, const QString& catetory, qint64 opFlags, const QString& keywords, const QVariant *value);
    virtual int onFilter(const QList<FilterKeyworkItem*> &filters);
    virtual ErrCode setFilter(const QString& item, const QString& keywords, const QVariant *value);

    virtual QHash<int, QString> getFilterFields();
    virtual void appendFilterField(int id, const QString& txt);

    virtual void initFilterFields();
    virtual void loadFilterFields();
    virtual void reloadFilterFields();

    virtual void loadFilterOperators(int fieldId);


    virtual void onFilterFieldChange(int fieldId, const QString& fieldText);

    virtual void clearFilter();

    /**
     * @brief called when onSearchFieldChange is called, to load default search keywords basing on search fields
     * @param fieldId
     * @param fieldText
     * @return Hash map, with uid and text
     */
    virtual QHash<QString, QString> getFilterKeywords(int fieldId, const QString& fieldText);

signals:
    void signalDeleteDone(ErrCode err, QString msg);
    void signalRequestReload();
private slots:
    void onHandleSignalDeleteDone(ErrCode err, QString msg);
    void on_btnImport_clicked();

    void on_tblList_itemDoubleClicked(QTableWidgetItem *item);
    void customMenuRequested(QPoint pos);
    void on_btnFilter_clicked();

    void on_btnAdd_clicked();

    void on_cbCategory_currentIndexChanged(int index);
    void onRequestReload();

protected:
    QStringList mHeader;
    Ui::UITableView *ui;
    qint32 mItemPerPage;
    quint32 mCurrentPage;
    quint32 mTotalPages;
    QMenu* mMenu;
    QList<UITableMenuAction*> mMenuActions;
    QHash<int, QString> mFilterFields;
    QList<FilterItem*> mFilterList; // filters, OR condition?
    // TODO: should be OR or AND condition? can select???
    bool mSuspendReloadOnDbUpdate;
    QList<UITableItem*> mItemList;
    QList<UITableMenuAction*> mCommonMenuActionList;
    QList<UITableMenuAction*> mSingleSelectedMenuActionList;
    QList<UITableMenuAction*> mMultiSelectedMenuActionList;

};

#endif // UITABLEVIEW_H
