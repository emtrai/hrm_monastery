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
 * Filename: dbsqlitemaptbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITEMAPTBL_H
#define DBSQLITEMAPTBL_H

#include "dbsqlitetbl.h"
#include "dbmodel.h"

class MapDbModel;

class DbSqliteMapTbl : public DbSqliteTbl
{
public:
    DbSqliteMapTbl(DbSqlite* db);
    DbSqliteMapTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    DbSqliteMapTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode, const QString&modelName);

    /**
     * @brief getListItems
     * SELECT * FROM "mapTblName"
     *  JOIN "modelTblName"
     *  ON "mapTblName"."fieldUid2Join" = "modelTblName"."fieldModelUid"
     *  WHERE "mapTblName"."fieldUid1Cond" = :uid
     * @param mapTblName
     * @param modelTblName
     * @param fieldUid2Join
     * @param fieldModelUid
     * @param fieldUid1Cond
     * @param builder
     * @param uid
     * @param status
     * @param selectedField
     * @return
     */
    virtual QList<DbModel*> getListItemsWithUid(const QString &mapTblName,
                                  const QString &modelTblName,
                                  const QString &fieldUid2Join,
                                  const QString &fieldModelUid,
                                  const QString &fieldUid1Cond,
                                  const DbModelBuilder &builder,
                                  const QString &uid,
                                  int status = MODEL_STATUS_MAX,
                                  const QString& selectedField = "*");
    virtual QString getListItemsQueryString(const QString &mapTblName,
                                          const QString &modelTblName,
                                          const QString &fieldUid2Join,
                                          const QString &fieldModelUid,
                                          const QString &cond = nullptr,
                                          int status = MODEL_STATUS_MAX,
                                          const QString& selectedField = "*");
    virtual QList<DbModel*> getListItemsWithCond(const QString &mapTblName,
                                          const QString &modelTblName,
                                          const QString &fieldUid2Join,
                                          const QString &fieldModelUid,
                                          const DbModelBuilder &builder,
                                          const QString &cond = nullptr,
                                          int status = MODEL_STATUS_MAX,
                                          const QString& selectedField = "*");
//    virtual QList<DbModel*> getListItemsOfUid2(const QString& uid2,
//                                                const DbModelBuilder &builder,
//                                                int modelStatus = MODEL_STATUS_MAX);
    virtual QList<DbModel*> getListItemsUids(const QString& uid1, const QString& uid2,
                                              const DbModelBuilder &builder,
                                              int modelStatus = MODEL_STATUS_MAX);
    virtual ErrCode updateModelStatusInDb(const QString& uid, int status = MODEL_STATUS_MAX);
protected:
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual ErrCode updateDbModelDataFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual QHash<QString, QString> getFieldsCheckExists(const DbModel* item);

    virtual const QString& getFieldNameUid1() const;
    virtual const QString& getFieldNameDbid1() const;
    virtual const QString& getFieldNameUid2() const;
    virtual const QString& getFieldNameDbid2() const;

    virtual ErrCode updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
                                                const QString &field,
                                                const DbModel *item);
    virtual ErrCode filterFieldCond(int fieldId,
                                    int operatorId,
                                    QString fieldValueName,
                                    const DbModel* parentModel,
                                    QString& cond,
                                    int& dataType,
                                    bool& isExact
                                    );

    virtual ErrCode filterFieldCond(const QList<FilterKeyworkItem*> &filters,
                                    QString& cond,
                                    QHash<QString, QString> &bindValues,
                                    const DbModel* parentModel = nullptr
                                    );
protected:
    QString mFieldNameUid1;
    QString mFieldNameDbId1;
    QString mFieldNameUid2;
    QString mFieldNameDbId2;
};

#endif // DBSQLITEMAPTBL_H
