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
 * Filename: specialistperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:11/3/2022
 * Brief:
 */
#include "specialistperson.h"

#include "logger.h"
#include "errcode.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"

SpecialistPerson::SpecialistPerson()
{
    traced;
}

DbModel *SpecialistPerson::build()
{
    return new SpecialistPerson();
}


QString SpecialistPerson::modelName() const
{
    return KModelNameSpecialistPerson;
}


DbModelHandler *SpecialistPerson::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlSpecialist);
}

const QString &SpecialistPerson::experienceHistory() const
{
    return mExperienceHistory;
}

void SpecialistPerson::setExperienceHistory(const QString &newExperienceHistory)
{
    mExperienceHistory = newExperienceHistory;
}

void SpecialistPerson::setPersonUid(const QString &uid)
{
    setUid1(uid);
}

void SpecialistPerson::setSpecialistUid(const QString &uid)
{
    setUid2(uid);
}
