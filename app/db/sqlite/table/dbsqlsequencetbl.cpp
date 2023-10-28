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
 * Filename: dbsqlsequencetbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/26/2023
 * Brief:
 */
#include "dbsqlsequencetbl.h"
#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "dbsqlite.h"
#include "utils.h"
#include <QSqlError>

const qint32 DbSqlSequenceTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqlSequenceTbl::DbSqlSequenceTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableSqliteSequence, KTableSqliteSequence, KVersionCode)
{
    traced;
}

ErrCode DbSqlSequenceTbl::getValue(const QString &tblname, qint64& value)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    int cnt = 0;
    logi("Search key '%s'", STR2CHA(tblname));
    QString queryString = QString("SELECT * FROM %1 WHERE %2 = :tblname")
                              .arg(name(), KFieldName);


    qry.prepare(queryString);
    dbgd("Query String '%s'", STR2CHA(queryString));
    logd("Bind value '%s'", STR2CHA(tblname));

    // TODO: check sql injection issue
    qry.bindValue( ":tblname", tblname);
    try {
        if( qry.exec() ) {
            logd("get next, isvalid=%d", qry.isValid());
            logd("Get all number of item %d", qry.size());
            while (qry.next()) {
                // qry.size may not support, so cannot use here
                // TODO: check if any better way to get the number of items;

                if (!qry.isNull(KFieldSeq)) {
                    bool ok = false;
                    value = qry.value(KFieldSeq).toInt(&ok);
                    if (ok) {
                        cnt++;
                        logd("Value '%lld'", value);
                    }
                    break;
                } else {
                    logd("value of name '%s' is empty", STR2CHA(tblname));
                }
            }
        }
        else {
            loge( "Failed to execute %s", STR2CHA(qry.executedQuery()) );
            loge( "Last error %s", STR2CHA(qry.lastError().text()) );
            err = ErrFailSqlQuery;
        }
    } catch(const std::runtime_error& ex) {
        loge("Runtime Exception! %s", ex.what());
        cnt = 0;
        err = ErrException;
    } catch (const std::exception& ex) {
        loge("Exception! %s", ex.what());
        cnt = 0;
        err = ErrException;
    } catch (...) {
        loge("Exception! Unknown");
        cnt = 0;
        err = ErrException;
    }
    logi("Found %d", cnt);
    if (cnt == 0) {
        err = ErrNotExist;
    }
    traceret(err);
    return err;
}

ErrCode DbSqlSequenceTbl::checkOrCreateTable()
{
    logd("this is system table, so nothing to do with this");
    return ErrNone;
}
