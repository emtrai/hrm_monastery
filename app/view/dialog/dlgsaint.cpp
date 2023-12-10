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
#include "countryctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "saint.h"
#include "dialogutils.h"
#include "stringdefs.h"
#include "mainwindow.h"
#include "modeldefs.h"

DlgSaint::DlgSaint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSaint),
    mSaint(nullptr),
    mCustomNameId(false)
{
    ui->setupUi(this);

    logd("Load country");
    QList<DbModel*> listCountry = COUNTRYCTL->getAllItemsFromDb();
    logd("Found %lld country", listCountry.length());
    ui->cbCountry->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCountry){
        ui->cbCountry->addItem(item->name(), item->uid());
    }
    // TODO: should manual clear item????

    if (ui->buttonBox->button(QDialogButtonBox::Cancel))
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_CANCEL);
    if (ui->buttonBox->button(QDialogButtonBox::Ok))
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText(STR_OK);
    if (ui->buttonBox->button(QDialogButtonBox::Save))
        ui->buttonBox->button(QDialogButtonBox::Save)->setText(STR_SAVE);
}

DlgSaint::~DlgSaint()
{
    traced;
    FREE_PTR(mSaint);
    delete ui;
}

void DlgSaint::accept()
{
    tracein;
    ErrCode ret = ErrNone;
    QString warning;
    QString name = ui->txtName->text().trimmed();
    QString nameid = ui->txtNameId->text().trimmed();
    if (name.isEmpty()) {
        loge("Empty String!");
        warning += tr("Thiếu tên");
        ret = ErrFailed;
    }
    if (nameid.isEmpty()) {
        loge("Empty nameid String!");
        warning += tr("Thiếu tên định danh");
        ret = ErrFailed;
    }

    QString dateStr = ui->txtFeastday->text().simplified();
    qint64 feastday = 0;
    if (!dateStr.isEmpty()) {
        bool isOk = false;
        feastday = DatetimeUtils::dateFromString(dateStr, DATE_FORMAT_DM, &isOk);
        logd("feastday %s -> %d", dateStr.toStdString().c_str(), feastday);
        if (!isOk || (feastday <= 0)) {
            warning += tr("Sai ngày bổn mạng (định dạng ngày/tháng");
            logd("Invalid feastday %s", dateStr.toStdString().c_str());
            ret = ErrFailed;
        }
    } else {
        dbgd("No feastday");
    }

    if (ret == ErrNone){
        if (mSaint == nullptr)
            mSaint = new Saint();
        mSaint->setName(name);
        mSaint->setNameId(nameid);
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
        logi("Save Saint to db: '%s'", MODELSTR2CHA(mSaint));
        ret = mSaint->save();
        logd("save saint ret=%d", ret);
    }
    if (ret == ErrNone){
        logd("all well, accept");
        QDialog::accept();
    } else {
        MAINWIN->showErrorBox(warning, ret);
    }

}

Saint *DlgSaint::saint() const
{
    return mSaint;
}

void DlgSaint::setName(const QString &name)
{
    ui->txtName->setText(name);
}


void DlgSaint::on_txtName_textChanged(const QString &arg1)
{
    if (!mCustomNameId) {
        bool ok = false;
        QString nameid = Utils::UidFromName(arg1, NO_VN_MARK_UPPER, &ok);
        if (ok) {
            ui->txtNameId->setText(nameid);
        } else {
            ui->txtNameId->setText("");
        }
    }
}


void DlgSaint::on_btnChangeNameId_clicked()
{
    tracein;
    QString txt = ui->txtNameId->text().trimmed();
    bool ok = false;
    QString nameId = DialogUtils::showInputDialog(this,
                                                  tr("Định danh"),
                                                  tr("Nhập mã định danh"), txt, &ok);
    if (ok && !nameId.isEmpty()) {
        mCustomNameId = true;
        ui->txtNameId->setText(nameId);
        logd("custom name id '%s'", STR2CHA(nameId));
    } else {
        logd("no name id (ok=%d) or name id is empty", ok);
    }
    traceout;
}

