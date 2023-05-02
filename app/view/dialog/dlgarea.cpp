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
 * Filename: dlgarea.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "dlgarea.h"
#include "communityctl.h"
#include "ui_dlgarea.h"
#include "logger.h"
#include "errcode.h"
#include "community.h"
#include "areactl.h"
#include "area.h"
#include "countryctl.h"
#include "communityctl.h"
#include "dialog/dlgsearchperson.h"
#include "dbmodel.h"


DlgArea::DlgArea(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgArea)
{
    ui->setupUi(this);
    loadData();
}

DlgArea::~DlgArea()
{
    tracein;
    delete ui;
    traceout;
}

ErrCode DlgArea::buildModel(DbModel *model, QString& errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    Area* comm = (Area*) model;
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
        QString name = ui->txtCode->text().trimmed();
        if (!name.isEmpty()) {
            comm->setNameId(name);
        }
    }
    if (err == ErrNone){
        comm->setAddr(ui->txtAddr->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setTel(ui->txtTel->text().trimmed());
    }
    if (err == ErrNone){
        comm->setEmail(ui->txtEmail->toPlainText().trimmed());
    }
    if (err == ErrNone){
        SET_VAL_FROM_CBOX(ui->cbCountry, comm->setCountryUid, comm->setCountryName, err);
    }
    if (err == ErrNone){
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, comm->setModelStatus, comm->setModelStatusName);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtStartDate, comm->setStartDate, DEFAULT_FORMAT_YMD);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtEndDate, comm->setEndDate, DEFAULT_FORMAT_YMD);
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtNote->toPlainText().trimmed());
    }
    traceret(err);
    return err;
}

ErrCode DlgArea::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    err = DlgCommonEditModel::fromModel(item);
    Area* comm = (Area*)model();
    if (err == ErrNone) {
        ui->txtName->setText(comm->name());
        ui->txtCode->setText(comm->nameId()); // TODO: auto generate???
        ui->txtStartDate->setText(Utils::date2String(comm->startDate(), DEFAULT_FORMAT_YMD));
        ui->txtEndDate->setText(Utils::date2String(comm->endDate(), DEFAULT_FORMAT_YMD));
        ui->txtAddr->setPlainText(comm->addr());
        Utils::setSelectItemComboxByData(ui->cbCountry, comm->countryUid());
        ui->txtTel->setText(comm->tel());
        ui->txtEmail->setPlainText(comm->email());
        Utils::setSelectItemComboxByData(ui->cbStatus, comm->modelStatus());
        ui->txtNote->setPlainText(comm->remark());
    }
    traceret(err);
    return err;
}

QDialogButtonBox *DlgArea::buttonBox()
{
    return ui->buttonBox;
}

DbModel *DlgArea::newModel()
{
    // TODO: check model parameter???
    return Area::build();
}

void DlgArea::loadData()
{
    tracein;
    loadList(ui->cbCountry, COUNTRYCTL);
    loadStatus();
    traceout;
}

void DlgArea::loadStatus()
{
    tracein;
    ui->cbStatus->clear();
    const QHash<int, QString>* statuses = DbModel::getModelStatusIdNameMap();
    logd("the number of status %lld", statuses->count());
    foreach (int key, statuses->keys()) {
        ui->cbStatus->addItem(statuses->value(key), key);
    }
    traceout;
}


void DlgArea::on_txtName_textChanged(const QString &arg1)
{
    tracein;
    onChangeNameIdTxt(ui->txtCode, arg1);
    traceout;
}


void DlgArea::on_btnChangeNameId_clicked()
{
    tracein;
    onEditnameId(ui->txtCode);
    traceout;
}
