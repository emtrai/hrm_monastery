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
 * Filename: dbsqliteinsertbuilder.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/22/2022
 * Brief:
 */
#include "dbsqliteinsertbuilder.h"
#include "logger.h"
#include "errcode.h"
#include "dbctl.h"
#include "dbsqlite.h"
class TableInsertItem {
public:
    TableInsertItem(const QString& name);
    TableInsertItem(const QString& name, const QString& value);
    TableInsertItem(const QString& name, qint32 value);

    TableFieldDatatype_t datatype() const;

    const QString &stringValue() const;
    const QString &name() const;

    qint32 int32Value() const;

    qint64 int64Value() const;

private:
    QString mName;
    TableFieldDatatype_t mDatatype;
    QString mStringValue;
    qint32 mInt32Value;
    qint64 mInt64Value;
};

TableInsertItem::TableInsertItem(const QString &name)
    : mName(name)
{}

TableInsertItem::TableInsertItem(const QString &name,  const QString &value)
    : TableInsertItem(name)
{
    mStringValue = value;
    mDatatype = TableFieldDatatype_t::TEXT;
}

TableInsertItem::TableInsertItem(const QString &name, qint32 value)
    : TableInsertItem(name)
{
    mInt32Value = value;
    mDatatype = TableFieldDatatype_t::INT32;
}

TableFieldDatatype_t TableInsertItem::datatype() const
{
    return mDatatype;
}

const QString &TableInsertItem::stringValue() const
{
    return mStringValue;
}

const QString &TableInsertItem::name() const
{
    return mName;
}

qint32 TableInsertItem::int32Value() const
{
    return mInt32Value;
}

qint64 TableInsertItem::int64Value() const
{
    return mInt64Value;
}

//********************************************************************
//********************************************************************
//********************************************************************

DbSqliteInsertBuilder *DbSqliteInsertBuilder::build(const QString &tblName)
{
    return new DbSqliteInsertBuilder(tblName);
}

DbSqliteInsertBuilder *DbSqliteInsertBuilder::addValue(
    const QString &name, const QString &value)
{
    traced;
    logd("tble %s", mName.toStdString().c_str());
    if (!value.isEmpty()) {
        logd("addValue %s: %s", name.toStdString().c_str(), value.toStdString().c_str());
        mFields.append(new TableInsertItem(name, value));
    } else {
        logd("Value is empty, do nothing");
    }


    return this;
}


DbSqliteInsertBuilder *DbSqliteInsertBuilder::addValue(
    const QString &name, qint32 value)
{
    traced;
    logd("addValue %s: %d", name.toStdString().c_str(), value);
    mFields.append(new TableInsertItem(name, value));

    return this;
}

QString DbSqliteInsertBuilder::buildSqlStatement(const QString* cond)
{
    QString fields;
    QString values;
    traced;
    foreach( TableInsertItem* item, mFields )
    {
        fields += item->name() + ",";
        switch (item->datatype()){
        case TEXT:
            // TODO: consider special values
            // SQL injection issue
            values += "\'" + item->stringValue() + "\'";
            break;
        case INT32:
            values += QString::number(item->int32Value());
            break;
        case INT64:
            values += QString::number(item->int64Value());
            break;
        default:
            // TODO handler error
            break;
        }
        values += ",";
    }
    if (!fields.isEmpty()){ // remove last ","
        fields.remove(fields.length() - 1, 1);
    }
    if (!values.isEmpty()){ // remove last ","
        values.remove(values.length() - 1, 1);
    }

    (void)cond;
    // TODO: condition?
    return QStringLiteral(
               "INSERT INTO %1(%2) VALUES(%3)")
        .arg(mName, fields, values);

    // TODO: USE BIND????
    // TODO: generate uid (random, unique)
//    QSqlQuery qry;
//    qry.prepare( "INSERT INTO tags (id, tag) VALUES (:id, :tag)" );
//    qry.bindValue( ":id", id );
//    qry.bindValue( ":tag", tag );

}

QSqlQuery* DbSqliteInsertBuilder::buildSqlQuery(const QString *cond)
{
    QString fields;
    QString values;
    traced;
    foreach( TableInsertItem* item, mFields )
    {
        fields += item->name() + ",";
        values += ":" + item->name() + ",";
    }
    if (!fields.isEmpty()){ // remove last ","
        fields.remove(fields.length() - 1, 1);
    }
    if (!values.isEmpty()){ // remove last ","
        values.remove(values.length() - 1, 1);
    }

    (void)cond;
    // TODO: condition?
    QString queryString = QStringLiteral(
               "INSERT INTO %1(%2) VALUES(%3)")
        .arg(mName, fields, values);
//    DB->openDb();
    QSqlQuery* qry = new QSqlQuery(SQLITE->currentDb());
    qry->prepare(queryString);
    logd("Query String '%s'", queryString.toStdString().c_str());
    foreach( TableInsertItem* item, mFields )
    {
        logd("Bind insert command %s, type %d",
             item->name().toStdString().c_str(), item->datatype());
        switch (item->datatype()){
        case TEXT:
            logd("Bind value %s", item->stringValue().toStdString().c_str());
            qry->bindValue(":" + item->name(), item->stringValue());
            break;
        case INT32:
            logd("Bind value %d", item->int32Value());
            qry->bindValue(":" + item->name(), item->int32Value());
            break;
        case INT64:
            logd("Bind value %d", item->int32Value());
            qry->bindValue(":" + item->name(), item->int64Value());
            break;
        default:
            // TODO handler error
            loge("UNSUPORT data type");
            break;
        }

    }
    return qry;
}


DbSqliteInsertBuilder::DbSqliteInsertBuilder(const QString& name): mName(name)
{}
