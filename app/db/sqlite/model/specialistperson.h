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

class SpecialistPerson : public MapDbModel
{
public:
    SpecialistPerson();
    static DbModel* build();
    virtual DbModelBuilder getBuilder();
    virtual QString modelName() const;
    const QString &experienceHistory() const;
    void setExperienceHistory(const QString &newExperienceHistory);
    void setPersonUid(const QString& uid);
    void setSpecialistUid(const QString& uid);
protected:
    virtual DbModelHandler *getDbModelHandler();
protected:
    QString mExperienceHistory;
};

#endif // SPECIALISTPERSON_H
