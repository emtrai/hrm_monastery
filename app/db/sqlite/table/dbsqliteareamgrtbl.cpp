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
 * Filename: dbsqliteareamgrtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#include "dbsqliteareamgrtbl.h"
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

const qint32 DbSqliteAreaMgrTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteAreaMgrTbl::DbSqliteAreaMgrTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableAreaPerson, KTableAreaPerson, KVersionCode)
{
    traced;

    mFieldNameUid1 = KFieldAreaUid;
    mFieldNameDbId1 = KFieldAreaDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
}

QList<DbModel *> DbSqliteAreaMgrTbl::getListPerson(const QString &areaUid, int status)
{
    traced;
    logi("areaUid '%s'", areaUid.toStdString().c_str());
    /*
     * SELECT * FROM "KTableAreaPerson"
     *  JOIN "KTablePerson"
     *  ON "KTableAreaPerson"."KFieldPersonUid" = "KTablePerson"."KFieldUid"
     *  WHERE "KTableAreaPerson"."KFieldAreaUid" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItems(KTableAreaPerson,
                                                         KTablePerson,
                                                         KFieldPersonUid,
                                                         KFieldUid,
                                                         KFieldAreaUid,
                                                         &Person::build,
                                                         areaUid,
                                                         status);

    tracede;
    return list;
}

void DbSqliteAreaMgrTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);// TODO: use this????
    tracede;
}

void DbSqliteAreaMgrTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
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
