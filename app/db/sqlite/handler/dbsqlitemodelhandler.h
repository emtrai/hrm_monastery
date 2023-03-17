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
 * Filename: dbsqlitemodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#ifndef DBSQLITEMODELHANDLER_H
#define DBSQLITEMODELHANDLER_H

#include <dbmodelhandler.h>
#include "table/dbsqlitetbl.h"
#include "errcode.h"
#include "dbmodel.h"

class DbSqlitePerson;

class DbSqliteModelHandler : public virtual DbModelHandler
{
public:
    DbSqliteModelHandler();
    DbSqliteModelHandler(const QString& name);

    /* Those are very generic functions */


    /**
     * @brief add model to db
     * @param model
     * @return ErrNone on success, error code otherwise
     */
    virtual ErrCode add(DbModel* model);

    virtual ErrCode update(DbModel* model);

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
    virtual ErrCode deleteHard(DbModel* model);

    /**
     * @brief Check if model exist in db
     * @param edu
     * @return true if exist, false otherwise
     */
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

    virtual DbModel *getByUid(const QString& uid, const DbModelBuilder& builder);
    virtual DbModel *getByUid(const QString& uid);
    virtual DbModel *getByNameId(const QString& nameId, const DbModelBuilder& builder);
    /**
     * @brief getName
     * @return Model Handler name
     */
    virtual const QString getName();

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
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

protected:
    virtual DbSqliteTbl* getMainTbl() = 0;
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getBuilder(const QString& modelName = nullptr);
    virtual DbModelBuilder getMainBuilder() = 0;
protected:
    QString mName;
};

#endif // DBSQLITEMODELHANDLER_H
