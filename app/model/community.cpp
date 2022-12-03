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
 * Filename: community.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "community.h"

#include "dbctl.h"
#include "logger.h"
#include "utils.h"

DbModel *Community::builder()
{
    return new Community();
}

Community::Community()
{

}

Community::Community(const Community &obj):DbModel(obj)
{
    setAddr(obj.addr());
}

Community::~Community()
{
    traced;
}

qint32 Community::level() const
{
    return mLevel;
}

void Community::setLevel(qint32 newLevel)
{
    mLevel = newLevel;
}

qint64 Community::createDate() const
{
    return mCreateDate;
}

void Community::setCreateDate(qint64 newCreateDate)
{
    mCreateDate = newCreateDate;
}

void Community::setCreateDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("create date string '%s'", date.toStdString().c_str());
    mCreateDate = Utils::dateFromString(date, format);
    logd("mCreateDate %ll", mCreateDate);
}

const QString &Community::parentUid() const
{
    return mParentUid;
}

void Community::setParentUid(const QString &newParentUid)
{
    mParentUid = newParentUid;
}

CommunityStatus Community::getStatus() const
{
    return status;
}

void Community::setStatus(CommunityStatus newStatus)
{
    status = newStatus;
}

//ErrCode_t Community::save()
//{
//    return DbCtl::getInstance()->dbCommunity()->add(this);
//}

//void Community::dump()
//{
//    traced;
//    QString log = QStringLiteral(
//                      "name '%1'\n"
//                      )
//                      .arg(name());
//    logd("%s", log.toStdString().c_str());
//}

qint64 Community::closeDate() const
{
    return mCloseDate;
}

void Community::setCloseDate(qint64 newCloseDate)
{
    mCloseDate = newCloseDate;
}

DbModelHandler *Community::getDbModelHandler()
{
    return DbCtl::getDb()->getCommunityModelHandler();
}

const QString &Community::communityCode() const
{
    return mCommunityCode;
}

void Community::setCommunityCode(const QString &newCommunityCode)
{
    mCommunityCode = newCommunityCode;
}

const QString &Community::areaName() const
{
    return mAreaName;
}

void Community::setAreaName(const QString &newAreaName)
{
    mAreaName = newAreaName;
}

qint64 Community::areaDbId() const
{
    return mAreaDbId;
}

void Community::setAreaDbId(qint64 newAreaDbId)
{
    mAreaDbId = newAreaDbId;
}

const QString &Community::areaUid() const
{
    return mAreaUid;
}

void Community::setAreaUid(const QString &newAreaUid)
{
    mAreaUid = newAreaUid;
}

const QString &Community::imgPath() const
{
    return mImgPath;
}

void Community::setImgPath(const QString &newImgPath)
{
    mImgPath = newImgPath;
}

qint64 Community::feastDate() const
{
    return mFeastDate;
}

void Community::setFeastDate(qint64 newFeastDate)
{
    mFeastDate = newFeastDate;
}

void Community::setFeastDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("mFeastDate date string '%s'", date.toStdString().c_str());
    mFeastDate = Utils::dateFromString(date, format);
    logd("mFeastDate %ll", mFeastDate);

}

bool Community::isValid()
{
    // TODO: check more and more;
    return !name().isEmpty() && !uid().isEmpty();
}

void Community::dump()
{
    traced;
#ifdef DEBUG_TRACE

    logd("DUMP:");
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- Addr %s", addr().toStdString().c_str());
    logd("- Feastday %s", Utils::date2String(mFeastDate).toStdString().c_str());
#endif //DEBUG_TRACE
}

const QString &Community::email() const
{
    return mEmail;
}

void Community::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
}

const QString &Community::tel() const
{
    return mTel;
}

void Community::setTel(const QString &newTel)
{
    mTel = newTel;
}

const QString &Community::country() const
{
    return mCountry;
}

void Community::setCountry(const QString &newCountry)
{
    mCountry = newCountry;
}

const QString &Community::province() const
{
    return mProvince;
}

void Community::setProvince(const QString &newProvince)
{
    mProvince = newProvince;
}

const QString &Community::addr() const
{
    return mAddr;
}

void Community::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
}

const QString &Community::church() const
{
    return mChurch;
}

void Community::setChurch(const QString &newChurch)
{
    mChurch = newChurch;
}
