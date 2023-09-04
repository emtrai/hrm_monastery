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
 * Filename: uipersoneventlistview.h
 * Author: Anh, Ngo Huy
 * Created date:5/7/2023
 * Brief:
 */
#ifndef UIPERSONEVENTLISTVIEW_H
#define UIPERSONEVENTLISTVIEW_H

#include "uicommonlistview.h"

class Person;

class UIPersonEventListView : public UICommonListView
{
public:
    explicit UIPersonEventListView(QWidget *parent = nullptr);
    virtual ~UIPersonEventListView();

    Person *person() const;
    ErrCode setPerson(const Person *newPerson);

protected:
    virtual int getViewType() { return VIEW_PERSON_EVENT;}
    virtual ModelController* getController();
    virtual QString getMainModelName();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getTitle();

    virtual void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);


    virtual QList<DbModel*> getListDbModels();
};

#endif // UIPERSONEVENTLISTVIEW_H
