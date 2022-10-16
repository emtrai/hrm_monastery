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
 * Filename: dbsqlitedeptmgrtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitedeptmgrtbl.h"

#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/deptmgr.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>


const qint32 DbSqliteDeptMgrTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteDeptMgrTbl::DbSqliteDeptMgrTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableDeptMgr, KTableDeptMgr, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldDepartmentUid;
    mFieldNameDbId1 = KFieldDepartmentDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

DbSqliteDeptMgrTbl::DbSqliteDeptMgrTbl(DbSqlite *db,
                                       const QString &baseName,
                                       const QString &name,
                                       qint32 versionCode):
    DbSqliteMapTbl(db, baseName, name, versionCode)
{
    traced;
}

void DbSqliteDeptMgrTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);
    builder->addField(KFieldTermUid, TEXT);
}

void DbSqliteDeptMgrTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteMapTbl::insertTableField(builder, item);
    DeptMgr* model = (DeptMgr*) item;
    builder->addValue(KFieldRoleUid, model->roleUid());
    builder->addValue(KFieldTermUid, model->termUid());
}

void DbSqliteDeptMgrTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteMapTbl::updateModelFromQuery(item, qry);
    DeptMgr* model = (DeptMgr*) item;
    model->setRoleUid(qry.value(KFieldRoleUid).toString());
    model->setTermUid(qry.value(KFieldTermUid).toString());
}
const QString &DbSqliteDeptMgrTbl::roleUid() const
{
    return mRoleUid;
}

void DbSqliteDeptMgrTbl::setRoleUid(const QString &newRoleUid)
{
    mRoleUid = newRoleUid;
}

const QString &DbSqliteDeptMgrTbl::termUid() const
{
    return mTermUid;
}

void DbSqliteDeptMgrTbl::setTermUid(const QString &newTermUid)
{
    mTermUid = newTermUid;
}
