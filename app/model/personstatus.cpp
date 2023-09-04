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
 * Filename: status.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/5/2022
 * Brief:
 */
#include "personstatus.h"
#include "logger.h"
#include "errcode.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

PersonStatus::PersonStatus():DbModel()
{
    traced;
}
DbModel *PersonStatus::build()
{
    PersonStatus* model = new PersonStatus();
    model->init();
    return model;
}

DbModelBuilder PersonStatus::getBuilder() const
{
    return &PersonStatus::build;
}

DbModelHandler *PersonStatus::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlPersonStatus);
}
