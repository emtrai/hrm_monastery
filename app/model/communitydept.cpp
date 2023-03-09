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
 * Filename: communitydept.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/31/2023
 * Brief:
 */
#include "communitydept.h"

#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"


CommunityDept::CommunityDept():DbModel()
{
    traced;
    mEstablishDate = 0;
    mStatus = 0;
}

DbModel *CommunityDept::build()
{
    CommunityDept* model = new CommunityDept();
    model->init();
    return model;
}

DbModelBuilder CommunityDept::getBuilder()
{
    return &CommunityDept::build;
}

QString CommunityDept::modelName() const
{
    return KModelNameCommDept;
}

const QString &CommunityDept::name() const
{
    return mName.isEmpty()?mDepartmentName:mName;
}

qint64 CommunityDept::establishDate() const
{
    return mEstablishDate;
}

void CommunityDept::setEstablishDate(qint64 newEstablishDate)
{
    mEstablishDate = newEstablishDate;
}

void CommunityDept::setEstablishDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("create date string '%s'", date.toStdString().c_str());
    mEstablishDate = Utils::dateFromString(date, format);
    logd("mEstablishDate %ll", mEstablishDate);
}

const QString &CommunityDept::email() const
{
    return mEmail;
}

void CommunityDept::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
}

const QString &CommunityDept::addr() const
{
    return mAddr;
}

void CommunityDept::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
}

const QString &CommunityDept::tel() const
{
    return mTel;
}

void CommunityDept::setTel(const QString &newTel)
{
    mTel = newTel;
}

const QString &CommunityDept::brief() const
{
    return mBrief;
}

void CommunityDept::setBrief(const QString &newBrief)
{
    mBrief = newBrief;
}

qint64 CommunityDept::status() const
{
    return mStatus;
}

void CommunityDept::setStatus(qint64 newStatus)
{
    mStatus = newStatus;
}

DbModelHandler *CommunityDept::getDbModelHandler()
{

    return DB->getModelHandler(KModelHdlCommDept);
}

qint64 CommunityDept::closedDate() const
{
    return mClosedDate;
}

void CommunityDept::setClosedDate(qint64 newClosedDate)
{
    mClosedDate = newClosedDate;
}

void CommunityDept::setClosedDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("close date string '%s'", date.toStdString().c_str());
    mClosedDate = Utils::dateFromString(date, format);
    logd("mClosedDate %ll", mClosedDate);
}


const QString & CommunityDept::communityName() const
{
    return mCommunityName;
}

void CommunityDept::setCommunityName(const QString & newCommunityName)
{
    mCommunityName = newCommunityName;
}

const QString &CommunityDept::departmentName() const
{
    return mDepartmentName;
}

void CommunityDept::setDepartmentName(const QString &newDepartmentName)
{
    mDepartmentName = newDepartmentName;
}

qint64 CommunityDept::communityDbId() const
{
    return mCommunityDbId;
}

void CommunityDept::setCommunityDbId(qint64 newCommunityDbId)
{
    mCommunityDbId = newCommunityDbId;
}

qint64 CommunityDept::departmentDbId() const
{
    return mDepartmentDbId;
}

void CommunityDept::setDepartmentDbId(qint64 newDepartmentDbId)
{
    mDepartmentDbId = newDepartmentDbId;
}

const QString &CommunityDept::departmentUid() const
{
    return mDepartmentUid;
}

void CommunityDept::setDepartmentUid(const QString &newDepartmentUid)
{
    mDepartmentUid = newDepartmentUid;
}

const QString &CommunityDept::communityUid() const
{
    return mCommunityUid;
}

void CommunityDept::setCommunityUid(const QString &newCommunityUid)
{
    mCommunityUid = newCommunityUid;
}

