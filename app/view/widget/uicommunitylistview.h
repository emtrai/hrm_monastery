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
#include "mainwindow.h"

class UICommunityListView : public UICommonListView, public MainWindownImportListener
{
public:
    explicit UICommunityListView(QWidget *parent = nullptr);
    virtual ~UICommunityListView();
    virtual void setupUI();
    virtual QString getName();
protected:
    virtual int getViewType() { return VIEW_COMMUNITY;}
    virtual void initHeader();
    virtual QString getTitle();
    virtual ModelController* getController();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getMainModelName();

    virtual QList<DbModel*> getListDbModels();

    virtual ErrCode fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx);

    virtual ImportTarget getImportTarget();

    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu,
                                                          UITableCellWidgetItem* item);

    virtual ErrCode onMenuActionExportListAllPeople(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExportListActivePeople(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListActivePerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListAllPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListManagers(QMenu* menu, UITableMenuAction* act);

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

    virtual void onMainWindownImportStart(ImportTarget target);
    virtual void onMainWindownImportEnd(ImportTarget target, ErrCode err, void* importData = nullptr);

private:
    /**
     * @brief Show list of people inside community
     * @param act
     * @param activeOnly true if show only active people in community
     * @return ErrNone on success, error code otherwise
     */
    ErrCode showListPeople(UITableMenuAction *act, bool activeOnly = false);
    ErrCode exportListPeople(UITableMenuAction *act, bool activeOnly = false);
};


#endif // UICOMMUNITYLISTVIEW_H
