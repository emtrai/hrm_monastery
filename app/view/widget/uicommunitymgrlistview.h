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
 * Filename: uicommunitymgrpeoplelistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#ifndef UICOMMUNITYMGRLISTVIEW_H
#define UICOMMUNITYMGRLISTVIEW_H

#include "uicommonlistview.h"
class Community;
class UICommunityMgrListView : public UICommonListView
{
public:
    explicit UICommunityMgrListView(QWidget *parent = nullptr);
    virtual ~UICommunityMgrListView();
    virtual QString getName();

    /**
     * @brief community
     *        Caller must not free returned value.
     *        If want to change returned value, clone it wit \ref CLONE_DBMODEL
     * @return
     */
    Community *community() const;
    /**
     * @brief Set community
     * @param newArea, will be cloned to this object, caller can freely control assigned value
     */
    ErrCode setCommunity(const Community *newArea);

    /**
     * @brief setup UI
     */
    virtual void setupUI();

protected:
    virtual int getViewType() { return VIEW_COMMUNITY_MGR;}
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
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    /**
     * @brief get list of item to show on list view
     * @return list of item to show on listview
     */
    virtual QList<DbModel*> getListDbModels();

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

};

#endif // UICOMMUNITYMGRLISTVIEW_H
