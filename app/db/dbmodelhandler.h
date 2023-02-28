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
 * Filename: dbmodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#ifndef DBMODELHANDLER_H
#define DBMODELHANDLER_H

#include "errcode.h"
#include <QList>
#include "dbmodel.h"

class DbModel;

class DbModelHandler
{
public:
    DbModelHandler();

    virtual ErrCode add(DbModel* model) = 0;
    virtual ErrCode update(DbModel* model) = 0;
    /**
     * @brief delete by change status to delete
     * @param model
     * @return
     */
    virtual ErrCode deleteSoft(DbModel* model) = 0;
    /**
     * @brief delete completely from db
     * @param model
     * @return
     */
    virtual ErrCode deleteHard(DbModel* model) = 0;

    /**
     * @brief Check if model exist in db
     * @param model
     * @return true if exists
     */
    virtual bool exist(const DbModel* model) = 0;

    virtual QList<DbModel*> getAll(DbModelBuilder builder, qint64 status = DB_RECORD_ACTIVE,
                                    const char* modelName = nullptr, int from = 0,
                                    int noItems = 0, int* total = nullptr) = 0;
    /**
     * @brief Get All, return as dictionary, map b/w uid and model
     * @param builder
     * @param modelName
     * @return
     */
    virtual QHash<QString, DbModel*> getAllInDict(DbModelBuilder builder, qint64 status = DB_RECORD_ACTIVE, const char* modelName = nullptr) = 0;
    virtual DbModel* getItem(const QString& uid, DbModelBuilder builder, const char* modelName = nullptr);
    virtual const QString getName() = 0;

    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);

    /**
     * @brief search EVERYTHING by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual int searchAll(const QString& keyword, QList<DbModel*>* outList = nullptr);

    virtual int filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       QList<DbModel*>* outList = nullptr);

    // TODO: implement filter with operator (equal, greater, in range, etc.)

    virtual DbModelBuilder getMainBuilder();
    virtual DbModel *getByName(const QString& name, const DbModelBuilder& builder);
    virtual DbModel *getByName(const QString& name);
    virtual DbModel *getByUid(const QString& uid, const DbModelBuilder& builder);
    virtual DbModel *getByUid(const QString& uid);
    virtual DbModel *getByNameId(const QString& nameId, const DbModelBuilder& builder);
    virtual DbModel *getByNameId(const QString& nameId);
};

#endif // DBMODELHANDLER_H
