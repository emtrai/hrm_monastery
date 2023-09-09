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
 * Filename: uisaintlistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UISAINTLISTVIEW_H
#define UISAINTLISTVIEW_H

#include "uicommonlistview.h"
#include "saint.h"

class UISaintListView : public UICommonListView
{
public:
    explicit UISaintListView(QWidget *parent = nullptr);
    virtual ~UISaintListView();
protected:
    virtual int getViewType() { return VIEW_SAINT;}
    virtual ModelController* getController();
    virtual QString getMainModelName();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getTitle();

    virtual void initFilterFields();

    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);
    virtual void initHeader();

    virtual int onFilter(int catetoryid, const QString& catetory, qint64 opFlags, const QString& keywords, const QVariant *value);

    virtual QList<DbModel*> getListDbModels();

};

#endif // UISAINTLISTVIEW_H
