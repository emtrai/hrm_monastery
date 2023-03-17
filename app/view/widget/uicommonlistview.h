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
 * Filename: uicommonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UICOMMONLISTVIEW_H
#define UICOMMONLISTVIEW_H

#include "uitableview.h"

#include "dbmodel.h"
#include "errcode.h"
#include "modelcontroller.h"
#include "dlgcommoneditmodel.h"

class UICommonListView : public UITableView, public CommonEditModelListener
{
public:
    explicit UICommonListView(QWidget *parent = nullptr);
    virtual ~UICommonListView();
protected:
    virtual QList<UITableItem*> getListItem(qint32 page, qint32 perPage, qint32 totalPages);
    virtual qint32 getTotalItems();
    virtual QList<DbModel*> getListItem();
    virtual ModelController* getController();
    virtual ErrCode onLoad();
    virtual ErrCode onReload();
    virtual void initHeader();
    virtual void updateItem(DbModel* item, UITableItem* tblItem);
    virtual void initFilterFields();
    virtual QHash<QString, QString> getFilterKeywords(int fieldId, const QString& fieldText);
    virtual void onViewItem(UITableWidgetItem *item);
    virtual void onAddItem(UITableWidgetItem *item);

    virtual void onDbModelReady(ErrCode ret, DbModel* model, DlgCommonEditModel* dlg);
    virtual DbModel* onNewModel();
protected:
    QList<DbModel*> mItemList;
};

#endif // UICOMMONLISTVIEW_H
