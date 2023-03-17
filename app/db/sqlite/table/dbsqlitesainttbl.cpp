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
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "saint.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteSaintTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteSaintTbl::DbSqliteSaintTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableSaint, KTableSaint, KVersionCode)
{
    traced;
}

void DbSqliteSaintTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldFullName, TEXT);
    builder->addField(KFieldOriginName, TEXT);
    builder->addField(KFieldGender, INT32);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldCountry, TEXT);
}

ErrCode DbSqliteSaintTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);
    Saint* saint = (Saint*) item;
    builder->addValue(KFieldFullName, saint->fullName());
    builder->addValue(KFieldOriginName, saint->originName());
    builder->addValue(KFieldGender, saint->gender());
    builder->addValue(KFieldFeastDay, saint->feastDay());
    builder->addValue(KFieldCountry, saint->country());
    tracede;
    return ErrNone;
}

void DbSqliteSaintTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Saint* saint = (Saint*) item;
    saint->setFullName(qry.value(KFieldFullName).toString());
    saint->setOriginName(qry.value(KFieldOriginName).toString());
    saint->setGender((Gender)qry.value(KFieldGender).toInt());
    saint->setFeastDay(qry.value(KFieldFeastDay).toInt());
    saint->setCountry(qry.value(KFieldCountry).toString());
}

QHash<QString, int> DbSqliteSaintTbl::getSearchFields()
{
    QHash<QString, int> inFields;// TODO: make as class member?
    inFields[KFieldName] = TEXT;
    inFields[KFieldFullName] = TEXT;
    inFields[KFieldOriginName] = TEXT;
    return inFields;
}

QList<QString> DbSqliteSaintTbl::getNameFields()
{
    traced;
    QList<QString> list;// TODO: make as class member?
    list.append(KFieldName);
    list.append(KFieldFullName);
    list.append(KFieldOriginName);
    return list;
}

ErrCode DbSqliteSaintTbl::search(const QString &keyword, QList<DbModel *> *outList,
                             qint64 dbStatus,
                             int from,
                             int noItems,
                             int* total)
{
    traced;
    return DbSqliteTbl::search(keyword, &Saint::build, outList, dbStatus, from, noItems, total);
}

