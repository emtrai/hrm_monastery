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

#include "stringdefs.h"
#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "person.h"
#include "community.h"
#include "filectl.h"
#include "prebuiltdefs.h"
#include "modeldefs.h"

CommunityPerson::CommunityPerson():MapDbModel(),
    mPerson(nullptr),
    mCommunity(nullptr)
{
    traced;
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

void CommunityPerson::_copyData(const DbModel& model)
{
    tracein;
    logd("clone person & community");
    const CommunityPerson* commper = static_cast<const CommunityPerson*>(&model);
    setPerson(commper->person());
    setCommunity(commper->community());
    traceout;
}

QString CommunityPerson::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltCommunityOfPersonTemplateName;
}

void CommunityPerson::initExportFields()
{

    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemPersonNameId, EXPORT_CALLBACK_STRING_IMPL(
                                              CommunityPerson,
                                              KModelNameCommPerson,
                                              person()->nameId));
    mExportCallbacks.insert(KItemFullName, EXPORT_CALLBACK_STRING_IMPL(
                                                CommunityPerson,
                                                KModelNameCommPerson,
                                                person()->displayName));
    mExportCallbacks.insert(KItemCommunityNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                 CommunityPerson,
                                                 KModelNameCommPerson,
                                                 community()->nameId));
    mExportCallbacks.insert(KItemCommunityName, EXPORT_CALLBACK_STRING_IMPL(
                                                    CommunityPerson,
                                                    KModelNameCommPerson,
                                                    community()->name));
    mExportCallbacks.insert(KItemStartDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                                    CommunityPerson,
                                                    KModelNameCommPerson,
                                                    startDate,
                                                    DEFAULT_FORMAT_YMD));
    mExportCallbacks.insert(KItemEndDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                                CommunityPerson,
                                                KModelNameCommPerson,
                                                endDate,
                                                DEFAULT_FORMAT_YMD));
    mExportCallbacks.insert(KItemStatus, EXPORT_CALLBACK_STRING_IMPL(
                                             CommunityPerson,
                                             KModelNameCommPerson,
                                             modelStatusName));
    mExportCallbacks.insert(KItemRemark, EXPORT_CALLBACK_STRING_IMPL(
                                             CommunityPerson,
                                             KModelNameCommPerson,
                                             remark));
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
    setCommunityUid(KUidNone);
    if (mCommunity && communityUid().isEmpty()) {
        setCommunityUid(mCommunity->uid());
    }
    traceout;
}

QString CommunityPerson::toString() const
{
    QString str = MapDbModel::toString();
    str += QString(":commuid('%1')")
               .arg(community()?community()->uid():"null");
    str += QString(":commnameid('%1')")
               .arg(community()?community()->nameId():"null");
    str += QString(":peruid('%1')")
               .arg(person()?person()->uid():"null");
    str += QString(":personnameid('%1')")
               .arg(person()?person()->nameId():"null");
    return str;
}

Person *CommunityPerson::person() const
{
    return mPerson;
}

void CommunityPerson::setPerson(const Person *newPerson)
{
    tracein;
    CLEAR_THEN_SET(mPerson, newPerson, Person);
    setPersonUid(KUidNone);
    if (mPerson && personUid().isEmpty()) {
        setPersonUid(mPerson->uid());
    }
    traceout;
}
