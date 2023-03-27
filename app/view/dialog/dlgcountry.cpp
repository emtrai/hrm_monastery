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
 * Filename: dlgcountry.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/28/2022
 * Brief:
 */
#include "dlgcountry.h"
#include "ui_dlgcountry.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "country.h"
#include "countryctl.h"

DlgCountry::DlgCountry(QWidget *parent) :
    DlgEditModel(parent)
{
    tracein;
}

DlgCountry::~DlgCountry()
{
    tracein;
}

DbModel *DlgCountry::newModel()
{
    return Country::build();
}


