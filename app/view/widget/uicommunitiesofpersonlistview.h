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
 * Filename: uipersoncommunitylistview.h
 * Author: Anh, Ngo Huy
 * Created date:5/20/2023
 * Brief:
 */
#ifndef UICOMMUNITIESOFPERSONLISTVIEW_H
#define UICOMMUNITIESOFPERSONLISTVIEW_H

#include "uicommonlistview.h"

class Person;

/**
 * @brief List of communities of person
 */
class UICommunitiesOfPersonListView : public UICommonListView
{
public:
    explicit UICommunitiesOfPersonListView(QWidget *parent = nullptr);
    virtual ~UICommunitiesOfPersonListView();
    void loadCommunityPerson(const QString& communityUid, bool isActive = true);

    /**
     * @brief Caller must not free this after use, if want to change/free, clone it
     * @return
     */
    const Person *person() const;
    ErrCode setPerson(const Person *per);

protected:
    virtual int getViewType() { return VIEW_COMMUNITIES_OF_PERSON_LIST;}
    virtual void initHeader();
    virtual QString getTitle();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual QList<UITableMenuAction*> getMenuMultiSelectedItemActions(const QMenu *menu,
                                                                       const QList<UITableItem *>& items);
    virtual ErrCode onMenuActionListPerson(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionListDepartment(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual void onEditItem(UITableCellWidgetItem *item);

protected:
    virtual ErrCode onLoad();
private:
    Person* mPerson;
};

#endif // UICOMMUNITIESOFPERSONLISTVIEW_H
