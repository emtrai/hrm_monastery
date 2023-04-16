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
#include "view/widget/uicommunitypersonlistview.h"
#include "view/widget/uicommdeptlistview.h"
#include "view/widget/uidepartmentpersonlistview.h"
#include "view/widget/uirolelistview.h"
#include "view/widget/uiarealistview.h"
#include "view/widget/uiareacontactpeoplelistview.h"
#include "view/widget/uidepartmentlistview.h"
#include "view/widget/uimissionlistview.h"
#include "view/widget/uispecialistlistview.h"
#include "view/widget/uieducationlistview.h"
#include "view/widget/uiworklistview.h"
#include "view/widget/uicountrylistview.h"
#include "view/widget/uiethniclistview.h"
#include "view/widget/uicourselistview.h"
#include "view/widget/uitextbrowser.h"

BaseView *UITableViewFactory::getView(ViewType type, QWidget *parent )
{
    BaseView* view = nullptr;
    tracein;
    logd("type %d", type);
    switch (type) {
    case VIEW_TEXT_BROWSER:
        view = new UITextBrowser(parent);
        break;
    case VIEW_PERSON:
        view = new UIPersonListView(parent);
        break;
    case VIEW_SAINT:
        view = new UISaintListView(parent);
        break;
    case VIEW_COMMUNITY:
        view = new UICommunityListView(parent);
        break;
    case VIEW_AREA:
        view = new UIAreaListView(parent);
        break;
    case VIEW_AREA_PERSON:
        view = new UIAreaContactPeopleListView(parent);
        break;
    case VIEW_COMMUNITY_PERSON:
        view = new UICommunityPersonListView(parent);
        break;
    case VIEW_DEPARTMENT:
        view = new UIDepartmentListView(parent);
        break;
    case VIEW_COMMUNITY_DEPT:
        view = new UICommDeptListView(parent);
        break;
    case VIEW_DEPARTMENT_PERSON:
        view = new UIDepartmentPersonListView(parent);
        break;
    case VIEW_MISSION:
        view = new UIMissionListView(parent);
        break;
    case VIEW_SPECIALIST:
        view = new UISpecialistListView(parent);
        break;
    case VIEW_EDUCATION:
        view = new UIEducationListView(parent);
        break;
    case VIEW_WORK:
        view = new UIWorkListView(parent);
        break;
    case VIEW_COUNTRY:
        view = new UICountryListView(parent);
        break;
    case VIEW_ETHNIC:
        view = new UIEthnicListView(parent);
        break;
    case VIEW_ROLE:
        view = new UIRoleListView(parent);
        break;
    case VIEW_COURSE:
        view = new UICourseListView(parent);
        break;
    default:
        break;
    }

    if (nullptr != view)
        view->setupUI();

    traceout;
    return view;
}
