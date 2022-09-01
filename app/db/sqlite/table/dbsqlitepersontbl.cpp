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

#include "defs.h"
#include "logger.h"

#include <QSqlQuery>

const qint32 DbSqlitePersonTbl::KVersionCode = VERSION_CODE(0,0,1);


DbSqlitePersonTbl::DbSqlitePersonTbl(DbSqlite* db):
    DbSqliteTbl(db, KTablePerson, KTablePerson, KVersionCode)
{}

void DbSqlitePersonTbl::addTableField(DbSqliteTableBuilder *builder)
{

    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldImgPath, TEXT);
    builder->addField(KFieldLastName, TEXT);
    builder->addField(KFieldFirstName, TEXT);
    builder->addField(KFieldPersonCode, TEXT);
    builder->addField(KFieldBirthDay, INT64);
    builder->addField(KFieldBirthPlace, TEXT);
    builder->addField(KFieldIDCard, TEXT);
    builder->addField(KFieldIDCardIssueDate, INT64);
    // just caching, official check mapping table holly vs person
    builder->addField(KFieldIHollyName, TEXT);
    builder->addField(KFieldINationalityName, TEXT);
    builder->addField(KFieldIEthnicName, TEXT);
    builder->addField(KFieldIDCardIssuePlace, TEXT);
    builder->addField(KFieldIEduName, TEXT);
}

void DbSqlitePersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{

    DbSqliteTbl::insertTableField(builder, item);
    Person* per = (Person*) item;
    builder->addValue(KFieldImgPath, per->imgPath());
    builder->addValue(KFieldLastName, per->lastName());
    builder->addValue(KFieldFirstName, per->firstName());
    builder->addValue(KFieldPersonCode, per->personCode());
    builder->addValue(KFieldBirthDay, per->birthday());
    builder->addValue(KFieldBirthPlace, per->birthPlace());
    builder->addValue(KFieldIDCard, per->idCard());
    builder->addValue(KFieldIHollyName, per->hollyName());
    builder->addValue(KFieldINationalityName, per->nationalityName());
    builder->addValue(KFieldIEthnicName, per->ethnicName());
    builder->addValue(KFieldIDCardIssueDate, per->idCardIssueDate());
    builder->addValue(KFieldIDCardIssuePlace, per->idCardIssuePlace());
    builder->addValue(KFieldIEduName, per->eduName());
}

void DbSqlitePersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Person* cmm = (Person*) item;
    cmm->setImgPath(qry.value(KFieldImgPath).toString());
    cmm->setLastName(qry.value(KFieldLastName).toString());
    cmm->setFirstName(qry.value(KFieldFirstName).toString());
    cmm->setPersonCode(qry.value(KFieldPersonCode).toString());
    cmm->setBirthday(qry.value(KFieldBirthDay).toInt());
    cmm->setBirthPlace(qry.value(KFieldBirthPlace).toString());
    cmm->setIdCard(qry.value(KFieldIDCard).toString());
    cmm->setHollyName(qry.value(KFieldIHollyName).toString());
    cmm->setNationalityName(qry.value(KFieldINationalityName).toString());
    cmm->setEthnicName(qry.value(KFieldIEthnicName).toString());
    cmm->setIdCardIssueDate(qry.value(KFieldIDCardIssueDate).toInt());
    cmm->setIdCardIssuePlace(qry.value(KFieldIDCardIssuePlace).toString());
    cmm->setEduName(qry.value(KFieldIEduName).toString());

}

