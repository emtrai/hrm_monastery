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
class DbSqlite;
class DbSqliteTableBuilder;
class DbSqliteInsertBuilder;
class DbSqliteUpdateBuilder;
class QSqlQuery;

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

    virtual ErrCode add(DbModel* item);
    virtual ErrCode update(DbModel* item);
    virtual ErrCode updateUid(const DbModel* item, const QString& uid);

    virtual bool isExist(const DbModel* item);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);
    virtual QList<DbModel*> getAll(const DbModelBuilder& builder);
    virtual DbModel* getModel(qint64 dbId, const DbModelBuilder& builder);


    virtual ErrCode checkOrCreateTable();
    virtual ErrCode onDbMigration(int oldVer, int newVer);
    virtual ErrCode onTblMigration(int oldVer, int newVer);

    virtual QList<QString> getNameFields();
    virtual DbModel *getByName(const QString& keyword, const DbModelBuilder& builder);
    virtual DbModel *getByUid(const QString& uid, const DbModelBuilder& builder);

    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);

    virtual QHash<QString, int> getSearchFields();
    virtual int search(const QString& keyword, const DbModelBuilder& builder, QList<DbModel*>* outList = nullptr);



    /**
     * @brief Search
     * @param keyword keyword
     * @param inFields fields to search (OR condition)
     * @param outList
     * @return the number of found items
     */
    virtual int search(const QString& keyword, const QHash<QString, int>& inFields,
                       const DbModelBuilder& builder,
                       QList<DbModel*>* outList = nullptr,
                       bool isExact = false);

    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);

    virtual int filter(int fieldId,
                       int operatorId,
                       const QString& keyword,
                       const DbModelBuilder& builder,
                       QList<DbModel*>* outList = nullptr);

protected:
    virtual QString getSqlCmdCreateTable();
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
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
    virtual QString getFilterQueryString(int fieldId, const QString& cond = nullptr);
    virtual QSqlQuery *getAllQuery();
    virtual QString getAllQueryString();
public:

    virtual DbSqlite *db() const;

protected: // allow derived class can get info
    QString mBaseName;
    QString mName;
    uint32_t mVersionCode;
    DbSqlite* mDb;
};

#endif // DBSQLITETBL_H
