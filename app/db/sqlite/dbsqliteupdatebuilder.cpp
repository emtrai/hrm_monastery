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
 * Filename: dbsqliteupdatebuilder.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/5/2022
 * Brief:
 */
#include "dbsqliteupdatebuilder.h"
#include "logger.h"
#include "errcode.h"
#include "dbctl.h"
#include "dbsqlite.h"

DbSqliteUpdateBuilder *DbSqliteUpdateBuilder::build(const QString &tblName)
{
    return new DbSqliteUpdateBuilder(tblName);
}

DbSqliteUpdateBuilder *DbSqliteUpdateBuilder::addValue(const QString &field, const QString &value, int dataType)
{
    traced;
    logd("add field %s, value %s", field.toStdString().c_str(), value.toStdString().c_str());
    if (!mValue.contains(field)) {
        mValue.insert(field, FieldValue(value, dataType));
    } else {
        logi("Field %s already exist", field.toStdString().c_str());
    }
    tracede;
    return this;
}

DbSqliteUpdateBuilder *DbSqliteUpdateBuilder::addValue(const QString &field, qint64 value)
{
    traced;
    logd("add field int %s, value %d", field.toStdString().c_str(), value);
    if (!mValue.contains(field)) {
        mValue.insert(field, FieldValue(QString("%1").arg(value), INT64));
    } else {
        logi("Field %s already exist", field.toStdString().c_str());
    }
    tracede;
    return this;
}

DbSqliteUpdateBuilder *DbSqliteUpdateBuilder::addCond(const QString &field, const QString &value, int dataType)
{
    traced;
    logd("add cond %s, value %s", field.toStdString().c_str(), value.toStdString().c_str());
    if (!mCondition.contains(field)) {
        mCondition.insert(field, FieldValue(value, dataType));
    } else {
        logi("Field %s already exist", field.toStdString().c_str());
    }
    return this;
}

QSqlQuery *DbSqliteUpdateBuilder::buildSqlQuery(const QString *cond)
{
    traced;
    QString conds;
    QString values;
    traced;
    foreach( QString field, mValue.keys() )
    {
        if (!values.isEmpty()) {
            values += ",";
        }
        values += QString("%1 = :val_%1").arg(field);
    }

    foreach( QString field, mCondition.keys() )
    {
        if (!conds.isEmpty()) {
            conds += " AND ";
        }
        conds += QString("%1 = :cond_%1").arg(field);
    }


    (void)cond;
    // TODO: condition?
    // TODO: type int vs string? if string, should put into ''?
    QString queryString = QStringLiteral(
                              "UPDATE %1 SET %2 WHERE %3")
                              .arg(mName, values, conds);
//    DB->openDb();
    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    foreach( QString field, mValue.keys() )
    {
        QString id = ":val_" + field;
        logd("Bind update command field '%s', value '%s'",
             id.toStdString().c_str(), mValue.value(field).value.toStdString().c_str());
        qry->bindValue(id, mValue.value(field).value); // TODO: check data type

    }
    foreach( QString field, mCondition.keys() )
    {
        QString id = ":cond_" + field;
        logd("Bind update cond field '%s', value '%s'",
             id.toStdString().c_str(), mCondition.value(field).value.toStdString().c_str());
        qry->bindValue(id, mCondition.value(field).value); // TODO: check datatype

    }
    return qry;
}

DbSqliteUpdateBuilder::DbSqliteUpdateBuilder(const QString& name):
    mName(name)
{
    traced;
}
