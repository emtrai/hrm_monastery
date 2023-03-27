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
    void setCommunity(Community *newCommunity);

protected:
    virtual int getViewType() { return VIEW_COMMUNITY_DEPT;}
    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu,
                                                          UITableCellWidgetItem* item);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    virtual QString getTitle();
    virtual void initHeader();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);
    virtual DbModel* onNewModel();

protected:
    virtual ErrCode onLoad();
private:
    Community* mCommunity;
};

#endif // UICOMMDEPTLISTVIEW_H
