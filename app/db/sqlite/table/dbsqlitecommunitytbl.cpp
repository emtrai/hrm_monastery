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
 * Filename: dbsqlitecommunitytbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "dbsqlitecommunitytbl.h"

#include "dbsqlite.h"
#include "dbsqlitedefs.h"
#include "defs.h"
#include "dbsqlitetablebuilder.h"
#include "community.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include "logger.h"
#include "errcode.h"

const qint32 DbSqliteCommunityTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCommunityTbl::DbSqliteCommunityTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableCommunity, KTableCommunity, KVersionCode)
{}


ErrCode DbSqliteCommunityTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    Community* cmm = (Community*) item;
    builder->addValue(KFieldAddr, cmm->addr());
    builder->addValue(KFieldTel, cmm->tel());
    builder->addValue(KFieldEmail, cmm->email());
    builder->addValue(KFieldCreateDate, cmm->createDate());
    builder->addValue(KFieldFeastDay, cmm->feastDate());
    builder->addValue(KFieldParentUid, cmm->parentUid());
    builder->addValue(KFieldStatus, (qint32) cmm->getStatus());
    builder->addValue(KFieldImgPath, cmm->imgPath());
    return ErrNone;
}

void DbSqliteCommunityTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Community* cmm = (Community*) item;
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setImgPath(qry.value(KFieldImgPath).toString());
    cmm->setParentUid(qry.value(KFieldParentUid).toString());
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setFeastDate(qry.value(KFieldFeastDay).toInt());
    cmm->setCreateDate(qry.value(KFieldCreateDate).toInt());
    cmm->setStatus((CommunityStatus)qry.value(KFieldStatus).toInt());
}


void DbSqliteCommunityTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldImgPath, TEXT);
    builder->addField(KFieldAddr, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldCEOUid, TEXT);
    builder->addField(KFieldChurchUid, TEXT);
    builder->addField(KFieldAreaUid, TEXT);
    builder->addField(KFieldLevel, INT32);
    builder->addField(KFieldParentUid, TEXT);
    builder->addField(KFieldCreateDate, INT64);
    builder->addField(KFieldFeastDay, INT64);
    builder->addField(KFieldDateFormat, TEXT);
    builder->addField(KFieldStatus, INT32); // stop, alive, etc.
    builder->addField(KFieldPreset, INT32); // 0: custom, 1: preset (from json)
}
