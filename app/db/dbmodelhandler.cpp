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
 * Filename: dbmodelhandler.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "dbmodelhandler.h"
#include "logger.h"
#include "errcode.h"
DbModelHandler::DbModelHandler()
{

}

int DbModelHandler::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    loge("Default search one, do nothing");
    return 0;
}

int DbModelHandler::searchAll(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    loge("Default search one, do nothing");
    return 0;
}

DbModel *DbModelHandler::getByName(const QString &name, const DbModelBuilder &builder)
{
    traced;
    loge("Default one, do nothing");
    return nullptr;
}

DbModel *DbModelHandler::getByName(const QString &name)
{
    traced;
    loge("Default one, do nothing");
    return nullptr;
}

