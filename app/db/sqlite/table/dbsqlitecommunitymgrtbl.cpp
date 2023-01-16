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
 * Filename: dbsqlitecommunitymgrtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/17/2023
 * Brief:
 */
#include "dbsqlitecommunitymgrtbl.h"


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
#include "dbctl.h"
#include "dbsqlite.h"

const qint32 DbSqliteCommunityMgrTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityMgrTbl::DbSqliteCommunityMgrTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableCommPerson, KTableCommPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldCommunityUid;
    mFieldNameDbId1 = KFieldCommunityDbId;
    mFieldNameUid2 = KFieldRoleUid;
    mFieldNameDbId2 = KFieldRoleDbId;
}

void DbSqliteCommunityMgrTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldPersonUid, TEXT);// Person plays in this role
    builder->addField(KFieldCourseUid, TEXT);//Course of person
    tracede;
}

void DbSqliteCommunityMgrTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
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
