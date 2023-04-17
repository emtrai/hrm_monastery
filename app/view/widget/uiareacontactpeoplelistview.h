/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: uiareacontactpeople.h
 * Author: Anh, Ngo Huy
 * Created date:4/16/2023
 * Brief:
 */
#ifndef UIAREACONTACTPEOPLELISTVIEW_H
#define UIAREACONTACTPEOPLELISTVIEW_H

#include "uicommonlistview.h"

class Area;
class UIAreaContactPeopleListView : public UICommonListView
{
public:
    explicit UIAreaContactPeopleListView(QWidget *parent = nullptr);
    virtual ~UIAreaContactPeopleListView();

    Area *area() const;
    void setArea(const Area *newArea);
    virtual void setupUI();

protected:
    virtual int getViewType() { return VIEW_AREA_PERSON;}
    virtual void initHeader();
    virtual QString getTitle();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);
    virtual QList<DbModel*> getListItem();

    virtual void onAddItem(UITableCellWidgetItem *item);
    virtual void onEditItem(UITableCellWidgetItem *item);
    virtual void onDeleteItem(const QList<UITableItem *>& selectedItems);
    virtual void onViewItem(UITableCellWidgetItem *item);
private:
    Area* mArea;
};

#endif // UIAREACONTACTPEOPLELISTVIEW_H
