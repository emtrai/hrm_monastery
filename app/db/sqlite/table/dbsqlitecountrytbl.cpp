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
 * Filename: dbsqlitecountrytbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/12/2022
 * Brief:
 */
#include "dbsqlitecountrytbl.h"


#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QHash>
#include "defs.h"
#include "logger.h"

const qint32 DbSqliteCountryTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCountryTbl::DbSqliteCountryTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCountry, KTableCountry, KVersionCode)
{
    tracein;
}

