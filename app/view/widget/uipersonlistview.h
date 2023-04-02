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
#include "importlistener.h"

class UIPersonListView : public UICommonListView, public ImportListener
{
public:
    explicit UIPersonListView(QWidget *parent = nullptr);
    virtual ~UIPersonListView();
    virtual void setupUI();
protected:
    virtual ErrCode onLoad();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);
    virtual void initHeader();
protected:
    virtual int getViewType() { return VIEW_PERSON;}
    virtual void importRequested(const QString& fpath);
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
    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExportListPerson(QMenu *menu, UITableMenuAction *act);
    virtual ErrCode onChangeCommunity(QMenu* menu, UITableMenuAction* act);
    virtual void onViewItem(UITableCellWidgetItem *item);
    virtual void onEditItem(UITableCellWidgetItem *item);
    virtual QString getTitle();

    virtual void initFilterFields();

    virtual int onFilter(int catetoryid,
                          const QString& catetory,
                          qint64 opFlags,
                          const QString& keywords, const QVariant *value);
    virtual DbModel* onNewModel();
    virtual QString getName();
    virtual void onImportStart(const QString& importName, const QString& fpath, ImportType type);
    virtual void onImportEnd(const QString& importName, ErrCode err, const QString& fpath, ImportType type);

private:
    void cleanUpItem();
};

#endif // UIPERSONLISTVIEW_H
