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
 * Filename: departctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#ifndef DEPARTCTL_H
#define DEPARTCTL_H

#include "modelcontroller.h"
#include "dbmodel.h"
#include "department.h"
#include <QHash>
#include <QList>
#include "utils.h"

#define DEPART (DepartCtl::getInstance())

class DepartCtl : public ModelController
{
    GET_INSTANCE_DECL(DepartCtl);
private:
    DepartCtl();
protected:
    // prebuilt data file name
    virtual const char* getPrebuiltFileName();
    // prebuilt data file type, i.e. csv
    virtual const char* getPrebuiltFileType();

    virtual DbModelBuilder getMainBuilder();
};

#endif // DEPARTCTL_H
