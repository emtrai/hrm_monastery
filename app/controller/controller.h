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
 * Filename: controller.h
 * Author: Anh, Ngo Huy
 * Created date:3/6/2023
 * Brief:
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include "errcode.h"

#define GET_INSTANCE_CONTROLLER_IMPL(className) \
className* className::gInstance = nullptr;\
    className* className::getInstance() { \
        if (gInstance == nullptr){ \
            gInstance = new className(); \
            gInstance->init();\
    } \
        return gInstance; \
}

class Controller
{
public:
    virtual void init();
    /**
     * @brief on load controller
     * @return
     */
    virtual ErrCode onLoad() = 0;

    /**
     * @brief Name of controller
     * @return
     */
    virtual QString getName() = 0;
};

#endif // CONTROLLER_H
