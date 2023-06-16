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
 * Filename: dbsqlitecommunity.h
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#ifndef DBSQLITECOMMUNITY_H
#define DBSQLITECOMMUNITY_H

#include "dbsqlitemodelhandler.h"
#include "dbcommunitymodelhandler.h"

#include "utils.h"

class DbSqliteCommunityTbl;
class Community;
class Person;

class DbSqliteCommunity : public DbSqliteModelHandler, public DbCommunityModelHandler
{
    GET_INSTANCE_DECL(DbSqliteCommunity);
public:
    DbSqliteCommunity();
    /**
     * @brief add model to db
     * @param model
     * @return ErrNone on success, error code otherwise
     */
    virtual ErrCode add(DbModel* model, bool notifyDataChange = true);
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);

    // TODO: mapping community & person stored in person tbl and community&person mapping table
    // risk of inconsistant data
    /**
     * @brief Get List of people in community. Caller must free data after use
     * @param communityUid
     * @return list of Person
     */
    virtual QList<Person*> getListPerson(const QString& communityUid,
                                           int modelStatus = MODEL_STATUS_MAX, const QString* perStatusUid = nullptr);
    virtual QList<CommunityPerson*> getListCommunityPerson(const QString& commUid, int modelStatus = MODEL_STATUS_MAX, const QString* perStatusUid = nullptr);

    /**
     * @brief Add person to community
     * @param comm
     * @param per
     * @param status
     * @param startdate
     * @param enddate
     * @param remark
     * @return
     */
    virtual ErrCode addPerson2Community(const Community *comm,
                                      const Person* per,
                                      int status = 0,
                                      qint64 startdate = 0,
                                      qint64 enddate = 0,
                                      const QString& remark = nullptr,
                                      bool notifyDbChange = true);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();

};

#endif // DBSQLITECOMMUNITY_H
