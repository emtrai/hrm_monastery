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

class UICommonListView : public UITableView, public CommonEditModelListener, public OnModelControllerListener
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
    virtual ImportTarget getImportTarget();
    virtual ModelController* importController();
    virtual bool hasExportMenuItem();
    virtual ModelController* exportController();
    virtual QList<UITableItem*> getListItem(qint32 page, qint32 perPage, qint32 totalPages);
    virtual qint32 getTotalItems();

    /**
     * @brief get list of item to show on list view
     * @return list of item to show on listview
     */
    virtual QList<DbModel*> getListItem();
    virtual ModelController* getController();
    virtual ErrCode onLoad();
    virtual ErrCode onReload();
    virtual void initHeader();
    virtual void updateItem(DbModel* item, UITableItem* tblItem, int idx);
    virtual void initFilterFields();
    virtual QHash<QString, QString> getFilterKeywords(int fieldId, const QString& fieldText);
    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual void onEditItem(UITableCellWidgetItem *item);

    virtual void onDbModelReady(ErrCode ret, DbModel* model, DlgCommonEditModel* dlg);
    virtual DbModel* onNewModel(const QString& modelName);
    virtual QString getName();
    virtual QString getMainModelName();
    virtual void onModelControllerDataUpdated(const DbModel *model);
    template<class T>
    void setListItem(const QList<T*>& list){
        RELEASE_LIST_DBMODEL(mItemList);
        foreach(T* item, list) {
            mItemList.append((DbModel*) item);
        }
    }
    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionExport(QMenu *menu, UITableMenuAction *act);
protected:
    QList<DbModel*> mItemList;
    DbModel* mParentModel; //
    bool mHasImportMenu;
    bool mHasExportMenu;
};

#endif // UICOMMONLISTVIEW_H
