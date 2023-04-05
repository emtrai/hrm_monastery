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
 * Filename: dbinfo.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/20/2022
 * Brief:
 */
#include "dbinfo.h"
#include "logger.h"
#include "errcode.h"
DbInfo::DbInfo()
{

}

const QString &DbInfo::uri() const
{
    return mUri;
}

void DbInfo::setUri(const QString &newUri)
{
    mUri = newUri;
}

const QString &DbInfo::username() const
{
    return mUsername;
}

void DbInfo::setUsername(const QString &newUsername)
{
    mUsername = newUsername;
}

const QString &DbInfo::password() const
{
    return mPassword;
}

void DbInfo::setPassword(const QString &newPassword)
{
    mPassword = newPassword;
}

int DbInfo::fromJson(const QString& jsonString){
    // TODO:
    return ErrNone;
}
QString DbInfo::toJsonString(){
    //TODO:
    return QString();
}

const QString &DbInfo::metaUri() const
{
    return mMetaUri;
}

void DbInfo::setMetaUri(const QString &newMetaUri)
{
    mMetaUri = newMetaUri;
}
