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
 * Filename: education.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef EDUCATION_H
#define EDUCATION_H

#include <QObject>
#include "dbmodel.h"

/**
 * @brief Education level - Trinh do hoc van
 */
class Education: public DbModel
{
public:
    Education();
    virtual ~Education();
public:
    static DbModel *build();
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;

protected:
    virtual DbModelHandler *getDbModelHandler() const;
};

#endif // EDUCATION_H
