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
 * Filename: dlgsaint.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/25/2022
 * Brief:
 */
#include "dlgsaint.h"
#include "ui_dlgsaint.h"
#include "logger.h"
#include "errcode.h"
#include "country.h"
#include "countryctl.h"
#include "utils.h"
#include "saint.h"

DlgSaint::DlgSaint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSaint),
    mSaint(nullptr)
{
    ui->setupUi(this);

    logd("Load country");
    QList<DbModel*> listCountry = COUNTRYCTL->getAllItemsFromDb();
    logd("Found %d country", listCountry.length());
    foreach(DbModel* item, listCountry){
        item->dump();
        ui->cbCountry->addItem(item->name(), item->uid());
    }
    // TODO: should manual clear item????

}

DlgSaint::~DlgSaint()
{
    if (mSaint) {
        delete mSaint;
        mSaint = nullptr;
    }
    delete ui;
}

void DlgSaint::accept()
{
    traced;
    ErrCode ret = ErrNone;
    QString warning;
    QString name = ui->txtName->text().trimmed();
    if (name.isEmpty()) {
        loge("Empty String!");
        warning += tr("Thiếu tên");
        ret = ErrFailed;
    }

    QString dateStr = ui->txtFeastday->text().simplified();
    qint64 feastday = 0;
    if (!dateStr.isEmpty()) {
        bool isOk = false;
        feastday = Utils::dateFromString(dateStr, DATE_FORMAT_DM, &isOk);
        logd("feastday %s -> %d", dateStr.toStdString().c_str(), feastday);
        if (!isOk || (feastday <= 0)) {
            warning += tr("Sai ngày bổn mạng (định dạng ngày/tháng");
            logd("Invalid feastday %s", dateStr.toStdString().c_str());
            ret = ErrFailed;
        }
    } else {
        logd("No feastday");
    }

    if (warning.isEmpty()){
        if (mSaint == nullptr)
            mSaint = new Saint();
        mSaint->setName(name);
        mSaint->setFeastDay( feastday);
        mSaint->setFullName(ui->txtFullName->text().trimmed());
        Gender gender = GENDER_UNKNOWN;
        if (ui->rbtMale->isChecked()) gender = Gender::MALE;
        if (ui->rbtFemale->isChecked()) gender = Gender::FEMALE;
        mSaint->setGender(gender);

        int index = ui->cbCountry->findText(ui->cbCountry->currentText());
        logd("index %d", index);
        if (index >= 0){
            QString countryuid = ui->cbCountry->itemData(index).toString();
            logd("countryuid %s", countryuid.toStdString().c_str());
            mSaint->setCountryUid(countryuid);
        }
        mSaint->dump();
        logi("Save Saint to db, name %s", mSaint->name().toStdString().c_str());
        ret = mSaint->save();
        logd("save saint ret=%d", ret);
        if (ret == ErrNone){
            logd("all well, accept");
            QDialog::accept();
        }
    } else {
        Utils::showErrorBox(warning);
    }

}


void DlgSaint::on_btnImport_clicked()
{

}

Saint *DlgSaint::saint() const
{
    return mSaint;
}

void DlgSaint::setName(const QString &name)
{
    ui->txtName->setText(name);
}

