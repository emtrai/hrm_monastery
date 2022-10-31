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

class DbSqlitePerson;

class DbSqliteModelHandler : public virtual DbModelHandler
{
public:
    DbSqliteModelHandler();

    /* Those are very generic functions */


    /**
     * @brief add model to db
     * @param model
     * @return ErrNone on success, error code otherwise
     */
    virtual ErrCode add(DbModel* model);

    /**
     * @brief Check if model exist in db
     * @param edu
     * @return true if exist, false otherwise
     */
    virtual bool exist(const DbModel* edu);
    virtual QList<DbModel*> getAll(DbModelBuilder builder, const char* modelName = nullptr);
    /**
     * @brief Get All, return as dictionary, map b/w uid and model
     * @param builder
     * @param modelName
     * @return
     */
    virtual QHash<QString, DbModel*> getAllInDict(DbModelBuilder builder, const char* modelName = nullptr);

    virtual DbModel* getModel(qint64 dbId);

    virtual DbModel *getByName(const QString& name, const DbModelBuilder& builder);
    virtual DbModel *getByName(const QString& name);
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
    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);

    virtual int filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       QList<DbModel*>* outList = nullptr);

protected:
    virtual DbSqliteTbl* getMainTbl() = 0;
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder* getBuilder(const QString& modelName);
    virtual DbModelBuilder* getMainBuilder();
};

#endif // DBSQLITEMODELHANDLER_H
