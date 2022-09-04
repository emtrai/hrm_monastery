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
 * Filename: work.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "work.h"
#include "logger.h"
#include "errcode.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

Work::Work()
{
    traced;
}

Work::~Work()
{
    traced;
}

DbModel *Work::builder()
{
    traced;
    return new Work();
}

DbModelHandler *Work::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlWork);
}

const QString &Work::remark() const
{
    return mRemark;
}

void Work::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}
