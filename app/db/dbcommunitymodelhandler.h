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
 * Filename: dbcommunitymodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:10/10/2022
 * Brief:
 */
#ifndef DBCOMMUNITYMODELHANDLER_H
#define DBCOMMUNITYMODELHANDLER_H

#include "dbmodelhandler.h"
#include <QList>

class Person;
class Community;

class DbCommunityModelHandler
{
public:

    virtual QList<DbModel*> getListPerson(const QString& uid) = 0;
    virtual ErrCode addPerson2Community(const Community *comm,
                                        const Person* per,
                                      int status = 0,
                                      qint64 startdate = 0,
                                      qint64 enddate = 0,
                                      const QString& remark = nullptr) = 0;
};

#endif // DBCOMMUNITYMODELHANDLER_H