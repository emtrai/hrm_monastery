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
 * Filename: dbsqlitecommunitypersontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqlitecommunitypersontbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "model/saintperson.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"

const qint32 DbSqliteCommunityPersonTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityPersonTbl::DbSqliteCommunityPersonTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableCommPerson, KTableCommPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldCommunityUid;
    mFieldNameDbId1 = KFieldCommunityDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<DbModel *> DbSqliteCommunityPersonTbl::getListPerson(const QString &communityUid, int status)
{
    traced;
    QSqlQuery qry;
    qint32 cnt = 0;
    if (communityUid.isEmpty()){
        loge("Invalid community uid");
        return QList<DbModel*>();
    }
    logi("CommunityUid '%s'", communityUid.toStdString().c_str());
    QString queryString = QString("SELECT * FROM %1 JOIN %2 ON %1.%3 = %2.%4 WHERE %1.%5 = :uid")
                              .arg(KTableCommPerson, KTablePerson)
                              .arg(KFieldPersonUid, KFieldUid, KFieldCommunityUid);

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", communityUid);
    // TODO: add query status
//    qry.bindValue( ":status", status); // TODO: support multiple status???
    QList<DbModel *> outList;
    cnt = runQuery(qry, &Person::build, &outList);

    logi("Found %d", cnt);
    tracede;
    return outList;
}

void DbSqliteCommunityPersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteMapTbl::updateModelFromQuery(item, qry);
    QString modelName = item->modelName();
    logd("update for map model '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePerson)
    {
        logd("update for person model");
        DbSqlitePersonTbl* tbl = dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));
        tbl->updateModelFromQuery(item, qry);


    } else {
        loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
    }
    tracede;
}
