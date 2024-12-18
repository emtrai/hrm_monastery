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
    virtual ~DbSqliteCommunity();
    /**
     * @brief add model to db
     * @param model
     * @return ErrNone on success, error code otherwise
     */
//    virtual ErrCode add(DbModel* model, bool notifyDataChange = true);
    virtual ErrCode add(DbModel* model, bool notifyDataChange = true);
    virtual ErrCode update(DbModel* model, bool notifyDataChange = true);
    virtual ErrCode update(DbModel* model,
                           const QHash<QString, QString> &inFields,
                           const QString& tableName, bool notifyDataChange = true);
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);

    // TODO: mapping community & person stored in person tbl and community&person mapping table
    // risk of inconsistant data
    /**
     * @brief Get List of people in community. Caller must free data after use
     * @param communityUid
     * @return list of Person
     */
    virtual QList<Person*> getListPerson(const QString& communityUid,
                                           int modelStatus = MODEL_STATUS_MAX,
                                          const QString* perStatusUid = nullptr);
    /**
     * @brief getListCommunityPerson
     * @param commUid
     * @param modelStatus
     * @param perStatusUid
     * @return List of CommunityPerson objects
     */
    virtual QList<DbModel*> getListCommunityPerson(const QString& commUid,
                                                    int modelStatus = MODEL_STATUS_MAX,
                                                    const QString* perStatusUid = nullptr);

    /**
     * @brief getListCommunityPerson
     * @param commUid
     * @param modelStatus
     * @param perStatusUid
     * @return List of CommunityPerson objects
     */
    virtual QList<DbModel*> getListCommunityPersonOfPerson(const QString& perUid,
                                                    int modelStatus = MODEL_STATUS_MAX);
    virtual QList<DbModel*> getListActiveCommunityPersonOfPerson(const QString& perUid);

    /**
     * @brief Add person to community
     * @param comm target community
     * @param per
     * @param status model status of mapping (active, inactive,etc.)
     * @param startdate start date
     * @param enddate end date
     * @param remark
     * @return
     */
    virtual ErrCode addPerson2Community(const Community *comm,
                                      const Person* per,
                                      bool updateCommPer = true, // update mapping comm & per
                                      int status = MODEL_STATUS_ACTIVE,
                                      qint64 startdate = 0,
                                      qint64 enddate = 0,
                                      const QString& remark = nullptr,
                                      bool notifyDbChange = true);
    /**
     * @brief Root/Top level community
     * @return Community model, null if not exist
     */
    virtual const Community* getRootCommunity();

    /**
     * @brief Get manager list of community
     * @param communityUid
     * @param outList list of CommunityManager model
     * @param modelStatus
     * @return
     */
    virtual ErrCode getManagersList(const QString &communityUid, QList<DbModel *> &outList,
                                    qint64 modelStatus = MODEL_STATUS_MAX);
    virtual ErrCode getManagersListWithRole(const QString &communityUid,
                                            QList<DbModel *> &outList,
                                            const QString& roleUid,
                                            qint64 modelStatus = MODEL_STATUS_MAX);
    virtual ErrCode getAllManagersList(QList<DbModel *> &outList,
                               qint64 modelStatus = MODEL_STATUS_MAX);
    virtual ErrCode getCEOList(const QString &communityUid, QList<DbModel *> &outList,
                       qint64 modelStatus = MODEL_STATUS_MAX);
    /**
     * @brief get current CEO
     * @param communityUid
     * @param ceo
     * @param isActiveCEO
     * @return ErrNoData if not found CEO, ErrNone if found
     */
    virtual ErrCode getCurrentCEO(const QString &communityUid,
                                  Person** ceo = nullptr,
                                  bool* isActiveCEO = nullptr);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();
    virtual ErrCode check2UpdateCEO(DbModel* comm);
private:
    Community* mRootCommunity;
};

#endif // DBSQLITECOMMUNITY_H
