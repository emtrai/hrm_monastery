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
 * Filename: uitableviewfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uitableviewfactory.h"
#include "logger.h"
#include "view/widget/uipersonlistview.h"
#include "view/widget/uisaintlistview.h"
#include "view/widget/uicommunitylistview.h"

UITableView *UITableViewFactory::getView(ViewType type, QWidget *parent )
{
    UITableView* view = nullptr;
    traced;
    logd("type %d", type);
    switch (type) {
    case PERSON:
        view = new UIPersonListView(parent);
        break;
    case SAINT:
        view = new UISaintListView(parent);
        break;
    case COMMUNITY:
        view = new UICommunityListView(parent);
        break;
    default:
        break;
    }

    if (nullptr != view)
        view->setupUI();

    return view;
}
