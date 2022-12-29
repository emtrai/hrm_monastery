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
 * Filename: uipersonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UIPERSONLISTVIEW_H
#define UIPERSONLISTVIEW_H

#include "uicommonlistview.h"
#include "person.h"

class UIPersonListView : public UICommonListView
{
public:
    explicit UIPersonListView(QWidget *parent = nullptr);
    virtual ~UIPersonListView();
protected:
    virtual ErrCode onLoad();
    virtual void updateItem(DbModel* item, UITableItem* tblItem);
    virtual void initHeader();
protected:
    virtual void importRequested(const QString& fpath);
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    /**
     * @brief Get menu actions list when an item is selected
     * @param menu
     * @param item
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuItemActions(const QMenu* menu, UITableWidgetItem* item);
    /**
     * @brief get menu action list when multi item is selected
     * @param menu
     * @param items
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuMultiItemActions(const QMenu* menu, const QList<UITableItem *>& items);

    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onChangeCommunity(QMenu* menu, UITableMenuAction* act);
    virtual void onViewItem(UITableWidgetItem *item);
    virtual void onEditItem(UITableWidgetItem *item);
    virtual void onDeleteItem(UITableWidgetItem *item);
    virtual QString getTitle();

    virtual void initFilterFields();

    virtual int onFilter(int catetoryid,
                          const QString& catetory,
                          qint64 opFlags,
                          const QString& keywords, const QVariant *value);

private:
    void cleanUpItem();
//public:
//    explicit UIPersonListView(QWidget *parent = nullptr);
//    virtual ~UIPersonListView();
//    quint32 currentPage() const;
//    void setCurrentPage(quint32 newCurrentPage);

//protected:
//    virtual void importRequested(const QString& fpath);
//private:
//    void cleanUpItem();
//private:
//    quint32 mCurrentPage;
//    quint32 mTotalPages;
};

#endif // UIPERSONLISTVIEW_H
