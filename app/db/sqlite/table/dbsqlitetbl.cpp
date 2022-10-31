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
#include "dbsqliteupdatebuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QHash>
#include "filter.h"

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

ErrCode_t DbSqliteTbl::add(DbModel *item)
{
    traced;
    ErrCode_t err = ErrNone;
    DbSqliteUpdateBuilder* updateBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    DbSqliteInsertBuilder* builder = DbSqliteInsertBuilder::build(name());
    err = insertTableField(builder, item);
//    QString sql = builder->buildSqlStatement();
//    logi("insert sql statement %s", sql.toStdString().c_str());
    if (err == ErrNone) {
        logd("Insert ");
        QSqlQuery* qry = builder->buildSqlQuery();
        err = db()->execQuery(qry);
        if (err == ErrNone) {
            qint64 lastDbId = qry->lastInsertId().toInt();
            logd("last insert id %lld", lastDbId);
            item->setDbId(lastDbId);
            if (item->uid().isEmpty()) {
                logd("Uid is empty, create new one and update");
                QString dbIdStr = QString("%1").arg(lastDbId);
                QString uid = item->buildUid(&dbIdStr);
                // TODO: check to make sure that uid does not exist?????
                updateBuilder = DbSqliteUpdateBuilder::build(name());
                updateBuilder->addCond(KFieldId, dbIdStr);
                updateBuilder->addValue(KFieldUid, uid);
                QSqlQuery* updateQry = updateBuilder->buildSqlQuery();
                err = db()->execQuery(updateQry);
                if (err == ErrNone) {
                    item->setUid(uid);
                }
            } else {
                logd("Uid already existed, skip updating");
            }

        }

        delete qry;
    } else {
        loge("Build insert sql command failed %d", err);
    }

    if (builder) {
        delete builder;
        builder = nullptr;
    }

    if (updateBuilder) {
        delete updateBuilder;
        updateBuilder = nullptr;
    }
    if (updateQry) {
        delete updateQry;
        updateQry = nullptr;
    }
    tracedr(err);
    return err;

}

ErrCode DbSqliteTbl::updateUid(const DbModel *item, const QString &uid)
{
    traced;
    ErrCode ret = ErrNone;
    tracedr(ret);
    return ret;
}

bool DbSqliteTbl::isExist(const DbModel *item)
{
    QSqlQuery qry;
    bool exist = false;
    bool queryOk = false;
    traced;
    if (!item->uid().isEmpty()) {
        QString queryString = QString("SELECT COUNT(*) "
                                      "FROM %1 WHERE %2 = :uid").arg(name(), KFieldUid);
        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        qry.bindValue( ":uid", item->uid() );
        queryOk = true;
    } else {
        QString cond;
        QHash<QString, QString> inFields = getFieldsCheckExists(item);
        if (!inFields.empty()) {
            foreach (QString field, inFields.keys()) {
                if (!cond.isEmpty()) {
                    cond += " AND ";
                }
                cond += QString("lower(%1) = :%2").arg(field,field);
            }

            QString queryString = QString("SELECT * FROM %1 WHERE %2")
                                      .arg(name(), cond);

            qry.prepare(queryString);
            logd("Query String '%s'", queryString.toStdString().c_str());

            // TODO: check sql injection issue
            foreach (QString field, inFields.keys()) {
                // TODO: if value is empty, data may not match
                // Check if value is empty, for string, it seem '', but for integer,
                // check again, as this process is string format, stupid
                qry.bindValue( ":" + field, inFields[field].trimmed().toLower());
                logd("bind :'%s' to '%s'", field.toStdString().c_str(),
                     inFields[field].trimmed().toLower().toStdString().c_str());
            }
            queryOk = true;
        } else {
            loge("no field to check exist");
            queryOk = false;
        }

    }
    logd("queryOk %d", queryOk);
    if (queryOk) {
        if( qry.exec() ) {
            int rows= 0;
            if (qry.next()) {
                rows = qry.value(0).toInt();
                exist = rows > 0;
            }
            else {
                logd("Not found any items, query '%s'", qry.lastQuery().toStdString().c_str() );
                exist = false;
            }
        } else {
            loge( "Failed to execute %s", qry.lastQuery().toStdString().c_str() );
            exist = false;
        }
    } else {
        exist = false;
        loge("Nothing to query");
    }


    logd("Exist %d", exist);
    return exist;
}

QHash<QString, QString> DbSqliteTbl::getFieldsCheckExists(const DbModel* item)
{
    QHash<QString, QString> list;
    list[KFieldName] = item->name(); // TODO: make as class member?
    return list;
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

int DbSqliteTbl::filter(int fieldId,
                        int operatorId,
                        const QString& keyword,
                        const DbModelBuilder& builder,
                        QList<DbModel*>* outList)
{
    traced;
    // TODO: implement it
    // TODO: check data typpe text vs integer
    QSqlQuery qry;
    qint32 cnt = 0;
    logi("Filter keyword '%s'", keyword.toStdString().c_str());
    QString cond;
    bool isOk = false;
    QString fieldName = getFieldNameFromId(fieldId, &isOk);
    if (isOk && !fieldName.isEmpty()) {
        switch (operatorId) {
        case FILTER_OP_EQUAL:
            cond = QString("lower(%1) = :keywordexact").arg(fieldName);
            break;
        case FILTER_OP_NOT_EQUAL:
            cond = QString("lower(%1) != :keywordexact").arg(fieldName);
            break;

        case FILTER_OP_CONTAIN:
            cond = QString("lower(%1) like :keyword").arg(fieldName);
            break;

        case FILTER_OP_NOT_CONTAIN:
            cond = QString("lower(%1) not like :keyword").arg(fieldName);
            break;
        case FILTER_OP_LESS:
            cond = QString("lower(%1) < :value").arg(fieldName);
            break;
        case FILTER_OP_LESS_EQ:
            cond = QString("lower(%1) <= :value").arg(fieldName);
            break;
        case FILTER_OP_GREATER:
            cond = QString("lower(%1) > :value").arg(fieldName);
            break;
        case FILTER_OP_GREATER_EQ:
            cond = QString("lower(%1) >= :value").arg(fieldName);
            break;
        default:
            break;
        }
    }
    logd("Query cond '%s'", cond.toStdString().c_str());
    QString queryString = getSearchQueryString(cond);

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
        loge( "Last error %s", qry.lastError().text().toStdString().c_str() );
    }

    logi("Found %d", cnt);

    return cnt;
}

QSqlQuery *DbSqliteTbl::getAllQuery()
{
    traced;
    QSqlQuery* qry = new QSqlQuery();

    traced;
    // TODO: check record status????
    QString queryString = getAllQueryString();
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    return qry;
}

QString DbSqliteTbl::getAllQueryString()
{
    traced;
    return getSearchQueryString();
}



QList<DbModel *> DbSqliteTbl::getAll(const DbModelBuilder& builder)
{

    traced;
    QSqlQuery* qry = getAllQuery();
    QList<DbModel*>list;

    if( qry->exec() )
    {
        while (qry->next()) {
            DbModel* item = builder();
            item->setDbId(qry->value(KFieldId).toInt());
            item->setDbStatus(qry->value(KFieldRecordStatus).toInt());
            item->setName(qry->value(KFieldName).toString());
            item->setUid(qry->value(KFieldUid).toString());
            // TODO: validate value before, i.e. toInt return ok
            updateModelFromQuery(item, *qry);
            list.append(item); // TODO: when it cleaned up?????
        }
    }
    else {
        loge( "Failed to execute %s", qry->lastQuery().toStdString().c_str() );
        loge( "Last error %s", qry->lastError().text().toStdString().c_str() );
    }

    logd("Found %d", (int)list.size());
    delete qry;
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
    logd("Create table '%s'", name().toStdString().c_str());
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

QString DbSqliteTbl::getSearchQueryString(const QString& cond)
{
    traced;
    QString queryString = QString("SELECT * FROM %1")
                              .arg(name());
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

QString DbSqliteTbl::getFilterQueryString(int fieldId, const QString &cond)
{
    traced;
    UNUSED(fieldId);
    QString queryString = getSearchQueryString(cond);
    tracede;
    return queryString;
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
    logi("Search keyword '%s'", keyword.toStdString().c_str());
    QString cond;
    foreach (QString field, inFields.keys()) {
        if (!cond.isEmpty()) {
            cond += " OR ";
        }
        logd("field %s", field.toStdString().c_str());
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

//    QString queryString = QString("SELECT * FROM %1 WHERE %2")
//                              .arg(name(), cond);
    logd("Query cond '%s'", cond.toStdString().c_str());
    QString queryString = getSearchQueryString(cond);

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

ErrCode DbSqliteTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
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
    return ErrNone;
}


DbSqlite *DbSqliteTbl::db() const
{
    return mDb;
}
