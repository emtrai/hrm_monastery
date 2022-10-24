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
 * Filename: department.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "department.h"

#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"


Department::Department()
{

}

DbModel *Department::builder()
{
    return new Department();
}

QString Department::modelName() const
{
    return KModelNameDepartment;
}

qint64 Department::parentDbId() const
{
    return mParentDbId;
}

void Department::setParentDbId(qint64 newParentDbId)
{
    mParentDbId = newParentDbId;
}

const QString &Department::parentUid() const
{
    return mParentUid;
}

void Department::setParentUid(const QString &newParentUid)
{
    mParentUid = newParentUid;
}

qint64 Department::communityDbId() const
{
    return mCommunityDbId;
}

void Department::setCommunityDbId(qint64 newCommunityDbId)
{
    mCommunityDbId = newCommunityDbId;
}

const QString &Department::remark() const
{
    return mRemark;
}

void Department::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}

qint64 Department::hoDPersonId() const
{
    return mHoDPersonId;
}

void Department::setHoDPersonId(qint64 newHoDPersonId)
{
    mHoDPersonId = newHoDPersonId;
}

qint64 Department::establishDate() const
{
    return mEstablishDate;
}

void Department::setEstablishDate(qint64 newEstablishDate)
{
    mEstablishDate = newEstablishDate;
}

void Department::setEstablishDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("create date string '%s'", date.toStdString().c_str());
    mEstablishDate = Utils::dateFromString(date, format);
    logd("mEstablishDate %ll", mEstablishDate);
}

const QString &Department::email() const
{
    return mEmail;
}

void Department::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
}

const QString &Department::addr() const
{
    return mAddr;
}

void Department::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
}

const QString &Department::tel() const
{
    return mTel;
}

void Department::setTel(const QString &newTel)
{
    mTel = newTel;
}

const QString &Department::brief() const
{
    return mBrief;
}

void Department::setBrief(const QString &newBrief)
{
    mBrief = newBrief;
}

qint64 Department::status() const
{
    return mStatus;
}

void Department::setStatus(qint64 newStatus)
{
    mStatus = newStatus;
}

DbModelHandler *Department::getDbModelHandler()
{

    return DB->getModelHandler(KModelHdlDept);
}

const QString &Department::communityName() const
{
    return mCommunityName;
}

void Department::setCommunityName(const QString &newCommunityName)
{
    mCommunityName = newCommunityName;
}

const QString &Department::communityUid() const
{
    return mCommunityUid;
}

void Department::setCommunityUid(const QString &newCommunityUid)
{
    mCommunityUid = newCommunityUid;
}

const QString &Department::shortName() const
{
    return mShortName;
}

void Department::setShortName(const QString &newShortName)
{
    mShortName = newShortName;
}
