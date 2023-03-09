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
 * Filename: workctl.h
 * Author: Anh, Ngo Huy
 * Created date:9/2/2022
 * Brief:
 */
#ifndef WORKCTL_H
#define WORKCTL_H

#include "modelcontroller.h"
#include "utils.h"
#include <QList>

#define WORKCTL INSTANCE(WorkCtl)

class Work;

class WorkCtl : public ModelController
{
    GET_INSTANCE_DECL(WorkCtl);
public:
    WorkCtl();
    virtual ~WorkCtl();
protected:
    virtual DbModelBuilder getMainBuilder();
    virtual const char *getPrebuiltFileName();
public:
    // COMMON FUNCTIONS
};

#endif // WORKCTL_H
