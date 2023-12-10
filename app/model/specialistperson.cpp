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

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "modeldefs.h"

SpecialistPerson::SpecialistPerson():MapDbModel()
{
    tracein;
    mSpecialist = nullptr;
    traceout;
}

SpecialistPerson::~SpecialistPerson()
{
    tracein;
    FREE_PTR(mSpecialist);
    traceout;
}

DbModel *SpecialistPerson::build()
{
    return new SpecialistPerson();
}

DbModelBuilder SpecialistPerson::getBuilder() const
{
    return &SpecialistPerson::build;
}

void SpecialistPerson::_copyData(const DbModel& model)
{
    tracein;
    const SpecialistPerson* item = static_cast<const SpecialistPerson*>(&model);
    setSpecialist(item->specialist());

    setExperienceHistory(item->experienceHistory());

    traceout;

}

QString SpecialistPerson::modelName() const
{
    return KModelNameSpecialistPerson;
}


DbModelHandler *SpecialistPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlSpecialist);
}

const DbModel *SpecialistPerson::specialist() const
{
    return mSpecialist;
}

void SpecialistPerson::setSpecialist(const DbModel *newSpecialist)
{
    tracein;
    FREE_PTR(mSpecialist);
    mSpecialist = CLONE_DBMODEL(newSpecialist);
    traceout;
}

QString SpecialistPerson::specialistName() const
{
    tracein;
    QString name;
    if (mSpecialist) {
        name = mSpecialist->name();
    } else {
        name = "";
    }
    traceout;
    return name;
}


const QString &SpecialistPerson::experienceHistory() const
{
    return mExperienceHistory;
}

void SpecialistPerson::setExperienceHistory(const QString &newExperienceHistory)
{
    CHECK_MODIFIED_THEN_SET(mExperienceHistory, newExperienceHistory, KItemExperience);
}

void SpecialistPerson::setPersonUid(const QString &uid)
{
    setUid1(uid);
}

void SpecialistPerson::setSpecialistUid(const QString &uid)
{
    setUid2(uid);
}

QString SpecialistPerson::specialistUid()
{
    return uid2();
}
