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

DbModel *Community::builder()
{
    return new Community();
}

Community::Community()
{

}

Community::~Community()
{
    traced;
}


const QString &Community::name() const
{
    return mName;
}

void Community::setName(const QString &newName)
{
    mName = newName;
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
