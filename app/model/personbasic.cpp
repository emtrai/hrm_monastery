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
 * Filename: personbasic.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "personbasic.h"
#include "logger.h"
#include "errcode.h"
#include "address.h"

PersonBasic::PersonBasic()
    :mBirthday(0)
{

}

const QString &PersonBasic::firstName() const
{
    return mFirstName;
}

void PersonBasic::setFirstName(const QString &newFirstName)
{
    mFirstName = newFirstName;
}

Address *PersonBasic::permanentAddr() const
{
    return mPermanentAddr;
}

void PersonBasic::setPermanentAddr(Address *newPermanentAddr)
{
    mPermanentAddr = newPermanentAddr;
}

Address *PersonBasic::livingAddr() const
{
    return mLivingAddr;
}

void PersonBasic::setLivingAddr(Address *newLivingAddr)
{
    mLivingAddr = newLivingAddr;
}

const QString &PersonBasic::birthPlace() const
{
    return mBirthPlace;
}

void PersonBasic::setBirthPlace(const QString &newBirthPlace)
{
    mBirthPlace = newBirthPlace;
}

qint64 PersonBasic::birthday() const
{
    return mBirthday;
}

void PersonBasic::setBirthday(qint64 newBirthday)
{
    mBirthday = newBirthday;
}

const QString &PersonBasic::lastName() const
{
    return mLastName;
}

void PersonBasic::setLastName(const QString &newLastName)
{
    mLastName = newLastName;
}


ErrCode PersonBasic::setNameFromFullName(const QString &name)
{
    QStringList items = name.trimmed().split(" ", Qt::SkipEmptyParts);
    //    QStringList parts;
    //    foreach (QString item, items){
    //        QString tmp = item.simplified();
    //        if (!tmp.isEmpty()){
    //            parts.append(item);
    //        }
    //    }

    if (items.length() > 0){
        mFirstName = items.last();
        items.removeLast();
    }
    mLastName = items.join(" ");

    return ErrNone;
}



QString PersonBasic::getFullName() const{
    //TODO:check localization
    return mLastName + " " + mFirstName;
}
