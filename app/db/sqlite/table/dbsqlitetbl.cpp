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
#include "dbsqlitedeletebuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QHash>
#include "filter.h"
#include "dbctl.h"
#include "dbsqlite.h"


DbSqliteTbl::~DbSqliteTbl(){
    traced;
}
DbSqliteTbl::DbSqliteTbl(DbSqlite* db)
{
    mDb = db;
    mFieldDataTypeMap[KFieldId] = INT64;
    mFieldDataTypeMap[KFieldUid] = TEXT;
    mFieldDataTypeMap[KFieldName] = TEXT;
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

int DbSqliteTbl::getDataType(const QString &field)
{
    int ret = -1;
    traced;
    logd("get data type for field %s", STR2CHA(field));
    if (this->mFieldDataTypeMap.contains(field)){
        ret = mFieldDataTypeMap.value(field);
    } else {
        loge("field '%s' not exist", STR2CHA(field));
    }
    tracedr(ret);
    return ret;
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
                updateBuilder->addCond(KFieldId, dbIdStr, INT64);
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

ErrCode DbSqliteTbl::updateTableField(DbSqliteUpdateBuilder *builder,
                                      const QList<QString>& updateField,
                                      const DbModel *item)
{
    traced;
    ErrCode err = ErrNone;
    logd("Add %d field to update", updateField.count());
    foreach (QString field, updateField) {
        logd("Update field %s", field.toStdString().c_str());
        if (field == KItemUid) {
            builder->addValue(KFieldUid, item->uid());
        } else if (field == KItemName){
            builder->addValue(KFieldName, item->name());
        }
    }
    tracedr(err);
    return err;
}

ErrCode DbSqliteTbl::updateTableCondition(DbSqliteUpdateBuilder *builder, const DbModel *item)
{
    traced;
    ErrCode err = ErrNone;
    builder->addCond(KFieldUid, item->uid());
    tracedr(err);
    return err;
}

ErrCode DbSqliteTbl::update(DbModel *item)
{
    traced;
    ErrCode_t err = ErrNone;
    DbSqliteUpdateBuilder* updateBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    QList<QString> updatedFields = item->updatedField();
    if (updatedFields.count() > 0){
        logd("build update builder");
        // TODO: check to make sure that uid does not exist?????
        updateBuilder = DbSqliteUpdateBuilder::build(name());
        err = updateTableCondition(updateBuilder, item);
        if (err == ErrNone) {
            err = updateTableField(updateBuilder, updatedFields, item);
            logd("updateTableField ret %d", err);
        } else {
            loge("update table cond failed err %d", err);
        }

        if (err == ErrNone){
            logd("Build SQL Query and execute");
            updateBuilder->addValue(KFieldLastUpdateItme, Utils::currentTimeMs());
            updateQry = updateBuilder->buildSqlQuery();
            err = db()->execQuery(updateQry);
            logd("execQuery err %d", err);
        } else {
            loge("Faild to update table field %d", err);
        }
    } else {
        loge("Nothing to update");
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
    // TODO:implement it
    tracedr(ret);
    return ret;
}

ErrCode DbSqliteTbl::update(const QString &uid, const QHash<QString, FieldValue>& fieldValues)
{
    traced;
    ErrCode_t err = ErrNone;
    DbSqliteUpdateBuilder* updateBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    QList<QString> updatedFields = fieldValues.keys();
    if (updatedFields.count() > 0){
        logd("build update builder");
        // TODO: check to make sure that uid does not exist?????
        updateBuilder = DbSqliteUpdateBuilder::build(name());
        updateBuilder->addCond(KFieldUid, uid);
        // TODO: data type? (int vs string, different representation)
        foreach (QString field, updatedFields) {
            logd("Update field %s", field.toStdString().c_str());
            updateBuilder->addValue(field, fieldValues[field].value, fieldValues[field].dataType);
        }

        if (err == ErrNone){
            logd("Build SQL Query and execute");
            updateBuilder->addValue(KFieldLastUpdateItme, Utils::currentTimeMs());
            // TODO: update history????
            updateQry = updateBuilder->buildSqlQuery();
            err = db()->execQuery(updateQry);
            logd("execQuery err %d", err);
        } else {
            loge("Faild to update table field %d", err);
        }
    } else {
        loge("Nothing to update");
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

ErrCode DbSqliteTbl::deleteSoft(DbModel *item)
{
    ErrCode err = ErrNone;
    traced;
    // TODO: made delete condition customize/dynamic by deleteCondition
//    QHash<QString, QString> deleteCond = deleteCondition(item);
//    if (deleteCond.count() > 0) {
//        update(item->uid(), )
//    }
    QHash<QString, FieldValue> fields;
    fields.insert(KFieldRecordStatus, FieldValue(QString::number(DB_RECORD_DElETED), INT32));
    err = update(item->uid(), fields);
    tracedr(err);
    return err;
}

ErrCode DbSqliteTbl::deleteHard(DbModel *item)
{
    traced;
    ErrCode_t err = ErrNone;
    DbSqliteDeleteBuilder* deleteBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    QList<QString> updatedFields = item->updatedField();
    logd("build delete builder");
    // TODO: check to make sure that uid does not exist?????
    deleteBuilder = DbSqliteDeleteBuilder::build(name());
    err = deleteCondition(deleteBuilder, item);
    if (err == ErrNone){
        logd("Build SQL Query and execute");
        updateQry = deleteBuilder->buildSqlQuery();
        err = db()->execQuery(updateQry);
        updateQry->clear();
        logd("execQuery err %d", err);
    } else {
        loge("Faild to delete item in table, err=%d", err);
    }


    if (deleteBuilder) {
        delete deleteBuilder;
        deleteBuilder = nullptr;
    }
    if (updateQry) {
        delete updateQry;
        updateQry = nullptr;
    }
    tracedr(err);
    return err;
}

bool DbSqliteTbl::isExist(const DbModel *item)
{
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
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
    item->setCreatedTime(qry.value(KFieldCreateTime).toInt());
    item->setLastUpdatedTime(qry.value(KFieldLastUpdateItme).toInt());
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
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
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
    QString queryString = getFilterQueryString(fieldId, cond);

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
                logd("found one build item");
                DbModel* item = builder();
                logd("Updae model from query");
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
//    DB->openDb();

    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());
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

DbModelBuilder DbSqliteTbl::mainModelBuilder()
{
    return nullptr;
}



QList<DbModel *> DbSqliteTbl::getAll(const DbModelBuilder& builder)
{

    traced;
    QSqlQuery* qry = getAllQuery();
    QList<DbModel*>list;
    // TODO: check status (delete or not, etc.0
    if( qry->exec() )
    {
        logd("Get all number of item %d", qry->size());
        while (qry->next()) {
            DbModel* item = builder();
            item->setDbId(qry->value(KFieldId).toInt());
            item->setDbStatus(qry->value(KFieldRecordStatus).toInt());
            item->setName(qry->value(KFieldName).toString());
            item->setUid(qry->value(KFieldUid).toString());
            item->setCreatedTime(qry->value(KFieldCreateTime).toInt());
            item->setLastUpdatedTime(qry->value(KFieldLastUpdateItme).toInt());
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
    traced;
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    DbModel* item = nullptr;
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
        item->setCreatedTime(qry.value(KFieldCreateTime).toInt());
        item->setLastUpdatedTime(qry.value(KFieldLastUpdateItme).toInt());
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
    list.append(QString("%1.%2").arg(name(), KFieldName)); // TODO: make as class member?
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

DbModel *DbSqliteTbl::getByUid(const QString &uid, const DbModelBuilder &builder)
{
    traced;
    QHash<QString, int> inFields;
    QList<DbModel*> outList;
    DbModel* retDb = nullptr;
    inFields.insert(QString("%1.%2").arg(name(), KFieldUid), TEXT); // TODO: make this common with name field above???
    logi("Search by uid '%s'", uid.toStdString().c_str());
    qint32 cnt = 0;
    cnt = DbSqliteTbl::search (uid, inFields, builder, &outList, true);

    logi("Found %d", cnt);
    if (cnt > 0 && outList.count() > 0){
        logd("Found, return 1st element");
        retDb = outList[0];
    } else {
        logi("Not found uid '%s'", uid.toStdString().c_str());
        // TODO: safe to print unicode log????
    }

    return retDb;
}

int DbSqliteTbl::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    int ret = 0;
    DbModelBuilder builder = mainModelBuilder();
    if (builder != nullptr) {
        ret = search(keyword, builder, outList);
    } else {
        logi("No buider, Default search one, DO NOTHING, derived class must implement this or implement mainModelBuilder");
        ret = 0;
    }
    return ret;
}

QHash<QString, int> DbSqliteTbl::getSearchFields()
{
    traced;
    QHash<QString, int> list;
    list[COLUMN_NAME(name(), KFieldName)] = TEXT; // TODO: make as class member?
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
    QString queryString = QString("SELECT * FROM %2")
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
//    DB->openDb();
    // TODO: implement it
    // TODO: exact and not exact match???
    QSqlQuery qry(SQLITE->currentDb());
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
    try {
        cnt = runQuery(qry, builder, outList);
    } catch(const std::runtime_error& ex) {
        loge("Runtime Exception! %s", ex.what());
        cnt = 0;
    } catch (const std::exception& ex) {
        loge("Exception! %s", ex.what());
        cnt = 0;
    } catch (...) {
        loge("Exception! Unknown");
        cnt = 0;
    }


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
    builder->addField(KFieldCreateTime, INT64);
    builder->addField(KFieldLastUpdateItme, INT64);
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
//    builder->addValue(KFieldRecordStatus, item->dbStatus());
    // TODO: timezone issue?
    // TODO: time of computer suddenly not correct/change???,
    // should store last open time than compare???
    builder->addValue(KFieldCreateTime, Utils::currentTimeMs());
    builder->addValue(KFieldLastUpdateItme, Utils::currentTimeMs());
    tracede;
    return ErrNone;
}

ErrCode DbSqliteTbl::deleteCondition(DbSqliteDeleteBuilder *builder, const DbModel *item)
{
    traced;
    ErrCode err = ErrNone;
    builder->addCond(KFieldUid, item->uid());
    tracedr(err);
    return err;
}


DbSqlite *DbSqliteTbl::db() const
{
    return mDb;
}
