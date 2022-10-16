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
 * Filename: dbsqlitecommunitydeptmaptbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitecommunitydeptmaptbl.h"


#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/saintperson.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteCommunityDeptMapTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityDeptMapTbl::DbSqliteCommunityDeptMapTbl(DbSqlite* db)
    : DbSqliteMapTbl(db, KTableCommDept, KTableCommDept, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldCommunityUid;
    mFieldNameDbId1 = KFieldCommunityDbId;
    mFieldNameUid2 = KFieldDepartmentUid;
    mFieldNameDbId2 = KFieldDepartmentDbId;
}

