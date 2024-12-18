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
 * Filename: eductl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "eductl.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "education.h"
#include "dbctl.h"
#include <QList>
#include "prebuiltdefs.h"
#include "controllerdefs.h"
#include "modeldefs.h"

#define SPLIT ','


GET_INSTANCE_CONTROLLER_IMPL(EduCtl);

EduCtl::EduCtl():ModelController(KControllerEdu, KModelHdlEdu)
{
    tracein;
}

DbModelBuilder EduCtl::getMainBuilder()
{
    return &Education::build;
}

const char *EduCtl::getPrebuiltFileName()
{
    return KPrebuiltEduCSVFileName;
}

