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
 * Filename: dlgimportlistresult.h
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#ifndef DLGIMPORTLISTRESULT_H
#define DLGIMPORTLISTRESULT_H

#include <QDialog>
#include <QList>
#include "dbmodel.h"

namespace Ui {
class DlgImportListResult;
class UIImportItem;
}

class UIImportItem
{
public:
    static UIImportItem* build(void* data);
    UIImportItem* addValue(const QString& val);

    const QStringList &valueList() const;
    void setValueList(const QStringList &newValueList);
    void *data() const;

private:
    UIImportItem(void* data);
private:
    QStringList mValueList;
    void* mData;
};
class DlgImportListResult : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportListResult(QWidget *parent = nullptr);
    ~DlgImportListResult();

    void setHeader(const QStringList &newHeader);

    virtual void setupUI();
    virtual const QList<DbModel *> &list() const;
    virtual void setup(const QList<DbModel *> &newList);
private:
    QStringList getFinalHeader();
protected:
    virtual QStringList getHeader();
    virtual void initHeader();
    virtual QList<UIImportItem*>* getItems();
    virtual ErrCode onLoad();
    virtual void showEvent( QShowEvent* event);
    virtual ErrCode saveItems(const QList<DbModel *> &list);
    void accept();
protected:
    QStringList mHeader;
    QList<DbModel*> mList;
private:
    Ui::DlgImportListResult *ui;
};

#endif // DLGIMPORTLISTRESULT_H
