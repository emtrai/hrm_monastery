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
 * Filename: dbsqlitearea.h
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#ifndef DBSQLITEAREA_H
#define DBSQLITEAREA_H

#include "dbsqlitemodelhandler.h"
#include "dbareamodelhandler.h"
#include "utils.h"

class DbSqliteTbl;

class DbSqliteArea : public DbSqliteModelHandler, public DbAreaModelHandler
{
    GET_INSTANCE_DECL(DbSqliteArea);
public:
    DbSqliteArea();
    virtual ~DbSqliteArea();
    /**
     * @brief Get List of contact people, caller must free resource after use
     * @param areaUid
     * @param status \ref DbModelStatus
     * @return AreaPerson model
     */
    virtual QList<DbModel*> getListContactPeople(const QString& areaUid, int status = MODEL_STATUS_MAX);

    /**
     * @brief get list of communities of area
     * @param areaUid
     * @param status
     * @return list of \ref Community
     */
    virtual QList<DbModel*> getListCommunities(const QString& areaUid, int status = MODEL_STATUS_MAX);

    /**
     * @brief Add contact person to db
     * @param area Area which persone belong to
     * @param person Person to be added
     * @param status status of contact person, i.e active (can contact), inactive (cannot contact)
     * @param roleUid role of person
     * @param startdate
     * @param enddate
     * @param remark
     * @return
     */
    virtual ErrCode addContactPerson(const DbModel* area,
                                     const DbModel* person,
                                    int status = MODEL_STATUS_MAX,
                                    const QString& roleUid = nullptr,
                                    qint64 startdate = 0,
                                    qint64 enddate = 0,
                                    const QString& remark = nullptr);
    /**
     * @brief delete completely from db, can be area, ara-person model, etc.
     * @param[in] model
     * @param[in] force if set true, it'll delete all dependence, else return error if found dependency
     * @param[out] msg Message in case of error
     * @return
     */
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);
    /**
     * @brief delete area model
     * @param model
     * @param force
     * @param msg
     * @return
     */
    ErrCode deleteHardArea(DbModel* model, bool force = false, QString* msg = nullptr);
protected:
    /**
     * @brief Get main table, usually Area table
     * @return
     */
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();
    virtual DbModelBuilder getBuilder(const QString& modelName);
};

#endif // DBSQLITEAREA_H
