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
 * Filename: dbspecialistmodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:11/4/2022
 * Brief:
 */
#ifndef DBSPECIALISTMODELHANDLER_H
#define DBSPECIALISTMODELHANDLER_H

#include <QString>

class DbModel;

class DbSpecialistModelHandler
{
public:
    virtual QList<DbModel*> getListPerson(const QString& specialistUid) = 0;

};

#endif // DBSPECIALISTMODELHANDLER_H
