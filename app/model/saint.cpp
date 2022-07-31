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
 * Filename: saint.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "saint.h"
#include "std.h"
#include <QString>
#include <QStringLiteral>
#include "utils.h"
#include "crypto.h"
#include "idbsaint.h"
#include "dbctl.h"

Saint::Saint()
{

}

QString Saint::nameid() const
{
    QString hash = Crypto::hashString(name());
    return hash;
}

const QString &Saint::name() const
{
    return mName;
}

void Saint::setName(const QString &newName)
{
    mName = newName;
}

qint64 Saint::feastDay() const
{
    return mFeastDay;
}

void Saint::setFeastDay(qint64 newFeastDay)
{
    mFeastDay = newFeastDay;
}

Gender Saint::gender() const
{
    return mGender;
}

void Saint::setGender(Gender newGender)
{
    mGender = newGender;
}

const QString &Saint::history() const
{
    return mHistory;
}

void Saint::setHistory(const QString &newHistory)
{
    mHistory = newHistory;
}

const QString &Saint::country() const
{
    return mCountry;
}

void Saint::setCountry(const QString &newCountry)
{
    mCountry = newCountry;
}

const QString &Saint::fullName() const
{
    return mFullName;
}

void Saint::setFullName(const QString &newFullName)
{
    mFullName = newFullName;
}

bool Saint::isValid()
{
    // TODO: add more checking for valid info
    return !name().isEmpty();
}

void Saint::dump()
{
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE

    logd("DUMP SAINT:");
    logd("- NameId %s", nameid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- FullName %s", fullName().toStdString().c_str());
    logd("- gender %d", gender());
    logd("- feastDay %lld (%s)", feastDay(),
         Utils::date2String(feastDay(), QString()).toStdString().c_str());
#endif //DEBUG_TRACE

}

QString Saint::toString()
{
    return QString("%1,%2").arg(nameid(), name());
}

ErrCode Saint::save()
{
    traced;
    ErrCode ret = ErrNone;
    IDbSaint* dbSaint = DbCtl::getInstance()->dbSaint();
    if (dbSaint != nullptr){
        ret = dbSaint->addSaint(this);
        if (ret == ErrExisted){ // alrady exist, judge as ok
            ret = ErrNone;
            logi("%s already exist", toString().toStdString().c_str());
        }
    }
    else{
        ret = ErrDbNotReady;
        loge("DbSaint not ready");
    }
    return ret;
}
