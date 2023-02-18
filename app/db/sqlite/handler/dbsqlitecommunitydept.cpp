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
 * Filename: DbSqliteCommunityDeptdept.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/1/2023
 * Brief:
 */
#include "dbsqlitecommunitydept.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "logger.h"
#include "defs.h"
#include "model/communitydept.h"
#include "person.h"
#include "community.h"
#include "handler/dbsqliteperson.h"
#include "dbpersonmodelhandler.h"
#include "table/dbsqlitecommdeptpersontbl.h"
#include "table/dbsqlitecommunitydepttbl.h"

GET_INSTANCE_IMPL(DbSqliteCommunityDept);


DbSqliteCommunityDept::DbSqliteCommunityDept():DbSqliteModelHandler()
{
    traced;
}

DbSqliteTbl *DbSqliteCommunityDept::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCommDept);
}


QList<DbModel *> DbSqliteCommunityDept::getListPerson(const QString &deptUid)
{
    traced;
    DbSqliteCommDeptPersonTbl* tbl = (DbSqliteCommDeptPersonTbl*)DbSqlite::getInstance()
                                           ->getTable(KTableCommDepartPerson);
    return tbl->getListPerson(deptUid);
}

QList<DbModel *> DbSqliteCommunityDept::getListDept(const QString &communityUid)
{
    DbSqliteCommunityDeptTbl* tbl = (DbSqliteCommunityDeptTbl*)DbSqlite::getInstance()
                                           ->getTable(KTableCommDept);
    return tbl->getListDepart(communityUid);
}

ErrCode DbSqliteCommunityDept::addPerson2Department(PersonDept* perdept)
{
    traced;
    ErrCode err = ErrNone;
    DbSqliteCommDeptPersonTbl* tbl = dynamic_cast<DbSqliteCommDeptPersonTbl*>(getTable(KModelNamePersonDept));
    err = tbl->add((DbModel*)perdept);
    tracedr(err);
    return err;
}

DbSqliteTbl *DbSqliteCommunityDept::getTable(const QString &modelName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameCommPerson) {
        tbl = DbSqlite::table(KTableCommPerson);
    } else if (modelName == KModelNamePersonDept) {
        logd("get table '%s'", KTableCommDepartPerson);
        tbl = (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableCommDepartPerson);
    } else { // TODO: check & implement more??
        tbl = getMainTbl();
    }
    tracede;
    return tbl;
}

DbModelBuilder DbSqliteCommunityDept::getMainBuilder()
{
    return Community::build;
}


const QString DbSqliteCommunityDept::getName()
{
    return KModelHdlCommDept;
}

