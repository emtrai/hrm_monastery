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
 * Filename: dbsqlitetbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITETBL_H
#define DBSQLITETBL_H

#include <QString>
#include "errcode.h"

#include "dbmodel.h"
#include <QHash>
#include "dbsqlitedefs.h"

class DbSqlite;
class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class DbSqliteUpdateBuilder;
class DbSqliteDeleteBuilder;
class QSqlQuery;
class FieldValue;

class DbSqliteTbl
{
public:
    DbSqliteTbl(DbSqlite* db);
    DbSqliteTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    virtual ~DbSqliteTbl();

    virtual const QString &baseName() const;
    virtual void setBaseName(const QString &newBaseName);

    virtual const QString &name() const;
    virtual void setName(const QString &newName);

    virtual uint32_t versionCode() const;
    virtual void setVersionCode(uint32_t newVersionCode);
    virtual ErrCode getDataType(const QString& field, int& dataType);

    virtual ErrCode add(DbModel* item);
    virtual ErrCode update(DbModel* item);
    virtual ErrCode updateUid(const DbModel* item, const QString& uid);
    /**
     * @brief update db
     * @param uid Uid for search
     * @param fieldValues updated field, in map of field id (i.e. KFieldName) and its value
     * @return
     */
    virtual ErrCode update(const QString& uid, const QHash<QString, FieldValue>& fieldValues);
    /**
     * @brief update db
     * @param uid Uid for search
     * @param fieldValues updated field, in map of field id (i.e. KFieldName) and its value in string format
     *                    type of filed will be search on \ref mFieldDataTypeMap basing on field name
     * @return
     */
    virtual ErrCode update(const QString& uid, const QHash<QString, QString>& fieldValues);

    /**
     * @brief delete by marking as delete
     * @param item
     * @return
     */
    virtual ErrCode deleteSoft(DbModel* item);
    /**
     * @brief delete completely form db
     * @param item
     * @return
     */
    virtual ErrCode deleteHard(DbModel* item);
    virtual ErrCode deleteHard(const QHash<QString, QString>& condition);

    virtual bool isExist(const DbModel* item);
    virtual bool isNameidExist(const QString& nameId);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);
    virtual QList<DbModel*> getAll(const DbModelBuilder& builder, qint64 status = DB_RECORD_ACTIVE,
                                    int from = 0, int noItems = 0, int* total = nullptr);
    virtual DbModel* getModel(qint64 dbId, const DbModelBuilder& builder);

    virtual ErrCode getColumnList(QHash<QString, QString>& colList);
    virtual ErrCode addTableColumn(const QHash<QString, TableFieldDatatype_t>& columnField);
    virtual ErrCode checkOrCreateTable();
    virtual ErrCode onDbMigration(qint64 oldVer, qint64 newVer);
    virtual ErrCode onTblMigration(qint64 oldVer);

    virtual QList<QString> getNameFields();
    virtual DbModel *getByUid(const QString& uid, const DbModelBuilder& builder);
    virtual DbModel *getByNameId(const QString& nameId, const DbModelBuilder& builder);

    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual ErrCode search(const QString& keyword, QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    virtual QHash<QString, int> getSearchFields();
    virtual ErrCode search(const QString& keyword, const DbModelBuilder& builder,
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    virtual QString toDbStatusCond(qint64 dbStatus);
    virtual void appendDbStatusCond(QString& cond, qint64 dbStatus);

    virtual QString toModelStatusCond(qint64 modelStatus);
    virtual void appendModelStatusCond(QString& cond, qint64 modelStatus);
    /**
     * @brief Search
     * @param keyword keyword
     * @param inFields fields to search(OR condition)
     * @param outList
     * @return the number of found items
     */
    virtual ErrCode search(const QString& keyword, const QHash<QString, int>& inFields,
                       const DbModelBuilder& builder,
                       QList<DbModel*>* outList = nullptr,
                       bool isExact = false,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    /**
     * @brief Search with with custom condition
     * @param searchCond hashmap, with key is field to search, value is value to search(FieldValue)
     * @param isAndCond
     * @param condTag information passed to getSearchQueryStringWithTag,
     *                which may be used for checking by derived class
     * @param builder
     * @param outList
     * @param isExact
     * @return
     */
    virtual ErrCode search(const QHash<QString, FieldValue>& searchCond,
                       bool isAndCond = true,
                       const QString& condTag = nullptr,
                       const DbModelBuilder& builder = nullptr,
                       QList<DbModel*>* outList = nullptr,
                       bool isExact = false,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);



    // TODO: separate it into 2 info: detail and brief????
    // as some model has a lots of information, such as person, community
    virtual ErrCode updateModelFromQuery(DbModel* item, const QSqlQuery& qry);

    virtual ErrCode filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       const DbModelBuilder& builder,
                       QList<DbModel*>* outList = nullptr,
                       qint64 dbStatus = DB_RECORD_ACTIVE,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);
    virtual ErrCode updateQueryromFields(const QHash<QString, QString>& fields,
                                         QSqlQuery &query,
                                         bool isMatchAllField = false,
                                         QString initQueryString = ""
                                         );
    virtual ErrCode getListItems(const QHash<QString, QString>& fields,
                               const DbModelBuilder& builder,
                               QList<DbModel*>* outList = nullptr,
                               bool isMatchAllField = true,
                               qint64 dbStatus = DB_RECORD_ACTIVE,
                               int from = 0,
                               int noItems = 0,
                               int* total = nullptr);

protected:
    virtual DbSqliteTableBuilder* getTableBuilder();
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);

    virtual ErrCode appendDeleteCondition(DbSqliteDeleteBuilder *builder, const DbModel *item);

    /**
     * @brief Update table field
     * @param builder
     * @param item
     * @return
     */
    virtual ErrCode updateTableField(DbSqliteUpdateBuilder* builder,
                                     const QList<QString>& updateField,
                                         const DbModel *item);
    virtual ErrCode updateTableCondition(DbSqliteUpdateBuilder* builder,
                                     const DbModel *item);
    virtual int runQuery(QSqlQuery& qry, const DbModelBuilder& builder,
                      QList<DbModel *> *outList = nullptr);
    virtual QString getSearchQueryString(const QString& cond = nullptr);
    virtual QString getSearchQueryStringWithTag(const QString& cond = nullptr, const QString& tag = nullptr);
    virtual QString getFilterQueryString(int fieldId, const QString& cond = nullptr);
    virtual QSqlQuery *getAllQuery(qint64 dbstatus = DB_RECORD_ACTIVE);
    virtual QString getAllQueryString(qint64 dbstatus = DB_RECORD_ACTIVE);
    virtual DbModelBuilder mainModelBuilder();
public:

    virtual DbSqlite *db() const;

protected: // allow derived class can get info
    QString mBaseName;
    QString mName;
    uint32_t mVersionCode;
    DbSqlite* mDb;
    QHash<QString, int> mFieldDataTypeMap;
};

#endif // DBSQLITETBL_H
