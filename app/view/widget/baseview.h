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
 * Filename: baseview.h
 * Author: Anh, Ngo Huy
 * Created date:12/17/2022
 * Brief:
 */
#ifndef BASEVIEW_H
#define BASEVIEW_H
#include "errcode.h"
#include <QWidget>
enum ViewType {
    NONE = 0,
    VIEW_TEXT_BROWSER,
    VIEW_PERSON,
    VIEW_SAINT,
    VIEW_COMMUNITY,
    VIEW_AREA,
    VIEW_AREA_PERSON,
    VIEW_COMMUNITY_PERSON,
    VIEW_COMMUNITY_DEPT,
    VIEW_DEPARTMENT,
    VIEW_DEPARTMENT_PERSON,
    VIEW_ROLE,
    VIEW_MISSION,
    VIEW_SPECIALIST,
    VIEW_EDUCATION,
    VIEW_WORK,
    VIEW_COUNTRY,
    VIEW_ETHNIC,
    VIEW_COURSE,
    VIEW_SUMMARY,
    VIEW_MAX,
    };

typedef std::function<ErrCode(void* data)> ActionFunc_t;


class BaseView
{
public:
    virtual void setupUI() {};
    virtual QWidget* getWidget() = 0;
    void *data() const;
    void setData(void *newData);

protected:
    virtual int getViewType() = 0;
protected:
    void* mData;
    ActionFunc_t mShowActionFunc;// action to run when view show
    bool mShowActionFuncRunOnce; // TODO: support other mode? run once, run on resume, run on pause, etc.???
};

#endif // BASEVIEW_H
