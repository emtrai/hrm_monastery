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
 * Filename: dbsqlitespecialisttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#include "dbsqlitespecialisttbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "specialist.h"
#include "defs.h"
#include "logger.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "modeldefs.h"

const qint32 DbSqliteSpecialistTbl::KVersionCode = VERSION_CODE(0,0,1);


DbSqliteSpecialistTbl::DbSqliteSpecialistTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableSpecialist, KTableSpecialist, KVersionCode,
                  KModelNameSpecialist)
{
    traced;
}

ErrCode DbSqliteSpecialistTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    if (item->name().isEmpty()) {
        if (!qry.isNull(KFieldSpecialistName)) { /* name may be in this field, not name field*/
            logd("update name from field %s", KFieldSpecialistName);
            item->setName(qry.value(KFieldSpecialistName).toString());
        }
        if (!qry.isNull(KFieldSpecialistUid)) { /* name may be in this field, not uid field*/
            logd("update uid from field %s", KFieldSpecialistUid);
            item->setUid(qry.value(KFieldSpecialistUid).toString());
        }
    }
    traceout;
    return err;
}
