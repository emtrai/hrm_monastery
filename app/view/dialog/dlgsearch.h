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
#include "errcode.h"

class Person;
namespace Ui {
class DlgSearch;
}

class DbModel;

class DlgSearch : public QDialog
{
    Q_OBJECT
public:

    virtual ~DlgSearch();

    bool getIsMultiSelection() const;
    void setIsMultiSelection(bool newIsMultiSelection);
    void enableGetAllSupport();
    void enableGetManagerSupport();

    virtual void setupUi();

    void clearAll();
    virtual const QList<DbModel *> &selectedItems() const;
    virtual const DbModel* selectedItem() const;

protected:

    explicit DlgSearch(QWidget *parent = nullptr, bool isMulti = false);
    void accept();
    virtual QString getTitle();
    virtual void initHeader();

    virtual ErrCode onSearch(const QString& keyword);
    virtual ErrCode doSearch(const QString& keyword, QList<DbModel*>& items) = 0;
    virtual ErrCode onGetAll();
    virtual ErrCode doGetAll(QList<DbModel*>& items) = 0;
    virtual ErrCode onGetManagers();
    virtual ErrCode doGetManager(QList<DbModel*>& items);
    virtual DbModel* getItemAtIdx(int idx);
    virtual QString getValueOfItemAt(int idx, int col, DbModel* item);
    virtual QString getValueOfItemCol(int idx, const DbModel* item);

    virtual void updatePage(qint32 page, bool updatePageInfo = true);
    /**
     * @brief update page info, 0 to clear
     * @param page
     * @param totalItems
     */
    virtual void updatePageInfo(qint32 page, qint32 totalItems = 0);
    virtual void cleanupTableItems();
    /**
     * @brief get item index from row index
     * @param rowIdx
     * @return >=0 item index, < 0 error
     */
    virtual qint32 getItemIdxFromRowIdx(qint32 rowIdx);
    /**
     * @brief get current selected page
     * @param index if set >= 0, use this index to get page number
     * @return page number (>0), 0 if invalid page
     */
    qint32 getCurrentSelectedPage(qint32 index = -1);
private slots:
    void on_btnSearch_clicked();

    void on_btnAll_clicked();

    void on_btnManagers_clicked();

    void on_cbPage_currentIndexChanged(int index);

protected:
    Ui::DlgSearch *ui;
    QStringList mHeader;
    QList<DbModel*> mSelectedItems;
    QList<DbModel*> mListItems;
    bool mIsMultiSelection;
};

#endif // DLGSEARCH_H
