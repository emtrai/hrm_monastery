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
 * Filename: uidepartmentpersonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#ifndef UIDEPARTMENTPERSONLISTVIEW_H
#define UIDEPARTMENTPERSONLISTVIEW_H

#include "uicommonlistview.h"

class Department;

class UIDepartmentPersonListView : public UICommonListView
{
public:
    explicit UIDepartmentPersonListView(QWidget *parent = nullptr);
    virtual ~UIDepartmentPersonListView();


    Department *department() const;
    void setDepartment(Department *newDepartment);

protected:
    virtual void initHeader();
    virtual void updateItem(DbModel* item, UITableItem* tblItem);

    virtual QList<UITableMenuAction*> getMenuItemActions(const QMenu* menu,
                                                          UITableWidgetItem* item);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    //    virtual ErrCode onMenuAction(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionDelete(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);

protected:
    virtual ErrCode onLoad();
private:
    Department* mDepartment;
};

#endif // UIDEPARTMENTPERSONLISTVIEW_H