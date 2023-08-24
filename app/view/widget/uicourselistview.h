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
 * Filename: uicourselistview.h
 * Author: Anh, Ngo Huy
 * Created date:3/9/2023
 * Brief:
 */
#ifndef UICOURSELISTVIEW_H
#define UICOURSELISTVIEW_H

#include "uicommonlistview.h"

class UICourseListView : public UICommonListView
{
public:
    explicit UICourseListView(QWidget *parent = nullptr);
    virtual ~UICourseListView();
    virtual void setupUI();
protected:

    virtual QString getTitle();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);
    virtual ErrCode onDeleteItem(const QList<UITableItem *>& selectedItems);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
protected:

    virtual int getViewType() { return VIEW_COURSE;}
    virtual QList<DbModel*> getListDbModels();
    virtual ModelController* getController();
    virtual void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);
};

#endif // UICOURSELISTVIEW_H
