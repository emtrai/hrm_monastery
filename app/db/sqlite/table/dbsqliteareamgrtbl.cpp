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
 * Filename: dbsqliteareamgrtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqliteareamgrtbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "table/dbsqlitedeptmgrtbl.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteAreaMgrTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteAreaMgrTbl::DbSqliteAreaMgrTbl(DbSqlite *db):
    DbSqliteDeptMgrTbl(db, KTableAreaPerson, KTableAreaPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldAreaUid;
    mFieldNameDbId1 = KFieldAreaDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}