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
 * Filename: dbsqlitecommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "dbsqlitecommunity.h"
#include "std.h"
#include "dbsqlitedefs.h"
#include "table/dbsqlitecommunitytbl.h"
#include "dbsqlite.h"

DbSqliteCommunity::DbSqliteCommunity()
{

}

ErrCode DbSqliteCommunity::add(const Community *comm)
{
    traced;
    ErrCode ret = ErrNone;
    if (!exist(comm)){
        ret = communityTbl()->add(comm);
    }
    else{
        ret = ErrExisted;
        loge("Community already exist");
    }

    logd("ret %d", ret);
    return ret;
}

bool DbSqliteCommunity::exist(const Community *comm)
{
    // TODO: implement it
    return false;
}

DbSqliteCommunityTbl *DbSqliteCommunity::communityTbl()
{
    if (mCommunityTbl == nullptr){
        mCommunityTbl =
            (DbSqliteCommunityTbl*)DbSqlite::getInstance()->getTable(KTableCommunity);
    }
    return mCommunityTbl;
}
