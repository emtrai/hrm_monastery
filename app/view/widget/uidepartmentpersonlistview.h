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

class CommunityDept;

class UIDepartmentPersonListView : public UICommonListView
{
public:
    explicit UIDepartmentPersonListView(QWidget *parent = nullptr);
    virtual ~UIDepartmentPersonListView();

    CommunityDept* communityDept() const;
    ErrCode setCommDept(const CommunityDept *commDept);

protected:
    virtual int getViewType() { return VIEW_DEPARTMENT_PERSON;}
    virtual QString getMainModelName();
    virtual ModelController* getController();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);

    virtual QList<DbModel*> getListDbModels();

    virtual void initHeader();
    virtual QString getTitle();

    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
};

#endif // UIDEPARTMENTPERSONLISTVIEW_H
