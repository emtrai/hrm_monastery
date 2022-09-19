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
//    QString sql = builder->buildSqlStatement();
//    logi("insert sql statement %s", sql.toStdString().c_str());
    QSqlQuery* qry = builder->buildSqlQuery();
    err = db()->execQuery(qry);
    delete qry;
    tracedr(err);
    return err;

}

bool DbSqliteTbl::isExist(const DbModel *item)
{
    QSqlQuery qry;
    bool exist = false;
    traced;
    QString queryString = QString("SELECT COUNT(*) "
                                  "FROM %1 WHERE %2 = :uid").arg(name(), KFieldUid);
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":uid", item->uid() );
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
    item->setUid(qry.value(KFieldUid).toString());
    item->setHistory(qry.value(KFieldHistory).toString());
    tracede;
}

QList<DbModel *> DbSqliteTbl::getAll(const DbModelBuilder& builder)
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
            item->setName(qry.value(KFieldName).toString());
            item->setUid(qry.value(KFieldUid).toString());
            // TODO: validate value before, i.e. toInt return ok
            updateModelFromQuery(item, qry);
            list.append(item); // TODO: when it cleaned up?????
        }
    }
    else {
        loge( "Failed to execute %s", queryString.toStdString().c_str() );
    }

    logd("Found %d", (int)list.size());
    tracede;
    return list;
}

DbModel *DbSqliteTbl::getModel(qint64 dbId, const DbModelBuilder& builder)
{
    QSqlQuery qry;
    ErrCode err = ErrNone;
    DbModel* item = nullptr;
    traced;
    // TODO: check record status????
    QString queryString = QString("SELECT * FROM %1 where id=:id").arg(name());
    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    qry.bindValue(":id", dbId);
    if(!qry.exec() ){
        err = ErrFailed;
        loge("faile to query sql");
    }
    if (err == ErrNone && !qry.next()) {
        err = ErrFailed;
        loge("no data");
    }

    if (err == ErrNone){
        item = builder();
        item->setDbId(qry.value(KFieldId).toInt());
        item->setDbStatus(qry.value(KFieldRecordStatus).toInt());
        item->setName(qry.value(KFieldName).toString());
        item->setUid(qry.value(KFieldUid).toString());
        updateModelFromQuery(item, qry);
    }


    tracede;
    return item;
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
    // Common table, with name and history field, together with default one is id, uid, status, history
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
    // TODO: consider to make Uid as common field???
    // i.e. uid can be hash of name, hash of 2 id in different table, etc.
    builder->addField(KFieldUid, TEXT);
    builder->addField(KFieldName, TEXT);
    builder->addField(KFieldHistory, TEXT);
}

void DbSqliteTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    if (!item->uid().isEmpty())
        builder->addValue(KFieldUid, item->uid());
    if (!item->name().isEmpty())
        builder->addValue(KFieldName, item->name());
    if (!item->history().isEmpty())
        builder->addValue(KFieldHistory, item->history());
    builder->addValue(KFieldRecordStatus, item->dbStatus());
    tracede;
}


DbSqlite *DbSqliteTbl::db() const
{
    return mDb;
}
