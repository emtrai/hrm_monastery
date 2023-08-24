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
#include "importlistener.h"

class UICommunityListView : public UICommonListView, public ImportListener
{
public:
    explicit UICommunityListView(QWidget *parent = nullptr);
    virtual ~UICommunityListView();
    virtual void setupUI();
protected:
    virtual int getViewType() { return VIEW_COMMUNITY;}
    virtual void initHeader();
    virtual ModelController* getController();
    virtual ErrCode fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx);
    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu,
                                                          UITableCellWidgetItem* item);
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    virtual QList<UITableMenuAction *> getMenuMultiSelectedItemActions(const QMenu *menu, const QList<UITableItem *> &items);
    virtual ErrCode onMenuActionExportListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExportListCommunity(QMenu *menu, UITableMenuAction *act);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListAllPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListManagers(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAddPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAddDepart(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
    virtual QString getTitle();

    virtual QString getName();
    virtual void onImportStart(const QString& importName, const QString& fpath, ImportType type);
    virtual void onImportEnd(const QString& importName, ErrCode err, const QString& fpath, ImportType type);

protected:
    virtual ErrCode onLoad();
    virtual ErrCode onReload();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
};


#endif // UICOMMUNITYLISTVIEW_H
