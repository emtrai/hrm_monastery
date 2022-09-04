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
 * Filename: dlgaddcommunityhistory.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/16/2022
 * Brief:
 */
#include "dlgaddcommunityhistory.h"
#include "ui_dlgaddcommunityhistory.h"
#include "logger.h"
#include "communityctl.h"
#include <QList>
#include "community.h"
#include "utils.h"
#include <QPushButton>

DlgAddCommunityHistory::DlgAddCommunityHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddCommunityHistory)
{
    ui->setupUi(this);
    mListCommunity = COMMUNITYCTL->getCommunityList();
    if (!mListCommunity.empty()){
        foreach(Community* item, mListCommunity){
            ui->cbCommunity->addItem(item->name(), item->dbId());
        }
    }
    ui->lblWarning->hide();

}

DlgAddCommunityHistory::~DlgAddCommunityHistory()
{
//    foreach (Community* item, mListCommunity) {
//        delete item;
//    }
//    mListCommunity.clear();
    delete ui;
}

const Community *DlgAddCommunityHistory::getCommunity()
{
    Community* com = nullptr;
    traced;
    logd("Reload provine of %s", ui->cbCommunity->currentText().toStdString().c_str());
    int index = ui->cbCommunity->findText(ui->cbCommunity->currentText());
    logd("index %d", index);
    if (index >= 0){
        // TODO: index > array, index of combo box match with index of list???
        com = mListCommunity.at(index);
    }
    return com;
}

qint64 DlgAddCommunityHistory::getDate()
{
    return Utils::dateFromString(ui->txtDate->text());
}

QString DlgAddCommunityHistory::getRemark()
{
    return ui->txtRemark->toPlainText().trimmed();
}

void DlgAddCommunityHistory::showEvent(QShowEvent *event)
{
    traced;
    QDialog::showEvent(event);
//    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);

}

void DlgAddCommunityHistory::accept()
{
    traced;
    QString warning;
    if (getDate() == 0) {
        logd("Invalid date, show warning");
        warning += "Invalid date";
    }
    if (getCommunity() == nullptr){
        if (!warning.isEmpty()) warning += ", ";
        warning += "Invalid community";
    }
    if (!warning.isEmpty()){
        ui->lblWarning->setText(warning);
        ui->lblWarning->show();
    } else {
        QDialog::accept();
    }
}

//void DlgAddCommunityHistory::on_buttonBox_clicked(QAbstractButton *button)
//{
//    traced;

//    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton( button );
//    logd("btn: %d", btn);
//    if (btn == QDialogButtonBox::StandardButton::Ok) {
//        if (getDate() == 0) {
//            logd("Invalid date, show warning");
//            ui->lblWarning->setText("Invalid date");
//            ui->lblWarning->show();
//        } else {
//            emit QDialog::accepted();
//        }
//    }
//}