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
 * Filename: education.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "education.h"

#include "crypto.h"


#include "dbctl.h"
#include "logger.h"


Education::Education()
{
    tracein;
}

DbModel *Education::build()
{
    Education* model = new Education();
    model->init();
    return model;
}

DbModelBuilder Education::getBuilder() const
{
    return &Education::build;
}

DbModelHandler *Education::getDbModelHandler() const
{
    return DbCtl::getInstance()->getDb()->getEduModelHandler();
}

