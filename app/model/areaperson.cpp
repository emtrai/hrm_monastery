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
 * Filename: areaperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#include "areaperson.h"

#include "logger.h"
#include "errcode.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"

AreaPerson::AreaPerson():mArea(nullptr), mPerson(nullptr)
{
    tracein;
}

AreaPerson::~AreaPerson()
{
    tracein;
    if (mPerson) delete mPerson;
    if (mArea) delete mArea;
    traceout;
}

DbModel *AreaPerson::build()
{
    return new AreaPerson();
}

void AreaPerson::clone(const DbModel *model)
{
    tracein;
    if (model) {
        copy(*(AreaPerson*)model);
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

DbModelBuilder AreaPerson::getBuilder() const
{
    return &AreaPerson::build;
}


QString AreaPerson::modelName() const
{
    return KModelNameAreaPerson;
}


DbModelHandler *AreaPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlArea);
}

void AreaPerson::copy(const AreaPerson &model)
{
    tracein;
    mRoleUid = model.mRoleUid;
    if (mPerson) delete mPerson;
    mPerson = model.mPerson->clone();
    if (mArea) delete mArea;
    mArea = model.mArea->clone();
    traceout;
}

const QString &AreaPerson::roleName()
{
    if (mRoleName.isEmpty() && !mRoleUid.isEmpty()) {
        logd("no role name, but has uid, update it");
        DbModel* model = getDbModelHandler()->getByUid(mRoleUid);
        if (model) {
            mRoleName = model->name();
            delete model;
        } else {
            logw("role uid '%s' not found", STR2CHA(mRoleUid));
        }
    }
    return mRoleName;
}

void AreaPerson::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}


DbModel *AreaPerson::area() const
{
    return mArea;
}

void AreaPerson::setArea(DbModel *newArea)
{
    mArea = newArea;
}

DbModel *AreaPerson::person() const
{
    return mPerson;
}

void AreaPerson::setPerson(DbModel *newPerson)
{
    mPerson = newPerson;
}

const QString &AreaPerson::roleUid() const
{
    return mRoleUid;
}

void AreaPerson::setRoleUid(const QString &newRoleUid)
{
    mRoleUid = newRoleUid;
    markItemAsModified(KItemRole);
}
