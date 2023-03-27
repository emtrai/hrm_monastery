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
 * Filename: dlgprovince.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/1/2022
 * Brief:
 */
#include "dlgprovince.h"
#include "ui_dlgprovince.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "province.h"
#include "provincectl.h"
#include "country.h"
#include "countryctl.h"
#include "view/dialog/dlgcountry.h"

DlgProvince::DlgProvince(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProvince),
    mProvince(nullptr)
{
    ui->setupUi(this);
    loadCountry();
    loadProvince();
}

DlgProvince::~DlgProvince()
{
    if (mProvince != nullptr){
        delete mProvince;
        mProvince = nullptr;
    }
    delete ui;
}

Province *DlgProvince::province() const
{
    return mProvince;
}

void DlgProvince::loadCountry()
{
    tracein;
    logd("Load country");
    QList<DbModel*> listCountry = COUNTRYCTL->getAllItemsFromDb();
    ui->cbCountry->clear();
    foreach(DbModel* item, listCountry){
        ui->cbCountry->addItem(item->name(), item->uid());
    }
}

void DlgProvince::loadProvince()
{
    tracein;
    logd("Load province");
    ui->cbProvince->clear();
    QString countryUid = Utils::getCurrentComboxDataString(ui->cbCountry);
    if (!countryUid.isEmpty()) {
        const QList<Province*>* list = INSTANCE(ProvinceCtl)->getProvinceList(countryUid);
        foreach(Province* item, *list){
            ui->cbProvince->addItem(item->name(), item->uid());
        }
    }
}

void DlgProvince::accept()
{
    tracein;
    ErrCode ret = ErrNone;
    QString name = ui->txtName->text().trimmed();
    if (!name.isEmpty()){
        if (mProvince == nullptr)
            mProvince = new Province();
        if (mProvince != nullptr){
            mProvince->setName(name); // TODO: validate info
            mProvince->setUid(Utils::UidFromName(name));
            QString countryUid = Utils::getCurrentComboxDataString(ui->cbCountry);
            mProvince->setCountryUid(countryUid);
            QString parentUid = Utils::getCurrentComboxDataString(ui->cbProvince);
            mProvince->setParentUid(parentUid);
            mProvince->dump();
            logi("Save new province to db");
            ret = mProvince->save();
        } else {
            ret = ErrNoMemory;
            loge("No memory");
        }
    } else {
        ret = ErrInvalidData;
        loge("Name field is empty");
    }

    logi("Add Province, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));
}

void DlgProvince::on_btnAddCountry_clicked()
{
    tracein;
    DlgCountry * dlg = new DlgCountry();
    if (dlg == nullptr) {
        loge("Open dlg country fail, No memory");
        return;
    }
    dlg->setIsSelfSave(true);

    if (dlg->exec() == QDialog::Accepted){
        loadCountry();
        loadProvince();
    }
    delete dlg;
}


void DlgProvince::on_btnAddCountry_triggered(QAction *arg1)
{
    tracein;
}

