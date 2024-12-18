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
 * Filename: event.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/10/2022
 * Brief:
 */
#include "event.h"

#include "dbctl.h"
#include "dbmodel.h"
#include "modeldefs.h"
Event::Event():DbModel()
{
    traced;
}

DbModel *Event::build()
{
    Event* model = new Event();
    model->init();
    return model;
}

DbModelBuilder Event::getBuilder() const
{
    return &Event::build;
}

QString Event::modelName() const
{
    return KModelNameEvent;
}


DbModelHandler *Event::getDbModelHandler() const
{

    return DB->getModelHandler(KModelHdlEvent);
}
