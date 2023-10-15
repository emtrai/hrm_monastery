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
class FilterKeyworkItem;

enum DbModelChangeType {
    DBMODEL_CHANGE_ADD = 0,
    DBMODEL_CHANGE_UPDATE,
    DBMODEL_CHANGE_DELETE,
    DBMODEL_CHANGE_MAX
};

class onDbModelHandlerListener {
public:
    virtual QString getName() const = 0;
    /**
     * @brief onDbModelHandlerDataUpdate
     * @param model
     * @param type type of change, i.e. update, delete, add \ref DbModelChangeType
     * @param err
     */
    virtual void onDbModelHandlerDataUpdate(DbModel* model, int type, ErrCode err) = 0;
};

class DbModelHandler
{
public:
    static void cleanUpModelList(QList<DbModel*>& list);
public:
    DbModelHandler();

    virtual ErrCode add(DbModel* model, bool notifyDataChange = true) = 0;
    virtual ErrCode update(DbModel* model, bool notifyDataChange = true) = 0;
    /**
     * @brief delete by change status to delete
     * @param model
     * @return
     */
    virtual ErrCode deleteSoft(DbModel* model) = 0;
    /**
     * @brief delete completely from db
     * @param[in] model
     * @param[in] force if set true, it'll delete all dependence, else return error if found dependency
     * @param[out] msg Message in case of error
     * @return
     */
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr) = 0;

    /**
     * @brief Check if model exist in db
     * @param model
     * @return true if exists
     */
    virtual bool exist(const DbModel* model) = 0;
    virtual bool isNameidExist(const QString& nameId, const char* modelName = nullptr) = 0;

    virtual QList<DbModel*> getAll(const char* modelName = nullptr,
                                    qint64 status = DB_RECORD_ACTIVE,
                                    int from = 0,
                                    int noItems = 0, int* total = nullptr);
    virtual QList<DbModel*> getAll(DbModelBuilder builder, qint64 status = DB_RECORD_ACTIVE,
                                    const char* modelName = nullptr, int from = 0,
                                    int noItems = 0, int* total = nullptr) = 0;
    /**
     * @brief Get total number of items (count)
     * @param modelStatus
     * @param req
     * @param dbStatus
     * @return > 0: the number of item, < 0: error code
     */
    virtual int getTotalItemCount(qint64 modelStatus = MODEL_STATUS_MAX,
                                      const QString& req = nullptr,
                                      qint64 dbStatus = DB_RECORD_ACTIVE) = 0;
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
     * @brief Search item by keywords, search everything
     * @param keyword
     * @param dbStatus
     * @param from
     * @param noItems The number of items
     * @param[out] total Total found items
     * @param[out] outList
     * @return Error Code
     */
    virtual ErrCode search(const QString& keyword,
                           QList<DbModel*>* outList = nullptr,
                           qint64 dbStatus = DB_RECORD_ACTIVE,
                           int from = 0,
                           int noItems = 0,
                           int* total = nullptr) = 0;
    /**
     * @brief Filter items, only filter active model
     * @param[in] fieldId Field id to be filter, \ref FilterField
     * @param[in] opFlags Operators, \ref FilterOperation
     * @param[in] keyword
     * @param[in] from from item idx
     * @param[in] noItems the number of items needs
     * @param[out] total Total found items
     * @return ErrNone if succeed
     */
    virtual ErrCode filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       const char* targetModelName = nullptr,
                       const DbModel* parentModel = nullptr,
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr) = 0;

    virtual ErrCode filter(const QList<FilterKeyworkItem*> &filters,
                           const char* targetModelName = nullptr,
                           const DbModel* parentModel = nullptr,
                           QList<DbModel*>* outList = nullptr,
                           qint64 dbStatus = DB_RECORD_ACTIVE,
                           int from = 0,
                           int noItems = 0,
                           int* total = nullptr) = 0;

    // TODO: implement filter with operator (equal, greater, in range, etc.)

    virtual DbModelBuilder getMainBuilder();
    virtual DbModelBuilder getBuilder(const QString& modelName);

    virtual DbModel *getByUid(const QString& uid, const DbModelBuilder& builder);
    virtual DbModel *getByUid(const QString& uid);
    virtual DbModel *getByNameId(const QString& nameId, const DbModelBuilder& builder);
    virtual DbModel *getByNameId(const QString& nameId);

    /**
     * @brief Get model by name, incasensitive
     * @param name
     * @param builder
     * @param firstOrLastMatch true if first match, false if last match
     * @return
     */
    virtual DbModel *getByName(const QString& name,
                               const DbModelBuilder& builder,
                               bool firstOrLastMatch = true);

    /**
     * @brief Get model by name, incasensitive
     * @param name
     * @param builder
     * @param firstOrLastMatch true if first match, false if last match
     * @return
     */
    virtual DbModel *getByName(const QString& name, bool firstOrLastMatch = true);

    void addListener(onDbModelHandlerListener* listener);
    void delListener(onDbModelHandlerListener* listener);
protected:
    void notifyDataChange(DbModel* model, int type, ErrCode err);
protected:

    QList<onDbModelHandlerListener*> mListeners;
};

#endif // DBMODELHANDLER_H
