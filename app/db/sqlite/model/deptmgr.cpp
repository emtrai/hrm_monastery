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
 * Filename: deptmgr.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "deptmgr.h"
#include "logger.h"
#include "errcode.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"

DeptMgr::DeptMgr()
{
    traced;
}

DbModel *DeptMgr::build()
{
    return new DeptMgr();
}


QString DeptMgr::modelName() const
{
    return KModelNameDeptMgr;
}


DbModelHandler *DeptMgr::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlDept);
}

const QString &DeptMgr::termUid() const
{
    return mTermUid;
}

void DeptMgr::setTermUid(const QString &newTermUid)
{
    mTermUid = newTermUid;
}

const QString &DeptMgr::roleUid() const
{
    return mRoleUid;
}

void DeptMgr::setRoleUid(const QString &newRoleUid)
{
    mRoleUid = newRoleUid;
}
