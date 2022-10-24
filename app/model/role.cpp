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
 * Filename: role.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "role.h"

#include "logger.h"
#include "errcode.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

Role::Role()
{
    traced;
}
DbModel *Role::builder()
{
    traced;
    return new Role();
}

QString Role::modelName() const
{
    return KModelNameRole;
}

const QString &Role::remark() const
{
    return mRemark;
}

void Role::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}

DbModelHandler *Role::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlRole);
}
