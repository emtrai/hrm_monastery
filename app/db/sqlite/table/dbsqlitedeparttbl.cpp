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
 * Filename: dbsqlitedeparttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "dbsqlitedeparttbl.h"
#include "department.h"

#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbctl.h"
#include "dbsqlite.h"


const qint32 DbSqliteDepartTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteDepartTbl::DbSqliteDepartTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableDepartment, KTableDepartment, KVersionCode)
{
    traced;
}


void DbSqliteDepartTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldRemark, TEXT);
    tracede;
}

ErrCode DbSqliteDepartTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                       const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code

    Department* model = (Department*) item;
    builder->addValue(KFieldRemark, model->remark());
    tracede;
    return ErrNone;
}

void DbSqliteDepartTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Department* model = (Department*) item;
    model->setRemark(qry.value(KFieldRemark).toString());
    tracede;
}
