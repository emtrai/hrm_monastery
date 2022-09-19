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

class DbSqliteModelHandler : public virtual DbModelHandler
{
public:
    DbSqliteModelHandler();

    /* Those are very generic functions */


    virtual ErrCode add(const DbModel* model);
    virtual bool exist(const DbModel* edu);
    virtual QList<DbModel*> getAll(DbModelBuilder builder, const char* modelName = nullptr);
    virtual DbModel* getModel(qint64 dbId);
    virtual const QString getName();
protected:
    virtual DbSqliteTbl* getMainTbl() = 0;
};

#endif // DBSQLITEMODELHANDLER_H
