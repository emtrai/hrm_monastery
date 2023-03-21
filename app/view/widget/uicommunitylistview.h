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
 * Filename: uicommunitylistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UICOMMUNITYLISTVIEW_H
#define UICOMMUNITYLISTVIEW_H

#include "uicommonlistview.h"
#include "dlgcommoneditmodel.h"

class UICommunityListView : public UICommonListView
{
public:
    explicit UICommunityListView(QWidget *parent = nullptr);
    virtual ~UICommunityListView();
protected:
    void initHeader();
    void updateItem(DbModel *item, UITableItem *tblItem);
    virtual QList<UITableMenuAction*> getMenuItemActions(const QMenu* menu,
                                                          UITableWidgetItem* item);
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    virtual QList<UITableMenuAction *> getMenuMultiItemActions(const QMenu *menu, const QList<UITableItem *> &items);
    virtual ErrCode onMenuActionExportListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExportListCommunity(QMenu *menu, UITableMenuAction *act);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListPersonHistory(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListManagers(QMenu* menu, UITableMenuAction* act);
//    virtual ErrCode onMenuAction(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAddPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAddDepart(QMenu* menu, UITableMenuAction* act);
//    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);
    virtual void onViewItem(UITableWidgetItem *item);
    virtual void onDeleteItem(UITableWidgetItem *item);
    void onEditItem(UITableWidgetItem *item);
    virtual QString getTitle();
protected:
    virtual ErrCode onLoad();
    virtual ErrCode onReload();
    virtual DbModel* onNewModel();
};


#endif // UICOMMUNITYLISTVIEW_H
