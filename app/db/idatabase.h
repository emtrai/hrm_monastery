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
    * Load database
    */
    virtual ErrCode_t loadDb(const DbInfo* dbInfo) = 0;
    virtual ErrCode validateDbInfo(const DbInfo* dbInfo) = 0;

    /**
     * @brief check if db exist
     * @return
     */
    virtual bool isDbExist() = 0;


    virtual DbModelHandler* getEduModelHandler() = 0;
    virtual DbModelHandler* getSaintModelHandler() = 0;
    virtual DbModelHandler* getSpecialistModelHandler() = 0;
    virtual DbModelHandler* getCommunityModelHandler() = 0;
    virtual DbModelHandler* getPersonModelHandler() = 0;
    virtual DbModelHandler* getDepartmentModelHandler() = 0;
    virtual DbModelHandler* getWorkModelHandler() = 0;
    virtual DbModelHandler* getCountryModelHandler() = 0;
    virtual DbModelHandler* getPersonStatusModelHandler() = 0;
    virtual DbModelHandler* getCourseModelHandler() = 0;
    virtual DbModelHandler* getEthnicModelHandler() = 0;

    virtual DbModelHandler* getModelHandler(const QString& name) = 0;

    virtual quint64 getDbVersion() = 0;
    virtual quint64 getDbVersionInMetadata(bool* ok = nullptr) = 0;
    virtual ErrCode updateDbVersionInMetadata(qint64 value) = 0;
    virtual ErrCode getMetadataValue(const QString& key, QString& value) = 0;
    virtual ErrCode getMetadataValue(const QString& key, qint64& value) = 0;
    virtual ErrCode updateMetadataValue(const QString& key, qint64 value) = 0;
    virtual ErrCode updateMetadataValue(const QString& key, const QString& value) = 0;
    virtual quint64 getCurrentPersonCodeNumber(bool* ok = nullptr) = 0;
    virtual quint64 getDbSeqNumber(const QString& tblName, bool* ok = nullptr) = 0;

    /**
     * @brief find avaible nameid which does not exist in db yet
     * @param initNameId initial nameid to search
     * @param availableNameId
     * @return ErrNone to succeed or error code
     */
    virtual ErrCode getAvailableNameId(const QString& modelName,
                                       const QString &initNameId, QString &availableNameId) = 0;
};

#endif // IDATABASE_H
