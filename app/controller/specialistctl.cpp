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

#define SPLIT ','

GET_INSTANCE_IMPL(SpecialistCtl);


SpecialistCtl::SpecialistCtl():
    Controller(KModelHdlSpecialist)
{

}

DbModel *SpecialistCtl::buildModel(void *items, const QString &fmt)
{
    Specialist* specialist = new Specialist();
    QStringList* itemList = (QStringList*) items;
    QString item = itemList->join(SPLIT);
    qint32 idx = 0;
    logd("Add item %s to list", item.toStdString().c_str());
    specialist->setNameId(itemList->at(idx++));
    specialist->setName(itemList->at(idx++));
    tracede;
    return specialist;
}


DbModelBuilder SpecialistCtl::getMainBuilder()
{
    return &Specialist::build;
}

const char *SpecialistCtl::getPrebuiltFileName()
{
    return KPrebuiltSpecialistCSVFileName;
}

const char *SpecialistCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}


QList<DbModel *> SpecialistCtl::getListPerson(const QString &specialistUid)
{
    logd("get list person of specialist uid %s", specialistUid.toStdString().c_str());
    DbSpecialistModelHandler* hdl = dynamic_cast<DbSpecialistModelHandler*>(DB->getSpecialistModelHandler());
    QList<DbModel*> list =  hdl->getListPerson(specialistUid);
    logd("No. item: %d", list.count());
    return list;
}

