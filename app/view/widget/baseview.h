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


typedef std::function<ErrCode(void* data)> ActionFunc_t;


class BaseView
{
public:
    BaseView();
    virtual int getViewType() = 0;
//    virtual ErrCode runAction() = 0;
protected:
    ActionFunc_t mShowActionFunc;// action to run when view show
    bool mShowActionFuncRunOnce; // TODO: support other mode? run once, run on resume, run on pause, etc.???
};

#endif // BASEVIEW_H
