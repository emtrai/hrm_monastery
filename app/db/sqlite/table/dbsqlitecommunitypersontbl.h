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
 * Filename: dbsqlitecommunitypersontbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITECOMMUNITYPERSONTBL_H
#define DBSQLITECOMMUNITYPERSONTBL_H

#include "dbsqlitemaptbl.h"
#include <QList>

class Person;
class CommunityPerson;
class DbSqliteCommunityPersonTbl : public DbSqliteMapTbl
{
public:
    DbSqliteCommunityPersonTbl(DbSqlite* db);
    virtual QList<Person*> getListPerson(const QString& communityUid, int status = MODEL_STATUS_MAX, const QString* perStatusUid = nullptr);
    virtual QList<CommunityPerson*> getListCommunityOfPerson(const QString& personUid,
                                                      int modelStatus = MODEL_STATUS_MAX);
    /**
     * @brief getListCommunityPerson
     * @param community
     * @param modelStatus
     * @return List of CommunityPerson objects
     */
    virtual QList<DbModel*> getListCommunityPerson(const QString& community,
                                                              int modelStatus = MODEL_STATUS_MAX);
    virtual QList<DbModel*> getListCommunityPerson(const QString& personUid,
                                                    const QString& commuid,
                                                    int modelStatus = MODEL_STATUS_MAX);

protected:
    virtual ErrCode updateDbModelDataFromQuery(DbModel* item, const QSqlQuery& qry);
    virtual QString getFilterQueryString(int fieldId, const QString& cond = nullptr);
    virtual QString getFilterQueryString(const QList<FilterKeyworkItem *> &filters,
                                         const QString& cond = nullptr);
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITECOMMUNITYPERSONTBL_H
