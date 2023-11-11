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
 * Filename: rolectl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "rolectl.h"

#include "logger.h"
#include "role.h"
#include "utils.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(RoleCtl)

RoleCtl::RoleCtl():
    ModelController(KControllerRole, KModelHdlRole)
{
    tracein;
}

RoleCtl::~RoleCtl()
{
    tracein;
}

Role *RoleCtl::getCEORole()
{
    logd("get ceo role default role '%s'", KRoleCEODefaultNameId);
    return static_cast<Role*>(getModelHandler()->getByNameId(KRoleCEODefaultNameId));
}

const char *RoleCtl::getPrebuiltFileName()
{
    return KPrebuiltRoleCSVFileName;
}

DbModelBuilder RoleCtl::getMainBuilder()
{
    return &Role::build;
}
