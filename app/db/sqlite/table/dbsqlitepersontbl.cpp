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
#include "dbsqliteupdatebuilder.h"
#include "dbsqlitedefs.h"
#include "person.h"
#include "dbsqlite.h"

#include "defs.h"
#include "logger.h"

#include <QSqlQuery>

#include "filter.h"
#include "utils.h"
#include "dbsqlitespecialistpersontbl.h"
#include "specialistperson.h"

const qint32 DbSqlitePersonTbl::KVersionCode = VERSION_CODE(0,0,1);


DbSqlitePersonTbl::DbSqlitePersonTbl(DbSqlite* db):
    DbSqliteTbl(db, KTablePerson, KTablePerson, KVersionCode)
{}

void DbSqlitePersonTbl::addTableField(DbSqliteTableBuilder *builder)
{

    tracein;
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
    // list of saint uid, separated by comma
    // TODO: table saint - person neccessary anymore????
    builder->addField(KFieldSaintUid, TEXT);
    builder->addField(KFieldFeastDay, TEXT);

//    builder->addField(KFieldEthnicName, TEXT);
//    builder->addField(KFieldEduName, TEXT);


    builder->addField(KFieldNationalityUid, TEXT);
    builder->addField(KFieldEthnicUid, TEXT);
    builder->addField(KFieldEduUid, TEXT);
    builder->addField(KFieldEduDetail, TEXT);
    builder->addField(KFieldCourseUid, TEXT);

    builder->addField(KFieldSpecialistInfo, TEXT);

    builder->addField(KFieldCountryUid, TEXT);
    builder->addField(KFieldProvinceUid, TEXT);
    builder->addField(KFieldAddr, TEXT);

    builder->addField(KFieldCommunityUid, TEXT);

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

    builder->addField(KFieldPersonStatusUid, TEXT);

    builder->addField(KFieldRetireDate, INT64);
    builder->addField(KFieldRetirePlace, TEXT);

    builder->addField(KFieldDeadDate, INT64);
    builder->addField(KFieldDeadPlace, TEXT);

    // current work
    builder->addField(KFieldWorkUid, TEXT);
    // THIS IS IMPORTANT NOTE, DON'T REMOVE IT
    // - ANY UPDATE ON THIS, MUST UPDATE Person::clone() as well
}

ErrCode DbSqlitePersonTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{

    DbSqliteTbl::insertTableField(builder, item);
    Person* per = (Person*) item;
    builder->addValue(KFieldImgId, per->imgId());
    builder->addValue(KFieldLastName, per->lastName());
    builder->addValue(KFieldFirstName, per->firstName());

    builder->addValue(KFieldBirthDay, per->birthday());
    builder->addValue(KFieldBirthPlace, per->birthPlace());

    builder->addValue(KFieldIDCard, per->idCard());
    builder->addValue(KFieldIDCardIssueDate, per->idCardIssueDate());
    builder->addValue(KFieldIDCardIssuePlace, per->idCardIssuePlace());

    builder->addValue(KFieldHollyName, per->hollyName());
    builder->addValue(KFieldSaintUid, per->saintUidListInString());
    builder->addValue(KFieldFeastDay, per->feastDay());

    builder->addValue(KFieldNationalityUid, per->nationalityUid());
    builder->addValue(KFieldEthnicUid, per->ethnicUid());
    builder->addValue(KFieldEduUid, per->eduUid());
    builder->addValue(KFieldEduDetail, per->eduDetail());
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


    builder->addValue(KFieldPersonStatusUid, per->personStatusUid());

    builder->addValue(KFieldRetireDate, per->retireDate());
    builder->addValue(KFieldRetirePlace, per->retirePlace());

    builder->addValue(KFieldDeadDate, per->deadDate());
    builder->addValue(KFieldDeadPlace, per->deadPlace());

    builder->addValue(KFieldSpecialistInfo, per->specialistInfo());

    // current work
    builder->addValue(KFieldWorkUid, per->currentWorkUid());

    // TODO: add field relate to list, like holly list, community, etc.
    return ErrNone;
}

// TODO: separate it into 2 info: detail and brief????
// as person has a lots of information
ErrCode DbSqlitePersonTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    // TODO: separate into short info and full info, to avoid consume too much memory?
    // TODO: paging to avoid too much memory?
    Person* cmm = (Person*) item;
    if (!qry.isNull(KFieldPersonUid)) {
        cmm->setUid(qry.value(KFieldPersonUid).toString());
    }
    if (!qry.isNull(KFieldPersonNameId)) {
        cmm->setNameId(qry.value(KFieldPersonNameId).toString());
    }
    if (!qry.isNull(KFieldPersonRemark)) {
        cmm->setRemark(qry.value(KFieldPersonRemark).toString());
    }
    cmm->setImgId(qry.value(KFieldImgId).toString());

    cmm->setLastName(qry.value(KFieldLastName).toString());
    cmm->setFirstName(qry.value(KFieldFirstName).toString());

    cmm->setBirthday(qry.value(KFieldBirthDay).toInt());
    cmm->setBirthPlace(qry.value(KFieldBirthPlace).toString());

    cmm->setIdCard(qry.value(KFieldIDCard).toString());
    cmm->setIdCardIssueDate(qry.value(KFieldIDCardIssueDate).toInt());
    cmm->setIdCardIssuePlace(qry.value(KFieldIDCardIssuePlace).toString());

    cmm->setHollyName(qry.value(KFieldHollyName).toString());
    cmm->setSaintUidList(qry.value(KFieldSaintUid).toString());
    // TODO: reconsider should store list of saint uid in person table or make mapping table???
    cmm->setFeastDay(qry.value(KFieldFeastDay).toInt());

    cmm->setNationalityUid(qry.value(KFieldNationalityUid).toString());
    if (!cmm->nationalityUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCountryModelHandler()->getByUid(cmm->nationalityUid());
        if (model) {
            cmm->setNationalityName(model->name());
            delete model;
        } else {
            logw("not found nationalityUid '%s'", STR2CHA(cmm->nationalityUid()));
        }
    }

    cmm->setEthnicUid(qry.value(KFieldEthnicUid).toString());
    if (!cmm->ethnicUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getEthnicModelHandler()->getByUid(cmm->ethnicUid());
        if (model) {
            cmm->setEthnicName(model->name());
            delete model;
        } else {
            logw("not found ethnicUid '%s'", STR2CHA(cmm->ethnicUid()));
        }
    }

    cmm->setEduUid(qry.value(KFieldEduUid).toString());
    cmm->setEduDetail(qry.value(KFieldEduDetail).toString());
    if (!cmm->eduUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getEduModelHandler()->getByUid(cmm->eduUid());
        if (model) {
            cmm->setEduName(model->name());
            delete model;
        } else {
            logw("not found eduUid '%s'", STR2CHA(cmm->eduUid()));
        }
    }

    cmm->setCourseUid(qry.value(KFieldCourseUid).toString());
    if (!cmm->courseUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCourseModelHandler()->getByUid(cmm->courseUid());
        if (model) {
            cmm->setCourseName(model->name());
            delete model;
        } else {
            logw("not found courseUid '%s'", STR2CHA(cmm->courseUid()));
        }
    }
    // TODO: set specialist list here????
    DbSqliteSpecialistPersonTbl* tblspecialist = (DbSqliteSpecialistPersonTbl*)SQLITE->getTable(KTableSpecialistPerson);
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tblspecialist != nullptr);
    logd("search specialis for per uid: %s", STR2CHA(cmm->uid()));
    QList<DbModel *> listspecialist = tblspecialist->getListSpecialist(cmm->uid());
    logd("no specialist: %d", listspecialist.size());
    if (listspecialist.size() > 0) {
        foreach(DbModel* model, listspecialist) {
            logd("model: '%s'", model?STR2CHA(model->toString()):"(null)");
            if (model && model->modelName() == KModelNameSpecialistPerson) {
                SpecialistPerson* perspecialist = (SpecialistPerson*) model;
                cmm->addSpecialistUid(perspecialist->specialistUid());
                cmm->addSpecialistName(perspecialist->specialist()->name());
            }
        }
        RELEASE_LIST_DBMODEL(listspecialist);
    }

    cmm->setCountryUid(qry.value(KFieldCountryUid).toString());
    if (!cmm->countryUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCountryModelHandler()->getByUid(cmm->countryUid());
        if (model) {
            cmm->setCountryName(model->name());
            delete model;
        } else {
            logw("not found countryUid '%s'", STR2CHA(cmm->countryUid()));
        }
    }

    cmm->setProvinceUid(qry.value(KFieldProvinceUid).toString());
    // TODO: province belong to a country, same name may exist in different company
    // it's a little complicated, comeback with this later
    cmm->setAddr(qry.value(KFieldAddr).toString());
    cmm->setChurchAddr(qry.value(KFieldChurchAddr).toString());
    cmm->setEmail(qry.value(KFieldEmail).toString());
    cmm->setTel(qry.value(KFieldTel).toString());
    cmm->setOtherContact(qry.value(KFieldContact).toString());

    cmm->setCommunityUid(qry.value(KFieldCommunityUid).toString());
    if (!cmm->communityUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getCommunityModelHandler()->getByUid(cmm->communityUid());
        if (model) {
            cmm->setCommunityName(model->name());
            delete model;
        } else {
            logw("not found communityUid '%s'", STR2CHA(cmm->communityUid()));
        }
    }
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
    if (!cmm->joinPICUid().isEmpty() && cmm->joinPICUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->joinPICUid());
        if (model) {
            cmm->setJoinPICName(model->name());
            delete model;
        } else {
            logw("not found joinPICUid '%s'", STR2CHA(cmm->joinPICUid()));
        }
    }

    cmm->setPreTrainJoinDate(qry.value(KFieldPreTrainDate).toInt());
    cmm->setPreTrainPICUid(qry.value(KFieldPreTrainPICUid).toString());
    if (!cmm->preTrainPICUid().isEmpty() && cmm->preTrainPICUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->preTrainPICUid());
        if (model) {
            cmm->setPreTrainPICName(model->name());
            delete model;
        } else {
            logw("not found preTrainPICUid '%s'", STR2CHA(cmm->preTrainPICUid()));
        }
    }

    cmm->setTrainJoinDate(qry.value(KFieldTrainDate).toInt());
    cmm->setTrainPICUid(qry.value(KFieldTrainPICUid).toString());
    if (!cmm->trainPICUid().isEmpty() && cmm->trainPICUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->trainPICUid());
        if (model) {
            cmm->setTrainPICName(model->name());
            delete model;
        } else {
            logw("not found trainPICUid '%s'", STR2CHA(cmm->trainPICUid()));
        }
    }

    cmm->setVowsDate(qry.value(KFieldVowsDate).toInt());
    cmm->setVowsCEOUid(qry.value(KFieldVowsCEOUid).toString());
    if (!cmm->vowsCEOUid().isEmpty() && cmm->vowsCEOUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->vowsCEOUid());
        if (model) {
            cmm->setVowsCEOName(model->name());
            delete model;
        } else {
            logw("not found vowsCEOUid '%s'", STR2CHA(cmm->vowsCEOUid()));
        }
    }

    cmm->setEternalVowsDate(qry.value(KFieldEternalVowsDate).toInt());
    cmm->setEternalVowsPICUid(qry.value(KFieldEternalVowsPICUid).toString());
    if (!cmm->eternalVowsPICUid().isEmpty() && cmm->eternalVowsPICUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->eternalVowsPICUid());
        if (model) {
            cmm->setEternalVowsPICName(model->name());
            delete model;
        } else {
            logw("not found eternalVowsPICUid '%s'", STR2CHA(cmm->eternalVowsPICUid()));
        }
    }



    cmm->setEternalVowsCEOUid(qry.value(KFieldEternalVowsCEOUid).toString());
    if (!cmm->eternalVowsCEOUid().isEmpty() && cmm->eternalVowsCEOUid() != cmm->uid()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonModelHandler()->getByUid(cmm->eternalVowsCEOUid());
        if (model) {
            cmm->setEternalVowsCEOName(model->name());
            delete model;
        } else {
            logw("not found eternalVowsCEOUid '%s'", STR2CHA(cmm->eternalVowsCEOUid()));
        }
    }

    cmm->setBankDate(qry.value(KFieldBankDate).toInt());
    cmm->setBankPlace(qry.value(KFieldBankPlace).toString());

    cmm->setGoldenDate(qry.value(KFieldGoldenDate).toInt());
    cmm->setGoldenPlace(qry.value(KFieldGoldenPlace).toString());

    cmm->setEternalDate(qry.value(KFieldEternalDate).toInt());
    cmm->setEternalPlace(qry.value(KFieldEternalPlace).toString());

    cmm->setPersonStatusUid(qry.value(KFieldPersonStatusUid).toString());
    if (!cmm->personStatusUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getPersonStatusModelHandler()->getByUid(cmm->personStatusUid());
        if (model) {
            cmm->setPersonStatusName(model->name());
            delete model;
        } else {
            logw("not found personStatusUid '%s'", STR2CHA(cmm->personStatusUid()));
        }
    }

    cmm->setRetireDate(qry.value(KFieldRetireDate).toInt());
    cmm->setRetirePlace(qry.value(KFieldRetirePlace).toString());

    cmm->setDeadDate(qry.value(KFieldDeadDate).toInt());
    cmm->setDeadPlace(qry.value(KFieldDeadPlace).toString());

    cmm->setCurrentWorkUid(qry.value(KFieldWorkUid).toString());
    if (!cmm->currentWorkUid().isEmpty()) {
        // TODO: caching data (i.e. list of person in management board) for fast accessing?
        // TODO: is it ok to call here? does it break any design?
        // as table calls directly to model handler
        DbModel* model = SQLITE->getWorkModelHandler()->getByUid(cmm->currentWorkUid());
        if (model) {
            cmm->setCurrentWorkName(model->name());
            delete model;
        } else {
            logw("not found work uid '%s'", STR2CHA(cmm->currentWorkUid()));
        }
    }
    //

    // TODO: add field relate to list, like holly list, community, etc.

    traceout;
    return err;
}

ErrCode DbSqlitePersonTbl::updateTableField(DbSqliteUpdateBuilder *builder,
                                            const QList<QString> &updateField,
                                            const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("Add %d field to update", updateField.count());
    Person* per = (Person*) item;
    foreach (QString field, updateField) {
        logd("Update field %s", field.toStdString().c_str());
        if (field == KItemFullName) {
            builder->addValue(KFieldFirstName, per->firstName());
            builder->addValue(KFieldLastName, per->lastName());
        } else if (field == KItemCommunity) {
            builder->addValue(KFieldCommunityUid, per->communityUid());
        } else if (field == KItemHolly) {
            builder->addValue(KFieldHollyName, per->hollyName());
            builder->addValue(KFieldSaintUid, per->saintUidListInString());
        } else if (field == KItemBirthplace) {
            builder->addValue(KFieldBirthPlace, per->birthPlace());
        } else if (field == KItemBirthday) {
            builder->addValue(KFieldBirthDay, per->birthday());
        } else if (field == KItemChristenDate) {
            builder->addValue(KFieldChristenDate, per->christenDate());
        } else if (field == KItemWork) {
            builder->addValue(KFieldWorkUid, per->currentWorkUid());
        } else if (field == KItemSpecialistInfo) {
            builder->addValue(KFieldSpecialistInfo, per->specialistInfo());
        } else if (field == KItemEduDetail) {
            builder->addValue(KFieldEduDetail, per->eduDetail());
        } else if (field == KItemNationality) {
            builder->addValue(KFieldNationalityUid, per->nationalityUid());
        } else if (field == KItemFeastDay) {
            builder->addValue(KFieldFeastDay, per->feastDay());
        } else if (field == KItemEucharistPlace) {
            builder->addValue(KFieldEucharistPlace, per->eucharistPlace());
        } else if (field == KItemEucharistDate) {
            builder->addValue(KFieldEucharistDate, per->eucharistDate());
        } else if (field == KItemOtherContact) {
            builder->addValue(KFieldContact, per->otherContact());
        } else if (field == KItemImg) {
            builder->addValue(KFieldImgId, per->imgId());
        } else if (field == KItemDeadPlace) {
            builder->addValue(KFieldDeadPlace, per->deadPlace());
        } else if (field == KItemDeadDate) {
            builder->addValue(KFieldDeadDate, per->deadDate());
        } else if (field == KItemRetirePlace) {
            builder->addValue(KFieldRetirePlace, per->retirePlace());
        } else if (field == KItemRetireDate) {
            builder->addValue(KFieldRetireDate, per->retireDate());
        } else if (field == KItemStatus) {
            builder->addValue(KFieldPersonStatusUid, per->personStatusUid());
        } else if (field == KItemEternalPlace) {
            builder->addValue(KFieldEternalPlace, per->eternalPlace());
        } else if (field == KItemEternalDate) {
            builder->addValue(KFieldEternalDate, per->eternalDate());
        } else if (field == KItemGoldenPlace) {
            builder->addValue(KFieldGoldenPlace, per->goldenPlace());
        } else if (field == KItemGoldenDate) {
            builder->addValue(KFieldGoldenDate, per->goldenDate());
        } else if (field == KItemBankPlace) {
            builder->addValue(KFieldBankPlace, per->bankPlace());
        } else if (field == KItemBankDate) {
            builder->addValue(KFieldBankDate, per->bankDate());
        } else if (field == KItemEternalVowsCEO) {
            builder->addValue(KFieldEternalVowsCEOUid, per->eternalVowsCEOUid());
        } else if (field == KItemEternalVowsPIC) {
            builder->addValue(KFieldEternalVowsPICUid, per->eternalVowsPICUid());
        } else if (field == KItemEternalVowsDate) {
            builder->addValue(KFieldEternalVowsDate, per->eternalVowsDate());
        } else if (field == KItemVowsDate) {
            builder->addValue(KFieldVowsDate, per->vowsDate());
        } else if (field == KItemVowsCEO) {
            builder->addValue(KFieldVowsCEOUid, per->vowsCEOUid());
        } else if (field == KItemTrainPIC) {
            builder->addValue(KFieldTrainPICUid, per->trainPICUid());
        } else if (field == KItemTrainDate) {
            builder->addValue(KFieldTrainDate, per->trainJoinDate());
        } else if (field == KItemPreTrainPIC) {
            builder->addValue(KFieldPreTrainPICUid, per->preTrainPICUid());
        } else if (field == KItemPreTrainDate) {
            builder->addValue(KFieldPreTrainDate, per->preTrainJoinDate());
        } else if (field == KItemJoinPIC) {
            builder->addValue(KFieldJoinPICUid, per->joinPICUid());
        } else if (field == KItemJoinDate) {
            builder->addValue(KFieldJoinDate, per->joinDate());
        } else if (field == KItemFamilyContact) {
            builder->addValue(KFieldFamilyContact, per->familyContact());
        } else if (field == KItemHollyDate) {
            builder->addValue(KFieldHollyDate, per->hollyDate());
        } else if (field == KItemHollyPlace) {
            builder->addValue(KFieldHollyPlace, per->hollyPlace());
        } else if (field == KItemChristenDate) {
            builder->addValue(KFieldChristenDate, per->christenDate());
        } else if (field == KItemChristenPlace) {
            builder->addValue(KFieldChristenPlace, per->christenPlace());
        } else if (field == KItemFamilyHistory) {
            builder->addValue(KFieldFamilyHistory, per->familyHistory());
        } else if (field == KItemMomAddr) {
            builder->addValue(KFieldMomAddr, per->momAddr());
        } else if (field == KItemMomBirthday) {
            builder->addValue(KFieldMomBirthDay, per->momBirthday());
        } else if (field == KItemMom) {
            builder->addValue(KFieldMomName, per->momName());
        } else if (field == KItemDadAddr) {
            builder->addValue(KFieldDadAddr, per->dadAddr());
        } else if (field == KItemDadBirthday) {
            builder->addValue(KFieldDadBirthDay, per->dadBirthday());
        } else if (field == KItemDad) {
            builder->addValue(KFieldDadName, per->dadName());
        } else if (field == KItemTel) {
            builder->addValue(KFieldTel, per->tel().join(';'));
        } else if (field == KItemEmail) {
            builder->addValue(KFieldEmail, per->email().join(';'));
        } else if (field == KItemChurchAddress) {
            builder->addValue(KFieldChurchAddr, per->churchAddr());
        } else if (field == KItemAddress) {
            builder->addValue(KFieldAddr, per->addr());
        } else if (field == KItemCountry) {
            builder->addValue(KFieldCountryUid, per->countryUid());
        } else if (field == KItemCourse) {
            builder->addValue(KFieldCourseUid, per->courseUid());
        } else if (field == KItemEdu) {
            builder->addValue(KFieldEduUid, per->eduUid());
        } else if (field == KItemIDcardIssuer) {
            builder->addValue(KFieldIDCardIssuePlace, per->idCardIssuePlace());
        } else if (field == KItemIDcardIssueDate) {
            builder->addValue(KFieldIDCardIssueDate, per->idCardIssueDate());
        } else if (field == KItemIDcard) {
            builder->addValue(KFieldIDCard, per->idCard());
        } else if (field == KItemEthnic) {
            builder->addValue(KFieldEthnicUid, per->ethnicUid());
        }

    }
    traceret(err);
    return err;
}
QHash<QString, int> DbSqlitePersonTbl::getSearchFields()
{
    QHash<QString, int> inFields;// TODO: make as class member?
    inFields[KFieldFirstName] = TEXT;
    inFields[KFieldLastName] = TEXT;
    inFields[KFieldHollyName] = TEXT;
    inFields[COLUMN_NAME(name(), KFieldEmail)] = TEXT;
    inFields[COLUMN_NAME(name(), KFieldTel)] = TEXT;
    inFields[KFieldIDCard] = TEXT;
    inFields[COLUMN_NAME(name(), KFieldAddr)] = TEXT;
    inFields[KFieldContact] = TEXT;
//    inFields[KFieldWorkName] = TEXT;
    return inFields;
}

QList<QString> DbSqlitePersonTbl::getNameFields()
{
    tracein;
    QList<QString> list;// TODO: make as class member?
    list.append(KFieldFirstName);
    list.append(KFieldLastName);
    return list;
}

QString DbSqlitePersonTbl::getSearchQueryString(const QString &cond)
{
    tracein;
    QString queryString = QString("SELECT *, (%2 || ' ' || %3) AS %4, %5.%8 AS %9 FROM %1 " \
                                  " LEFT JOIN %5 ON %1.%6 = %5.%7 ")
                              .arg(name())
                              .arg(KFieldLastName, KFieldFirstName, KFieldFullName)
                              .arg(KTableCommunity, KFieldCommunityUid, KFieldUid, KFieldName, KFieldCommunityName)
                                ;

    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

QString DbSqlitePersonTbl::getFilterQueryString(int fieldId, const QString &cond)
{
    tracein;
    logd("fieldId %d", fieldId);
    QString joinQuery;
    QString selectQuery;
    switch (fieldId) {
    case FILTER_FIELD_EDUCATION:
        selectQuery = QString("%1.%2 AS %3, %1.%4 AS %5, %1.%6 AS %7")
                        .arg(KTableEdu, KFieldName, KFieldEduName)
                        .arg(KFieldUid, KFieldEduUid)
                        .arg(KFieldNameId, KFieldEduNameId)
            ;
        joinQuery = QString("LEFT JOIN %2 ON %1.%4 = %2.%3")
                        .arg(name(), KTableEdu)
                        .arg(KFieldUid, KFieldEduUid)
            ;
        break;
    case FILTER_FIELD_COMMUNITY:
        selectQuery = QString("%1.%2 AS %7, %1.%4 AS %5, %1.%3 AS %6")
                        .arg(KTableCommunity) // 1
                        .arg(KFieldUid, KFieldNameId) // 2 3
                        .arg(KFieldName, KFieldCommunityName) // 4 5
                        .arg(KFieldCommunityNameId, KFieldCommunityUid) // 6 7
            ;
        joinQuery = QString("LEFT JOIN %2 ON %1.%4 = %2.%3")
                        .arg(name(), KTableCommunity) // 1, 2
                        .arg(KFieldUid, KFieldCommunityUid) // 3, 4
            ;
        break;
    case FILTER_FIELD_COURSE:
        selectQuery = QString("%1.%2 AS %7, %1.%4 AS %5, %1.%3 AS %6")
                          .arg(KTableCourse) // 1
                          .arg(KFieldUid, KFieldNameId) // 2 3
                          .arg(KFieldName, KFieldCourseName) // 4 5
                          .arg(KFieldCourseNameId, KFieldCourseUid) // 6 7
            ;
        joinQuery = QString("LEFT JOIN %2 ON %1.%4 = %2.%3")
                        .arg(name(), KTableCourse) // 1, 2
                        .arg(KFieldUid, KFieldCourseUid) // 3, 4
            ;
        break;
    case FILTER_FIELD_WORK:
        selectQuery = QString("%1.%2 AS %7, %1.%4 AS %5, %1.%3 AS %6")
                          .arg(KTableWork) // 1
                          .arg(KFieldUid, KFieldNameId) // 2 3
                          .arg(KFieldName, KFieldWorkName) // 4 5
                          .arg(KFieldWorkNameId, KFieldWorkUid) // 6 7
            ;
        joinQuery = QString("LEFT JOIN %2 ON %1.%4 = %2.%3")
                        .arg(name(), KTableWork) // 1, 2
                        .arg(KFieldUid, KFieldWorkUid) // 3, 4
            ;
        break;
    default: // TODO: implement more
        break;
    }

    logd("selectQuery: '%s'", STR2CHA(selectQuery));
    logd("joinQuery: '%s'", STR2CHA(joinQuery));

    QString queryString = QString("SELECT *, (%2 || ' ' || %3) AS %4 %5 FROM %1 %6")
                              .arg(name())
                              .arg(KFieldLastName, KFieldFirstName, KFieldFullName)
                              .arg((selectQuery.isEmpty()?"":"," + selectQuery))
                              .arg((joinQuery.isEmpty()?"":joinQuery))
        ;
    if (!cond.isEmpty()) {
        queryString += QString(" WHERE %1").arg(cond);
    }
    logd("queryString: %s", queryString.toStdString().c_str());
    return queryString;
}

ErrCode DbSqlitePersonTbl::updateCommunity(const QString &uid, const QString &communityUid)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("update community for person, uid '%s', community uid '%s'", STR2CHA(uid), STR2CHA(communityUid));
    QHash<QString, FieldValue> fields;
    fields[KFieldCommunityUid] = FieldValue(communityUid);
    ret = this->update(uid, fields);
    traceret(ret);
    return ret;
}

QHash<QString, QString> DbSqlitePersonTbl::getFieldsCheckExists(const DbModel *item)
{
    tracein;
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

