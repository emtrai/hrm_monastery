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
 * Filename: dbsqliteperson.h
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#ifndef DBSQLITEPERSON_H
#define DBSQLITEPERSON_H

#include "dbsqlitemodelhandler.h"
#include "dbpersonmodelhandler.h"
#include <QHash>
#include "utils.h"
#include "table/dbsqlitetbl.h"
#include "errcode.h"

class DbSqlitePersonTbl;
class DbSqlitePerson : public DbPersonModelHandler
{
    GET_INSTALCE_DECL(DbSqlitePerson);
public:
    DbSqlitePerson();
    virtual const QString getName();
    virtual ErrCode add(DbModel* model);
    virtual ErrCode add2Table(DbModel* model, DbSqliteTbl* tbl);
    virtual bool exist(const DbModel* edu);
    virtual QList<DbModel*> getAll(DbModelBuilder builder, const char* modelName = nullptr);
    virtual DbModel* getModel(qint64 dbId);

    virtual ErrCode addEvent(const QString& personUid, const QString& eventUid,
                             qint64 date, const QString& title, const QString& remark);
    virtual QList<PersonEvent*>* getListEvents(const QString& personUid,
                                               const QString* eventUid = nullptr,
                                               qint64 date = 0);
    virtual DbModel *getByName(const QString& name, const DbModelBuilder& builder);
    virtual DbModel *getByName(const QString& name);
    /**
     * @brief Search item by keywords
     * @param keyword
     * @param outList
     * @return the number of found items
     */
    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);
protected:
    virtual DbSqliteTbl* getMainTbl();
private:

};

#endif // DBSQLITEPERSON_H
