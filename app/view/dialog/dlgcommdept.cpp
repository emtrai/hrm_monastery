/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dlgcommdept.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/8/2023
 * Brief:
 */
#include "dlgcommdept.h"
#include "ui_dlgcommdept.h"
#include "communitydeptctl.h"
#include "departctl.h"
#include "logger.h"
#include "errcode.h"
#include "communitydept.h"
#include "dbmodel.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include "datetimeutils.h"

DlgCommDept::DlgCommDept(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgCommDept),
    mCommunity(nullptr)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Thoát"));
    ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Lưu"));
    loadDept();
    loadStatus();
}

DlgCommDept::~DlgCommDept()
{
    tracein;
    delete ui;
    if (mCommunity) delete mCommunity;
    traceout;
}

void DlgCommDept::setupUI()
{

}

ErrCode DlgCommDept::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    CommunityDept* comm = (CommunityDept*) model;
    if (!comm){
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
        SET_VAL_FROM_CBOX(ui->cbDept, comm->setDepartmentUid, comm->setDepartmentName, err);
    }
    if ((err == ErrNone) && mCommunity) {
        comm->setCommunityUid(mCommunity->uid());
        comm->setCommunityNameId(mCommunity->nameId());
        comm->setCommunityName(mCommunity->name());
        comm->setCommunityDbId(mCommunity->dbId());
    }

    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET_RET(ui->txtEstablishDate, comm->setEstablishDate, DEFAULT_FORMAT_YMD, err);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET_RET(ui->txtCloseDate, comm->setClosedDate, DEFAULT_FORMAT_YMD, err);
    }
    if (err == ErrNone){
        comm->setAddr(ui->txtAddr->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setTel(ui->txtTel->text().trimmed());
    }
    if (err == ErrNone){
        comm->setEmail(ui->txtEmail->text().trimmed());
    }
    if (err == ErrNone){
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, comm->setModelStatus, comm->setModelStatusName);
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtRemark->toPlainText().trimmed());
    }
    traceret(err);
    return err;
}

ErrCode DlgCommDept::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    err = DlgCommonEditModel::fromModel(item);
    CommunityDept* comm = (CommunityDept*)model();
    if (err == ErrNone) {
        ui->txtName->setText(comm->name());
        ui->txtNameId->setText(comm->nameId()); // TODO: auto generate???
        Utils::setSelectItemComboxByData(ui->cbDept, comm->departmentUid());
        ui->txtEstablishDate->setText(DatetimeUtils::date2String(comm->establishDate(), DEFAULT_FORMAT_YMD));
        ui->txtCloseDate->setText(DatetimeUtils::date2String(comm->closedDate(), DEFAULT_FORMAT_YMD));
        ui->txtAddr->setPlainText(comm->addr());
        ui->txtTel->setText(comm->tel());
        ui->txtEmail->setText(comm->email());
        ui->txtAddr->setPlainText(comm->addr());
        Utils::setSelectItemComboxByData(ui->cbStatus, comm->modelStatus());
        ui->txtRemark->setPlainText(comm->remark());
    }
    traceret(err);
    return err;

}

DbModel *DlgCommDept::newModel()
{

    // TODO: check model parameter???
    return CommunityDept::build();
}

void DlgCommDept::loadDept()
{
    tracein;
    loadList(ui->cbDept, DEPART, false);
    traceout;
}

void DlgCommDept::loadStatus()
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

void DlgCommDept::on_cbDept_currentIndexChanged(int index)
{
    tracein;
    logd("index=%d", index);
    if (index >= 0) {
        QString uid;
        QVariant value = ui->cbDept->itemData(index);
        if (!value.isNull()) {
            uid = value.toString();
        } else {
            loge("not item data");
        }
        logd("uid=%s", STR2CHA(uid));
        if (!uid.isEmpty()) {
            DbModel* model = DEPART->getModelByUid(uid);
            if (model) {
                ui->txtName->setText(model->name());
                ui->txtNameId->setText(QString("%1_%2").arg((mCommunity?mCommunity->nameId():"")).arg(model->nameId()));
                delete model;
            } else {
                loge("not found dept with uid '%s'", STR2CHA(uid));
            }
        } else {
            loge("unknown uid");
        }
    }
    traceout;
}

DbModel *DlgCommDept::community() const
{
    return mCommunity;
}

void DlgCommDept::setCommunity(const DbModel *newCommunity)
{
    if (mCommunity) delete mCommunity;
    if (newCommunity) {
        mCommunity = newCommunity->clone();
        on_cbDept_currentIndexChanged(ui->cbDept->currentIndex());
    } else {
        loge("Set communityf failed, null input");
    }
}

