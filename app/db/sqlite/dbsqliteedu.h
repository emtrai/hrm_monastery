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
 * Filename: dbsqliteedu.h
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#ifndef DBSQLITEEDU_H
#define DBSQLITEEDU_H

#include <idbedu.h>
#include <QList>
#include "education.h"
#include "dbmodelhandler.h"
#include "dbsqlitemodelhandler.h"

class DbSqliteEduTbl;


class DbSqliteEdu : public DbSqliteModelHandler
{
public:
    static DbSqliteEdu* getInstance();

protected:
    virtual DbSqliteTbl* getMainTbl();
private:
    DbSqliteEdu();

private:
    static DbSqliteEdu* gInstance;
};

#endif // DBSQLITEEDU_H
