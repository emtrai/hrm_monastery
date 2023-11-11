/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: communitymanager.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#include "communitymanager.h"
#include "logger.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "person.h"
#include "role.h"
#include "course.h"
#include "community.h"
#include "prebuiltdefs.h"
#include "stringdefs.h"

CommunityManager::CommunityManager():MapDbModel(),
    mPerson(nullptr),
    mCommunity(nullptr),
    mRole(nullptr),
    mCourse(nullptr)
{
    traced;
}

CommunityManager::~CommunityManager()
{
    tracein;
    FREE_PTR(mPerson);
    FREE_PTR(mCommunity);
    FREE_PTR(mRole);
    FREE_PTR(mCourse);
    traceout;
}

DbModel *CommunityManager::build()
{
    CommunityManager* item = new CommunityManager();
    item->init();
    return item;
}

DbModelBuilder CommunityManager::getBuilder() const
{
    return &CommunityManager::build;
}


QString CommunityManager::modelName() const
{
    return KModelNameCommManager;
}

void CommunityManager::clone(const DbModel *model)
{
    tracein;
    if (model) {
        MapDbModel::clone(model);
        if (model->modelName() == modelName()) {
            logd("clone person & community");
            CommunityManager* commper = (CommunityManager*) model;
            CLEAR_THEN_SET(mPerson, commper->person(), Person);
            CLEAR_THEN_SET(mCommunity, commper->community(), Community);
            CLEAR_THEN_SET(mRole, commper->role(), Role);
            CLEAR_THEN_SET(mCourse, commper->course(), Course);
        }
    }
    traceout;
}

QString CommunityManager::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltCommunityManagerExportTemplateName;
}

void CommunityManager::initExportFields()
{

    tracein;
    MapDbModel::initExportFields();
    mExportCallbacks.insert(KItemPersonNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                   CommunityManager,
                                                   KModelNameCommManager,
                                                   person()->nameId));

    mExportCallbacks.insert(KItemFullName, EXPORT_CALLBACK_STRING_IMPL(
                                               CommunityManager,
                                               KModelNameCommManager,
                                               person()->displayName));
    mExportCallbacks.insert(KItemCommunityNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                      CommunityManager,
                                                      KModelNameCommManager,
                                                      community()->nameId));
    mExportCallbacks.insert(KItemCommunityName, EXPORT_CALLBACK_STRING_IMPL(
                                                    CommunityManager,
                                                    KModelNameCommManager,
                                                    community()->name));
    mExportCallbacks.insert(KItemRole, EXPORT_CALLBACK_STRING_IMPL(
                                                    CommunityManager,
                                                    KModelNameCommManager,
                                                    roleName));
    mExportCallbacks.insert(KItemTerm, EXPORT_CALLBACK_STRING_IMPL(
                                           CommunityManager,
                                           KModelNameCommManager,
                                           courseName));
    mExportCallbacks.insert(KItemEmail, EXPORT_CALLBACK_STRING_IMPL(
                                           CommunityManager,
                                           KModelNameCommManager,
                                            personEmail));
    mExportCallbacks.insert(KItemTel, EXPORT_CALLBACK_STRING_IMPL(
                                            CommunityManager,
                                            KModelNameCommManager,
                                            personTel));
    traceout;
}

void CommunityManager::setCommunityUid(const QString &newUid1)
{
    if (newUid1 != uid1()) {
        setUid1(newUid1);
        markItemAsModified(KItemCommunity);
    }
}

void CommunityManager::setPersonUid(const QString &newUid1)
{
    if (newUid1 != uid2()) {
        setUid2(newUid1);
        markItemAsModified(KItemPerson);
    }
}


DbModelHandler *CommunityManager::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCommunity);
}

Role *CommunityManager::role() const
{
    return mRole;
}

void CommunityManager::setRole(Role *newRole)
{
    tracein;
    if (!((mRole == newRole) ||
        (newRole && mRole && (newRole->uid() == mRole->uid())))) {
        markItemAsModified(KItemRole);
    }
    CLEAR_THEN_SET(mRole, newRole, Role);
    traceout;
}

QString CommunityManager::roleUid() const
{
    return mRole?mRole->uid():"";
}

QString CommunityManager::roleName() const
{
    return mRole?mRole->name():"";

}

Course *CommunityManager::course() const
{
    return mCourse;
}

void CommunityManager::setCourse(Course *newCourse)
{
    tracein;
    if (!((newCourse == mCourse) ||
        (newCourse && mCourse && (newCourse->uid() == mCourse->uid())))) {
        markItemAsModified(KItemCourse);
    }
    CLEAR_THEN_SET(mCourse, newCourse, Course);
    traceout;
}

QString CommunityManager::courseUid() const
{
    return mCourse?mCourse->uid():"";
}

QString CommunityManager::courseName() const
{
    return mCourse?mCourse->name():"";

}

Community *CommunityManager::community() const
{
    return mCommunity;
}

void CommunityManager::setCommunity(const Community *newCommunity)
{
    tracein;
    CLEAR_THEN_SET(mCommunity, newCommunity, Community);
    if (mCommunity && communityUid().isEmpty()) {
        setCommunityUid(mCommunity->uid());
    }
    traceout;
}

QString CommunityManager::communityNameId() const
{
    return mCommunity?mCommunity->nameId():"";
}

QString CommunityManager::toString() const
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
    str += QString(":rolenameid('%1')")
               .arg(role()?role()->nameId():"null");
    str += QString(":coursenameid('%1')")
               .arg(course()?course()->nameId():"null");
    return str;
}

Person *CommunityManager::person() const
{
    return mPerson;
}

void CommunityManager::setPerson(const Person *newPerson)
{
    tracein;
    CLEAR_THEN_SET(mPerson, newPerson, Person);
    if (mPerson && personUid().isEmpty()) {
        setPersonUid(mPerson->uid());
    }
    traceout;
}

QString CommunityManager::personNameId() const
{
    return mPerson?mPerson->nameId():"";
}

QString CommunityManager::personName() const
{
    return mPerson?mPerson->displayName():"";

}

QString CommunityManager::personEmail() const
{
    return mPerson?mPerson->emailString():"";

}

QString CommunityManager::personTel() const
{
    return mPerson?mPerson->telString():"";

}
