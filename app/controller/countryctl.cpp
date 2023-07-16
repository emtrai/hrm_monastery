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
 * Filename: countryctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/13/2022
 * Brief:
 */
#include "countryctl.h"
#include "logger.h"
#include "country.h"
#include "defs.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(CountryCtl)

CountryCtl::CountryCtl():ModelController(KControllerCountry, KModelHdlCountry)
{
    tracein;
}

CountryCtl::~CountryCtl()
{
    tracein;
}

DbModelBuilder CountryCtl::getMainBuilder()
{
    return &Country::build;
}

const char *CountryCtl::getPrebuiltFileName()
{
    return KPrebuiltCountryCSVFileName;
}


