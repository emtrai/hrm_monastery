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
    builder->addField(KFieldImgId, TEXT);

    builder->addField(KFieldLastName, TEXT);
    builder->addField(KFieldFirstName, TEXT);

    builder->addField(KFieldPersonCode, TEXT);

    builder->addField(KFieldBirthDay, INT64);
    builder->addField(KFieldBirthPlace, TEXT);

    builder->addField(KFieldIDCard, TEXT);
    builder->addField(KFieldIDCardIssueDate, INT64);
    builder->addField(KFieldIDCardIssuePlace, TEXT);

    // just caching, official check mapping table holly vs person
    builder->addField(KFieldHollyName, TEXT);
    builder->addField(KFieldFeastDay, TEXT);

//    builder->addField(KFieldEthnicName, TEXT);
//    builder->addField(KFieldIEduName, TEXT);


    builder->addField(KFieldNationalityUid, TEXT);
    builder->addField(KFieldEthnicUid, TEXT);
    builder->addField(KFieldIEduUid, TEXT);
    builder->addField(KFieldCourseUid, TEXT);

    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldProvinceUid, TEXT);
    builder->addField(KFieldAddr, TEXT);

    builder->addField(KFieldCommunityUid, TEXT);
    builder->addField(KFieldAreaUid, TEXT);
    builder->addField(KFieldDepartmentUid, TEXT);

    builder->addField(KFieldChurchAddr, TEXT);

    builder->addField(KFieldEmail, TEXT);
    builder->addField(KFieldTel, TEXT);
    builder->addField(KFieldContact, TEXT);

    builder->addField(KFieldDadName, TEXT);
    builder->addField(KFieldDadAddr, TEXT);
    builder->addField(KFieldDadBirthDay, INT64);


    builder->addField(KFieldMomName, TEXT);
    builder->addField(KFieldMomAddr, TEXT);
    builder->addField(KFieldMomBirthDay, INT64);


    builder->addField(KFieldFamilyHistory, TEXT);
    builder->addField(KFieldFamilyContact, TEXT);

    builder->addField(KFieldChristenDate, INT64);
    builder->addField(KFieldChristenPlace, TEXT);

    builder->addField(KFieldEucharistDate, INT64);
    builder->addField(KFieldEucharistPlace, TEXT);

    builder->addField(KFieldHollyDate, INT64);
    builder->addField(KFieldHollyPlace, TEXT);

    builder->addField(KFieldJoinDate, INT64);
    builder->addField(KFieldJoinPICUid, TEXT);

    builder->addField(KFieldPreTrainDate, INT64);
    builder->addField(KFieldPreTrainPICUid, TEXT);

    builder->addField(KFieldTrainDate, INT64);
    builder->addField(KFieldTrainPICUid, TEXT);

    builder->addField(KFieldVowsDate, INT64);
    builder->addField(KFieldVowsCEOUid, TEXT);


    builder->addField(KFieldEternalVowsDate, INT64);
    builder->addField(KFieldEternalVowsPICUid, TEXT);
    builder->addField(KFieldEternalVowsCEOUid, TEXT);


    builder->addField(KFieldBankDate, INT64);
    builder->addField(KFieldBankPlace, TEXT);

    builder->addField(KFieldGoldenDate, INT64);
    builder->addField(KFieldGoldenPlace, TEXT);

    builder->addField(KFieldEternalDate, INT64);
    builder->addField(KFieldEternalPlace, TEXT);

    builder->addField(KFieldStatusUid, TEXT);

    builder->addField(KFieldRetireDate, INT64);
    builder->addField(KFieldRetirePlace, TEXT);

    builder->addField(KFieldDeadDate, INT64);
    builder->addField(KFieldDeadPlace, TEXT);
}

ErrCode DbSqlitePersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{

    DbSqliteTbl::insertTableField(builder, item);
    Person* per = (Person*) item;
    builder->addValue(KFieldImgId, per->imgId());
    builder->addValue(KFieldLastName, per->lastName());
    builder->addValue(KFieldFirstName, per->firstName());
    builder->addValue(KFieldPersonCode, per->personCode());

    builder->addValue(KFieldBirthDay, per->birthday());
    builder->addValue(KFieldBirthPlace, per->birthPlace());

    builder->addValue(KFieldIDCard, per->idCard());
    builder->addValue(KFieldIDCardIssueDate, per->idCardIssueDate());
    builder->addValue(KFieldIDCardIssuePlace, per->idCardIssuePlace());

    builder->addValue(KFieldHollyName, per->hollyName());
    builder->addValue(KFieldFeastDay, per->feastDay());

    builder->addValue(KFieldNationalityUid, per->nationalityUid());
    builder->addValue(KFieldEthnicUid, per->ethnicUid());
    builder->addValue(KFieldIEduUid, per->eduUid());
    builder->addValue(KFieldCourseUid, per->courseUid());

    builder->addValue(KFieldCountryUid, per->countryUid());
    builder->addValue(KFieldProvinceUid, per->provinceUid());
    builder->addValue(KFieldAddr, per->addr());
    builder->addValue(KFieldChurchAddr, per->churchAddr());
    if (per->email().length() > 0)
        builder->addValue(KFieldEmail, per->email().join(';'));
    if (per->tel().length() > 0)
        builder->addValue(KFieldTel, per->tel().join(';'));

    builder->addValue(KFieldContact, per->otherContact());

    builder->addValue(KFieldCommunityUid, per->communityUid());
    builder->addValue(KFieldAreaUid, per->areaUid());
    builder->addValue(KFieldDepartmentUid, per->departUid());

    builder->addValue(KFieldDadName, per->dadName());
    builder->addValue(KFieldDadAddr, per->dadAddr());
    builder->addValue(KFieldDadBirthDay, per->dadBirthday());

    builder->addValue(KFieldMomName, per->momName());
    builder->addValue(KFieldMomAddr, per->momAddr());
    builder->addValue(KFieldMomBirthDay, per->momBirthday());

    builder->addValue(KFieldFamilyHistory, per->familyHistory());
    builder->addValue(KFieldFamilyContact, per->familyContact());

    builder->addValue(KFieldChristenDate, per->christenDate());
    builder->addValue(KFieldChristenPlace, per->christenPlace());


    builder->addValue(KFieldEucharistDate, per->eucharistDate());
    builder->addValue(KFieldEucharistPlace, per->eucharistPlace());

    builder->addValue(KFieldHollyDate, per->hollyDate());
    builder->addValue(KFieldHollyPlace, per->hollyPlace());

    builder->addValue(KFieldJoinDate, per->joinDate());
    builder->addValue(KFieldJoinPICUid, per->joinPICUid());

    builder->addValue(KFieldPreTrainDate, per->preTrainJoinDate());
    builder->addValue(KFieldPreTrainPICUid, per->preTrainPICUid());

    builder->addValue(KFieldTrainDate, per->trainJoinDate());
    builder->addValue(KFieldTrainPICUid, per->trainPICUid());

    builder->addValue(KFieldVowsDate, per->vowsDate());
    builder->addValue(KFieldVowsCEOUid, per->vowsCEOUid());

    builder->addValue(KFieldEternalVowsDate, per->eternalVowsDate());
    builder->addValue(KFieldEternalVowsPICUid, per->eternalVowsPICUid());
    builder->addValue(KFieldEternalVowsCEOUid, per->eternalVowsCEOUid());

    builder->addValue(KFieldBankDate, per->bankDate());
    builder->addValue(KFieldBankPlace, per->bankPlace());

    builder->addValue(KFieldGoldenDate, per->goldenDate());
    builder->addValue(KFieldGoldenPlace, per->goldenPlace());

    builder->addValue(KFieldEternalDate, per->eternalDate());
    builder->addValue(KFieldEternalPlace, per->eternalPlace());


    builder->addValue(KFieldStatusUid, per->statusUid());

    builder->addValue(KFieldRetireDate, per->retireDate());
    builder->addValue(KFieldRetirePlace, per->retirePlace());

    builder->addValue(KFieldDeadDate, per->deadDate());
    builder->addValue(KFieldDeadPlace, per->deadPlace());

    // TODO: add field relate to list, like holly list, community, etc.
    return ErrNone;
}

void DbSqlitePersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Person* cmm = (Person*) item;
    cmm->setImgId(qry.value(KFieldImgId).toString());

    cmm->setLastName(qry.value(KFieldLastName).toString());
    cmm->setFirstName(qry.value(KFieldFirstName).toString());
    cmm->setPersonCode(qry.value(KFieldPersonCode).toString());

    cmm->setBirthday(qry.value(KFieldBirthDay).toInt());
    cmm->setBirthPlace(qry.value(KFieldBirthPlace).toString());

    cmm->setIdCard(qry.value(KFieldIDCard).toString());
    cmm->setIdCardIssueDate(qry.value(KFieldIDCardIssueDate).toInt());
    cmm->setIdCardIssuePlace(qry.value(KFieldIDCardIssuePlace).toString());

    cmm->setHollyName(qry.value(KFieldHollyName).toString());
    cmm->setFeastDay(qry.value(KFieldFeastDay).toString());

    cmm->setNationalityUid(qry.value(KFieldNationalityUid).toString());
    cmm->setEthnicUid(qry.value(KFieldEthnicUid).toString());

    cmm->setEduUid(qry.value(KFieldIEduUid).toString());

    cmm->setCourseUid(qry.value(KFieldCourseUid).toString());

    cmm->setCountryUid(qry.value(KFieldCountryUid).toString());
    cmm->setProvinceUid(qry.value(KFieldProvinceUid).toString());
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setChurchAddr(qry.value(KFieldChurchAddr).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setOtherContact(qry.value(KFieldContact).toString());

    cmm->setCommunityUid(qry.value(KFieldCommunityUid).toString());
    cmm->setDepartUid(qry.value(KFieldDepartmentUid).toString());
    cmm->setAreaUid(qry.value(KFieldAreaUid).toString());
    // TODO: search and set name

    cmm->setDadName(qry.value(KFieldDadName).toString());
    cmm->setDadAddr(qry.value(KFieldDadAddr).toString());
    cmm->setDadBirthday(qry.value(KFieldDadBirthDay).toInt());

    cmm->setMomName(qry.value(KFieldMomName).toString());
    cmm->setMomAddr(qry.value(KFieldMomAddr).toString());
    cmm->setMomBirthday(qry.value(KFieldMomBirthDay).toInt());

    cmm->setFamilyHistory(qry.value(KFieldFamilyHistory).toString());
    cmm->setFamilyContact(qry.value(KFieldFamilyContact).toString());

    cmm->setChristenDate(qry.value(KFieldChristenDate).toInt());
    cmm->setChristenPlace(qry.value(KFieldChristenPlace).toString());

    cmm->setEucharistDate(qry.value(KFieldEucharistDate).toInt());
    cmm->setEucharistPlace(qry.value(KFieldEucharistPlace).toString());

    cmm->setHollyDate(qry.value(KFieldHollyDate).toInt());
    cmm->setHollyPlace(qry.value(KFieldHollyPlace).toString());

    cmm->setJoinDate(qry.value(KFieldJoinDate).toInt());
    cmm->setJoinPICUid(qry.value(KFieldJoinPICUid).toString());

    cmm->setPreTrainJoinDate(qry.value(KFieldPreTrainDate).toInt());
    cmm->setPreTrainPICUid(qry.value(KFieldPreTrainPICUid).toString());


    cmm->setTrainJoinDate(qry.value(KFieldTrainDate).toInt());
    cmm->setTrainPICUid(qry.value(KFieldTrainPICUid).toString());

    cmm->setVowsDate(qry.value(KFieldVowsDate).toInt());
    cmm->setVowsCEOUid(qry.value(KFieldVowsCEOUid).toString());


    cmm->setEternalVowsDate(qry.value(KFieldEternalVowsDate).toInt());
    cmm->setEternalVowsPICUid(qry.value(KFieldEternalVowsPICUid).toString());
    cmm->setEternalVowsCEOUid(qry.value(KFieldEternalVowsCEOUid).toString());


    cmm->setBankDate(qry.value(KFieldBankDate).toInt());
    cmm->setBankPlace(qry.value(KFieldBankPlace).toString());

    cmm->setGoldenDate(qry.value(KFieldGoldenDate).toInt());
    cmm->setGoldenPlace(qry.value(KFieldGoldenPlace).toString());

    cmm->setEternalDate(qry.value(KFieldEternalDate).toInt());
    cmm->setEternalPlace(qry.value(KFieldEternalPlace).toString());

    cmm->setStatusUid(qry.value(KFieldStatusUid).toString());

    cmm->setRetireDate(qry.value(KFieldRetireDate).toInt());
    cmm->setRetirePlace(qry.value(KFieldRetirePlace).toString());

    cmm->setDeadDate(qry.value(KFieldDeadDate).toInt());
    cmm->setDeadPlace(qry.value(KFieldDeadPlace).toString());


    // TODO: add field relate to list, like holly list, community, etc.
}
QHash<QString, int> DbSqlitePersonTbl::getSearchFields()
{
    QHash<QString, int> inFields;// TODO: make as class member?
    inFields[KFieldFirstName] = TEXT;
    inFields[KFieldLastName] = TEXT;
    inFields[KFieldHollyName] = TEXT;
    inFields[KFieldEmail] = TEXT;
    inFields[KFieldTel] = TEXT;
    inFields[KFieldIDCard] = TEXT;
    inFields[KFieldAddr] = TEXT;
    inFields[KFieldContact] = TEXT;
    return inFields;
}

QList<QString> DbSqlitePersonTbl::getNameFields()
{
    traced;
    QList<QString> list;// TODO: make as class member?
    list.append(KFieldFirstName);
    list.append(KFieldLastName);
    return list;
}
QHash<QString, QString> DbSqlitePersonTbl::getFieldsCheckExists(const DbModel *item)
{
    traced;
    const Person* per = (Person*)item;
    // TODO: make as class member?
    QHash<QString, QString> list;// = DbSqliteTbl::getFieldsCheckExists(item);
    list[KFieldLastName] = per->lastName();
    list[KFieldFirstName] = per->firstName();
    list[KFieldBirthDay] = QString("%1").arg(per->birthday());
    list[KFieldBirthPlace] = per->birthPlace();
    list[KFieldIDCard] = per->idCard();
    list[KFieldHollyName] = per->hollyName();
    return list;
}

