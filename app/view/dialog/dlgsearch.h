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
 * Filename: DlgSearch.h
 * Author: Anh, Ngo Huy
 * Created date:9/20/2022
 * Brief:
 */
#ifndef DLGSEARCH_H
#define DLGSEARCH_H

#include <QDialog>

class Person;
namespace Ui {
class DlgSearch;
}

class DbModel;

class DlgSearch : public QDialog
{
    Q_OBJECT
public:

    ~DlgSearch();

    bool getIsMultiSelection() const;
    void setIsMultiSelection(bool newIsMultiSelection);
    void enableGetAllSupport();

    virtual void setupUi();

    virtual void clearAll();
    virtual void clearSelectedItem();

    virtual const QList<DbModel *> &selectedItems() const;
    virtual DbModel* selectedItem() const;

protected:

    explicit DlgSearch(QWidget *parent = nullptr, bool isMulti = false);
    void accept();
    virtual QString getTitle();
    virtual void initHeader();

//    virtual void reload();

    virtual int onSearch(const QString& keyword) = 0;
    virtual int onGetAll();
    virtual DbModel* getItemAtIdx(int idx) = 0;
    virtual QString getValueOfItemAt(int idx, int col, QString header, DbModel* item);
    virtual int query(std::function<int()> queryfunc);
private slots:
    void on_btnSearch_clicked();

    void on_btnAll_clicked();

protected:
    Ui::DlgSearch *ui;
    QStringList mHeader;
    QList<DbModel*> mSelectedItems;
    bool mIsMultiSelection;
};

#endif // DLGSEARCH_H
