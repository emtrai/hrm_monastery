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
 * Filename: dlgethnic.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/28/2022
 * Brief:
 */
#include "dlgethnic.h"
#include "ui_dlgethnic.h"
#include "utils.h"
#include "logger.h"
#include "errcode.h"
#include "ethnic.h"
#include "countryctl.h"
#include "stringdefs.h"
#include "modeldefs.h"


DlgEthnic::DlgEthnic(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgEthnic),
    mEthnic(nullptr)
{
    ui->setupUi(this);
    loadCountry();
}

DlgEthnic::~DlgEthnic()
{
    delete ui;
}

ErrCode DlgEthnic::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    Ethnic* comm = (Ethnic*) model;
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        comm->setMarkModified(true); // start marking fields which are modified
    }
    if (err == ErrNone) {
        QString name = ui->txtName->text().trimmed();
        if (!name.isEmpty()) {
            comm->setName(name);
        }
    }
    if (err == ErrNone){
        QString name = ui->txtNameId->text().trimmed();
        if (!name.isEmpty()) {
            comm->setNameId(name);
        }
    }
    if (err == ErrNone){
        SET_VAL_FROM_CBOX(ui->cbCountry, comm->setCountryUid, comm->setCountryName, err);
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtRemark->toPlainText().trimmed());
    }
    traceret(err);
    return err;

}

ErrCode DlgEthnic::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNameEthnic) {
        err = DlgCommonEditModel::fromModel(item);
        Ethnic* comm = (Ethnic*)model();
        if (!comm) {
            err = ErrNoData;
            loge("allocate model failed");
        }
        if (err == ErrNone) {
            ui->txtName->setText(comm->name());
            ui->txtNameId->setText(comm->nameId());
            Utils::setSelectItemComboxByData(ui->cbCountry, comm->countryUid());
            ui->txtRemark->setPlainText(comm->remark());
        }
    } else {
        err = ErrInvalidArg;
        loge("null item or invalid type");
    }
    traceret(err);
    return err;

}

DbModel *DlgEthnic::newModel()
{
    return Ethnic::build();

}

void DlgEthnic::loadCountry()
{
    tracein;
    Utils::buildComboxFromModel(ui->cbCountry, COUNTRYCTL);
    traceout;
}

void DlgEthnic::on_txtName_textChanged(const QString &arg1)
{
    DlgCommonEditModel::onChangeNameIdTxt(ui->txtNameId, arg1);

}


void DlgEthnic::on_txtChange_clicked()
{
    DlgCommonEditModel::onEditnameId(ui->txtNameId);
    if (ui->txtNameId->text().length() == 0) { // custome nameid is null, make it auto generate
        onChangeNameIdTxt(ui->txtNameId, ui->txtName->text());
    }

}

