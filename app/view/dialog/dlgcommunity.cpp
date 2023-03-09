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
 * Filename: dlgcommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "dlgcommunity.h"
#include "communityctl.h"
#include "ui_dlgcommunity.h"
#include "logger.h"
#include "errcode.h"
#include "community.h"
#include "areactl.h"
#include "area.h"
#include "countryctl.h"
#include "communityctl.h"
#include "dialog/dlgsearchperson.h"
#include "dbmodel.h"


DlgCommunity::DlgCommunity(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgCommunity)
{
    ui->setupUi(this);
#ifdef TEST_ENABLE
    ui->frmCommTestBtn->setVisible(true);
#else
    ui->frmCommTestBtn->setVisible(false);
#endif //TEST_ENABLE
    loadData();
}

DlgCommunity::~DlgCommunity()
{
    traced;
    delete ui;
    tracede;
}

ErrCode DlgCommunity::buildModel(DbModel *model, QString& errMsg)
{
    traced;
    ErrCode err = ErrNone;
    Community* comm = (Community*) model;
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
        } else {
            err = ErrInvalidData;
            errMsg += tr("Thiếu Tên");
            loge("Lack of name");
        }
    }
    if (err == ErrNone){
        comm->setNameId(ui->txtCode->text().trimmed());
    }
    if (err == ErrNone){
        SET_VAL_FROM_CBOX(ui->cbArea, comm->setAreaUid, comm->setAreaName);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtFeaseDay, comm->setFeastDate, DATE_FORMAT_MD);
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
        SET_VAL_FROM_CBOX(ui->cbCountry, comm->setCountryUid, comm->setCountry);
    }
    if (err == ErrNone){
        comm->setChurch(ui->txtChurch->toPlainText().trimmed());
    }
    if (err == ErrNone){
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, comm->setStatus, comm->setStatusName);
    }
    if (err == ErrNone){
        SET_DATE_VAL_FROM_WIDGET(ui->txtEstablishDate, comm->setCreateDate);
    }
    if (err == ErrNone){
        SET_VAL_FROM_CBOX(ui->cbParentCommunity, comm->setParentUid, comm->setParentName);
    }
    if (err == ErrNone){
        SET_VAL_FROM_TEXTBOX(ui->txtCEO, KItemUid, comm->setCurrentCEOUid, comm->setCurrentCEO);
    }
    if (err == ErrNone){
        comm->setContact(ui->txtContact->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setIntro(ui->txtIntro->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtNote->toPlainText().trimmed());
    }
    tracedr(err);
    return err;
}

ErrCode DlgCommunity::fromModel(const DbModel *item)
{
    traced;
    ErrCode err = ErrNone;
    err = DlgCommonEditModel::fromModel(item);
    Community* comm = (Community*)model();
    if (err == ErrNone) {
        ui->txtName->setText(comm->name());
        ui->txtCode->setText(comm->nameId()); // TODO: auto generate???
        Utils::setSelectItemComboxByData(ui->cbArea, comm->areaUid());
        ui->txtEstablishDate->setText(Utils::date2String(comm->createDate()));
        ui->txtFeaseDay->setText(Utils::date2String(comm->feastDate(), DATE_FORMAT_MD));
        ui->txtAddr->setPlainText(comm->addr());
        Utils::setSelectItemComboxByData(ui->cbCountry, comm->countryUid());
        ui->txtTel->setText(comm->tel());
        ui->txtEmail->setPlainText(comm->email());
        ui->txtChurch->setPlainText(comm->church());
        Utils::setSelectItemComboxByData(ui->cbStatus, comm->getStatus());
        // TODO: image???
        Utils::setSelectItemComboxByData(ui->cbParentCommunity, comm->parentUid());
        SET_TEXTBOX_FROM_VALUE(ui->txtCEO, KItemUid,
                               comm->currentCEOUid(), comm->currentCEO());


        ui->txtIntro->setPlainText(comm->intro());
        ui->txtContact->setPlainText(comm->contact());
        ui->txtNote->setPlainText(comm->remark());
    }
    tracedr(err);
    return err;
}

DbModel *DlgCommunity::newModel()
{
    // TODO: check model parameter???
    return Community::build();
}

bool DlgCommunity::onValidateData(QString &msg)
{
    traced;
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    return true;
}

void DlgCommunity::loadData()
{
    traced;
    loadList(ui->cbArea, AREACTL);
    loadList(ui->cbCountry, COUNTRYCTL);
    loadList(ui->cbParentCommunity, COMMUNITYCTL);
    loadStatus();
    tracede;
}

void DlgCommunity::loadStatus()
{
    traced;
    ui->cbStatus->clear();
    const QHash<int, QString>* statuses = DbModel::getStatusIdNameMap();
    logd("the number of status %d", statuses->count());
    foreach (int key, statuses->keys()) {
        ui->cbStatus->addItem(statuses->value(key), key);
    }
    tracede;
}


void DlgCommunity::on_btnSearchCEO_clicked()
{
    traced;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(false);

    if (dlg->exec() == QDialog::Accepted){
        Person* per = (Person*)dlg->selectedItem();
        if (per != nullptr) {
            ui->txtCEO->setText(per->getFullName());
            logd("setProperty %s", per->uid().toStdString().c_str());
            ui->txtCEO->setProperty(KItemUid, per->uid());
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    tracede;
}

