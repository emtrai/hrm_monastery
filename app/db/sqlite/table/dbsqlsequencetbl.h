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
 * Filename: dbsqlsequencetbl.h
 * Author: Anh, Ngo Huy
 * Created date:3/26/2023
 * Brief:
 */
#ifndef DBSQLSEQUENCETBL_H
#define DBSQLSEQUENCETBL_H

#include "dbsqlitetbl.h"

class DbSqlSequenceTbl : public DbSqliteTbl
{
public:
    DbSqlSequenceTbl(DbSqlite *db);
    virtual ErrCode getValue(const QString& tblname, qint64& value);
    virtual ErrCode checkOrCreateTable();
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLSEQUENCETBL_H
