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
 * Filename: uidepartmentlistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#ifndef UICOMMDEPTLISTVIEW_H
#define UICOMMDEPTLISTVIEW_H

#include "uicommonlistview.h"

class Community;

class UICommDeptListView : public UICommonListView
{
public:
    explicit UICommDeptListView(QWidget *parent = nullptr);
    virtual ~UICommDeptListView();
    Community *community() const;
    ErrCode setCommunity(const Community *newCommunity);

protected:
    virtual int getViewType() { return VIEW_COMMUNITY_DEPT;}
    virtual QString getMainModelName();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual ModelController* getController();
    virtual void initFilterFields();
    virtual QString getTitle();
    virtual void initHeader();

    // Use default delete flow of common list view & table list view
    // As there is no special thing we need to handle

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu,
                                                          UITableCellWidgetItem* item);

    /**
     * @brief Menu item to list of managing people of dept
     * @param menu
     * @param act
     * @return
     */
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);

    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    /**
     * @brief get list of item to show on list view
     * @return list of item to show on listview
     */
    virtual QList<DbModel*> getListDbModels();
};

#endif // UICOMMDEPTLISTVIEW_H
