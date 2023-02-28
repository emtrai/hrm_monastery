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
 * Filename: countryctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/13/2022
 * Brief:
 */
#ifndef COUNTRYCTL_H
#define COUNTRYCTL_H

#include "controller.h"
#include "dbmodel.h"
#include "country.h"
#include "commonctl.h"
#include "utils.h"

#define COUNTRYCTL (CountryCtl::getInstance())

class CountryCtl : public CommonCtl
{
    GET_INSTANCE_DECL(CountryCtl);
protected:
    CountryCtl();
    virtual ~CountryCtl();
    virtual DbModelBuilder getMainBuilder();
protected:
    virtual const char *getPrebuiltFileName();
};

#endif // COUNTRYCTL_H
