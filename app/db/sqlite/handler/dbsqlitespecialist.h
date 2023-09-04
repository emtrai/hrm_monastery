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
 * Filename: dbsqlitespecialist.h
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#ifndef DBSQLITESPECIALIST_H
#define DBSQLITESPECIALIST_H

#include "dbsqlitemodelhandler.h"
#include "dbspecialistmodelhandler.h"

//class Specialist;
class DbSqliteSpecialistTbl;

class DbSqliteSpecialist : public DbSqliteModelHandler, public DbSpecialistModelHandler
{
    GET_INSTANCE_DECL(DbSqliteSpecialist)
public:
    DbSqliteSpecialist();
    virtual QList<DbModel*> getListPerson(const QString& specialistUid);
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);

protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();


};

#endif // DBSQLITESPECIALIST_H
