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
 * Filename: dbsqlitemissiontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "dbsqlitemissiontbl.h"


#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "mission.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"

const qint32 DbSqliteMissionTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteMissionTbl::DbSqliteMissionTbl(): DbSqliteMissionTbl(nullptr)
{}
DbSqliteMissionTbl::DbSqliteMissionTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableMission, KTableMission, KVersionCode)
{
    tracein;
}

void DbSqliteMissionTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
}

ErrCode DbSqliteMissionTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item);
    // TODO: remark field???
    traceout;
    return ErrNone;
}

ErrCode DbSqliteMissionTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteTbl::updateModelFromQuery(item, qry);
    traceout;
    return err;
}

