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
 * Filename: dbsqlitetbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#include "dbsqlitetbl.h"
#include "logger.h"
#include <QSqlQuery>
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "logger.h"
#include "errcode.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

DbSqliteTbl::~DbSqliteTbl(){
    traced;
}
DbSqliteTbl::DbSqliteTbl(DbSqlite* db)
{
    mDb = db;
}

DbSqliteTbl::DbSqliteTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode)
    : DbSqliteTbl(db)
{
    mBaseName = baseName;
    mName = name;
    mVersionCode = versionCode;
}

const QString &DbSqliteTbl::baseName() const
{
    return mBaseName;
}

void DbSqliteTbl::setBaseName(const QString &newBaseName)
{
    mBaseName = newBaseName;
}

const QString &DbSqliteTbl::name() const
{
    return mName;
}

void DbSqliteTbl::setName(const QString &newName)
{
    mName = newName;
}

uint32_t DbSqliteTbl::versionCode() const
{
    return mVersionCode;
}

void DbSqliteTbl::setVersionCode(uint32_t newVersionCode)
{
    mVersionCode = newVersionCode;
}

ErrCode_t DbSqliteTbl::add(const DbModel *item)
{
    traced;
    ErrCode_t err = ErrNone;
    DbSqliteInsertBuilder* builder = DbSqliteInsertBuilder::build(name());
    insertTableField(builder, item);
    QString sql = builder->buildSqlStatement();
    logi("insert sql statement %s", sql.toStdString().c_str());
    err = db()->execQuery(sql);

    return err;

}

bool DbSqliteTbl::isExist(const DbModel *item)
{
    QSqlQuery qry;
    bool exist = false;
    traced;
    QString queryString = QString("SELECT COUNT(*) "
                                  "FROM %1 WHERE %2 = :uuid").arg(name(), KFieldUuid);
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uuid", item->nameid() );
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

void DbSqliteTbl::updateModelFromQuery(DbModel* item, const QSqlQuery& qry)
{
    traced;
    item->setName(qry.value(KFieldName).toString());
    item->setUuid(qry.value(KFieldUuid).toString());
    item->setHistory(qry.value(KFieldHistory).toString());
}

QList<DbModel *> DbSqliteTbl::getAll(DbModelBuilder builder)
{
    QSqlQuery qry;

    traced;
    // TODO: check record status????
    QString queryString = QString("SELECT * FROM %1").arg(name());
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    QList<DbModel*>list;

    if( qry.exec() )
    {
        while (qry.next()) {
            DbModel* item = builder();
            item->setDbId(qry.value(KFieldId).toInt());
            item->setDbStatus(qry.value(KFieldRecordStatus).toInt());
            // TODO: validate value before, i.e. toInt return ok
            updateModelFromQuery(item, qry);
            list.append(item); // TODO: when it cleaned up?????
        }
    }
    else {
        loge( "Failed to execute %s", queryString.toStdString().c_str() );
    }

    logd("Found %d", (int)list.size());
    return list;
}

ErrCode DbSqliteTbl::checkOrCreateTable()
{
    traced;
    ErrCode err = ErrNone;
    // TODO: should checktable before creating???
    // TODO: insert to management table
    QString sql = getSqlCmdCreateTable();

    if (!sql.isNull()){
        err = db()->execQuery(sql);
//        QSqlQuery qry;
//        qry.prepare( sql );
//        if( qry.exec() ) {
//            logd("Created table %s", name().toStdString().c_str());
//        }
//        else
//        {
//            qFatal( "Failed to create table images" );
//            err = ErrFailed;
//        }
    }
    else{
        loge("Invalid sql command");
        err = ErrFailed;
    }
    tracedr(err);
    return err;
}

ErrCode DbSqliteTbl::onDbMigration(int oldVer, int newVer)
{
    traced;
    return ErrNone;
}

ErrCode DbSqliteTbl::onTblMigration(int oldVer, int newVer)
{
    traced;
    return ErrNone;

}

QString DbSqliteTbl::getSqlCmdCreateTable()
{
    traced;
    // TODO; support multi language
    // Common table, with name and history field, together with default one is id, uuid, status, history
    DbSqliteTableBuilder* builder = DbSqliteTableBuilder::build(name());
    // common field
    builder->addField(KFieldRecordStatus, INT32);
    // specific field
    addTableField(builder);
    QString sql = builder->buildSqlStatement();
    logi("Create statement %s", sql.toStdString().c_str());

    return sql;
}

void DbSqliteTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    // TODO: consider to make Uuid as common field???
    // i.e. uuid can be hash of name, hash of 2 id in different table, etc.
    builder->addField(KFieldUuid, TEXT);
    builder->addField(KFieldName, TEXT);
    builder->addField(KFieldHistory, TEXT);
}

void DbSqliteTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    if (!item->nameid().isEmpty())
        builder->addValue(KFieldUuid, item->nameid());
    if (!item->name().isEmpty())
        builder->addValue(KFieldName, item->name());
    if (!item->history().isEmpty())
        builder->addValue(KFieldHistory, item->history());
}


DbSqlite *DbSqliteTbl::db() const
{
    return mDb;
}
