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
 * Filename: dbsqlitesaint.h
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#ifndef DBSQLITESAINT_H
#define DBSQLITESAINT_H

#include <QHash>
#include "dbsqlitemodelhandler.h"

class DbSqliteSaintTbl;

class DbSqliteSaint : public DbSqliteModelHandler
{
public:
    DbSqliteSaint();
    static DbSqliteSaint* getInstance();
    virtual const QString getName();
    virtual DbModel *getByName(const QString& name);
    virtual DbModel *getByUid(const QString& name);

protected:
    virtual DbSqliteTbl* getMainTbl();
private:

private:
    static DbSqliteSaint* gInstance;
};

#endif // DBSQLITESAINT_H
