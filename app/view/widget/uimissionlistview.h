/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: uimissionlistview.h
 * Author: Anh, Ngo Huy
 * Created date:2/12/2023
 * Brief:
 */
#ifndef UIMISSIONLISTVIEW_H
#define UIMISSIONLISTVIEW_H

#include "uicommonlistview.h"

class UIMissionListView : public UICommonListView
{
public:
    explicit UIMissionListView(QWidget *parent = nullptr);
    virtual ~UIMissionListView();
    virtual QString getName();
protected:

    virtual int getViewType() { return VIEW_MISSION;}
    virtual ModelController* getController();
    virtual QString getTitle();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);
    virtual QString getMainModelName();
    virtual void initFilterFields();
};

#endif // UIMISSIONLISTVIEW_H
