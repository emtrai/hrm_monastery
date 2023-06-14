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
 * Filename: dlgconfirmupdatepeoplecomm.cpp
 * Author: Anh, Ngo Huy
 * Created date:6/13/2023
 * Brief:
 */
#include "dlgconfirmupdatepeoplecomm.h"
#include "ui_dlgconfirmupdatepeoplecomm.h"
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QDialog>
#include "logger.h"
#include "errcode.h"
#include "stringdefs.h"
#include "utils.h"
#include "person.h"
#include "community.h"

DlgConfirmUpdatePeopleComm::DlgConfirmUpdatePeopleComm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfirmUpdatePeopleComm),
    mCommunity(nullptr)
{
    tracein;
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_CANCEL);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(STR_OK);
    ui->buttonBox->button(QDialogButtonBox::Help)->setText(STR_HELP);
    traceout;
}

DlgConfirmUpdatePeopleComm::~DlgConfirmUpdatePeopleComm()
{
    tracein;
    delete ui;
    FREE_PTR(mCommunity);
    RELEASE_LIST_DBMODEL(mPersonList);
    traceout;
}

void DlgConfirmUpdatePeopleComm::on_buttonBox_clicked(QAbstractButton *button)
{
    tracein;
    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton( button );
    logd("button 0x%x", btn);
    switch (btn) {
    case QDialogButtonBox::StandardButton::Ok:
        QDialog::accept();
        break;
    case QDialogButtonBox::StandardButton::Help:
        // TODO: implement this
        Utils::showDlgUnderDev(tr("Giúp đỡ phần xác nhận cập nhật cộng đoàn"));
        QDialog::close();
        break;
    case QDialogButtonBox::StandardButton::Cancel:
        QDialog::close();
        break;
    default:
        loge("Unknown button 0x%x", btn);
        QDialog::close();
        break;
    }
    traceout;
}

const QList<DbModel *> DlgConfirmUpdatePeopleComm::personList() const
{
    return mPersonList;
}

void DlgConfirmUpdatePeopleComm::setPersonList(const QList<DbModel *> &newPersonList)
{
    tracein;
    RELEASE_LIST_DBMODEL(mPersonList);
    if (newPersonList.size() > 0) {
        mPersonList = CLONE_LIST_DBMODEL(newPersonList);
        ui->txtPeopleList->clear();
        if (mPersonList.size() > 0) {
            foreach(DbModel* item, newPersonList) {
                Person* per = (Person*) item;
                if (per) {
                    ui->txtPeopleList->appendPlainText(per->displayName());
                } else {
                    loge("Something went wrong, list has null item");
                }
            }
        } else {
            loge("No person list, out-of-memory?");
        }
    }
    traceout;
}

const Community *DlgConfirmUpdatePeopleComm::community() const
{
    return mCommunity;
}

void DlgConfirmUpdatePeopleComm::setCommunity(const Community *newCommunity)
{
    tracein;
    FREE_PTR(mCommunity);
    if (newCommunity) {
        mCommunity = CLONE_MODEL(newCommunity, Community);
        if (mCommunity) {
            ui->lblTitle->setText(
                QString(tr("Bạn có muốn đổi thành cộng đoàn '%1' cho các Nữ tu?"))
                    .arg(mCommunity->name()));
        } else {
            Utils::showErrorBox(tr("Lỗi hết bộ nhớ"));
            loge("No memory to allocat community");
        }
    }
    traceout;
}

bool DlgConfirmUpdatePeopleComm::addCommunityHistory()
{
    return ui->chkCommunityHistory->isChecked();
}

bool DlgConfirmUpdatePeopleComm::addPersonEvent()
{
    return ui->chkPersonEvent->isChecked();
}
