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
 * Filename: dbsqliteupdatebuilder.h
 * Author: Anh, Ngo Huy
 * Created date:10/5/2022
 * Brief:
 */
#ifndef DBSQLITEUPDATEBUILDER_H
#define DBSQLITEUPDATEBUILDER_H


#include <QString>
#include <QObject>
#include <QList>
#include <QSqlQuery>
#include "dbsqlitedefs.h"

class DbSqliteUpdateBuilder
{
public:
    static DbSqliteUpdateBuilder* build(const QString& tblName);
    DbSqliteUpdateBuilder* addValue(const QString& field, const QString& value);
    DbSqliteUpdateBuilder* addValue(const QString& field, qint64 value);
    DbSqliteUpdateBuilder* addCond(const QString& field, const QString& value);
    QSqlQuery* buildSqlQuery(const QString* cond = nullptr);
private:

    DbSqliteUpdateBuilder(const QString& name);

private:
    QString mName;
    QHash<QString, QString> mValue;
    QHash<QString, QString> mCondition;
};

#endif // DBSQLITEUPDATEBUILDER_H
