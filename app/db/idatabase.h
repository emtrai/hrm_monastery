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
 * Filename: idatabase.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef IDATABASE_H
#define IDATABASE_H

//#include "person.h"
#include "dbinfo.h"
#include "errcode.h"
#include "dbmodelhandler.h"

class Person;
class Community;
class Saint;

// THIS IS ONE INTERFACE FOR ALL OPERATION RELATING TO DB
// TODO: should separate person, community, saint, etc. into separate class????
// It's quite stupid when putting everything here
class IDatabase
{
public:
    IDatabase();


    /**
    * Add person to datablase
    *
    */
//    virtual ErrCode_t addPerson(const Person* person) = 0;

    /**
    * Load database
    */
    virtual ErrCode_t loadDb(const DbInfo* dbInfo) = 0;



    virtual DbModelHandler* getEduModelHandler() = 0;
    virtual DbModelHandler* getSaintModelHandler() = 0;
    virtual DbModelHandler* getSpecialistModelHandler() = 0;
    virtual DbModelHandler* getCommunityModelHandler() = 0;

    virtual DbModelHandler* getModelHandler(const QString& name) = 0;
};

#endif // IDATABASE_H
