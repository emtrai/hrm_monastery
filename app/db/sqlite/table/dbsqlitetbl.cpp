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
    item->setDbId(qry.value(KFieldId).toInt());
    item->setName(qry.value(KFieldName).toString());
    item->setUid(qry.value(KFieldUid).toString());
    item->setHistory(qry.value(KFieldHistory).toString());
    item->setDbStatus(qry.value(KFieldRecordStatus).toInt());
    tracede;
}

int DbSqliteTbl::runQuery(QSqlQuery &qry, const DbModelBuilder& builder,
                       QList<DbModel *> *outList)
{
    traced;
    int cnt = 0;
    if( qry.exec() )
    {
        while (qry.next()) {
            // qry.size may not support, so cannot use here
            // TODO: check if any better way to get the number of items;
            cnt++;
            if (outList != nullptr){
                DbModel* item = builder();
                updateModelFromQuery(item, qry);
                outList->append(item); // TODO: when it cleaned up?????
            }
        }
    }
    else {
        loge( "Failed to execute %s", qry.executedQuery().toStdString().c_str() );
    }

    logi("Found %d", cnt);

    return cnt;
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

QList<QString> DbSqliteTbl::getNameFields()
{
    traced;
    QList<QString> list;
    list.append(KFieldName); // TODO: make as class member?
    return list;
}

DbModel *DbSqliteTbl::getByName(const QString &keyword, const DbModelBuilder &builder)
{
    traced;
    QHash<QString, int> inFields;
    QList<DbModel*> outList;
    DbModel* retDb = nullptr;
    QList<QString> nameFields = getNameFields();
    foreach (QString name, nameFields) {
        inFields[name] = TEXT;
    }
    logi("Search by name '%s'", keyword.toStdString().c_str());
    qint32 cnt = 0;
    cnt = DbSqliteTbl::search (keyword, inFields, builder, &outList, true);

    logi("Found %d", cnt);
    if (cnt > 0 && outList.count() > 0){
        logd("Found, return 1st element");
        retDb = outList[0];
    } else {
        logi("Not found name '%s'", keyword.toStdString().c_str());
        // TODO: safe to print unicode log????
    }

    return retDb;
}

int DbSqliteTbl::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    logi("Default search one, DO NOTHING, derived class must implement this");
    return 0;
}

QHash<QString, int> DbSqliteTbl::getSearchFields()
{
    traced;
    QHash<QString, int> list;
    list[KFieldName] = TEXT; // TODO: make as class member?
    return list;
}

int DbSqliteTbl::search(const QString &keyword, const DbModelBuilder &builder, QList<DbModel *> *outList)
{
    traced;
    QHash<QString, int> inFields = getSearchFields();
    logi("Search '%s'", keyword.toStdString().c_str());
    qint32 cnt = 0;
    cnt = search (keyword, inFields, builder, outList);

    logi("Found %d", cnt);

    return cnt;
}

int DbSqliteTbl::search(const QString& keyword, const QHash<QString, int>& inFields,
                        const DbModelBuilder& builder,
                        QList<DbModel*>* outList,
                        bool isExact)
{
    traced;
    // TODO: implement it
    // TODO: exact and not exact match???
    QSqlQuery qry;
    qint32 cnt = 0;
    logi("Search Saint '%s'", keyword.toStdString().c_str());
    QString cond;
    foreach (QString field, inFields.keys()) {
        if (!cond.isEmpty()) {
            cond += " OR ";
        }
        if (inFields.value(field) == TEXT){
            if (isExact) {
                cond += QString("lower(%1) = :keywordexact").arg(field);
            } else {
                cond += QString("lower(%1) like :keyword").arg(field);
            }
        } else {
            // TODO: assump on INT32/64
            cond += QString("%1 = :value").arg(field);
        }
    }

    QString queryString = QString("SELECT * FROM %1 WHERE %2")
                              .arg(name(), cond);

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    qry.bindValue( ":keyword", QString("%%1%").arg(keyword.trimmed().toLower()) );
    qry.bindValue( ":keywordexact", QString("%1").arg(keyword.trimmed().toLower()) );
    qry.bindValue( ":value", QString("%1").arg(keyword.trimmed().toLower()) );
    cnt = runQuery(qry, builder, outList);

    logi("Found %d", cnt);

    return cnt;
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
