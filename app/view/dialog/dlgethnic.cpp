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

DlgEthnic::DlgEthnic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgEthnic),
    mEthnic(nullptr)
{
    ui->setupUi(this);

    QList<DbModel *> list = COUNTRYCTL->getAllItemsFromDb();
    foreach (DbModel* item, list) {
        ui->cbCountry->addItem(item->name(), item->dbId());

    }
}

DlgEthnic::~DlgEthnic()
{
    if (mEthnic != nullptr){
        delete mEthnic;
        mEthnic = nullptr;
    }
    delete ui;
}

Ethnic *DlgEthnic::ethnic() const
{
    return mEthnic;
}

void DlgEthnic::accept()
{
    tracein;
    ErrCode ret = ErrNone;
    QString name = ui->txtName->text().trimmed();
    QString currtex = ui->cbCountry->currentText();
    qint64 countryDbId = 0;
    if (!currtex.isEmpty()){
        int index = ui->cbCountry->findText(currtex);
        logd("index %d", index);
        if (index > 0){
            countryDbId = ui->cbCountry->itemData(index).toInt();
        }
    }
    if (!name.isEmpty() && (countryDbId >= 0)){
        if (mEthnic == nullptr)
            mEthnic = new Ethnic();
        if (mEthnic != nullptr){
            mEthnic->setName(name);

            mEthnic->setUid(Utils::UidFromName(name + QString("%1").arg(countryDbId)));


            mEthnic->setCountryDbId(countryDbId);

            logi("Save new country to db");
            ret = mEthnic->save();
        } else {
            ret = ErrNoMemory;
            loge("No memory");
        }
    } else {
        ret = ErrInvalidData;
        loge("Name field is empty");
    }

    logi("Add Ethnic, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));
}
