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
 * Filename: loader.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "loaderctl.h"
#include "dbctl.h"
#include "logger.h"
#include "errcode.h"
#include "personctl.h"
#include "communityctl.h"
#include "saintctl.h"
#include "location.h"
#include "eductl.h"
#include "specialistctl.h"
#include "countryctl.h"
#include "provincectl.h"
#include "ethnicctl.h"
#include "missionctl.h"

LoaderCtl* LoaderCtl::gInstance = nullptr;

LoaderCtl::LoaderCtl()
{
    mListener = nullptr;
}


LoaderCtl* LoaderCtl::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new LoaderCtl();
        gInstance->registerAll();
    }
    return gInstance;
}

void LoaderCtl::add2Loader( Controller* ctl)
{
    traced;
    mListCtl.append(ctl);
}

void LoaderCtl::registerAll()
{
    traced;
    add2Loader(DbCtl::getInstance());
    add2Loader(Location::getInstance());
    add2Loader(SaintCtl::getInstance());
    add2Loader(EduCtl::getInstance());
    add2Loader(SpecialistCtl::getInstance());
    add2Loader(CountryCtl::getInstance());
    add2Loader(ProvinceCtl::getInstance());
    add2Loader(EthnicCtl::getInstance());
    add2Loader(MissionCtl::getInstance());
    add2Loader(PersonCtl::getInstance());
    add2Loader(CommunityCtl::getInstance());
}

void LoaderCtl::onLoad()
{
    traced;
    foreach(  Controller* ctl, mListCtl )
        ctl->onLoad();
    // TODO: call on separate thread?
    // TODO: timeout???
    if (mListener != nullptr){
        mListener(ERR_NONE, mListenerData);
    }
}

void LoaderCtl::setOnFinishLoadListener(OnFinishLoadListener_t listener, void* data){
    mListener = listener;
    mListenerData = data;
    // TODO: check if mListener exist or make list of listener???
}
