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
 * Filename: dbsqlitepersontbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/21/2022
 * Brief:
 */
#include "dbsqlitepersontbl.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqlitedefs.h"
#include "person.h"
#include "dbsqlite.h"

const qint32 DbSqlitePersonTbl::KVersionCode = VERSION_CODE(0,0,1);


DbSqlitePersonTbl::DbSqlitePersonTbl(DbSqlite* db):
    DbSqliteTbl(db, KTablePerson, KTablePerson, KVersionCode)
{}

ErrCode_t DbSqlitePersonTbl::addPerson(const Person *person)
{
    traced;
    ErrCode_t err = ErrNone;
    QString sql = DbSqliteInsertBuilder::build(name())
                      ->addValue(KFieldLastName, person->lastName())
                      ->addValue(KFieldFirstName, person->firstName())
                      ->addValue(KFieldBirthDay, person->birthday())

                      ->buildSqlStatement();
    logi("insert sql statement %s", sql.toStdString().c_str());
    err = db()->execQuery(sql);

    return err;
}


QString DbSqlitePersonTbl::getSqlCmdCreateTable() {
    traced;
    QString sql = DbSqliteTableBuilder::build(name())
                      ->addField(KFieldLastName, TEXT)
                      ->addField(KFieldFirstName, TEXT)
                      ->addField(KFieldPersonCode, TEXT)
                      ->addField(KFieldBirthDay, INT64)

                      ->buildSqlStatement();
    logi("Create statement %s", sql.toStdString().c_str());

    return sql;
}
