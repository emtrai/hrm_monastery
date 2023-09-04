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
 * Filename: dbsqliteevent.h
 * Author: Anh, Ngo Huy
 * Created date:9/10/2022
 * Brief:
 */
#ifndef DBSQLITEEVENT_H
#define DBSQLITEEVENT_H

#include "dbsqlitemodelhandler.h"

class DbSqliteEvent : public DbSqliteModelHandler
{
    GET_INSTANCE_DECL(DbSqliteEvent)
public:
    DbSqliteEvent();
    virtual ErrCode deleteHard(DbModel* model, bool force = false, QString* msg = nullptr);
protected:
    virtual DbSqliteTbl *getMainTbl();
    virtual DbModelBuilder getMainBuilder();
};

#endif // DBSQLITEEVENT_H
