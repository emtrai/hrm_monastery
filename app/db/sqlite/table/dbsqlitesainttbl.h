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
 * Filename: dbsqlitesainttbl.h
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#ifndef DBSQLITESAINTTBL_H
#define DBSQLITESAINTTBL_H

#include "dbsqlitetbl.h"


class DbSqlite;
class Saint;

class DbSqliteSaintTbl : public DbSqliteTbl
{
public:
    DbSqliteSaintTbl(DbSqlite* db);
    ErrCode_t add(const Saint* item);

    bool isExist(const Saint* item);

protected:
    virtual QString getSqlCmdCreateTable();


private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITESAINTTBL_H
