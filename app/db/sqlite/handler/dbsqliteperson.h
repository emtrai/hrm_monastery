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
 * Filename: dbsqliteperson.h
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#ifndef DBSQLITEPERSON_H
#define DBSQLITEPERSON_H

#include "dbsqlitemodelhandler.h"
#include "dbpersonmodelhandler.h"
#include <QHash>
#include "utils.h"
#include "table/dbsqlitetbl.h"
#include "errcode.h"

class DbSqlitePersonTbl;
class Person;
class DbSqlitePerson : public DbSqliteModelHandler, public DbPersonModelHandler
{
    GET_INSTANCE_DECL(DbSqlitePerson);
public:
    DbSqlitePerson();
    virtual const QString getName();
    virtual ErrCode add(DbModel* model, bool notifyDataChange = true);
//    virtual ErrCode update(DbModel* model);
    virtual ErrCode add2Table(DbModel* model, DbSqliteTbl* tbl);

    /**
     * @brief delete by change status to delete
     * @param model
     * @return
     */
    virtual ErrCode deleteSoft(DbModel* model);
    /**
     * @brief delete completely from db
     * @param model
     * @return
     */
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);

    virtual bool exist(const DbModel* edu);
    virtual QList<DbModel*> getAll(DbModelBuilder builder, qint64 status = DB_RECORD_ACTIVE,
                                    const char* modelName = nullptr, int from = 0,
                                    int noItems = 0, int* total = nullptr);
    /**
     * @brief Get All, return as dictionary, map b/w uid and model
     * @param builder
     * @param modelName
     * @return
     */
    virtual QHash<QString, DbModel*> getAllInDict(DbModelBuilder builder, qint64 status = DB_RECORD_ACTIVE, const char* modelName = nullptr);

    virtual DbModel* getModel(qint64 dbId);

    virtual ErrCode addEvent(const QString& personUid, const QString& eventUid,
                             qint64 date, const QString& title, const QString& remark);
    virtual ErrCode getListEvents(const QString& personUid,
                                               QList<DbModel*>& list,
                                               const QString* eventUid = nullptr,
                                               qint64 date = 0);
    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual ErrCode search(const QString& keyword,
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);
    virtual ErrCode filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       const char* targetModelName = nullptr,
                       const DbModel* parentModel = nullptr,
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    virtual QList<DbModel*> getSpecialistList(const QString& personUid);

    virtual ErrCode updateCommunity(const QString& personUid, const QString& communityUid);
    // TODO: mapping community & person stored in person tbl and community&person mapping table
    // risk of inconsistant data
    // this function will try to get list of person in community using data (communityuid) in person table
    virtual ErrCode getListPersonInCommunity(const QString& communityUid, qint32 status, QList<DbModel*>& list);
    virtual ErrCode getListCommunitesOfPerson(const QString& perUid, qint32 modelStatus, QList<CommunityPerson*>& list);

    virtual ErrCode update(DbModel* model, bool notifyDataChange = true);
    virtual ErrCode update(DbModel* model,
                           const QHash<QString, QString> &inFields,
                           const QString& tableName, bool notifyDataChange = true);

    virtual ErrCode check2UpdateCommunity(Person* per);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbModelBuilder getMainBuilder();
    virtual DbSqliteTbl* getTable(const QString& modelName);
private:

};

#endif // DBSQLITEPERSON_H
