/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dbmodelfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/5/2023
 * Brief:
 */
#include "dbmodelfactory.h"
#include "logger.h"

DbModelFactory* DbModelFactory::mInstance = nullptr;

DbModelFactory *DbModelFactory::getInstance()
{
    if (!mInstance) {
        mInstance = new DbModelFactory();
        if (mInstance) {
            mInstance->addBuilders();
        } else {
            loge("No memory?");
        }
    }
    return mInstance;
}

DbModelFactory::DbModelFactory()
{
    traced;
}

DbModelFactory::~DbModelFactory()
{
    traced;
}

DbModelBuilder DbModelFactory::_getBuilder(const QString &modelName)
{
    tracein;
    DbModelBuilder builder = nullptr;
    logd("modelName %s", STR2CHA(modelName));

    if (mList.contains(modelName)){
        builder = mList.value(modelName);
    } else {
        loge("model name '%s' not exist to get builder", STR2CHA(modelName));
    }
    traceout;
    return builder;
}

DbModelBuilder DbModelFactory::getBuilder(const QString &modelName)
{
    return getInstance()->_getBuilder(modelName);
}

#include "area.h"
#include "areaperson.h"
#include "person.h"
#include "community.h"
#include "specialist.h"
void DbModelFactory::addBuilders()
{
    tracein;
    _addBuilder(KModelNameArea, &Area::build);
    _addBuilder(KModelNameAreaPerson, &AreaPerson::build);
    _addBuilder(KModelNamePerson, &Person::build);
    _addBuilder(KModelHdlSpecialist, &Specialist::build);
    _addBuilder(KModelNameCommunity, &Community::build);

    traceout;
}

ErrCode DbModelFactory::_addBuilder(const QString &name, DbModelBuilder builder)
{
    tracein;
    ErrCode err = ErrNone;
    if (name.isEmpty() || !builder) {
        err = ErrInvalidArg;
        loge("name or build null/empty");
    }
    if (err == ErrNone && !mList.contains(name)) {
        logd("add new builder for model '%s'", STR2CHA(name));
        mList.insert(name, builder);
    }
    traceret(err);
    return err;
}
