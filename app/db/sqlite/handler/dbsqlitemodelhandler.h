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


#define CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency, msg, force, itemToSearch, itemToSet, tableName, builder) \
do { \
    QList<DbModel*> list; \
    int count = 0; \
    logd("Check to delete table %s", tableName); \
    err = getListItems(itemToSearch, tableName, false, &count, &list, builder); \
    if (err == ErrNone && count > 0) { \
        if (!force) { \
            errDependency = true; \
            if (msg) *msg += QString("'%1' existed in %2.").arg(model->name(), tableName); \
        } else { \
            foreach (DbModel* model, list) { \
                logd("force Update '%s'", STR2CHA(model->toString())); \
                tmpErr = update(model, itemToSet, tableName); \
                logd("Update result=%d", tmpErr); \
            } \
        } \
    } \
    RELEASE_LIST_DBMODEL(list); \
} while(0)

#define CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, tableName, builder) \
do { \
    QList<DbModel*> list; \
    int count = 0; \
    logd("Check to delete table %s", tableName); \
    err = getListItems(itemToSearch, tableName, true, &count, &list, builder); \
    if (err == ErrNone && count > 0) { \
        if (!force) { \
            errDependency = true; \
            if (msg) *msg += QString("'%1' existed in %2.").arg(model->name(), tableName); \
        } else { \
            foreach (DbModel* model, list) { \
                logd("force delete '%s'", STR2CHA(model->toString())); \
                tmpErr = DbSqliteModelHandler::deleteHard(model, force, msg, tableName); \
                logd("Delete result=%d", tmpErr); \
            } \
        } \
    } \
    RELEASE_LIST_DBMODEL(list); \
} while(0)


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
    virtual ErrCode add(DbModel* model, bool notifyDataChange = true);

    virtual ErrCode update(DbModel* model);
    virtual ErrCode update(DbModel* model, const QHash<QString, QString> &inFields, const QString& tableName);

    /**
     * @brief delete by change status to delete
     * @param model
     * @return
     */
    virtual ErrCode deleteSoft(DbModel* model);
    /**
     * @brief delete completely from db
     * @param[in] model
     * @param[in] force if set true, it'll delete all dependence, else return error if found dependency
     * @param[out] msg Message in case of error
     * @return
     */
    virtual ErrCode deleteHard(DbModel* model, bool force, QString* msg);
    virtual ErrCode deleteHard(DbModel* model, bool force, QString* msg, const QString& tableName);
    virtual ErrCode doDeleteHard(DbSqliteTbl* tbl, DbModel* model, bool force, QString* msg);

    /**
     * @brief Check if model exist in db
     * @param edu
     * @return true if exist, false otherwise
     */
    virtual bool exist(const DbModel* edu);

    virtual bool isNameidExist(const QString& nameId, const char* modelName = nullptr);
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
    /**
     * @brief checkFields
     * @param inFields
     * @param tableName
     * @param isMatchAllField
     * @param total
     * @return
     */
    virtual ErrCode getListItems(const QHash<QString, QString>& inFields,
                                const QString& tableName,
                                bool isMatchAllField = true,
                                int* total = nullptr,
                                QList<DbModel*>* outList = nullptr,
                                const DbModelBuilder& builder = nullptr);
protected:
    virtual DbSqliteTbl* getMainTbl() = 0;
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getBuilder(const QString& modelName = nullptr);
    virtual DbModelBuilder getMainBuilder() = 0;
protected:
    QString mName;
};

#endif // DBSQLITEMODELHANDLER_H
