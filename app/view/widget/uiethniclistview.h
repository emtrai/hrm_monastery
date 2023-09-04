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
 * Filename: uiethniclistview.h
 * Author: Anh, Ngo Huy
 * Created date:2/14/2023
 * Brief:
 */
#ifndef UIETHNICLISTVIEW_H
#define UIETHNICLISTVIEW_H

#include "uicommonlistview.h"

class UIEthnicListView : public UICommonListView
{
public:
    explicit UIEthnicListView(QWidget *parent = nullptr);
    virtual ~UIEthnicListView();
    virtual QString getName();
protected:

    virtual int getViewType() { return VIEW_ETHNIC;}
    virtual ModelController* getController();
    virtual QString getTitle();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getMainModelName();
    virtual void initFilterFields();

    virtual void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
};

#endif // UIETHNICLISTVIEW_H
