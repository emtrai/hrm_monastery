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
 * Filename: dbsqlitetablebuilder.h
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#ifndef DBSQLITETABLEBUILDER_H
#define DBSQLITETABLEBUILDER_H

#include <QString>
#include <QList>

#include "dbsqlitedefs.h"

class TableFieldItem;


class DbSqliteTableBuilder
{
public:
    static DbSqliteTableBuilder* build(const QString& tblName);
    DbSqliteTableBuilder* addField(const QString& name, TableFieldDatatype_t dataType);
    QString buildSqlStatement();
private:

    DbSqliteTableBuilder(const QString& name);
private:
    QString mName;
    QList<TableFieldItem*> mFields;
};

#endif // DBSQLITETABLEBUILDER_H
