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
 * Filename: specialist.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#include "specialist.h"
#include "logger.h"
#include "dbctl.h"
#include "modeldefs.h"

Specialist::Specialist():DbModel()
{
    traced;
}

Specialist::~Specialist()
{
    traced;
}

DbModel *Specialist::build()
{
    Specialist* model = new Specialist();
    model->init();
    return model;
}

DbModelBuilder Specialist::getBuilder() const
{
    return &Specialist::build;
}

QString Specialist::modelName() const
{
    return KModelNameSpecialist;
}

DbModelHandler *Specialist::getDbModelHandler() const
{
    return DbCtl::getInstance()->getDb()->getSpecialistModelHandler();
}


