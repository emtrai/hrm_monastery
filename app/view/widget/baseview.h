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
    VIEW_COMMUNITIES_OF_PERSON_LIST, // list of communities of person
    VIEW_SAINT,
    VIEW_COMMUNITY,
    VIEW_AREA,
    VIEW_AREA_PERSON,
    VIEW_PEOPLE_IN_COMMUNITY_LIST, // list of people of community
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
    VIEW_PERSON_EVENT,
    VIEW_MAX,
    };

typedef std::function<ErrCode(void* data)> ActionFunc_t;
typedef std::function<ErrCode(void* data)> FreeDataFunc_t;

/**
 * @brief Base view class for content UI of program
 */
class BaseView
{
public:
    BaseView();
    ~BaseView();
    /**
     * @brief Setup UI
     */
    virtual void setupUI() {};

    /**
     * @brief return view widget itself
     * @return
     */
    virtual QWidget* getWidget() = 0;

    /**
     * @brief data kept by view, caller just use, not free or have any change on value
     * @return
     */
    void *data() const;

    /**
     * @brief set data
     * Data will be freed by freecb if set, else, caller must free data after use
     * @param newData   data to be set, accept null,mean clear current data
     * @param freecb    callback to free data, called in destructor
     */
    void setData(void *newData, FreeDataFunc_t freecb = nullptr);

    virtual void onStopped();
    virtual void onPaused();
    virtual void onShown();
    /**
     * @brief view is ready to fetch data or not
     *        in some case, view needs to do some initialization before it can fetch
     *        data to update.
     * @return true if view is ready to fetch data
     */
    virtual bool ready2FetchData();
    /**
     * @brief view type \ref ViewType of view
     * @return \ref ViewType
     */
    virtual int getViewType() = 0;
protected:
    void* mData;
    ActionFunc_t mShowActionFunc;// action to run when view show
    FreeDataFunc_t mFreeFunc;
    bool mShowActionFuncRunOnce; // TODO: support other mode? run once, run on resume, run on pause, etc.???
    bool mIsShown; // should define more state, but for now, I'm lazy to do that, true/false is enough
};

#endif // BASEVIEW_H
