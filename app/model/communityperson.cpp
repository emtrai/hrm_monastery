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

CommunityPerson::CommunityPerson()
{
    tracein;
}

CommunityPerson::~CommunityPerson()
{
    tracein;
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


DbModelHandler *CommunityPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCommunity);
}

DbModel *CommunityPerson::community() const
{
    return mCommunity;
}

void CommunityPerson::setCommunity(const DbModel *newCommunity)
{
    tracein;
    CLEAR_THEN_SET(mCommunity, newCommunity, DbModel);
    traceout;
}

DbModel *CommunityPerson::person() const
{
    return mPerson;
}

void CommunityPerson::setPerson(const DbModel *newPerson)
{
    tracein;
    CLEAR_THEN_SET(mPerson, newPerson, DbModel);
    traceout;
}
