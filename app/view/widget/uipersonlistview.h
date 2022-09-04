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
 * Filename: uipersonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UIPERSONLISTVIEW_H
#define UIPERSONLISTVIEW_H

#include "uicommonlistview.h"
#include "person.h"

class UIPersonListView : public UICommonListView
{
public:
    explicit UIPersonListView(QWidget *parent = nullptr);
    virtual ~UIPersonListView();
protected:
    virtual ErrCode onLoad();
    virtual void updateItem(DbModel* item, UITableItem* tblItem);
    virtual void initHeader();
protected:
    virtual void importRequested(const QString& fpath);
    virtual void onViewItem(qint32 idx);
private:
    void cleanUpItem();
//public:
//    explicit UIPersonListView(QWidget *parent = nullptr);
//    virtual ~UIPersonListView();
//    quint32 currentPage() const;
//    void setCurrentPage(quint32 newCurrentPage);

//protected:
//    virtual void importRequested(const QString& fpath);
//private:
//    void cleanUpItem();
//private:
//    quint32 mCurrentPage;
//    quint32 mTotalPages;
};

#endif // UIPERSONLISTVIEW_H
