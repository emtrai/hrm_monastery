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
/**
 * @brief UI List of contact people of an area
 */
class UIAreaContactPeopleListView : public UICommonListView
{
public:
    explicit UIAreaContactPeopleListView(QWidget *parent = nullptr);
    virtual ~UIAreaContactPeopleListView();

    /**
     * @brief area
     *        Caller must not free returned value.
     *        If want to change returned value, clone it wit \ref CLONE_DBMODEL
     * @return
     */
    Area *area() const;
    /**
     * @brief Set area
     * @param newArea, will be cloned to this object, caller can freely control assigned value
     */
    ErrCode setArea(const Area *newArea);

    /**
     * @brief setup UI
     */
    virtual void setupUI();

protected:
    virtual int getViewType() { return VIEW_AREA_PERSON;}
    virtual QString getMainModelName();
    virtual void initHeader();
    virtual ImportTarget getImportTarget();
    virtual QString getTitle();
    virtual void initFilterFields();
    virtual ModelController* getController();
    /**
     * @brief add one item of list
     * @param item
     * @param tblItem
     * @param idx
     */
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);

    /**
     * @brief get list of item to show on list view
     * @return list of item to show on listview
     */
    virtual QList<DbModel*> getListItem();

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);

};

#endif // UIAREACONTACTPEOPLELISTVIEW_H
