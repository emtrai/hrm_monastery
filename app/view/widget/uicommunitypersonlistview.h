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
#ifndef UICOMMUNITYPERSONLISTVIEW_H
#define UICOMMUNITYPERSONLISTVIEW_H

#include "uicommonlistview.h"

class Community;

class UICommunityPersonListView : public UICommonListView
{
public:
    explicit UICommunityPersonListView(QWidget *parent = nullptr);
    virtual ~UICommunityPersonListView();
    void loadCommunityPerson(const QString& communityUid, bool isActive = true);

    Community *community() const;
    void setCommunity(const Community *newCommunity);

protected:
    virtual int getViewType() { return VIEW_COMMUNITY_PERSON;}
    virtual void initHeader();
    virtual QString getTitle();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual QList<UITableMenuAction*> getMenuMultiSelectedItemActions(const QMenu *menu,
                                                               const QList<UITableItem *>& items);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
//    virtual ErrCode onMenuAction(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionDelete(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);

protected:
    virtual ErrCode onLoad();
private:
    Community* mCommunity;
};

#endif // UICOMMUNITYPERSONLISTVIEW_H
