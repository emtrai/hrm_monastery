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
 * Filename: baseview.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/17/2022
 * Brief:
 */
#include "baseview.h"
#include "logger.h"


BaseView::BaseView():
    mData(nullptr),
    mShowActionFunc(nullptr),
    mFreeFunc(nullptr),
    mShowActionFuncRunOnce(false)
{
    traced;
}

BaseView::~BaseView()
{
    tracein;
    if (mFreeFunc && mData) {
        mFreeFunc(mData);
    }
    mData = nullptr;
    traceout;
}

void *BaseView::data() const
{
    return mData;
}

void BaseView::setData(void *newData, FreeDataFunc_t freecb)
{
    tracein;
    // free old data if exist
    if (mData && mFreeFunc) {
        mFreeFunc(mData);
    }
    if (!newData) {
        logd("reset data");
    }
    mData = newData;
    mFreeFunc = freecb;
    traceout;
}
