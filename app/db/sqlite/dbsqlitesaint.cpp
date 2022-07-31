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
 * Filename: dbsqlitesaint.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "dbsqlitesaint.h"
#include "saint.h"
#include "std.h"
#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "table/dbsqlitesainttbl.h"

DbSqliteSaint::DbSqliteSaint():
    mSaintTbl(nullptr)
{
    traced;
}


ErrCode DbSqliteSaint::addSaint(const Saint *saint)
{
    traced;
    ErrCode_t err = ErrNone;

    if (saint != nullptr){
        if (!saintTbl()->isExist(saint)){
            err = saintTbl()->add(saint);
        }
        else{
            err = ErrExisted;
            loge("Saint %s already exist", saint->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }



    return err;
}

bool DbSqliteSaint::exist(const Saint *saint)
{
    traced;

    return saintTbl()->isExist(saint);;
}

DbSqliteSaintTbl *DbSqliteSaint::saintTbl()
{
    traced;
    if (mSaintTbl == nullptr){
        mSaintTbl = (DbSqliteSaintTbl*)DbSqlite::getInstance()->getTable(KTableSaint);
    }
    return mSaintTbl;
}
