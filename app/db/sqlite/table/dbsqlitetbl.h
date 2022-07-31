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
 * Filename: dbsqlitetbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITETBL_H
#define DBSQLITETBL_H

#include <QString>
#include "errcode.h"

#include "dbsqlitedefs.h"

class DbSqlite;


class DbSqliteTbl
{
public:
    DbSqliteTbl(DbSqlite* db);
    DbSqliteTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);
    virtual ~DbSqliteTbl();

    const QString &baseName() const;
    void setBaseName(const QString &newBaseName);

    const QString &name() const;
    void setName(const QString &newName);

    uint32_t versionCode() const;
    void setVersionCode(uint32_t newVersionCode);


    virtual ErrCode_t checkOrCreateTable();
protected:
    virtual QString getSqlCmdCreateTable() = 0;

public:

    DbSqlite *db() const;

protected: // allow derived class can get info
    QString mBaseName;
    QString mName;
    uint32_t mVersionCode;
    DbSqlite* mDb;
};

#endif // DBSQLITETBL_H
