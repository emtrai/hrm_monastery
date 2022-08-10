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
 * Filename: province.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#include "province.h"
#include "logger.h"
#include "errcode.h"

Province::Province()
{

}

const QString &Province::id() const
{
    return mId;
}

void Province::setId(const QString &newId)
{
    mId = newId;
}

const QString &Province::name() const
{
    return mName;
}

void Province::setName(const QString &newName)
{
    mName = newName;
}


bool Province::isValid()
{
    return !(name().isEmpty() || id().isEmpty());
}

void Province::dump()
{
    // TODO: dump to stdout, sdderr or file???
#ifdef DEBUG_TRACE

    logd("DUMP Province:");
    logd("- id %s", id().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    // TODO: province???
#endif //DEBUG_TRACE
}

