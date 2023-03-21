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
 * Filename: country.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#include "country.h"
#include "logger.h"
#include "errcode.h"
#include "province.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "provincectl.h"


Country::Country()
{
    traced;
}

Country::Country(const Country &country):DbModel(country)
{
    traced;
}
Country::~Country()
{
    traced;
}

DbModelBuilder Country::getBuilder()
{
    return &Country::build;
}


DbModelHandler *Country::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCountry);
}

DbModel *Country::build()
{
    Country* model = new Country();
    model->init();
    return model;
}

