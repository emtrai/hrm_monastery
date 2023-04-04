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
 * Filename: statusctl.h
 * Author: Anh, Ngo Huy
 * Created date:9/6/2022
 * Brief:
 */
#ifndef PERSONSTATUSCTL_H
#define PERSONSTATUSCTL_H

#include "modelcontroller.h"
#include "utils.h"
#include <QList>

class PersonStatus;

class PersonStatusCtl : public ModelController
{
    GET_INSTANCE_DECL(PersonStatusCtl);
private:
    PersonStatusCtl();
protected:
    // prebuilt data file name
    virtual const char* getPrebuiltFileName();

    virtual DbModelBuilder getMainBuilder();
};

#endif // PERSONSTATUSCTL_H