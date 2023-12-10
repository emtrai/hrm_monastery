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
 * Filename: dbsqlitedeletebuilder.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/22/2022
 * Brief:
 */
#include "dbsqlitedeletebuilder.h"
#include "logger.h"
#include "dbsqlite.h"

DbSqliteDeleteBuilder *DbSqliteDeleteBuilder::build(const QString &tblName)
{
    return new DbSqliteDeleteBuilder(tblName);
}

DbSqliteDeleteBuilder *DbSqliteDeleteBuilder::addCond(const QString &field, const QString &value, int dataType)
{
    tracein;
    dbgtrace;
    logd("add field %s, value %s", field.toStdString().c_str(), value.toStdString().c_str());
    if (!mCondition.contains(field)) {
        mCondition.insert(field, FieldValue(value, dataType));
    } else {
        logi("Field %s already exist", field.toStdString().c_str());
    }
    traceout;
    return this;
}

QSqlQuery *DbSqliteDeleteBuilder::buildSqlQuery(const QString *cond)
{
    tracein;
    QString conds;
    QString values;
    dbgtrace;
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
                              "DELETE FROM %1 WHERE %2")
                              .arg(mName, conds);
    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());

    foreach( QString field, mCondition.keys() )
    {
        QString id = ":cond_" + field;
        logd("Bind cond field '%s', value '%s'",
             id.toStdString().c_str(), mCondition.value(field).value.toStdString().c_str());
        qry->bindValue(id, mCondition.value(field).value); // TODO: check datat type????

    }
    traceout;
    return qry;
}

DbSqliteDeleteBuilder::DbSqliteDeleteBuilder(const QString& name):
    mName(name)
{
    traced;
}
