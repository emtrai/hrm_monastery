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
 * Filename: dlgperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/26/2022
 * Brief:
 */
#include "dlgperson.h"
#include "ui_dlgperson.h"
#include "config.h"
#include "logger.h"
#include <QFileDialog>
#include "filectl.h"
#include "person.h"
#include "utils.h"
#include "saintctl.h"

#include <QRegularExpression>
#include <QValidator>
#include <QMessageBox>
#include <QDialogButtonBox>

#include <QRegularExpressionMatch>
#include "view/widget/uimulticomboxview.h"

#include "community.h"

#include "eductl.h"
#include "specialistctl.h"
#include "countryctl.h"
#include "provincectl.h"
#include "ethnicctl.h"
#include "communityctl.h"

DlgPerson::DlgPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPerson)
{
    setupUI();


}

DlgPerson::~DlgPerson()
{
    delete ui;
}

void DlgPerson::on_btnImport_clicked()
{
    traced;
    QString fname = QFileDialog::getOpenFileName(
                                this,
                                tr("Open file"),
                                FileCtl::getAppDataDir(),
                                tr("CSV Files (*.csv);;Excel (*.xlsx)"));
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        Person* person = new Person();
        ErrCode ret = person->fromCSVFile(fname);
        if (ret == ErrNone){
            logd("Parse ok, fill dialog");
            person->dump();
            if (!person->personCode().isEmpty())
                ui->txtCode->setText(person->personCode());

            if (!person->getFullName().isEmpty())
                ui->txtName->setText(person->getFullName());

            if (person->christenDate() > 0){
                QString date = Utils::date2String(person->christenDate());
                if (!date.isEmpty()){
                    ui->txtChristenDate->setText(date);
                }
            }
        }
        delete person;
    }
    else {
        logd("Nothing be selected");
    }
}

void DlgPerson::setupUI()
{
    traced;
    ui->setupUi(this);
    this->setWindowState(Qt::WindowState::WindowMaximized);

    ui->txtCode->setText(Config::getNextPersonalCode());

    // Education

    logd("Load Education");
    QList<Education*> list = EduCtl::getInstance()->getListEdu();
    foreach(Education* edu, list){

        ui->cbEdu->addItem(edu->name());
    }

    // Saints

    logd("Load Saint");
    UIMultiComboxView *cbSaints = new UIMultiComboxView();
    QList<Saint*> saints = SaintCtl::getInstance()->getListSaints();
    foreach (Saint* saint, saints) {
        logd(">> Saint %s", saint->name().toStdString().c_str());
         cbSaints->addItem(saint->name(), saint->dbId());
    }

    ui->wgSaint->layout()->addWidget(cbSaints);

    // specialist

    logd("Load specialist");
    UIMultiComboxView *cbSpecialist = new UIMultiComboxView();
    QList<Specialist*> specialists = SpecialistCtl::getInstance()->getAll();
    foreach (Specialist* specialist, specialists) {
//        logd(">> specialist %s", name.toStdString().c_str());
        cbSpecialist->addItem(specialist->name(), specialist->dbId());
    }
    ui->wgSpecialist->layout()->addWidget(cbSpecialist);


    // Country
    logd("Load country");
    QList<Country*> listCountry = CountryCtl::getInstance()->getCountryList();
    foreach(Country* item, listCountry){

        ui->cbNationality->addItem(item->name(), item->shortName());
        ui->cbCountry->addItem(item->name(), item->shortName());
    }

    logd("Load ethic");
    // Someone may have US nationality, but Ethic is Kinh, as their original is VN
    const QHash<QString, QList<Ethnic*>*> listEthnics = ETHNIC->getEthnicList();
    foreach(QString country, listEthnics.keys()){
        foreach(Ethnic * ethnic, *(listEthnics.value(country)))
            ui->cbEthic->addItem(QString("%1,%2").arg(
                                     ethnic->countryShortName(), ethnic->name()),
                             ethnic->uid());
    }

    // State/Province
    logd("Load state/province");
    const QList<Province*>* listProvince = PROVINCE->getProvinceList(ui->cbCountry->currentText());
    if (listProvince != nullptr){
        foreach(Province* item, *listProvince){
            ui->cbProvince->addItem(item->name(), item->uid());
        }
    }

    // Community
    logd("Load community");
    QList<Community*> listCommunity = COMMUNITYCTL->getCommunityList();
    foreach(Community* item, listCommunity){
        ui->cbCommunity->addItem(item->name(), item->uid());
    }
    // Call connect here may cause call is called twice, it's because
    // Qt check func name, if it's in format on_buttonBox_clicked --> auto register slot
    //    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
    //            this, SLOT(on_buttonBox_clicked(QAbstractButton*)));
}

//void DlgPerson::accept()
//{
//    traced;
//}

void DlgPerson::on_buttonBox_clicked( QAbstractButton * button )
{
    traced;

    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton( button );
    logd("btn: %d", btn);
    if (btn == QDialogButtonBox::StandardButton::Save)
    {
        QRegularExpression re("(\\d{4}[\\/\\.\\-](0?[1-9]|1[012])[\\/\\.\\-](0?[1-9]|[12][0-9]|3[01]))|\\d{4}");

        QString birthday = ui->txtBirthday->text();
        logd("check match birthday %s", birthday.toStdString().c_str());
        QRegularExpressionMatch match = re.match(birthday);
        if (!match.hasMatch())
        {
            QMessageBox msgBox;
            msgBox.setText("Birthday not suitable");
            msgBox.setStandardButtons(QMessageBox::Cancel);
            msgBox.addButton(QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            msgBox.exec();
        }

    }
}


void DlgPerson::on_btnEditNation_clicked()
{
    traced;
}


void DlgPerson::on_cbCountry_currentIndexChanged(int index)
{
    traced;
    logd("Reload provine of %s", ui->cbCountry->currentText().toStdString().c_str());
    logd("index %d", index);
    if (index >= 0){
        QString shortName = ui->cbCountry->itemData(index).toString();
        const QList<Province*>* listProvince = PROVINCE->getProvinceList(shortName);
        ui->cbProvince->clear();
        if (listProvince != nullptr){
            foreach(Province* item, *listProvince){
                ui->cbProvince->addItem(item->name(), item->uid());
            }
        }
    }

}

