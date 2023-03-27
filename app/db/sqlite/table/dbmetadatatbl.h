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
 * Filename: dbmetadatatbl.h
 * Author: Anh, Ngo Huy
 * Created date:3/25/2023
 * Brief:
 */
#ifndef DBMETADATATBL_H
#define DBMETADATATBL_H

#include "dbsqlitetbl.h"

class DbMetadataTbl : public DbSqliteTbl
{
public:
    DbMetadataTbl();
public:
    DbMetadataTbl(DbSqlite *db);
    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual ErrCode getMetadataValue(const QString& key, QString& value);
    virtual ErrCode addMetadataValue(const QString& key, const QString& value);
    virtual ErrCode updateMetadataValue(const QString& key, const QString& value);
private:
    static const qint32 KVersionCode;
};

#endif // DBMETADATATBL_H
