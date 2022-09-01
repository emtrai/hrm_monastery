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
 * Filename: dlgcountry.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/28/2022
 * Brief:
 */
#include "dlgcountry.h"
#include "ui_dlgcountry.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "country.h"
#include "countryctl.h"

DlgCountry::DlgCountry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCountry),
    mCountry(nullptr)
{
    traced;
    ui->setupUi(this);
    ui->cbRegion->addItems(CountryCtl::getInstance()->getRegionList());
    ui->cbContinent->addItems(CountryCtl::getInstance()->getContinentList());
}

DlgCountry::~DlgCountry()
{
    traced;
    if (mCountry != nullptr) {
        delete mCountry;
        mCountry = nullptr;
    }
    delete ui;
}

void DlgCountry::accept()
{
    traced;
    ErrCode ret = ErrNone;
    QString name = ui->txtName->text().trimmed();
    QString shortname = ui->txtShortName->text().trimmed();
    if (!name.isEmpty() && !shortname.isEmpty()){
        if (mCountry == nullptr)
            mCountry = new Country();
        if (mCountry != nullptr){
            mCountry->setName(name);
            mCountry->setShortName(shortname);
            mCountry->setUid(Utils::UidFromName(name));
            // TODO: short name????
            mCountry->setRegion(ui->cbRegion->currentText().trimmed());
            mCountry->setContinent(ui->cbContinent->currentText().trimmed());
            mCountry->dump();
            logi("Save new country to db");
            ret = mCountry->save();
        } else {
            ret = ErrNoMemory;
            loge("No memory");
        }
    } else {
        ret = ErrInvalidData;
        loge("Name field is empty");
    }

    logi("Add Country, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));
}

Country *DlgCountry::country() const
{
    return mCountry;
}


