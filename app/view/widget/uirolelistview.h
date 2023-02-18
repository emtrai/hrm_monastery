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
 * Filename: uirolelistview.h
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#ifndef UIROLELISTVIEW_H
#define UIROLELISTVIEW_H

#include "uicommonlistview.h"

class UIRoleListView : public UICommonListView
{

public:
    explicit UIRoleListView(QWidget *parent = nullptr);
    virtual ~UIRoleListView();
protected:

    virtual QString getTitle();
    virtual DbModel* onNewModel();
protected:
    virtual ErrCode onLoad();
};

#endif // UIROLELISTVIEW_H
