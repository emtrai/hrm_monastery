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
 * Filename: dbsqlitearea.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#include "dbsqlitearea.h"
#include "area.h"
#include "community.h"
#include "person.h"
#include "logger.h"
#include "defs.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "table/dbsqliteareamgrtbl.h"
#include "table/dbsqlitecommunitytbl.h"
#include "model/areaperson.h"
#include "model/areacommunity.h"

DbSqliteArea::DbSqliteArea()
{
    traced;
}

const QString DbSqliteArea::getName()
{
    return KModelHdlArea;
}

QList<DbModel *> DbSqliteArea::getListPersonInCharges(const QString &areaUid, int status)
{
    traced;
    DbSqliteAreaMgrTbl* tbl = (DbSqliteAreaMgrTbl*)DbSqlite::getInstance()
                                          ->getTable(KTableAreaPerson);
    QList<DbModel *> list = tbl->getListPerson(areaUid, status);
    tracedr(list.count());
    return list;
}

QList<DbModel *> DbSqliteArea::getListCommunities(const QString &areaUid, int status)
{
    traced;
    DbSqliteCommunityTbl* tbl = (DbSqliteCommunityTbl*)DbSqlite::getInstance()
                                  ->getTable(KTableCommunity);
    QList<DbModel *> list = tbl->getListCommunitiesInArea(areaUid, status);
    tracedr(list.count());
    return list;
}

ErrCode DbSqliteArea::addPersonInChargeOfArea(const Area *area, const Person *per, int status, qint64 startdate, qint64 enddate, const QString &remark)
{
    traced;
    ErrCode err = ErrNone;
//    DbSqliteAreaMgrTbl* tbl = (DbSqliteAreaMgrTbl*)DbSqlite::getInstance()
//                                          ->getTable(KTableAreaPerson);
    logd("Build AreaPerson map object");
//    AreaPerson* model = new AreaPerson();
    SAVE_MAP_MODEL(AreaPerson, area, per, status, startdate, enddate, remark);

    tracedr(err);
    return err;
}


DbSqliteTbl *DbSqliteArea::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTableArea);
}

DbSqliteTbl *DbSqliteArea::getTable(const QString &modelName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameAreaComm) {
        tbl = DbSqlite::table(KTableAreaCommunity);
    } else if (modelName == KModelNameAreaPerson) {
        tbl = DbSqlite::table(KTableAreaPerson);
    } else {
        tbl = getMainTbl();
    }
    tracede;
    return tbl;
}
