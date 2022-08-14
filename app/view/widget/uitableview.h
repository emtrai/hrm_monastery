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
 * Filename: uitableview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UITABLEVIEW_H
#define UITABLEVIEW_H

#include <QFrame>
#include <QStringList>

#include "errcode.h"

class UITableItem;

typedef QList<UITableItem*> (*onRequestData)(qint32 page, qint32 perPage, qint32 totalPages);
typedef qint32 (*onRequestTotalData)();

namespace Ui {
class UITableView;
}


class UITableItem
{
public:
    static UITableItem* build(void* data);
    UITableItem* addValue(const QString& val);

    const QStringList &valueList() const;
    void setValueList(const QStringList &newValueList);

private:
    UITableItem(void* data);
private:
    QStringList mValueList;
    void* mData;
};

class UITableView : public QFrame
{
    Q_OBJECT

public:
    explicit UITableView(QWidget *parent = nullptr);
    virtual ~UITableView();
    void setHeader(const QStringList &newHeader);


    void setFpDataReq(onRequestData newFpDataReq);

    void setFpTotalDataReq(onRequestTotalData newFpTotalDataReq);

    qint32 itemPerPage() const;
    void setItemPerPage(qint32 newItemPerPage);

    virtual void initHeader();
    virtual void setupUI();
protected:
    virtual QStringList getHeader();
    virtual void showEvent(QShowEvent *ev);
    virtual void onUpdatePage(qint32 page);
    virtual QList<UITableItem*> getListItem(qint32 page, qint32 perPage, qint32 totalPages);
    virtual qint32 getTotalItems();
    virtual ErrCode onLoad();
protected:
    QStringList mHeader;
private:
    Ui::UITableView *ui;
    onRequestData mFpDataReq;
    onRequestTotalData mFpTotalDataReq;
    qint32 mItemPerPage;
};

#endif // UITABLEVIEW_H
