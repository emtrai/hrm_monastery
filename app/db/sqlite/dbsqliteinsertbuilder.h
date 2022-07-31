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
 * Filename: dbsqliteinsertbuilder.h
 * Author: Anh, Ngo Huy
 * Created date:7/22/2022
 * Brief:
 */
#ifndef DBSQLITEINSERTBUILDER_H
#define DBSQLITEINSERTBUILDER_H

#include <QString>
#include <QObject>
#include <QList>

#include "dbsqlitedefs.h"

class TableInsertItem;

class DbSqliteInsertBuilder
{
public:
    static DbSqliteInsertBuilder* build(const QString& tblName);
    DbSqliteInsertBuilder* addValue(const QString& name, const QString& value);
    DbSqliteInsertBuilder* addValue(const QString& name, qint32 value);
    QString buildSqlStatement(const QString* cond = nullptr);
private:

    DbSqliteInsertBuilder(const QString& name);

private:
    QString mName;
    QList<TableInsertItem* > mFields;

};

#endif // DBSQLITEINSERTBUILDER_H
