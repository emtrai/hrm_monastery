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
 * Filename: dlgeditmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/16/2023
 * Brief:
 */
#include "dlgeditmodel.h"
#include "ui_dlgeditmodel.h"
#include "dbmodel.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include "stringdefs.h"


DlgEditModel::DlgEditModel(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgEditModel)
{
    ui->setupUi(this);
}

DlgEditModel::~DlgEditModel()
{
    delete ui;
}

QDialogButtonBox *DlgEditModel::buttonBox()
{
    return ui->buttonBox;
}

ErrCode DlgEditModel::buildModel(DbModel *model, QString& errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        model->setMarkModified(true); // start marking fields which are modified
    }
    if (err == ErrNone) {
        QString name = ui->txtName->text().trimmed();
        if (!name.isEmpty()) {
            model->setName(name);
        } else {
            err = ErrInvalidData;
            errMsg += tr("Thiếu Tên");
            loge("Lack of name");
        }
    }
    if (err == ErrNone){
        QString nameId = ui->txtNameId->text().trimmed();
        if (!nameId.isEmpty()) {
            model->setNameId(nameId);
        } else {
            err = ErrInvalidData;
            errMsg += tr("Thiếu Tên định danh");
            loge("Lack of nameid");
        }
    }
    if (err == ErrNone){
        model->setRemark(ui->txtRemark->toPlainText().trimmed());
    }
    traceret(err);
    return err;

}

ErrCode DlgEditModel::fromModel(const DbModel *model)
{
    tracein;
    ErrCode err = ErrNone;
    err = DlgCommonEditModel::fromModel(model);
    if (err == ErrNone) {
        ui->txtName->setText(model->name());
        ui->txtNameId->setText(model->nameId());
        ui->txtRemark->setPlainText(model->remark());
    }
    traceret(err);
    return err;
}

DbModel *DlgEditModel::newModel()
{
    return mModel;
}

bool DlgEditModel::onValidateData(QString &msg)
{
    return true;
}


void DlgEditModel::on_txtName_textChanged(const QString &arg1)
{
    this->onChangeNameIdTxt(ui->txtNameId, arg1);
}


void DlgEditModel::on_btnChangeCode_clicked()
{
    this->onEditnameId(ui->txtNameId);
    if (ui->txtNameId->text().length() == 0) { // custome nameid is null, make it auto generate
        onChangeNameIdTxt(ui->txtNameId, ui->txtName->text());
    }
}

