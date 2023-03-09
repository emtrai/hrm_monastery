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
 * Filename: dbsqlitearea.h
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#ifndef DBSQLITEAREA_H
#define DBSQLITEAREA_H

#include "dbsqlitemodelhandler.h"
#include "dbareamodelhandler.h"

class DbSqliteTbl;

class DbSqliteArea : public DbSqliteModelHandler, public DbAreaModelHandler
{
public:
    DbSqliteArea();
    static DbSqliteArea* getInstance();
    virtual const QString getName();
    virtual QList<DbModel*> getListPersonInCharges(const QString& areaUid, int status = 0);
    virtual QList<DbModel*> getListCommunities(const QString& areaUid, int status = 0);
    virtual ErrCode addPersonInChargeOfArea(const Area *area,
                                            const Person* per,
                                            int status = 0,
                                            qint64 startdate = 0,
                                            qint64 enddate = 0,
                                            const QString& remark = nullptr);
protected:
    virtual DbSqliteTbl* getMainTbl();
    virtual DbSqliteTbl* getTable(const QString& modelName);
    virtual DbModelBuilder getMainBuilder();
private:

private:
    static DbSqliteArea* gInstance;
};

#endif // DBSQLITEAREA_H
