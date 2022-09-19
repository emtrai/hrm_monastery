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
 * Filename: dbsqliteeventtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/10/2022
 * Brief:
 */
#include "dbsqliteeventtbl.h"
#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "event.h"

const qint32 DbSqliteEventTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteEventTbl::DbSqliteEventTbl():
    DbSqliteEventTbl(nullptr)
{}
DbSqliteEventTbl::DbSqliteEventTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableEvent, KTableEvent, KVersionCode)
{
    traced;
}

void DbSqliteEventTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldRemark, TEXT);
    tracede;
}

void DbSqliteEventTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                         const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);

    Event* model = (Event*) item;
    builder->addValue(KFieldRemark, model->remark());
    tracede;
}

void DbSqliteEventTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Event* model = (Event*) item;
    model->setRemark(qry.value(KFieldRemark).toString());
    tracede;
}
