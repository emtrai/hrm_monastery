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
 * Filename: uicommunitypersonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/10/2022
 * Brief:
 */
#ifndef UIPEOPLEINCOMMUNITYLISTVIEW_H
#define UIPEOPLEINCOMMUNITYLISTVIEW_H

#include "uicommonlistview.h"

class Community;

/**
 * @brief List of people in community
 */
class UIPeopleInCommunityListView : public UICommonListView
{
public:
    explicit UIPeopleInCommunityListView(QWidget *parent = nullptr);
    virtual ~UIPeopleInCommunityListView();
    virtual QString getName();

    Community *community() const;
    ErrCode setCommunity(const Community *newCommunity);

protected:
    virtual int getViewType() { return VIEW_PEOPLE_IN_COMMUNITY_LIST;}
    virtual QString getMainModelName();
    virtual ModelController* getController();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getTitle();

    virtual void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);

    virtual QList<DbModel*> getListDbModels();
};

#endif // UIPEOPLEINCOMMUNITYLISTVIEW_H
