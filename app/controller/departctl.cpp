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
 * Filename: departctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#include "departctl.h"

#include "logger.h"
#include "department.h"
#include "dbcommdepatmodelhandler.h"
#include "defs.h"
#include "dbctl.h"
#include "utils.h"
#include <QFile>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "prebuiltdefs.h"
#include "controllerdefs.h"
#include "modeldefs.h"

GET_INSTANCE_CONTROLLER_IMPL(DepartCtl)

DepartCtl::DepartCtl():ModelController(KControllerDept, KModelHdlDept)
{
    tracein;
}

DbModelBuilder DepartCtl::getMainBuilder()
{
    return &Department::build;
}

const char *DepartCtl::getPrebuiltFileName()
{
    return KPrebuiltDeptJsonFileName;
}

const char *DepartCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
}
