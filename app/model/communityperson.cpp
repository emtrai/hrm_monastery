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
 * Filename: communityperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/16/2022
 * Brief:
 */
#include "communityperson.h"
#include "logger.h"
#include "errcode.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "person.h"
#include "community.h"

CommunityPerson::CommunityPerson():
    mPerson(nullptr),
    mCommunity(nullptr)
{
    tracein;
}

CommunityPerson::~CommunityPerson()
{
    tracein;
    FREE_PTR(mPerson);
    FREE_PTR(mCommunity);
    traceout;
}

DbModel *CommunityPerson::build()
{
    CommunityPerson* item = new CommunityPerson();
    item->init();
    return item;
}

DbModelBuilder CommunityPerson::getBuilder() const
{
    return &CommunityPerson::build;
}


QString CommunityPerson::modelName() const
{
    return KModelNameCommPerson;
}

void CommunityPerson::clone(const DbModel *model)
{
    tracein;
    if (model) {
        MapDbModel::clone(model);
        if (model->modelName() == modelName()) {
            CommunityPerson* commper = (CommunityPerson*) model;
            CLEAR_THEN_SET(mPerson, commper->person(), Person);
            CLEAR_THEN_SET(mCommunity, commper->community(), Community);
        }
    }
    traceout;
}

void CommunityPerson::setCommunityUid(const QString &newUid1)
{
    if (newUid1 != uid1()) {
        setUid1(newUid1);
        markItemAsModified(KItemCommunity);
    }
}

void CommunityPerson::setPersonUid(const QString &newUid1)
{
    if (newUid1 != uid2()) {
        setUid2(newUid1);
        markItemAsModified(KItemPerson);
    }
}


DbModelHandler *CommunityPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCommunity);
}

Community *CommunityPerson::community() const
{
    return mCommunity;
}

void CommunityPerson::setCommunity(const Community *newCommunity)
{
    tracein;
    CLEAR_THEN_SET(mCommunity, newCommunity, Community);
    if (mCommunity && communityUid().isEmpty()) {
        setCommunityUid(mCommunity->uid());
    }
    traceout;
}

Person *CommunityPerson::person() const
{
    return mPerson;
}

void CommunityPerson::setPerson(const Person *newPerson)
{
    tracein;
    CLEAR_THEN_SET(mPerson, newPerson, Person);
    if (mPerson && personUid().isEmpty()) {
        setPersonUid(mPerson->uid());
    }
    traceout;
}
