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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "personctl.h"
#include "dbctl.h"
#include "logger.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "person.h"

PersonCtl* PersonCtl::gInstance = nullptr;

ErrCode PersonCtl::addPerson(Person *person)
{
    traced;
    ErrCode_t err = ErrNone;
    // TODO: check null
    err = person->save();

    return err;
}

ErrCode PersonCtl::addPerson(const QString &fname)
{
    traced;
    Person *person = new Person();
    ErrCode ret = person->fromCSVFile(fname);

    if (ret == ErrNone){
        ret = person->save();
    }

    delete person;
    tracedr(ret);
    return ret;
}

ErrCode PersonCtl::AddListPersons(const QString &fname)
{
    traced;

}

PersonCtl::PersonCtl()
{

}


PersonCtl::~PersonCtl()
{
    traced;
}


PersonCtl *PersonCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new PersonCtl();
    }

    return gInstance;
}

void PersonCtl::onLoad()
{
    traced;
}
