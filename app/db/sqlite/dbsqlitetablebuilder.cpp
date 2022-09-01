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
 * Filename: dbsqlitetablebuilder.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#include "dbsqlitetablebuilder.h"
#include "logger.h"
#include "errcode.h"
#include <QStringLiteral>


class TableFieldItem {
public:
    TableFieldItem(const QString& name, TableFieldDatatype_t type);
    const QString &name() const;

    TableFieldDatatype_t datatype() const;

private:
    QString mName;
    TableFieldDatatype_t mDatatype;
};

TableFieldItem::TableFieldItem(const QString& name, TableFieldDatatype_t type){
    mName = name;
    mDatatype = type;
}

const QString &TableFieldItem::name() const
{
    return mName;
}

TableFieldDatatype_t TableFieldItem::datatype() const
{
    return mDatatype;
}
DbSqliteTableBuilder::DbSqliteTableBuilder(const QString& name)
{
    mName = name;
}

DbSqliteTableBuilder* DbSqliteTableBuilder::build(const QString& tblName){
    traced;
    return new DbSqliteTableBuilder(tblName);
}

DbSqliteTableBuilder* DbSqliteTableBuilder::addField(const QString& name, TableFieldDatatype_t dataType){
    traced;
    logd("Add field %s", name.toStdString().c_str());
    mFields.append(new TableFieldItem(name, dataType));
    return this;
}

QString DbSqliteTableBuilder::buildSqlStatement()
{
    QString fields;

    foreach( TableFieldItem* item, mFields )
    {
        fields += ", " + item->name() + " " + getDateTypeString(item->datatype());
    }


    /**
     * Table
     * - id (seq)
     * - uid (unique)
     * - status: delete, active, etc.
     */
    return QStringLiteral(
                "CREATE TABLE IF NOT EXISTS %1 "
                "(%2 INTEGER PRIMARY KEY AUTOINCREMENT"
                "%3)")
        .arg(mName, KFieldId, fields);
}
