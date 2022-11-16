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
class Area;

class UIAreaListView : public UICommonListView
{
public:
    explicit UIAreaListView(QWidget *parent = nullptr);
    virtual ~UIAreaListView();
protected:
    void initHeader();
    void updateItem(DbModel *item, UITableItem *tblItem);
    virtual QList<UITableMenuAction*> getMenuItemActions(const QMenu* menu,
                                                          UITableWidgetItem* item);
    virtual ErrCode onMenuActionAdd(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionDelete(QMenu* menu, UITableMenuAction* act);
    //    virtual ErrCode onMenuActionView(QMenu* menu, UITableMenuAction* act);
    virtual void onViewItem(UITableWidgetItem *item);
    virtual Controller* getController();

    virtual QList<DbModel*> getListItem();
};

#endif // UIAREALISTVIEW_H