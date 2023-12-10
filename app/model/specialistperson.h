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
 * Filename: specialistperson.h
 * Author: Anh, Ngo Huy
 * Created date:11/3/2022
 * Brief:
 */
#ifndef SPECIALISTPERSON_H
#define SPECIALISTPERSON_H

#include "mapdbmodel.h"
#include "specialist.h"

class SpecialistPerson : public MapDbModel
{
public:
    SpecialistPerson();
    virtual ~SpecialistPerson();
    static DbModel* build();
    virtual DbModelBuilder getBuilder() const;


    virtual QString modelName() const;
    const QString &experienceHistory() const;
    void setExperienceHistory(const QString &newExperienceHistory);
    void setPersonUid(const QString& uid);
    void setSpecialistUid(const QString& uid);
    QString specialistUid();
    QString specialistName() const;

    const DbModel *specialist() const;
    void setSpecialist(const DbModel *newSpecialist);

protected:
    virtual DbModelHandler *getDbModelHandler() const;
    virtual void _copyData(const DbModel& model);
protected:
    QString mExperienceHistory;
    DbModel* mSpecialist;
};

#endif // SPECIALISTPERSON_H
