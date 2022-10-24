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
 * Filename: dbsqlitedept.h
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#ifndef DBSQLITEDEPT_H
#define DBSQLITEDEPT_H

#include "dbsqlitemodelhandler.h"
#include "dbdepartmentmodelhandler.h"

class DbSqliteTbl;

class DbSqliteDept : public DbSqliteModelHandler, public DbDepartmentModelHandler
{
public:
    DbSqliteDept();
    static DbSqliteDept* getInstance();
    virtual const QString getName();
    virtual QList<DbModel*> getListPerson(const QString& deptUid);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
private:

private:
    static DbSqliteDept* gInstance;
};

#endif // DBSQLITEDEPT_H
