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
#include "utils.h"
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
#include "areactl.h"
#include "departctl.h"
#include "coursectl.h"
#include "workctl.h"
#include "statusctl.h"
#include "eventctl.h"
#include "rolectl.h"

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
    add2Loader(SAINTCTL);
    add2Loader(EduCtl::getInstance());
    add2Loader(SpecialistCtl::getInstance());
    add2Loader(CountryCtl::getInstance());
#ifndef SKIP_PERSON_PROVINE
    add2Loader(ProvinceCtl::getInstance());
#endif
    add2Loader(EthnicCtl::getInstance());
    add2Loader(MissionCtl::getInstance());
    add2Loader(PersonCtl::getInstance());
    add2Loader(INSTANCE(CourseCtl));
    add2Loader(INSTANCE(WorkCtl));
    add2Loader(INSTANCE(StatusCtl));
    add2Loader(INSTANCE(StatusCtl));
    add2Loader(INSTANCE(EventCtl));
    add2Loader(CommunityCtl::getInstance());
    add2Loader(AreaCtl::getInstance());
    add2Loader(DepartCtl::getInstance());
    add2Loader(RoleCtl::getInstance());
}

void LoaderCtl::onLoad()
{
    traced;
    if (mListener) mListener->onStart();
    foreach(  Controller* ctl, mListCtl )
        ctl->onLoad();
    // TODO: call on separate thread?
    // TODO: timeout???
    if (mListener != nullptr){
        mListener->onFinish(ErrNone, mListenerData);
    }
}

void LoaderCtl::setOnFinishLoadListener(LoaderListener* listener, void* data){
    mListener = listener;
    mListenerData = data;
    // TODO: check if mListener exist or make list of listener???
}
