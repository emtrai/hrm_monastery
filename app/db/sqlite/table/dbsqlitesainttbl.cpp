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
 * Filename: dbsqlitesainttbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "dbsqlitesainttbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "std.h"
#include "dbsqlitetablebuilder.h"
#include "saint.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>

const qint32 DbSqliteSaintTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteSaintTbl::DbSqliteSaintTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableSaint, KTableSaint, KVersionCode)
{

}


ErrCode_t DbSqliteSaintTbl::add(const Saint *item)
{
    traced;
    ErrCode_t err = ErrNone;
    QString sql = DbSqliteInsertBuilder::build(name())
                      ->addValue(KFieldNameId, item->nameid())
                      ->addValue(KFieldName, item->name())
                      ->addValue(KFieldGender, item->gender())
                      ->addValue(KFieldFeastDay, item->feastDay())
                      ->addValue(KFieldCountry, item->country())
                      ->addValue(KFieldHistory, item->history())

                      ->buildSqlStatement();
    logi("insert sql statement %s", sql.toStdString().c_str());
    err = db()->execQuery(sql);

    return err;

}

bool DbSqliteSaintTbl::isExist(const Saint *item)
{
    QSqlQuery qry;
    bool exist = false;
    traced;
    QString queryString = QString("SELECT COUNT(*) FROM %1 WHERE %2 = :nameid").arg(name(), KFieldNameId);
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":nameid", item->nameid() );
    if( qry.exec() )
    {
        int rows= 0;
        if (qry.next()) {
            rows = qry.value(0).toInt();
            exist = rows > 0;
        }
        else {
            logd("Not found any items");
            exist = false;
        }
    }
    else {
        loge( "Failed to execute %s", queryString.toStdString().c_str() );
        exist = false;
    }

    logd("Exist %d", exist);
    return exist;
}

QString DbSqliteSaintTbl::getSqlCmdCreateTable()
{
    traced;
    // TODO; support multi language
    QString sql = DbSqliteTableBuilder::build(name())
                      ->addField(KFieldNameId, TEXT)
                      ->addField(KFieldName, TEXT)
                      ->addField(KFieldGender, TEXT)
                      ->addField(KFieldFeastDay, INT64)
                      ->addField(KFieldCountry, TEXT)
                      ->addField(KFieldHistory, TEXT)

                      ->buildSqlStatement();
    logi("Create statement %s", sql.toStdString().c_str());

    return sql;
}
