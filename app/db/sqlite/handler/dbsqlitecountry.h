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
 * Filename: dbsqlitecountry.h
 * Author: Anh, Ngo Huy
 * Created date:8/12/2022
 * Brief:
 */
#ifndef DBSQLITECOUNTRY_H
#define DBSQLITECOUNTRY_H

#include "dbsqlitemodelhandler.h"

class DbSqliteCountry : public DbSqliteModelHandler
{
public:
    DbSqliteCountry();

protected:
    virtual DbSqliteTbl *getMainTbl();
    virtual DbModelBuilder getBuilder(const QString& modelName = nullptr);
};

#endif // DBSQLITECOUNTRY_H
