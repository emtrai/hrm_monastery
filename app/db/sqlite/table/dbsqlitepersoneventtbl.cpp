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
#include "personstatus.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include "personevent.h"
#include "dbctl.h"
#include "dbsqlite.h"

/**
 * VERSION 0.0.1:
 * + KFieldDate
 * + KFieldEndDate
 * + KFieldEventUid
 * + KFieldPersonId
 */

#define VERSION_CODE_1 VERSION_CODE(0,0,1)

const qint32 DbSqlitePersonEventTbl::KVersionCode = VERSION_CODE_1;

DbSqlitePersonEventTbl::DbSqlitePersonEventTbl():
    DbSqlitePersonEventTbl(nullptr)
{}
DbSqlitePersonEventTbl::DbSqlitePersonEventTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTablePersonEvent, KTablePersonEvent, KVersionCode)
{
    tracein;
}

DbSqlitePersonEventTbl::~DbSqlitePersonEventTbl()
{
    tracein;
}

void DbSqlitePersonEventTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldEventUid, TEXT);
    // person id instead of person uid, due to silly mistake so cannot change... f*c* me
    builder->addField(KFieldPersonId, TEXT);
    traceout;
}

ErrCode DbSqlitePersonEventTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                         const DbModel *item)
{
    tracein;
    DbSqliteTbl::insertTableField(builder, item);

    PersonEvent* model = (PersonEvent*) item;
    builder->addValue(KFieldDate, model->date());
    builder->addValue(KFieldEndDate, model->endDate());
    builder->addValue(KFieldName, model->name());
    builder->addValue(KFieldEventUid, model->eventUid());
    builder->addValue(KFieldPersonId, model->personUid());
    traceout;
    return ErrNone;
}

ErrCode DbSqlitePersonEventTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    }
    if (err == ErrNone) {
        PersonEvent* model = (PersonEvent*) item;
        if (qry.value(KFieldPersonEventUid).isValid()) {
            model->setUid(qry.value(KFieldPersonEventUid).toString());
        }
        if (qry.value(KFieldPersonEventNameId).isValid()) {
            model->setNameId(qry.value(KFieldPersonEventNameId).toString());
        }
        model->setName(qry.value(KFieldName).toString());
        model->setPersonUid(qry.value(KFieldPersonId).toString());
        model->setEventUid(qry.value(KFieldEventUid).toString());
        model->setEventName(qry.value(KFieldEventName).toString());
        model->setDate(qry.value(KFieldDate).toInt());
        model->setEndDate(qry.value(KFieldEndDate).toInt());
    }
    if (err == ErrNone) {
        logd("updated for model '%s'", STR2CHA(item->toString()));
    }
    traceret(err);
    return err;
}

ErrCode DbSqlitePersonEventTbl::getListEvents(const QString &personUid,
                                                           QList<DbModel*>& list,
                                                            const QString *eventUid,
                                                            qint64 date)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode ret = ErrNone;
    tracein;
    QString queryString = QString("SELECT *, "
                                  "%2.%5 AS %6, "
                                  "%1.%4 AS %7, "
                                  "%1.%8 AS %9, "
                                  "%2.%8 AS %10 "
                                  "FROM %1 LEFT JOIN %2 ON %1.%3 = %2.%4")
                              .arg(name(), KTableEvent) //1 & 2
                              .arg(KFieldEventUid, KFieldUid) //3 & 4
                              .arg(KFieldName, KFieldEventName) //5 & 6
                              .arg(KFieldPersonEventUid, KFieldNameId) //7 & 8
                              .arg(KFieldPersonEventNameId, KFieldEventNameId) //9 & 10
                                ;
    bool hasEid = false;
    bool hasDate = false;

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
        appendDbStatusCond(condString, DB_RECORD_ACTIVE);
        if (!condString.isEmpty())
            queryString += " WHERE " + condString;

        queryString += " ORDER BY name ASC";
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
            while (qry.next()) {
                PersonEvent* item = (PersonEvent*)PersonEvent::build();
                ErrCode tmpret = updateDbModelDataFromQuery(item, qry);
                if (tmpret == ErrNone) {
    //                item->setDbId(qry.value(KFieldId).toInt());
    //                item->setDbStatus(qry.value(KFieldDbStatus).toInt());
    //                item->setName(qry.value(KFieldName).toString());
    //                item->setUid(qry.value(KFieldUid).toString());
    //                item->setPersonUid(qry.value(KFieldPersonId).toString());
    //                item->setEventUid(qry.value(KFieldEventId).toString());
    //                item->setDate(qry.value(KFieldDate).toInt());
    //                item->setEndDate(qry.value(KFieldEndDate).toInt());
                    list.append(item); // TODO: when it cleaned up?????
                } else {
                    loge("faield to update for item ret=%d", tmpret);
                }
            }
        }
        else {
            loge( "Failed to execute %s", queryString.toStdString().c_str() );
            ret = ErrFailed;
        }

    } else {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }


    logd("ret %d", ret);
    traceret(ret);
    return ret;
}

ErrCode DbSqlitePersonEventTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
                                                            const QString &field,
                                                            const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNamePersonEvent) {
            PersonEvent* comm = (PersonEvent*) item;
            if (field == KItemEvent) {
                builder->addValue(KFieldEventUid, comm->eventUid());

            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());

            } else if (field == KItemPerson) {
                builder->addValue(KFieldPersonId, comm->personUid());

            } else if (field == KItemDate) {
                builder->addValue(KFieldDate, comm->date());

            } else {
                err = DbSqliteTbl::updateBuilderFieldFromModel(builder, field, item);
            }
        } else {
            loge("Model '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceret(err);
    return err;

}
