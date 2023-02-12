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
 * Filename: dbdepartmentmodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:10/24/2022
 * Brief:
 */
#ifndef DBCOMMDEPATMODELHANDLER_H
#define DBCOMMDEPATMODELHANDLER_H

#include "dbmodelhandler.h"
#include <QList>
class CommunityDept;
class PersonDept;
class DbCommDeptModelHandler
{
public:
    virtual QList<DbModel*> getListPerson(const QString& deptUid) = 0;
    virtual QList<DbModel*> getListDept(const QString& communityUid) = 0;
    virtual ErrCode addPerson2Department(PersonDept* perDept) = 0;
};

#endif // DBCOMMDEPATMODELHANDLER_H
