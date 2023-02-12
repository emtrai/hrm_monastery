/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dbsqlitecommunitydept.h
 * Author: Anh, Ngo Huy
 * Created date:2/1/2023
 * Brief:
 */
#ifndef DBSQLITECOMMUNITYDEPT_H
#define DBSQLITECOMMUNITYDEPT_H

#include "dbsqlitemodelhandler.h"
#include "dbcommdepatmodelhandler.h"

class PersonDept;

class DbSqliteCommunityDept : public DbSqliteModelHandler, public DbCommDeptModelHandler
{
    GET_INSTANCE_DECL(DbSqliteCommunityDept);
public:
    DbSqliteCommunityDept();
    virtual const QString getName();
    virtual QList<DbModel*> getListPerson(const QString& deptUid);
    virtual QList<DbModel*> getListDept(const QString& communityUid);
    virtual ErrCode addPerson2Department(PersonDept* perdept);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();
private:

};

#endif // DBSQLITECOMMUNITYDEPT_H
