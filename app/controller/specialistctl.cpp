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
 * Filename: specialistctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "specialistctl.h"

#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "specialist.h"
#include "dbctl.h"
#include "dbspecialistmodelhandler.h"
#include "prebuiltdefs.h"

#define SPLIT ','

GET_INSTANCE_CONTROLLER_IMPL(SpecialistCtl);


SpecialistCtl::SpecialistCtl():
    ModelController(KModelHdlSpecialist)
{
    tracein;
}

SpecialistCtl::~SpecialistCtl()
{
    tracein;
}

DbModelBuilder SpecialistCtl::getMainBuilder()
{
    return &Specialist::build;
}

const char *SpecialistCtl::getPrebuiltFileName()
{
    return KPrebuiltSpecialistCSVFileName;
}


QList<DbModel *> SpecialistCtl::getListPerson(const QString &specialistUid)
{
    tracein;
    QList<DbModel*> list;
    logd("get list person of specialist uid '%s'", STR2CHA(specialistUid));
    if (!specialistUid.isEmpty()) {
        DbSpecialistModelHandler* hdl = dynamic_cast<DbSpecialistModelHandler*>(DB->getSpecialistModelHandler());
        if (hdl) {
            list =  hdl->getListPerson(specialistUid);
        } else {
            loge("Not found handler");
        }
    } else {
        loge("Invalid arg");
    }
    logd("No. item: %lld", list.count());
    traceout;
    return list;
}

