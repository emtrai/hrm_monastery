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
 * Filename: dbmetadata.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/25/2023
 * Brief:
 */
#include "dbmetadatatbl.h"
#include "dbsqlitedefs.h"
#include "errcode.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "defs.h"
#include "logger.h"
#include "area.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include "dbctl.h"
#include "dbsqlite.h"
#include "utils.h"
#include <QSqlError>
#include "datetimeutils.h"

const qint32 DbMetadataTbl::KVersionCode = VERSION_CODE(0,0,1);

DbMetadataTbl::DbMetadataTbl():
    DbMetadataTbl(nullptr)
{}
DbMetadataTbl::DbMetadataTbl(DbSqlite* db)
    :DbSqliteTbl(db, KTableMetadata, KTableMetadata, KVersionCode)
{
    traced;
}

void DbMetadataTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    builder->addField(KFieldKey, TEXT);
    builder->addField(KFieldValue, TEXT);
    builder->addField(KFieldRemark, TEXT);
    builder->addField(KFieldDbCreateTime, INT64);
    builder->addField(KFieldLastDbUpdateItme, INT64);
    traceout;
}

ErrCode DbMetadataTbl::getMetadataValue(const QString &key, QString& value)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    int cnt = 0;
    logi("Search key '%s'", STR2CHA(key));
    QString queryString = QString("SELECT * FROM %1 WHERE %2 = :key")
                              .arg(name(), KFieldKey);


    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    logd("Bind value '%s'", STR2CHA(key));

    // TODO: check sql injection issue
    qry.bindValue( ":key", key);
    try {
        if( qry.exec() ) {
            logd("get next, isvalid=%d", qry.isValid());
            logd("Get all number of item %d", qry.size());
            while (qry.next()) {
                // qry.size may not support, so cannot use here
                // TODO: check if any better way to get the number of items;

                if (!qry.isNull(KFieldValue)) {
                    value = qry.value(KFieldValue).toString();
                    cnt++;
                    logd("Value '%s'", STR2CHA(value));
                    break;
                } else {
                    logd("value of key '%s' is empty", STR2CHA(key));
                }
            }
        } else {
            loge( "Failed to execute %s", STR2CHA(qry.executedQuery()) );
            loge( "Last error %s", STR2CHA(qry.lastError().text()) );
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

ErrCode DbMetadataTbl::addMetadataValue(const QString &key, const QString &value)
{
    tracein;
    ErrCode_t err = ErrNone;
    DbSqliteInsertBuilder* builder = nullptr;
    QSqlQuery* qry = nullptr;
    if (key.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument, no key");
    }
    if (err == ErrNone) {
        builder = DbSqliteInsertBuilder::build(name());
        if (!builder) {
            err = ErrNoMemory;
            loge("create insert builder failed, no memory");
        }
    }
    if (err == ErrNone) {
        logi("Add metadata '%s':'%s'", STR2CHA(key), STR2CHA(value));
        builder->addValue(KFieldKey, key);
        builder->addValue(KFieldValue, value);
        builder->addValue(KFieldDbCreateTime, DatetimeUtils::currentTimeMs());
        builder->addValue(KFieldLastDbUpdateItme, DatetimeUtils::currentTimeMs());
        logd("build Insert sql query");
        qry = builder->buildSqlQuery();
        if (!qry) {
            err = ErrNoMemory;
            loge("create insert qry failed, no memory");
        }
    }
    if (err == ErrNone) {
        dbg(LOG_DEBUG, "Execute query to add meta data value");
        err = db()->execQuery(qry);
    }
    logife(err, "failed to add meta data");

    FREE_PTR(builder);
    FREE_PTR(qry);
    traceret(err);
    return err;
}

ErrCode DbMetadataTbl::updateMetadataValue(const QString &key, const QString &value)
{
    tracein;
    ErrCode_t err = ErrNone;
    DbSqliteUpdateBuilder* builder = nullptr;
    QSqlQuery* updateQry = nullptr;

    if (key.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument, no key");
    }
    if (err == ErrNone) {
        builder = DbSqliteUpdateBuilder::build(name());
        if (!builder) {
            err = ErrNoMemory;
            loge("create update builder failed, no memory");
        }
    }
    if (err == ErrNone) {
        logi("Update metadata '%s':'%s'", STR2CHA(key), STR2CHA(value));
        builder->addValue(KFieldKey, key);
        builder->addValue(KFieldValue, value);
        builder->addValue(KFieldLastDbUpdateItme, DatetimeUtils::currentTimeMs());

        logd("build Update sql query");
        updateQry = builder->buildSqlQuery();
        if (!updateQry) {
            err = ErrNoMemory;
            loge("create update qry failed, no memory");
        }
    }
    if (err == ErrNone) {
        dbg(LOG_DEBUG, "Execute query to update meta data value");
        err = db()->execQuery(updateQry);
    }
    logife(err, "failed to update meta data");
    FREE_PTR(builder);
    FREE_PTR(updateQry);
    traceret(err);
    return err;
}
