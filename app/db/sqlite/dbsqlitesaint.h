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
 * Filename: dbsqlitesaint.h
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#ifndef DBSQLITESAINT_H
#define DBSQLITESAINT_H

#include <idbsaint.h>

class DbSqliteSaintTbl;

class DbSqliteSaint : public IDbSaint
{
public:
    DbSqliteSaint();
    virtual ErrCode addSaint(const Saint* saint);
    virtual bool exist(const Saint* saint);

private:
     DbSqliteSaintTbl *saintTbl();

private:
    DbSqliteSaintTbl* mSaintTbl;
};

#endif // DBSQLITESAINT_H
