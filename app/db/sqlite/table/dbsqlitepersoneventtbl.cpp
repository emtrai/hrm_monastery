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
 * Filename: dbsqlitepersoneventtbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#include "dbsqlitepersoneventtbl.h"

#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "status.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "personevent.h"
#include "dbctl.h"
#include "dbsqlite.h"

const qint32 DbSqlitePersonEventTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqlitePersonEventTbl::DbSqlitePersonEventTbl():
    DbSqlitePersonEventTbl(nullptr)
{}
DbSqlitePersonEventTbl::DbSqlitePersonEventTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTablePersonEvent, KTablePersonEvent, KVersionCode)
{
    traced;
}

DbSqlitePersonEventTbl::~DbSqlitePersonEventTbl()
{
    traced;
}

void DbSqlitePersonEventTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldEventUid, TEXT);
    builder->addField(KFieldPersonId, TEXT);
    tracede;
}

ErrCode DbSqlitePersonEventTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                         const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item);

    PersonEvent* model = (PersonEvent*) item;
    builder->addValue(KFieldDate, model->date());
    builder->addValue(KFieldEndDate, model->endDate());
    builder->addValue(KFieldName, model->name());
    builder->addValue(KFieldEventUid, model->eventUid());
    builder->addValue(KFieldPersonId, model->personUid());
    tracede;
    return ErrNone;
}

void DbSqlitePersonEventTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    PersonEvent* model = (PersonEvent*) item;
    model->setName(qry.value(KFieldName).toString());
    model->setPersonUid(qry.value(KFieldPersonId).toString());
    model->setEventUid(qry.value(KFieldEventUid).toString());
    model->setDate(qry.value(KFieldDate).toInt());
    model->setEndDate(qry.value(KFieldEndDate).toInt());
    tracede;
}

QList<PersonEvent *> *DbSqlitePersonEventTbl::getListEvents(const QString &personUid,
                                                            const QString *eventUid,
                                                            qint64 date)
{
    traced;
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode ret = ErrNone;
    traced;
    QString queryString = QString("SELECT * "
                                  "FROM %1").arg(name());
    bool hasEid = false;
    bool hasDate = false;
    QList<PersonEvent*>* list = nullptr;

    QString condString;
    if (!personUid.isEmpty()) {
        condString += QString("%1 = :pid").arg(KFieldPersonId);
        if (eventUid != nullptr && !eventUid->isEmpty()) {
            condString += QString("AND %1 = :eid").arg(KFieldEventUid);
            hasEid = true;
        }
        if (date > 0) {
            condString += QString("AND %1 = :date").arg(KFieldDate);
            hasDate = true;
        }
        if (!condString.isEmpty())
            queryString += " WHERE " + condString;

        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        qry.bindValue( ":pid", personUid);
        if (hasEid)
            qry.bindValue( ":eid", *eventUid );
        if (hasDate)
            qry.bindValue( ":date", date );

        if( qry.exec() )
        {
            list = new QList<PersonEvent*>();
            while (qry.next()) {
                PersonEvent* item = (PersonEvent*)PersonEvent::build();
                item->setDbId(qry.value(KFieldId).toInt());
                item->setDbStatus(qry.value(KFieldDbStatus).toInt());
                item->setName(qry.value(KFieldName).toString());
                item->setUid(qry.value(KFieldUid).toString());
                item->setPersonUid(qry.value(KFieldPersonId).toString());
                item->setEventUid(qry.value(KFieldEventId).toString());
                item->setDate(qry.value(KFieldDate).toInt());
                item->setEndDate(qry.value(KFieldEndDate).toInt());
                list->append(item); // TODO: when it cleaned up?????
            }
        }
        else {
            loge( "Failed to execute %s", queryString.toStdString().c_str() );
            ret = ErrFailed;
        }

    } else {
        ret = ErrInvalidArg;
    }


    logd("ret %d", ret);
    return list;
}
