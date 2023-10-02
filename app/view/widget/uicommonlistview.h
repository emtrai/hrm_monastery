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

enum ImportTarget;

class UICommonListView : public UITableView,
                         public CommonEditModelListener,
                         public OnModelControllerListener
{
public:
    explicit UICommonListView(QWidget *parent = nullptr);
    virtual ~UICommonListView();
    virtual void setupUI();
    ErrCode setParentModel(const DbModel *newParentModel);
    DbModel *parentModel() const;

protected:
    virtual QList<UITableMenuAction*> getMenuCommonActions(const QMenu* menu);
    virtual bool hasImportMenuItem();
    virtual bool hasExportMenuItem();
    /**
     * Import target of list view, i.e. area, community, etc.
     * @return
     */
    virtual ImportTarget getImportTarget();
    virtual ModelController* importController();
    virtual ModelController* exportController();


    virtual ErrCode getListTableRowItems(qint32 page, qint32 perPage, qint32 totalPages,
                                                      QList<UITableItem*>& items);
    virtual qint32 getTotalModelItems();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);

    /**
     * @brief get list of item to show on list view
     * @return list of item to show on listview
     */
    virtual QList<DbModel*> getListDbModels();
    virtual ModelController* getController() = 0;

    virtual ErrCode onLoad();
    virtual ErrCode onReload();

    virtual void initHeader();

    virtual void onUpdatePageDone(ErrCode err, qint32 page, qint32 totalpages, qint32 totalItems);
    virtual void initFilterFields();
    /**
     * @brief Preset keyword to filter, i.e. list of status to be filtered, etc.
     * @param fieldId field id to be search, i.e name, status, etc.
     * @param fieldText
     * @return hash map between keyword and correspoinding id (i.e uid)
     */
    virtual QHash<QString, QString> getFilterKeywords(int fieldId, const QString& fieldText);

    /**
     * @brief do filters basing on keyword.
     *        filter/search by value first, if it's empty, search with raw keyword
     * @param catetoryid category/field id, i.e. community
     * @param catetory category in name
     * @param opFlags operator, i.e >, <, contain, etc.
     * @param keywords keyword to search
     * @param value value to match with keyword if any (String type)
     * @return number of found items
     */
    virtual int onFilter(int catetoryid,
                         const QString& catetory,
                         qint64 opFlags,
                         const QString& keywords, const QVariant *value);
    virtual ErrCode _onFilter(ModelController* ctrl,
                          int catetoryid,
                          qint64 opFlags,
                          const QString& keywords,
                          const char* targetModelName = nullptr,
                          const DbModel* parentModel = nullptr,
                          QList<DbModel*>* outList = nullptr);

    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExport(QMenu *menu, UITableMenuAction *act);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

    virtual void onDbModelReady(ErrCode ret, DbModel* model, DlgCommonEditModel* dlg);
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getName();
    virtual QString getMainModelName();
    virtual void onModelControllerDataUpdated(const DbModel *model);

protected:
    QList<DbModel*> mModelList;
    /**
     * set true if view supports import
     * derived class should set this value accordingly
     */
    bool mHasImportMenu;
    /**
     * set true if view supports export
     * derived class should set this value accordingly
     */
    bool mHasExportMenu;
    /**
     * set true if support sorting table list view
     */
    bool mSortItem;

    /**
     * Parent model of items in list view.
     * i.e list view of community department, parent model is Community as
     * list view is list of department of a community
     */
    DbModel* mParentModel;
};

#endif // UICOMMONLISTVIEW_H
