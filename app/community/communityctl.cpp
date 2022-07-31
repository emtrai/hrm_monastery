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
 * Created date:7/16/2022
 * Brief:
 */
#include "communityctl.h"
#include "std.h"

CommunityCtl* CommunityCtl::gInstance = nullptr;

CommunityCtl::CommunityCtl()
{

}

CommunityCtl *CommunityCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new CommunityCtl;
    }
    return gInstance;
}

void CommunityCtl::onLoad()
{
    traced;
}

ErrCode CommunityCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}

ErrCode CommunityCtl::loadFromDb()
{
    traced;

}
