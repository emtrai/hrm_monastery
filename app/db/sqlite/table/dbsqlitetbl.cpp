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
#include "dbsqlite.h"
#include "exception.h"
#include "datetimeutils.h"

DbSqliteTbl::~DbSqliteTbl(){
    tracein;
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

DbSqliteTbl::DbSqliteTbl(DbSqlite *db, const QString &baseName,
                         const QString &name, qint32 versionCode,
                         const QString &modelName):DbSqliteTbl(db, baseName, name, versionCode)
{
    mHandleModelName = modelName;
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

const QString& DbSqliteTbl::getHandleModelName() const
{
    return mHandleModelName;
}

uint32_t DbSqliteTbl::versionCode() const
{
    return mVersionCode;
}

void DbSqliteTbl::setVersionCode(uint32_t newVersionCode)
{
    mVersionCode = newVersionCode;
}

ErrCode DbSqliteTbl::getDataType(const QString &field, int& dataType)
{
    ErrCode err = ErrNone;
    tracein;
    if (field.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone) {
        logd("get data type for field %s", STR2CHA(field));
        if (this->mFieldDataTypeMap.contains(field)){
            dataType = mFieldDataTypeMap.value(field);
        } else {
            loge("field '%s' not exist", STR2CHA(field));
            err = ErrNotExist;
        }
    }
    traceret(err);
    return err;
}

ErrCode_t DbSqliteTbl::add(DbModel *item)
{
    tracein;
    ErrCode_t err = ErrNone;
    DbSqliteUpdateBuilder* updateBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    DbSqliteInsertBuilder* builder = DbSqliteInsertBuilder::build(name());
    item->dump();
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
                // insert ok, set record as active
                updateBuilder->addValue(KFieldDbStatus, DB_RECORD_ACTIVE);
//                updateBuilder->addValue(KFieldNameId, item->nameId());
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
    traceret(err);
    return err;

}

ErrCode DbSqliteTbl::updateBuilderFromModel(DbSqliteUpdateBuilder *builder,
                                      const QList<QString>& updateField,
                                      const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !builder || updateField.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        logd("Add %lld field to update", updateField.count());
        foreach (QString field, updateField) {
            logd("Update field %s", STR2CHA(field));
            err = updateBuilderFieldFromModel(builder, field, item);
            if (err != ErrNone) {
                loge("update builder field '%s' for model '%s' failed, err=%d",
                     STR2CHA(field), MODELSTR2CHA(item), err);
                break;
            }
//            if (field == KItemUid) {
//                builder->addValue(KFieldUid, item->uid());
//            } else if (field == KItemName){
//                builder->addValue(KFieldName, item->name());
//            } else if (field == KItemNameId){
//                builder->addValue(KFieldNameId, item->nameId());
//            } else if (field == KItemRemark){
//                builder->addValue(KFieldRemark, item->remark());
//            }  else {
//                logd("Field '%s' not support here, may supported by derive class",
//                     STR2CHA(field));
//            }
        }
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !builder || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        logd("Update field %s", STR2CHA(field));
        if (field == KItemUid) {
            builder->addValue(KFieldUid, item->uid());
        } else if (field == KItemName){
            builder->addValue(KFieldName, item->name());
        } else if (field == KItemNameId){
            builder->addValue(KFieldNameId, item->nameId());
        } else if (field == KItemRemark){
            builder->addValue(KFieldRemark, item->remark());
        } else if (field == KItemDbHistory){
            builder->addValue(KFieldDbHistory, item->dbHistory());
        }  else {
            logw("Field '%s' not support here, may supported by derive class",
                 STR2CHA(field));
        }
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::updateTableCondition(DbSqliteUpdateBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    builder->addCond(KFieldUid, item->uid());
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::update(DbModel *item)
{
    tracein;
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
            err = updateBuilderFromModel(updateBuilder, updatedFields, item);
            logd("updateBuilderFromModel ret %d", err);
        } else {
            loge("update table cond failed err %d", err);
        }

        if (err == ErrNone){
            logd("Build SQL Query and execute");
            updateBuilder->addValue(KFieldLastDbUpdateItme, DatetimeUtils::currentTimeMs());
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
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::updateUid(const DbModel *item, const QString &uid)
{
    tracein;
    ErrCode ret = ErrNone;
    // TODO:implement it
    traceret(ret);
    return ret;
}

ErrCode DbSqliteTbl::update(const QString &uid, const QHash<QString, FieldValue>& fieldValues)
{
    tracein;
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
            if (mFieldDataTypeMap.contains(field)) {
                logd("Update field %s for table '%s'", STR2CHA(field), STR2CHA(name()));
                updateBuilder->addValue(field,
                                        fieldValues[field].value,
                                        fieldValues[field].dataType);
            } else {
                logw("Field '%s' is not exist in table '%s'",
                     STR2CHA(field), STR2CHA(name()));
                // TODO: still continue here, so that model handler can add common fields
                // and some field may not exist
                // FIXME: ignore error & continue, is it ok??
            }
        }

        if (err == ErrNone){
            logd("Build SQL Query and execute");
            updateBuilder->addValue(KFieldLastDbUpdateItme, DatetimeUtils::currentTimeMs());
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
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::update(const QString &uid, const QHash<QString, QString> &inFields)
{
    ErrCode err = ErrNone;
    tracein;
    QHash<QString, FieldValue> fieldValues;
    int datatype = 0;
    logd("update for uid '%s', number of field %lld", STR2CHA(uid), inFields.size());
    TRY {
    logd("convert to field value");
    foreach (QString field, inFields.keys()) {
        err = getDataType(field, datatype);
        if (err == ErrNone) {
            logd("field '%s' data type %d", STR2CHA(field), datatype);
            QString value = inFields[field].trimmed();
            fieldValues.insert(field, FieldValue(value, datatype));
        } else {
            logw("get data type for field '%s' failed, err=%d", STR2CHA(field), err);
            // TODO: still continue here, so that model handler can add common fields
            // and some field may not exist
            // FIXME: ignore error & continue, is it ok??
//            break;
        }
    }
    if (err == ErrNone && (fieldValues.size() == 0)) {
        err = ErrNoData;
        loge("no data to update");
    }
    if (err == ErrNone) {
        logd("do update");
        err = update(uid, fieldValues);
    }
    } CATCH(err, "Failed to update '%s'", STR2CHA(uid));
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::deleteSoft(DbModel *item)
{
    ErrCode err = ErrNone;
    tracein;
    // TODO: made delete condition customize/dynamic by appendDeleteCondition
//    QHash<QString, QString> deleteCond = appendDeleteCondition(item);
//    if (deleteCond.count() > 0) {
//        update(item->uid(), )
//    }
    QHash<QString, FieldValue> fields;
    fields.insert(KFieldDbStatus, FieldValue(QString::number(DB_RECORD_DElETED), INT32));
    err = update(item->uid(), fields);
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::deleteHard(DbModel *item)
{
    tracein;
    ErrCode_t err = ErrNone;
    DbSqliteDeleteBuilder* deleteBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;

    logd("build delete builder");
    // TODO: check to make sure that uid does not exist?????
    deleteBuilder = DbSqliteDeleteBuilder::build(name());
    err = appendDeleteCondition(deleteBuilder, item);
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
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::deleteHard(const QHash<QString, QString> &condition)
{
    tracein;
    ErrCode_t err = ErrNone;
    DbSqliteDeleteBuilder* deleteBuilder = nullptr;
    QSqlQuery* updateQry = nullptr;
    if (condition.isEmpty()) {
        err = ErrInvalidArg;
        loge("Empty condition hash map");
    }
    if (err == ErrNone) {
        logd("build delete builder");
        // TODO: check to make sure that uid does not exist?????
        deleteBuilder = DbSqliteDeleteBuilder::build(name());
        if (!deleteBuilder) {
            err = ErrNoMemory;
            logd("lack of memory");
        }
    }
    if (err == ErrNone){
        foreach(QString field, condition.keys()) {
            logd("Add condition '%s', value '%s'",
                 STR2CHA(field), STR2CHA(condition[field]));
            deleteBuilder->addCond(field, condition[field]);
        }
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
    traceret(err);
    return err;
}

bool DbSqliteTbl::isExist(const DbModel *item)
{
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    bool exist = false;
    bool queryOk = false;
    ErrCode err = ErrNone;
    tracein;
    if (!item->uid().isEmpty()) {
        QString queryString = QString("SELECT COUNT(*) "
                                      "FROM %1 WHERE %2 = :uid").arg(name(), KFieldUid);
        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        qry.bindValue( ":uid", item->uid() );
        queryOk = true;
    } else if (!item->nameId().isEmpty()) {
        QString queryString = QString("SELECT COUNT(*) "
                                      "FROM %1 WHERE %2 = :nameid").arg(name(), KFieldNameId);
        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        logd("bind nameid '%s'", STR2CHA(item->nameId()));
        qry.bindValue( ":nameid", item->nameId() );
        queryOk = true;
    } else {
        QString cond;
        int datatype = 0;
        QHash<QString, QString> inFields = getFieldsCheckExists(item);

        bool modelStatusExist = false;
        if (!inFields.empty()) {
            foreach (QString field, inFields.keys()) {
                if (field == KFieldModelStatus) {
                    modelStatusExist = true;
                    continue;
                }
                if (!cond.isEmpty()) {
                    cond += " AND ";
                }
                err = getDataType(field, datatype);
                if (err != ErrNone) {
                    break;
                }
                logd("field '%s' data type %d", STR2CHA(field), datatype);
                if (datatype == TEXT) {
                    cond += QString("lower(%1) = :%1_TEXT").arg(field);
                } else {
                    cond += QString("%1 = :%1_INT").arg(field);
                }
            }
            if (modelStatusExist) {
                bool ok = false;
                QString modelStatusStr = inFields.value(KFieldModelStatus);
                int status = modelStatusStr.toInt(&ok);
                if (ok) {
                    appendModelStatusCond(cond, status);
                } else {
                    loge("convert status value failed, value '%s'", STR2CHA(modelStatusStr));
                }
            }

            if (err == ErrNone) {
                QString queryString = QString("SELECT * FROM %1 WHERE %2 ORDER BY NAME ASC")
                                          .arg(name(), cond);

                qry.prepare(queryString);
                logd("Query String '%s'", queryString.toStdString().c_str());

                // TODO: check sql injection issue
                foreach (QString field, inFields.keys()) {
                    if (field == KFieldModelStatus) {
                        continue;
                    }
                    // TODO: if value is empty, data may not match
                    // Check if value is empty, for string, it seem '', but for integer,
                    // check again, as this process is string format, stupid
                    err = getDataType(field, datatype);
                    if (err != ErrNone) {
                        break;
                    }
                    if (datatype == TEXT) {
                        qry.bindValue( QString(":%1_TEXT").arg(field), inFields[field].trimmed().toLower());
                        logd("bind txt :'%s_TEXT' to '%s'", STR2CHA(field),
                             STR2CHA(inFields[field].trimmed().toLower()));
                    } else {
                        bool ok = false;
                        qint64 value = inFields[field].toLong(&ok);
                        if (ok) {
                            qry.bindValue( QString(":%1_INT").arg(field), value);
                            logd("bind int :'%s_INT' to '%d'", STR2CHA(field), value);
                        } else {
                            loge("cannot convert string '%s' to int", STR2CHA(inFields[field]));
                            queryOk = false;
                            err = ErrFailedConvert;
                            break;
                        }
                    }
                }
            }
            queryOk = (err == ErrNone);
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

bool DbSqliteTbl::isNameidExist(const QString &nameId)
{
    QSqlQuery qry(SQLITE->currentDb());
    bool exist = false;
    bool queryOk = false;
    tracein;
    if (!nameId.isEmpty()) {
        logd("nameid to check exist '%s'", STR2CHA(nameId));
        QString queryString = QString("SELECT COUNT(*) "
                                      "FROM %1 WHERE %2 = :nameid").arg(name(), KFieldNameId);
        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        qry.bindValue( ":nameid", nameId);
        queryOk = true;
    } else {
        queryOk = false;
        loge("empty nameid");

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
    tracein;
    QHash<QString, QString> list;
    if (item) {
        list[KFieldNameId] = item->nameId(); // TODO: make as class member?
    } else {
        logw("item is null, tbl '%s'", STR2CHA(name()));
    }
    traceout;
    return list;
}


ErrCode DbSqliteTbl::updateDbModelDataFromQuery(DbModel* item, const QSqlQuery& qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        item->setNameId(qry.value(KFieldNameId).toString().trimmed());
        item->setDbId(qry.value(KFieldId).toInt());
        item->setName(qry.value(KFieldName).toString().trimmed());
        item->setUid(qry.value(KFieldUid).toString());
        item->setDbHistory(qry.value(KFieldDbHistory).toString());
        item->setDbStatus(qry.value(KFieldDbStatus).toInt());
        item->setDbCreatedTime(qry.value(KFieldDbCreateTime).toInt());
        item->setLastDbUpdatedTime(qry.value(KFieldLastDbUpdateItme).toInt());
        item->setRemark(qry.value(KFieldRemark).toString());
    }
    traceout;
    return err;
}

ErrCode DbSqliteTbl::filterFieldCond(int fieldId,
                                int operatorId,
                                QString fieldValueName,
                                const DbModel* parentModel,
                                QString& cond,
                                int& dataType,
                                bool& isExact
                                )
{
    tracein;
    bool isOk = false;
    QString fieldName;
    QString field;
    ErrCode err = ErrNone;
    logd("fieldId %d or operatorId %d", fieldId, operatorId);
    if (fieldId >= FILTER_FIELD_MAX || operatorId >= FILTER_OP_MAX) {
        loge("invalid fieldId %d or operatorId %d", fieldId, operatorId);
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        fieldName = getFieldNameFromId(fieldId, &isOk);
        if (!isOk || fieldName.isEmpty()) {
            err = ErrNotFound;
            loge("not found fiedName for fieldId %d", fieldId);
        }
        logd("fieldName '%s'", STR2CHA(fieldName));
    }

    if (err == ErrNone) {
        switch (fieldId) {
        case FILTER_FIELD_NAME:
            field = QString("%1.%2").arg(name(), fieldName);
            dataType = TEXT;
            break;
        case FILTER_FIELD_MODEL_STATUS:
            field = fieldName;
            dataType = INT64;
            break;
        case FILTER_FIELD_FULL_NAME:
            field = fieldName;
            dataType = TEXT;
            break;
            // TODO: support more filter???
        default:
            field = fieldName;
            dataType = TEXT;
            break;
        }
    }
    logd("field '%s'", STR2CHA(field));
    if (err == ErrNone) {
        switch (operatorId) {
        case FILTER_OP_EQUAL:
            cond = QString("lower(%1) = ").arg(field);
            isExact = true;
            break;
        case FILTER_OP_NOT_EQUAL:
            cond = QString("lower(%1) != ").arg(field);
            isExact = true;
            break;

        case FILTER_OP_CONTAIN:
            cond = QString("lower(%1) like ").arg(field);
            isExact = false;
            break;

        case FILTER_OP_NOT_CONTAIN:
            cond = QString("lower(%1) not like ").arg(field);
            isExact = false;
            break;
        case FILTER_OP_LESS:
            cond = QString("%1 < ").arg(field);
            isExact = true;
            break;
        case FILTER_OP_LESS_EQ:
            cond = QString("%1 <= ").arg(field);
            isExact = true;
            break;
        case FILTER_OP_GREATER:
            cond = QString("%1 > ").arg(field);
            isExact = true;
            break;
        case FILTER_OP_GREATER_EQ:
            cond = QString("%1 >= ").arg(field);
            isExact = true;
            break;
        default:
            err = ErrNotSupport;
            loge("operatorId 0x%x not support", operatorId);
            break;
        }
    }
    if (err == ErrNone) {
        cond += fieldValueName;
    }

    logd("cond '%s'", STR2CHA(cond));
    logd("isExact %d", isExact);
    traceret(err);
    return err;
}


ErrCode DbSqliteTbl::filter(int fieldId,
                            int operatorId,
                            const QString& keyword,
                            const DbModelBuilder& builder,
                            const DbModel* parentModel,
                            QList<DbModel*>* outList,
                            qint64 dbStatus,
                            int from,
                            int noItems,
                            int* total)
{
    tracein;
    // TODO: implement it
    // TODO: check data typpe text vs integer
    //    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    ErrCode err = ErrNone;
    QString cond;
    int fieldDataType = 0;
    QString fieldValueName = ":fieldValue";
    bool isFieldValueExact = false;
    logi("Filter keyword '%s'", STR2CHA(keyword));
    err = filterFieldCond(fieldId, operatorId, fieldValueName, parentModel,
                          cond, fieldDataType, isFieldValueExact);
    if (err == ErrNone) {
        appendDbStatusCond(cond, dbStatus);
    }
    if (err == ErrNone) {
        logd("Query cond '%s'", STR2CHA(cond));
        QString queryString = getFilterQueryString(fieldId, cond);

        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        logd("bind fieldValueName %s='%s'", STR2CHA(fieldValueName), STR2CHA(keyword));
        if (isFieldValueExact) {
            qry.bindValue(fieldValueName, QString("%1").arg(keyword.trimmed().toLower()) );
        } else {
            qry.bindValue(fieldValueName, QString("\%%1\%").arg(keyword.trimmed().toLower()) );
        }
        cnt = runQuery(qry, builder, outList);

        logi("Found %d for keyword '%s'", cnt, STR2CHA(keyword));
    } else {
        loge("get filter cond failed err %d", err);
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::filterFieldCond(const QList<FilterKeyworkItem *> &filters,
                                     QString &cond,
                                     QHash<QString, QString> &bindValues,
                                     const DbModel* parentModel)
{
    tracein;
    ErrCode err = ErrNone;
    int fieldDataType = 0;
    bool isFieldValueExact = false;
    logd("no of filters %lld", filters.size());
    if (filters.size() > 0) {
        int cnt = 0;
        foreach (FilterKeyworkItem* item, filters) {
            if (!item) continue;
            QString fieldValueName = QString(":fieldValue_%1_%2").arg(item->catetoryid).arg(cnt++);
            QString conditem;
            err = filterFieldCond(item->catetoryid, item->opFlags, fieldValueName,
                                  parentModel, conditem, fieldDataType, isFieldValueExact);
            if (err != ErrNone) {
                loge("failed to filter filed cond, err %d", err);
                break;
            }
            if (cond.isEmpty()) {
                cond = conditem;
            } else {
                cond = QString("(%1) AND (%2)").arg(cond).arg(conditem);
            }
            if (isFieldValueExact) {
                bindValues.insert(fieldValueName,
                                  QString("%1").arg(item->keyword.trimmed().toLower()) );
            } else {
                bindValues.insert(fieldValueName,
                                  QString("\%%1\%").arg(item->keyword.trimmed().toLower()) );
            }
        }
    } else {
        loge("No filter!!");
        err = ErrNoData;
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::filter(const QList<FilterKeyworkItem *> &filters,
                            const DbModelBuilder &builder,
                            const DbModel* parentModel,
                            QList<DbModel *> *outList, qint64 dbStatus,
                            int from, int noItems, int *total)
{
    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    ErrCode err = ErrNone;
    QString cond;
    QHash<QString, QString> bindValues;
    QString queryString;
    logd("no filter item %lld", filters.size());
    err = filterFieldCond(filters, cond, bindValues, parentModel);
    if (err == ErrNone) {
        appendDbStatusCond(cond, dbStatus);
    }
    if (err == ErrNone) {
        logd("Query cond '%s'", STR2CHA(cond));
        queryString = getFilterQueryString(filters, cond);
        if (queryString.isEmpty()) {
            err = ErrInvalidQuery;
            loge("Invalid query string");
        }
    }
    if (err == ErrNone) {
        qry.prepare(queryString);
        logd("Query String '%s'", STR2CHA(queryString));

        foreach (QString key, bindValues.keys()) {
            logd("bind key %s='%s'", STR2CHA(key), STR2CHA(bindValues.value(key)));
            qry.bindValue(key, bindValues.value(key));
        }
        cnt = runQuery(qry, builder, outList);

        logi("Found %d items", cnt);
    } else {
        loge("get filter cond failed err %d", err);
    }
    traceret(err);
    return err;
}

//ErrCode DbSqliteTbl::filter(int fieldId,
//                        int operatorId,
//                        const QString& keyword,
//                        const DbModelBuilder& builder,
//                        const DbModel* parentModel,
//                        QList<DbModel*>* outList,
//                        qint64 dbStatus,
//                        int from,
//                        int noItems,
//                        int* total)
//{
//    tracein;
//    // TODO: implement it
//    // TODO: check data typpe text vs integer
////    DB->openDb();
//    QSqlQuery qry(SQLITE->currentDb());
//    qint32 cnt = 0;
//    ErrCode err = ErrNone;
//    logi("Filter keyword '%s'", keyword.toStdString().c_str());
//    QString cond;
//    bool isOk = false;
//    QString fieldName = getFieldNameFromId(fieldId, &isOk);
//    QString field;
//    switch (fieldId) {
//    case FILTER_FIELD_NAME:
//        field = QString("%1.%2").arg(name(), fieldName);
//        break;
//    default:
//        field = fieldName;
//        break;
//    }
//    if (isOk && !fieldName.isEmpty()) {
//        switch (operatorId) {
//        case FILTER_OP_EQUAL:
//            cond = QString("lower(%1) = :keywordexact").arg(field);
//            break;
//        case FILTER_OP_NOT_EQUAL:
//            cond = QString("lower(%1) != :keywordexact").arg(field);
//            break;

//        case FILTER_OP_CONTAIN:
//            cond = QString("lower(%1) like :keyword").arg(field);
//            break;

//        case FILTER_OP_NOT_CONTAIN:
//            cond = QString("lower(%1) not like :keyword").arg(field);
//            break;
//        case FILTER_OP_LESS:
//            cond = QString("%1 < :value").arg(field);
//            break;
//        case FILTER_OP_LESS_EQ:
//            cond = QString("%1 <= :value").arg(field);
//            break;
//        case FILTER_OP_GREATER:
//            cond = QString("%1 > :value").arg(field);
//            break;
//        case FILTER_OP_GREATER_EQ:
//            cond = QString("%1 >= :value").arg(field);
//            break;
//        default:
//            break;
//        }
//    }
//    appendDbStatusCond(cond, dbStatus);
//    logd("Query cond '%s'", cond.toStdString().c_str());
//    QString queryString = getFilterQueryString(fieldId, cond);

//    qry.prepare(queryString);
//    logd("Query String '%s'", queryString.toStdString().c_str());

//    // TODO: check sql injection issue
//    logd("bind keyword='%s'", STR2CHA(keyword));
//    qry.bindValue( ":keyword", QString("%1%").arg(keyword.trimmed().toLower()) );
//    qry.bindValue( ":keywordexact", QString("%1").arg(keyword.trimmed().toLower()) );
//    qry.bindValue( ":value", QString("%1").arg(keyword.trimmed().toLower()) );
//    cnt = runQuery(qry, builder, outList);

//    logi("Found %d", cnt);
//    traceout;
//    return err;
//}

ErrCode DbSqliteTbl::updateQueryromFields(const QHash<QString, QString>& inFields,
                                          QSqlQuery &qry, bool isMatchAllField,
                                          QString initQueryString)
{
    tracein;
    QString cond;
    ErrCode err = ErrNone;
    int datatype = 0;
    if (inFields.empty()) {
        loge("no field to check exist");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        foreach (QString field, inFields.keys()) {
            err = getDataType(field, datatype);
            if (err == ErrNone) {
                if (!cond.isEmpty()) {
                    cond += isMatchAllField?" AND ":" OR ";
                }
                logd("field '%s' data type %d", STR2CHA(field), datatype);
                if (datatype == TEXT) {
                    cond += QString("lower(%1) = :%1_TEXT").arg(field);
                } else {
                    cond += QString("%1 = :%1_INT").arg(field);
                }
            } else {
                if (!isMatchAllField && (err == ErrNotExist)) {
                    err = ErrNone;
                    logi("field '%s' not exist, but don't need to match all field here so just skip it",
                         STR2CHA(field));
                } else {
                    loge("get data type for field '%s' failed, err=%d", STR2CHA(field), err);
                    break;
                }
            }
        }
    }
    if (err == ErrNone) {
        QString queryString = initQueryString.isEmpty()?(QString("SELECT * FROM %1").arg(name())):initQueryString;
        queryString += " WHERE " + cond;

        qry.prepare(queryString);
        logd("Query String '%s'", queryString.toStdString().c_str());

        // TODO: check sql injection issue
        foreach (QString field, inFields.keys()) {
            // TODO: if value is empty, data may not match
            // Check if value is empty, for string, it seem '', but for integer,
            // check again, as this process is string format, stupid
            err = getDataType(field, datatype);
            if (err == ErrNone) {
                if (datatype == TEXT) {
                    qry.bindValue( QString(":%1_TEXT").arg(field), inFields[field].trimmed().toLower());
                    logd("bind txt :'%s' to '%s'", STR2CHA(field),
                         STR2CHA(inFields[field].trimmed().toLower()));
                } else {
                    bool ok = false;
                    qint64 value = inFields[field].toLong(&ok);
                    if (ok) {
                        qry.bindValue( QString(":%1_INT").arg(field), value);
                        logd("bind int :'%s' to '%lld'", STR2CHA(field), value);
                    } else {
                        loge("cannot convert string '%s' to int", STR2CHA(inFields[field]));
                        err = ErrSqlFailed;
                        break;
                    }
                }
            } else {
                if (!isMatchAllField && (err == ErrNotExist)) {
                    err = ErrNone;
                    logi("field '%s' not exist, but don't need to match all field here so just skip it",
                         STR2CHA(field));
                } else {
                    loge("get data type for field '%s' failed, err=%d", STR2CHA(field), err);
                    break;
                }
            }
        }
    }
    logd("err %d", err);

    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::getListItems(const QHash<QString, QString>& inFields,
                                  const DbModelBuilder &builder,
                                  QList<DbModel *> *outList,
                                  bool isMatchAllField,
                                  qint64 dbStatus, int from, int noItems,
                                  int *total)
{
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    tracein;
    err = updateQueryromFields(inFields, qry, isMatchAllField);
    int cnt = 0;
    logd("err %d", err);
    if (err == ErrNone) {
        try {
            cnt = runQuery(qry, builder, outList);
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
    }
    if (total) *total = cnt;
    logd("cnt=%d", cnt);
    traceret(err);
    return err;
}

int DbSqliteTbl::getTotalItemCount(qint64 modelStatus,const QString& req, qint64 dbStatus)
{

    tracein;
    QSqlQuery qry(SQLITE->currentDb());
    int cnt = 0;
    int ret = 0;
    QString cond;
    ErrCode err = ErrNone;
    dbg(LOG_DEBUG, "Get tital item count modelStatus '%lld', dbStatus '%lld' req '%s'",
        modelStatus, dbStatus, STR2CHA(req));
    appendDbStatusCond(cond, dbStatus);
    if (mFieldDataTypeMap.contains(KFieldModelStatus)) {
        appendModelStatusCond(cond, modelStatus);
    } else {
        logd("field %s not exist in table '%s'", KFieldModelStatus, STR2CHA(name()));
    }
    QString queryString = getCountTotalQueryString(cond, req);

    qry.prepare(queryString);
    dbg(LOG_DEBUG, "Query String '%s'", STR2CHA(queryString));

    try {
        if (qry.exec() && qry.next()) {
            cnt = qry.value(0).toInt();;
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

    dbg(LOG_DEBUG, "Found %d", cnt);
    if (err == ErrNone) {
        ret = cnt;
    } else {
        ret = -(err);
        loge("get total count err=%d", err);
    }
    traceret(ret);
    return ret;
}

int DbSqliteTbl::runQuery(QSqlQuery &qry, const DbModelBuilder& builder,
                       QList<DbModel *> *outList)
{
    tracein;
    int cnt = 0;
    ErrCode err = ErrNone;
    if( qry.exec() )
    {
        logd("get next, isvalid=%d", qry.isValid());
        logd("Get all number of item %d", qry.size());
        while (qry.next()) {
            // qry.size may not support, so cannot use here
            // TODO: check if any better way to get the number of items;
            cnt++;
            if (outList && builder){
                logd("found one build item");
                DbModel* item = builder();
                logd("Updae model from query");
                err = updateDbModelDataFromQuery(item, qry);
                if (err == ErrNone) {
                    logd("add model '%s' to outlist", STR2CHA(item->toString()));
                    outList->append(item); // TODO: when it cleaned up?????
                } else {
                    logw("Update model from query err=%d", err);
                }
            }
        }
    }
    else {
        loge( "Failed to execute %s", STR2CHA(qry.executedQuery()) );
        loge( "Last error %s", STR2CHA(qry.lastError().text()) );
    }

    logi("Found %d", cnt);
    traceret(cnt);
    return cnt;
}

QSqlQuery *DbSqliteTbl::getAllQuery(qint64 dbstatus)
{
    tracein;
//    DB->openDb();

    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());
    tracein;
    // TODO: check record status????
    QString queryString = getAllQueryString(dbstatus);
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    return qry;
}

QString DbSqliteTbl::getAllQueryString(qint64 dbstatus)
{
    tracein;
    QString cond;
    logd("status to get all query string 0x%x", dbstatus);
    if (dbstatus & DB_RECORD_ACTIVE) {
        cond = QString("%1.%2=%3").arg(name(), KFieldDbStatus).arg(DB_RECORD_ACTIVE);
    }
    if (dbstatus & DB_RECORD_DElETED) {
        if (!cond.isEmpty()) {
            cond += " OR ";
        }
        cond += QString("%1.%2=%3").arg(name(), KFieldDbStatus).arg(DB_RECORD_DElETED);
    }
    return getSearchQueryString(cond.isEmpty()?nullptr:cond);
}


DbModelBuilder DbSqliteTbl::mainModelBuilder()
{
    return nullptr;
}

QString DbSqliteTbl::getCountTotalQueryString(const QString &cond, const QString &req)
{
    tracein;
    QString ret = QString("SELECT count(*) FROM %1").arg(name());
    if (!cond.isEmpty()) {
        ret += QString(" WHERE %1").arg(cond);
    }
    logd("Querry string '%s'", STR2CHA(ret));
    traceout;
    return ret;
}


QList<DbModel *> DbSqliteTbl::getAll(const DbModelBuilder& builder, qint64 status,
                                     int from, int noItems, int* total)
{

    tracein;
    QSqlQuery* qry = getAllQuery(status);
    QList<DbModel*>list;
    ErrCode err = ErrNone;
    // TODO: check status (delete or not, etc.0
    if( qry->exec() )
    {
        logd("Get all number of item %d", qry->size());
        while (qry->next()) {
            DbModel* item = builder();
            item->setNameId(qry->value(KFieldNameId).toString());
            item->setDbId(qry->value(KFieldId).toInt());
            item->setDbStatus(qry->value(KFieldDbStatus).toInt());
            item->setName(qry->value(KFieldName).toString());
            item->setUid(qry->value(KFieldUid).toString());
            item->setDbCreatedTime(qry->value(KFieldDbCreateTime).toInt());
            item->setLastDbUpdatedTime(qry->value(KFieldLastDbUpdateItme).toInt());
            // TODO: validate value before, i.e. toInt return ok
            err = updateDbModelDataFromQuery(item, *qry);
            if (err == ErrNone) {
                logd("add model '%s' to outlist", STR2CHA(item->toString()));
                list.append(item); // TODO: when it cleaned up?????
            } else {
                logw("Update model from query err=%d", err);
            }
        }
    }
    else {
        loge( "Failed to execute %s", qry->lastQuery().toStdString().c_str() );
        loge( "Last error %s", qry->lastError().text().toStdString().c_str() );
    }

    logd("Found %d", (int)list.size());
    delete qry;
    traceout;
    return list;
}

DbModel *DbSqliteTbl::getModel(qint64 dbId, const DbModelBuilder& builder)
{
    tracein;
//    DB->openDb();
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    DbModel* item = nullptr;
    // TODO: check record status????
    QString queryString = QString("SELECT * FROM %1 where id=:id ORDER BY NAME ASC").arg(name());
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
        item->setNameId(qry.value(KFieldNameId).toString());
        item->setDbId(qry.value(KFieldId).toInt());
        item->setDbStatus(qry.value(KFieldDbStatus).toInt());
        item->setName(qry.value(KFieldName).toString());
        item->setUid(qry.value(KFieldUid).toString());
        item->setDbCreatedTime(qry.value(KFieldDbCreateTime).toInt());
        item->setLastDbUpdatedTime(qry.value(KFieldLastDbUpdateItme).toInt());
        err = updateDbModelDataFromQuery(item, qry);
    }

    if (err != ErrNone && item){
        loge("Get model failed, err=%d", err);
        delete item;
    }

    traceout;
    return item;
}

ErrCode DbSqliteTbl::getColumnList(QHash<QString, QString>& colList)
{
    tracein;
    ErrCode err = ErrNone;
    QString sql = QString("PRAGMA table_info('%1')").arg(mName);
    QSqlQuery qry(SQLITE->currentDb());
    qry.prepare(sql);
    logd("sql '%s'", STR2CHA(sql));
    err = db()->execQuery(&qry);
    if (err == ErrNone) {
        if( qry.exec() ) {
            logd("get next, isvalid=%d", qry.isValid());
            logd("Get all number of item %d", qry.size());
            while (qry.next()) {
                QString name = qry.value("name").toString();
                QString type = qry.value("type").toString();
                colList.insert(name, type);
                logd("name '%s', type '%s'", STR2CHA(name), STR2CHA(type));
            }
        }
        else {
            loge( "Failed to execute %s", STR2CHA(qry.executedQuery()) );
            loge( "Last error %s", STR2CHA(qry.lastError().text()) );
        }
    } else {
        loge("Execute '%s' failed, err=%d", STR2CHA(sql), err);
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::addTableColumn(const QHash<QString, TableFieldDatatype_t> &columnField)
{
    tracein;
    ErrCode err = ErrNone;
    QHash<QString, QString> collist;
    err = getColumnList(collist);
    if (err == ErrNone) {
        if (columnField.size() > 0) {
            logd("Add %d column", columnField.size());

            QString dataTypeString;
            TableFieldDatatype_t dataType;
            QString sql;
            foreach( QString name, columnField.keys() ) {
                if (collist.contains(name)) {
                    logi("Column '%s' existed, skip", STR2CHA(name));
                    continue;
                }
                dataType = columnField[name];
                logd("Add column '%s', datatype %d", STR2CHA(name), dataType);
                dataTypeString = getDateTypeString(dataType);
                if (!dataTypeString.isEmpty()) {
                    sql = QString("ALTER TABLE %1 ADD %2 %3 ;").arg(mName, name, dataTypeString);
                    if (!sql.isEmpty()){
                        logd("Sql '%s'", STR2CHA(sql));
                        err = db()->execQuery(sql);
                    } else{
                        loge("Invalid sql command");
                        err = ErrFailed;
                    }
                } else {
                    err = ErrInvalidData;
                    loge("invalid data type for column '%s', %d", STR2CHA(name), columnField[name]);
                }

                if (err == ErrNone) {
                    if (!mFieldDataTypeMap.contains(name)) {
                        logd("update mFieldDataTypeMap name '%s', type %d ", STR2CHA(name), dataType);
                        mFieldDataTypeMap[name] = dataType;
                    } else {
                        logd("mFieldDataTypeMap name '%s' exist", STR2CHA(name));
                    }
                } else {
                    loge("Alter table for column '%s' failed, err=%d", STR2CHA(name), err);
                    break;
                }
            }
        }
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::checkOrCreateTable()
{
    tracein;
    ErrCode err = ErrNone;
    logd("Create table '%s'", name().toStdString().c_str());
    // TODO: should checktable before creating???
    // TODO: insert to management table
    DbSqliteTableBuilder* builder = getTableBuilder();
    QString sql;
    if (builder) {
        mFieldDataTypeMap = builder->fields();
        sql = builder->buildSqlStatement();
        delete builder;
    } else {
        loge("build table builder failed");
        err = ErrNoBuilder;
    }

    if (err == ErrNone) {
        if (!sql.isEmpty()){
            err = db()->execQuery(sql);
        } else{
            loge("Invalid sql command");
            err = ErrFailed;
        }
    }

#ifdef DEBUG_TRACE
    logd("mFieldDataTypeMap of '%s'", STR2CHA(name()));
    foreach (QString field, mFieldDataTypeMap.keys()) {
        logd("%s: %d", STR2CHA(field), mFieldDataTypeMap[field]);
    }
#endif
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::onDbMigration(qint64 oldVer, qint64 newVer)
{
    tracein;
    loge("WARNING!!! Version of DB change from old 0x%lx to 0x%lx, but derived class not handle it!!!!!",
         oldVer, newVer);
    return ErrNone;
}

ErrCode DbSqliteTbl::onTblMigration(qint64 oldVer)
{
    tracein;
    loge("WARNING!!! Version of Table change from old 0x%lx to 0x%lx, but derived class not handle it!!!!!",
         oldVer, mVersionCode);
    return ErrNone;
}

QList<QString> DbSqliteTbl::getNameFields()
{
    tracein;
    QList<QString> list;
    list.append(QString("%1.%2").arg(name(), KFieldName)); // TODO: make as class member?
    return list;
}

DbModel *DbSqliteTbl::getByUid(const QString &uid, const DbModelBuilder &builder)
{
    tracein;
    QHash<QString, int> inFields;
    QList<DbModel*> outList;
    DbModel* retDb = nullptr;
    inFields.insert(QString("%1.%2").arg(name(), KFieldUid), TEXT); // TODO: make this common with name field above???
    logi("Search by uid '%s'", STR2CHA(uid));
    ErrCode err = ErrNone;
    err = DbSqliteTbl::search(uid, inFields, builder, &outList, true);

    logi("Search err=%d", err);
    if (err == ErrNone && outList.count() > 0){
        logd("Found, return 1st element");
        retDb = CLONE_DBMODEL(outList[0]);
    } else {
        logi("Not found uid '%s'", uid.toStdString().c_str());
        // TODO: safe to print unicode log????
    }
    RELEASE_LIST_DBMODEL(outList);
    traceout;
    return retDb;
}

DbModel *DbSqliteTbl::getByNameId(const QString &nameId, const DbModelBuilder &builder)
{
    tracein;
    QHash<QString, int> inFields;
    QList<DbModel*> outList;
    DbModel* retDb = nullptr;
    inFields.insert(QString("%1.%2").arg(name(), KFieldNameId), TEXT); // TODO: make this common with name field above???
    logi("Search by nameid '%s'", nameId.toStdString().c_str());
    ErrCode err = ErrNone;
    err = DbSqliteTbl::search(nameId, inFields, builder, &outList, true);

    logi("Search err=%d", err);
    if (err == ErrNone && outList.count() > 0){
        logd("Found, return 1st element");
        retDb = outList[0];
    } else {
        logi("Not found nameId '%s'", nameId.toStdString().c_str());
        // TODO: safe to print unicode log????
    }
    traceout;
    return retDb;

}

ErrCode DbSqliteTbl::search(const QString &keyword, QList<DbModel *> *outList,
                        qint64 dbStatus,
                        int from,
                        int noItems,
                        int* total)
{
    tracein;
    ErrCode ret = ErrNone;
    DbModelBuilder builder = mainModelBuilder();
    if (builder != nullptr) {
        ret = search(keyword, builder, outList, dbStatus, from, noItems, total);
    } else {
        logi("No buider, Default search one, DO NOTHING, derived class must implement this or implement mainModelBuilder");
        ret = ErrNoBuilder;
    }
    return ret;
}

QHash<QString, int> DbSqliteTbl::getSearchFields()
{
    tracein;
    QHash<QString, int> list;
    list[COLUMN_NAME(name(), KFieldName)] = TEXT; // TODO: make as class member?
    return list;
}

ErrCode DbSqliteTbl::search(const QString &keyword, const DbModelBuilder &builder, QList<DbModel *> *outList,
                        qint64 dbStatus,
                        int from,
                        int noItems,
                        int* total)
{
    tracein;
    QHash<QString, int> inFields = getSearchFields();
    logi("Search '%s', status=%d", keyword.toStdString().c_str(), dbStatus);
    logd("from %d noItems %d", from, noItems);
    ErrCode err = ErrNone;
    err = search(keyword, inFields, builder, outList, dbStatus, from, noItems);
    if (err == ErrNone && outList) {
        logd("found %d item, total %d", outList->size(), total?*total:0);
    }
    logi("search err=%d", err);
    traceret(err);
    return err;
}

QString DbSqliteTbl::toDbStatusCond(qint64 dbStatus)
{
    tracein;
    logd("dbStatus=0x%llx", dbStatus);
    QString condStatus;
    const QList<int>* statusList = DbModel::getDbStatusList();
    foreach (int status, (*statusList)) {
        if ((status & dbStatus) != 0) {
            if (!condStatus.isEmpty()) {
                condStatus += " OR ";
            }
            condStatus += QString("%1.%2=%3").arg(name(), KFieldDbStatus).arg(status);
        }
    }
    logd("Status cond string=%s", STR2CHA(condStatus));
    traceout;
    return condStatus;
}

void DbSqliteTbl::appendDbStatusCond(QString& cond, qint64 dbStatus)
{
    tracein;
    logd("append db status 0x%llx to cond", dbStatus);
    QString condStatus = toDbStatusCond(dbStatus);
    if (!condStatus.isEmpty()) {
        if (cond.isEmpty()) {
            cond = condStatus;
        } else {
            cond = QString("(%1) AND (%2)").arg(cond, condStatus);
        }
    }
    logd("cond with cond='%s'", STR2CHA(cond));
    traceout;
}

QString DbSqliteTbl::toModelStatusCond(qint64 modelStatus)
{
    tracein;
    logd("modelStatus=0x%llx", modelStatus);
    QString condStatus;
    if (modelStatus != MODEL_STATUS_MAX) {
        const QList<int>* statusList = DbModel::getModelStatusList();
        foreach (int status, (*statusList)) {
            if ((status & modelStatus) != 0) {
                if (!condStatus.isEmpty()) {
                    condStatus += " OR ";
                }
                condStatus += QString("(%1.%2=%3)").arg(name(), KFieldModelStatus).arg(status);
            }
        }
    }
    logd("Status cond string=%s", STR2CHA(condStatus));
    traceout;
    return condStatus;
}

void DbSqliteTbl::appendModelStatusCond(QString& cond, qint64 modelStatus)
{
    tracein;
    logd("append model status 0x%llx to cond", modelStatus);
    QString condStatus = toModelStatusCond(modelStatus);
    if (!condStatus.isEmpty()) {
        if (cond.isEmpty()) {
            cond = condStatus;
        } else {
            cond = QString("(%1) AND (%2)").arg(cond, condStatus);
        }
    }
    logd("cond with modelStatus='%s'", STR2CHA(cond));
    traceout;
}
QString DbSqliteTbl::getSearchQueryString(const QString& cond)
{
    tracein;
    return getSearchQueryStringWithTag(cond, nullptr);
}

QString DbSqliteTbl::getSearchQueryStringWithTag(const QString &cond, const QString &tag)
{
    tracein;
    QString queryString = QString("SELECT * FROM %2")
                              .arg(name());
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    queryString += " ORDER BY name ASC ";
    logd("queryString: %s", queryString.toStdString().c_str());
    traceout;
    return queryString;
}

QString DbSqliteTbl::getFilterQueryString(int fieldId, const QString &cond)
{
    tracein;
    UNUSED(fieldId);
    QString queryString = getSearchQueryString(cond);
    traceout;
    return queryString;
}

QString DbSqliteTbl::getFilterQueryString(const QList<FilterKeyworkItem *> &filters,
                                          const QString &cond)
{
    tracein;
    UNUSED(filters);
    QString queryString = getSearchQueryString(cond);
    traceout;
    return queryString;
}

ErrCode DbSqliteTbl::search(const QString& keyword, const QHash<QString, int>& inFields,
                        const DbModelBuilder& builder,
                        QList<DbModel*>* outList,
                        bool isExact,
                        qint64 dbStatus,
                        int from,
                        int noItems,
                        int* total)
{
    tracein;
//    DB->openDb();
    // TODO: implement it
    // TODO: exact and not exact match???

//    QSqlQuery qry(SQLITE->currentDb());
    QSqlQuery qry(SQLITE->currentDb());
    ErrCode err = ErrNone;
    int cnt = 0;
    logi("Search keyword '%s', dbStatus=0x%x", STR2CHA(keyword), dbStatus);
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
    appendDbStatusCond(cond, dbStatus);

//    QString queryString = QString("SELECT * FROM %1 WHERE %2")
//                              .arg(name(), cond);
    logd("Query cond '%s'", cond.toStdString().c_str());
    QString queryString = getSearchQueryString(cond);

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    logd("Bind value '%s'", STR2CHA(keyword.trimmed().toLower()));

    // TODO: check sql injection issue
    qry.bindValue( ":keyword", QString("%%1%").arg(keyword.trimmed().toLower()) );
    qry.bindValue( ":keywordexact", QString("%1").arg(keyword.trimmed().toLower()) );
    qry.bindValue( ":value", QString("%1").arg(keyword.trimmed().toLower()) );
    try {
        cnt = runQuery(qry, builder, outList);
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
    traceret(err);
    return err;
}

ErrCode DbSqliteTbl::search(const QHash<QString, FieldValue> &searchCond,
                        bool isAndCond, const QString& condTag,
                        const DbModelBuilder &builder,
                        QList<DbModel *> *outList, bool isExact,
                        qint64 dbStatus,
                        int from,
                        int noItems,
                        int* total)
{
    tracein;
    //    DB->openDb();
    // TODO: implement it
    // TODO: exact and not exact match???

    QSqlQuery qry(SQLITE->currentDb());
    qint32 cnt = 0;
    QString cond;
    ErrCode err = ErrNone;
    bool modelStatusExist = false;
    foreach (QString field, searchCond.keys()) {
        if (field == KFieldModelStatus) {
            modelStatusExist = true;
            continue;
        }
        if (!cond.isEmpty()) {
            if (isAndCond)
                cond += " AND ";
            else
                cond += " OR ";
        }
        logd("field %s", field.toStdString().c_str());
        if (searchCond.value(field).dataType == TEXT){
            if (isExact) {
                cond += QString("lower(%1) = :keyword_%1").arg(field);
            } else {
                cond += QString("lower(%1) like :keyword_%1").arg(field);
            }
        } else {
            // TODO: assump on INT32/64
            cond += QString("%1 = :keyword_%1").arg(field);
        }
    }
    appendDbStatusCond(cond, dbStatus);
    if (modelStatusExist) {
        appendModelStatusCond(cond, searchCond.value(KFieldModelStatus).valueInt());
    }
    //    QString queryString = QString("SELECT * FROM %1 WHERE %2")
    //                              .arg(name(), cond);
    logd("Query cond '%s'", cond.toStdString().c_str());
    QString queryString = getSearchQueryStringWithTag(cond, condTag);

    qry.prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    // TODO: check sql injection issue
    foreach (QString field, searchCond.keys()) {
        if (field == KFieldModelStatus) {
            continue;
        }
        if (searchCond.value(field).dataType == TEXT){
            logd("bind text value, field='%s', value='%s'", STR2CHA(field), STR2CHA(searchCond.value(field).valueString()));
            qry.bindValue( QString(":keyword_%1").arg(field),
                          QString("%1").arg(searchCond.value(field).valueString().trimmed().toLower()) );
        } else {
            logd("bind int value, field='%s', value='%ld'", STR2CHA(field), searchCond.value(field).valueInt());
            qry.bindValue( QString(":keyword_%1").arg(field),
                          QString("%1").arg(searchCond.value(field).valueInt()) );
        }
    }

    try {
        cnt = runQuery(qry, builder, outList);
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
    traceret(err);
    return err;
}

DbSqliteTableBuilder* DbSqliteTbl::getTableBuilder()
{
    tracein;
    // TODO; support multi language
    // Common table, with name and history field, together with default one is id, uid, status, history
    DbSqliteTableBuilder* builder = DbSqliteTableBuilder::build(name());
    // common field
    builder->addField(KFieldDbStatus, INT32); // this is important field, to judge if record is delete or not
                                              // not allow derived class to ignore it by overriding addTableField
                                              // TODO: make common function to put all mandatory record???
    // specific field
    addTableField(builder);

    return builder;
}

void DbSqliteTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    // TODO: consider to make Uid as common field???
    // i.e. uid can be hash of name, hash of 2 id in different table, etc.
    builder->addField(KFieldUid, TEXT);
    builder->addField(KFieldNameId, TEXT);
    builder->addField(KFieldName, TEXT);
    builder->addField(KFieldDbHistory, TEXT); // history of this field in db
    builder->addField(KFieldDbCreateTime, INT64);
    builder->addField(KFieldLastDbUpdateItme, INT64);
    builder->addField(KFieldRemark, TEXT);
}

ErrCode DbSqliteTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    if (!item->uid().isEmpty())
        builder->addValue(KFieldUid, item->uid());
    if (!item->nameId().isEmpty())
        builder->addValue(KFieldNameId, item->nameId());
    if (!item->name().isEmpty())
        builder->addValue(KFieldName, item->name());
    if (!item->dbHistory().isEmpty())
        builder->addValue(KFieldDbHistory, item->dbHistory());
    if (!item->remark().isEmpty())
        builder->addValue(KFieldRemark, item->remark());
//    builder->addValue(KFieldDbStatus, item->dbStatus());
    // TODO: timezone issue?
    // TODO: time of computer suddenly not correct/change???,
    // should store last open time than compare???
    builder->addValue(KFieldDbCreateTime, DatetimeUtils::currentTimeMs());
    builder->addValue(KFieldLastDbUpdateItme, DatetimeUtils::currentTimeMs());
    builder->addValue(KFieldDbStatus, DB_RECORD_NOT_READY); // NOT ready, until we update uid
                                                            // TODO: Fix me if this is neccessary or correct
    traceout;
    return ErrNone;
}

ErrCode DbSqliteTbl::appendDeleteCondition(DbSqliteDeleteBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    builder->addCond(KFieldUid, item->uid());
    traceret(err);
    return err;
}


DbSqlite *DbSqliteTbl::db() const
{
    return mDb;
}
