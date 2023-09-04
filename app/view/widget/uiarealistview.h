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
 * Filename: uiarealistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#ifndef UIAREALISTVIEW_H
#define UIAREALISTVIEW_H

#include "uicommonlistview.h"
#include "baseview.h"
class Area;

class UIAreaListView : public UICommonListView
{
public:
    explicit UIAreaListView(QWidget *parent = nullptr);
    virtual ~UIAreaListView();
    virtual QString getName();
protected:
    virtual int getViewType() { return VIEW_AREA;}
    virtual QString getTitle();
    void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx);
    virtual void initFilterFields();

    virtual ModelController* getController();
    
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    /**
     * @brief Model name to be used for checking/create model
     *        This is name of moodel which list view to show
     * @return
     */
    virtual QString getMainModelName();

    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu,
                                                          UITableCellWidgetItem* item);
    virtual ErrCode onMenuActionViewContactPeople(QMenu* menu, UITableMenuAction* act);
};

#endif // UIAREALISTVIEW_H
