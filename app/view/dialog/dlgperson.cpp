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
#include "personctl.h"
#include "person.h"
#include "utils.h"
#include "saintctl.h"
#include "statusctl.h"

#include <QRegularExpression>
#include <QValidator>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPixmap>

#include <QRegularExpressionMatch>
#include <QInputDialog>
#include <QLineEdit>
#include "view/widget/uimulticomboxview.h"
#include "view/dialog/dlgaddcommunityhistory.h"
#include "view/dialog/dlghtmlviewer.h"
#include "view/dialog/dlgsaint.h"
#include "view/dialog/dlgcountry.h"
#include "view/dialog/dlgethnic.h"
#include "view/dialog/dlgcourse.h"
#include "view/dialog/dlgprovince.h"
#include "view/dialog/dlgaddpersonevent.h"

#include "community.h"

#include "eductl.h"
#include "specialistctl.h"
#include "countryctl.h"
#include "provincectl.h"
#include "ethnicctl.h"
#include "communityctl.h"
#include "exportfactory.h"
#include "importfactory.h"
#include "education.h"
#include "course.h"
#include "coursectl.h"
#include "work.h"
#include "workctl.h"
#include "status.h"
#include "statusctl.h"
#include "personevent.h"
#include "areactl.h"
#include "area.h"
#include "departctl.h"
#include "department.h"

#define SPLIT_EMAIL_PHONE ";"

#define SET_VAL_FROM_WIDGET(widget,func) \
                do { \
                    QString val = widget->text().trimmed();\
                    func(val);\
                } while (0)


#define SET_DATE_VAL_FROM_WIDGET(widget,func) \
do { \
    QString val = widget->text().trimmed();\
    func(0);\
    if (!val.isEmpty()){ \
        bool isOk = false;\
        qint64 date = Utils::dateFromString(val, DATE_FORMAT_YMD, &isOk);\
        if (isOk && date > 0){\
            func(date);\
        }\
    } \
} while (0)


#define SET_VAL_FROM_CBOX(widget,func, functxt) \
do { \
        QString currtxt = widget->currentText().trimmed();\
        if (!currtxt.isEmpty()){ \
            int index = widget->findText(currtxt);\
            logd("item %s, index %d", currtxt.toStdString().c_str(), index);\
            if (index >= 0){ \
                QVariant value = widget->itemData(index);\
                if (!value.isNull()) {\
                    func(value.toString());\
                    functxt(currtxt);\
                }\
            }\
        }\
} while (0)


const char* const KUiMultiComboxNameSaint = "saint";
const char* const KUiMultiComboxNameSpecialist = "specialist";
const char* const KUidNone = "";

DlgPerson::DlgPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPerson),
    mPerson(nullptr),
    cbSaints(nullptr),
    cbSpecialist(nullptr),
    mEditMode(Mode::NEW)
{
    traced;
    setupUI();
}

DlgPerson::~DlgPerson()
{
    if (mPerson)
        delete mPerson;
    if (cbSaints)
        delete cbSaints;
    if (cbSpecialist)
        delete cbSpecialist;

    delete ui;
}

void DlgPerson::on_btnImport_clicked()
{
    traced;
    ErrCode ret = ErrNone;
    Person* per = nullptr;
    QString fname = QFileDialog::getOpenFileName(
                                this,
                                tr("Open file"),
                                FileCtl::getAppDataDir(),
                                tr("CSV Files (*.csv);;Excel (*.xlsx)"));
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        per = person(true);
        ret = ImportFactory::importFrom(per, fname, IMPORT_CSV);
    } else {
        logd("Nothing be selected");
        ret = ErrCancelled;
    }
    if (ret == ErrNone){
        fromPerson(per);
    } // TODO: if error, should popup message or just print error log???
}

void DlgPerson::setupUI()
{
    traced;
    /*
     * SET UP UI
     */
    ui->setupUi(this);

    DIALOG_SIZE_SHOW(this);

    ui->txtCode->setText(Config::getNextPersonalCode());


    QStringList communityListHdr;
    // TODO: translation
    communityListHdr.append("Id");
    communityListHdr.append("Date");
    communityListHdr.append("End Date");
    communityListHdr.append("Event");
    communityListHdr.append("Title");
    communityListHdr.append("Remark");
    ui->tblEvents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblEvents->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblEvents->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    ui->tblEvents->setShowGrid(true);
    ui->tblEvents->setColumnCount(communityListHdr.count());
//    ui->tblCommunityList->setMinimumWidth(500);
    ui->tblEvents->setMinimumHeight(200);

    ui->tblEvents->setHorizontalHeaderLabels(communityListHdr);

    // Call connect here may cause call is called twice, it's because
    // Qt check func name, if it's in format on_buttonBox_clicked --> auto register slot
    //    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
    //            this, SLOT(on_buttonBox_clicked(QAbstractButton*)));

    /*
     * LOAD DATA
     */

    loadStatus();

    // Education
    loadEdu();

    // Saints
    loadSaints();

    // specialist
    loadSpecialist();

    // Country
    loadCountry();

    logd("Load ethic");
    loadEthnic();

    // State/Province
    logd("Load state/province");
    loadProvince();

    // Community
    loadCommunity();

    // Course
    loadCourse();

    // work
    loadWork();

    // load Event
    loadEvent(true);

    loadArea();
    loadDepartment();

}

Person *DlgPerson::buildPerson()
{
    traced;
    // TODO: backup data to restore later??
    Person* per = person();
    // Image
    per->setImgPath(ui->lblImgPath->text().trimmed());
    // person code
    per->setPersonCode(ui->txtCode->text().trimmed());

    // holly uid
    QList<QVariant> saints = cbSaints->valueItems();
    per->clearSaintUid();
    foreach (QVariant id, saints){
        per->addSaintUid(id.toString());
    }

    // holly name
    per->setHollyName(ui->txtSaint->text().trimmed());
    SET_DATE_VAL_FROM_WIDGET(ui->txtFeastDay, per->setFeastDay);

    // set name from full name
    per->setNameFromFullName(ui->txtName->text().trimmed());

    // birthday
    SET_DATE_VAL_FROM_WIDGET(ui->txtBirthday, per->setBirthday);

    // birthplace
    per->setBirthPlace(ui->txtBirthplace->text().trimmed());

    // nationality
    SET_VAL_FROM_CBOX(ui->cbCountry, per->setNationalityUid, per->setNationalityName);

    // ethnic name
    SET_VAL_FROM_CBOX(ui->cbEthic, per->setEthnicUid, per->setEthnicName);

    // id card
    per->setIdCard(ui->txtIDCard->text().trimmed());

    // id card issue date
    SET_DATE_VAL_FROM_WIDGET(ui->txtIdCardDate, per->setIdCardIssueDate);


    // edu name
    SET_VAL_FROM_CBOX(ui->cbEdu, per->setEduUid, per->setEduName);

    //specialist
    QList<QVariant> specialist = cbSpecialist->valueItems();
    per->clearSpecialistUid();
    foreach (QVariant id, specialist){
        per->addSpecialistUid(id.toString());
    }

    SET_VAL_FROM_CBOX(ui->cbCourse, per->setCourseUid, per->setCourse);
    SET_VAL_FROM_CBOX(ui->cbCountry, per->setCountryUid, per->setCountryName);

    SET_VAL_FROM_CBOX(ui->cbProvince, per->setProvinceUid, per->setProvinceName);
    per->setAddr(ui->txtAddr->toPlainText().trimmed());
    per->setChurchAddr(ui->txtChurch->toPlainText().trimmed());
    per->setEmail(ui->txtEmail->text().split(SPLIT_EMAIL_PHONE));
    per->setTel(ui->txtPhone->text().split(SPLIT_EMAIL_PHONE));
    per->setOtherContact(ui->txtOtherContact->toPlainText().trimmed());

    SET_VAL_FROM_CBOX(ui->cbArea, per->setAreaUid, per->setAreaName);
    SET_VAL_FROM_CBOX(ui->cbDepart, per->setDepartUid, per->setDepartName);
    SET_VAL_FROM_CBOX(ui->cbCommunity, per->setCommunityUid, per->setCommunityName);


    per->setDadName(ui->txtDad->text().trimmed());
    SET_DATE_VAL_FROM_WIDGET(ui->txtDadBirth, per->setDadBirthday);
    per->setDadAddr(ui->txtDadAddr->text().trimmed());

    per->setMomName(ui->txtMom->text().trimmed());
    SET_DATE_VAL_FROM_WIDGET(ui->txtMonBirth, per->setMomBirthday);
    per->setDadAddr(ui->txtMonAddr->text().trimmed());

    per->setFamilyHistory(ui->txtFamilyHistory->toPlainText().trimmed());
    per->setFamilyContact(ui->txtFamilyContact->toPlainText().trimmed() );

    SET_DATE_VAL_FROM_WIDGET(ui->txtChristenDate, per->setChristenDate);
    per->setChristenPlace(ui->txtChristenPlace->text().trimmed());


    SET_DATE_VAL_FROM_WIDGET(ui->txtEucharisDate, per->setEucharistDate);
    per->setEucharistPlace(ui->txtEucharistPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtHollyDate, per->setHollyDate);
    per->setHollyPlace(ui->txtHollyPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtJoinDate, per->setJoinDate);
    SET_VAL_FROM_CBOX(ui->cbJoinPIC, per->setJoinPICUid, per->setJoinPICName);


    SET_DATE_VAL_FROM_WIDGET(ui->txtPreTrainJoinDate, per->setPreTrainJoinDate);
    SET_VAL_FROM_CBOX(ui->cbPreTrainJoinPIC, per->setPreTrainPICUid, per->setPreTrainPICName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtTrainJoinDate, per->setTrainJoinDate);
    SET_VAL_FROM_CBOX(ui->cbTrainPIC, per->setTrainPICUid, per->setTrainPICName);


    SET_DATE_VAL_FROM_WIDGET(ui->txtVowsDate, per->setVowsDate);
    SET_VAL_FROM_CBOX(ui->cbVowsCEO, per->setVowsCEOUid, per->setVowsCEOName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtEternalVowsDate, per->setEternalVowsDate);
    SET_VAL_FROM_CBOX(ui->cbEternalVowsPIC, per->setEternalVowsPICUid, per->setEternalVowsPICName);
    SET_VAL_FROM_CBOX(ui->cbEternalVowsCEO, per->setEternalVowsCEOUid, per->setEternalVowsCEOName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtBankDate, per->setBankDate);
    per->setBankPlace(ui->txtBankPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtGoldenDate, per->setGoldenDate);
    per->setGoldenPlace(ui->txtGoldenPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtEternalDate, per->setEternalDate);
    per->setEternalPlace(ui->txtEternalPlace->text().trimmed());

    SET_VAL_FROM_CBOX(ui->cbStatus, per->setStatusUid, per->setStatusName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtRetireDate, per->setRetireDate);
    per->setRetirePlace(ui->txtRetirePlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtDeadDate, per->setDeadDate);
    per->setDeadPlace(ui->txtDeadPlace->text().trimmed());

    //event
//    QList<QVariant> specialist = ui->tblEvents->item
//    per->clearSpecialistUid();
//    foreach (QVariant id, specialist){
//        per->addSpecialistUid(id.toString());
//    }

    per->dump();
    return per;

}

ErrCode DlgPerson::fromPerson(const Person *per)
{
    traced;
    ErrCode ret = ErrNone;
    if (per == nullptr){
        ret = ErrInvalidArg; // TODO: should raise assert instead???
    }
    // TODO: check and valid data??
    if (!per->imgPath().isEmpty()) {
        // TODO: validate image path
        QPixmap p(per->imgPath());
        ui->lblImg->setPixmap(p.scaledToHeight( ui->lblImg->height()));
        ui->lblImgPath->setText(per->imgPath());
    }

    ui->txtCode->setText(per->personCode());
    ui->txtName->setText(per->getFullName());
    ui->txtBirthday->setText(Utils::date2String(per->birthday()));

    // TODO: set saint list
    // cbSaints

    ui->txtSaint->setText(per->hollyName());
    ui->txtFeastDay->setText(Utils::date2String(per->feastDay(), DATE_FORMAT_MD));
    ui->txtBirthplace->setText(per->birthPlace());
    // TODO: country, ethinic, nationality, etc.
    // cbCountry
    // cbEthic
    ui->txtIDCard->setText(per->idCard());

    ui->txtIdCardDate->setText(Utils::date2String(per->idCardIssueDate()));
    ui->txtIdCardPlace->setText(per->idCardIssuePlace());
    // cbEdu
    // cbSpecialist
    // cbCourse
    // cbCountry
    // cbProvince
    //
    ui->txtAddr->setPlainText(per->addr());
    ui->txtChurch->setPlainText(per->churchAddr());
    ui->txtEmail->setText(per->email().join(SPLIT_EMAIL_PHONE));
    ui->txtPhone->setText(per->tel().join(SPLIT_EMAIL_PHONE));
    ui->txtOtherContact->setPlainText(per->otherContact());

    ui->txtDad->setText(per->dadName());
    ui->txtDadBirth->setText(Utils::date2String(per->dadBirthday()));
    ui->txtDadAddr->setText(per->dadAddr());

    ui->txtMom->setText(per->momName());
    ui->txtMonBirth->setText(Utils::date2String(per->momBirthday()));
    ui->txtMonAddr->setText(per->momAddr());

    ui->txtFamilyHistory->setPlainText(per->familyHistory());
    ui->txtFamilyContact->setPlainText(per->familyContact());

    ui->txtChristenDate->setText(Utils::date2String(per->christenDate()));
    ui->txtChristenPlace->setText(per->christenPlace());


    ui->txtEucharisDate->setText(Utils::date2String(per->eucharistDate()));
    ui->txtEucharistPlace->setText(per->eucharistPlace());

    ui->txtHollyDate->setText(Utils::date2String(per->hollyDate()));
    ui->txtHollyPlace->setText(per->hollyPlace());

    ui->txtJoinDate->setText(Utils::date2String(per->joinDate()));
    //cbJoinPIC
    ui->txtPreTrainJoinDate->setText(Utils::date2String(per->preTrainJoinDate()));
    //cbPreTrainJoinPIC
    ui->txtTrainJoinDate->setText(Utils::date2String(per->trainJoinDate()));
    //cbTrainPIC
    ui->txtVowsDate->setText(Utils::date2String(per->vowsDate()));
    //cbVowsCEO
    ui->txtEternalVowsDate->setText(Utils::date2String(per->eternalVowsDate()));
    //cbEternalVowsPIC
    //cbEternalVowsCEO

    ui->txtBankDate->setText(Utils::date2String(per->bankDate()));
    ui->txtBankPlace->setText(per->bankPlace());

    ui->txtGoldenDate->setText(Utils::date2String(per->goldenDate()));
    ui->txtGoldenPlace->setText(per->goldenPlace());

    ui->txtEternalDate->setText(Utils::date2String(per->eternalDate()));
    ui->txtEternalPlace->setText(per->eternalPlace());

    // cbStatus

    ui->txtRetireDate->setText(Utils::date2String(per->retireDate()));
    ui->txtRetirePlace->setText(per->retirePlace());

    ui->txtDeadDate->setText(Utils::date2String(per->deadDate()));
    ui->txtDeadPlace->setText(per->deadPlace());

    tracedr(ret);
    return ret;
}

ErrCode DlgPerson::onNewItem(UIMultiComboxView *ui, const QString &value, bool silent)
{
    traced;
    ErrCode ret = ErrNone;
    logd("do silent %d", silent);
    logd("name %s", ui->name().toStdString().c_str());


    // TODO: separate processing to multi functions????
    if (ui->name() == KUiMultiComboxNameSaint) {
        if (silent) {
            // TODO: implement it
            ret = ErrNotSupport;
        } else {
            DlgSaint *dlg = new DlgSaint(this);
            dlg->setName(value.trimmed());
            dlg->setModal(true);
            if (dlg->exec() == QDialog::Accepted){
                Saint* saint = dlg->saint();
                if (saint != nullptr) {
                    ui->addItem(saint->name(), saint->uid());
                    ret = ErrNone;
                } else {
                    ret = ErrFailed;
                }
            } else {
                ret = ErrSkip;
            }
            delete dlg;
        }
    } else if (ui->name() == KUiMultiComboxNameSpecialist) {
        bool ok = false;
        QMessageBox msgBox;

        QString specialistName = QInputDialog::getText(this, tr("New Specialist"),
                                                tr("Specialist name"), QLineEdit::Normal,
                                                "", &ok);
        if (!specialistName.isEmpty()){
            Specialist* item = new Specialist();
            item->setUid(Utils::UidFromName(specialistName));
            item->setName(specialistName);
            logd("Save new specilist item %s", specialistName.toStdString().c_str());
            ret = item->save();
            logd("save ret=%d", ret);
            if (ret == ErrNone){
                ui->addItem(item->name(), item->uid());
            }
            delete item;
        } else {
            ret = ErrInvalidData;
            loge("invalid specialist name");
        }

    }

    return ret;
}

void DlgPerson::onItemAdded(UIMultiComboxView *cb, const QString &name, const QVariant &value)
{
    traced;
    logd("name %s", cb->name().toStdString().c_str());


    // TODO: separate processing to multi functions????
    if (cb->name() == KUiMultiComboxNameSaint) {
        QHash<QString, QVariant> items = cb->items();
        QString str;
        foreach (QString value, items.keys()) {
            if (!str.isEmpty()) str.append(" ");
            str.append(value);
        }
        ui->txtSaint->setText(str);
    }


}

void DlgPerson::onItemDeleted(UIMultiComboxView *cb, const QString &name, const QVariant &value)
{
    traced;
    logd("name %s", cb->name().toStdString().c_str());


    // TODO: separate processing to multi functions????
    if (cb->name() == KUiMultiComboxNameSaint) {
        QHash<QString, QVariant> items = cb->items();
        QString str;
        foreach (QString value, items.keys()) {
            if (!str.isEmpty()) str.append(" ");
            str.append(value);
        }
        ui->txtSaint->setText(str);
    }
}

void DlgPerson::onClearAll()
{
    traced;
}

void DlgPerson::loadEdu()
{
    traced;
    logd("Load Education");
    QList<Education*> list = EduCtl::getInstance()->getListEdu();
    ui->cbEdu->clear();
    foreach(Education* edu, list){

        ui->cbEdu->addItem(edu->name());
    }
}

void DlgPerson::loadSaints()
{
    traced;
    logd("Load Saint");
    if (cbSaints == nullptr) {

        cbSaints = new UIMultiComboxView(KUiMultiComboxNameSaint, ui->wgSaint);
        cbSaints->setListener(this);

        ui->wgSaint->layout()->addWidget(cbSaints);
    }
    cbSaints->clearAll();
    QList<Saint*> saints = SaintCtl::getInstance()->getListSaints();

    foreach (Saint* saint, saints) {
        logd(">> Saint %s", saint->name().toStdString().c_str());
        cbSaints->addItem(saint->name(), saint->uid());
    }

}

void DlgPerson::loadSpecialist()
{
    traced;
    logd("Load specialist");
    if (cbSpecialist == nullptr){
        cbSpecialist = new UIMultiComboxView(KUiMultiComboxNameSpecialist, ui->wgSaint);
        cbSpecialist->setListener(this);
        ui->wgSpecialist->layout()->addWidget(cbSpecialist);
    }
    cbSpecialist->clearAll();
    QList<Specialist*> specialists = SpecialistCtl::getInstance()->getAll();
    foreach (Specialist* specialist, specialists) {
        //        logd(">> specialist %s", name.toStdString().c_str());
        cbSpecialist->addItem(specialist->name(), specialist->uid());
    }

}


void DlgPerson::loadEthnic()
{
    traced;

    // Someone may have US nationality, but Ethic is Kinh, as their original is VN
    logd("Reload course");
    ui->cbCourse->clear();
    QList<Course*> listCourse = INSTANCE(CourseCtl)->getCourseList();
    foreach(Course* item, listCourse){
        ui->cbCourse->addItem(item->name(), item->uid());
    }
}


void DlgPerson::loadCountry()
{
    traced;

    logd("Load country");
    QList<Country*> listCountry = CountryCtl::getInstance()->getCountryList();
    ui->cbNationality->clear();
    ui->cbCountry->clear();
    foreach(Country* item, listCountry){

        ui->cbNationality->addItem(item->name(), item->uid());
        ui->cbCountry->addItem(item->name(), item->uid());
    }
}

void DlgPerson::loadProvince()
{
    traced;
    QString countryUid = Utils::getCurrentComboxDataString(ui->cbCountry);
    ui->cbProvince->clear();
    if (!countryUid.isEmpty()){
        const QList<Province*>* listProvince = PROVINCE->getProvinceList(countryUid);
        if (listProvince != nullptr){
            foreach(Province* item, *listProvince){
                ui->cbProvince->addItem(item->name(), item->uid());
            }
        }
    }
}

void DlgPerson::loadWork()
{
    traced;
    ui->cbWork->clear();
    QList<Work*> list = INSTANCE(WorkCtl)->getWorkList();
    ui->cbWork->addItem(tr("Không xác đinh"), KUidNone);
    foreach(Work* item, list){
        ui->cbWork->addItem(item->name(), item->uid());
    }

}

void DlgPerson::loadEvent(bool reloadAll)
{
    traced;
    QTableWidget* tbl = ui->tblEvents;
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    logd("reloadAll %d", reloadAll);
    if (reloadAll) {
        cleanEvent();
        Person* per = person();
        if (per != nullptr) {
            QList<DbModel*> list = INSTANCE(PersonCtl)->getListEvent(per);
            foreach(DbModel* item, list){
                mListPersonEvent.append(new PersonEvent((PersonEvent*)item));
            }
        }
    }

    qint32 col = 0;
    qint32 idx = tbl->rowCount();
    logd("idx=%d", idx);
    logd("mListPersonEvent cnt=%d", mListPersonEvent.count());
    foreach (PersonEvent* event, mListPersonEvent) {
        col = 0;
        logd("idx=%d", idx);
        tbl->insertRow(idx);
        tbl->setItem(idx, col++, new QTableWidgetItem(event->uid()));
        tbl->setItem(idx, col++, new QTableWidgetItem(Utils::date2String(event->date())));
        tbl->setItem(idx, col++, new QTableWidgetItem(Utils::date2String(event->endDate())));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->eventName()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->name()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->remark()));
        idx++;
    }

}

void DlgPerson::cleanEvent()
{
    traced;
    if (!mListPersonEvent.empty()) {
        foreach(PersonEvent* item, mListPersonEvent) {
            delete item; // TODO: is it safe????, should check if iterator is supported
        }
        mListPersonEvent.clear();
    }
}

void DlgPerson::loadCommunity()
{
    traced;
    logd("Load community");
    QList<Community*> listCommunity = COMMUNITYCTL->getCommunityList();
    ui->cbCommunity->clear();
    ui->cbCommunity->addItem(tr("Không xác đinh"), KUidNone);
    foreach(Community* item, listCommunity){
        ui->cbCommunity->addItem(item->name(), item->uid());
    }

}

void DlgPerson::loadStatus()
{
    traced;
    ui->cbStatus->clear();
    QList<Status*> listItems = INSTANCE(StatusCtl)->getStatusList();
    foreach(Status* item, listItems){
        ui->cbStatus->addItem(item->name(), item->uid());
    }

}

void DlgPerson::loadArea()
{
    traced;
    ui->cbArea->clear();
    QList<Area*> listItems = INSTANCE(AreaCtl)->getAreaList();
    ui->cbArea->addItem(tr("Không xác đinh"), KUidNone);
    foreach(Area* item, listItems){
        ui->cbArea->addItem(item->name(), item->uid());
    }
}

void DlgPerson::loadDepartment()
{
    traced;
    ui->cbDepart->clear();
    QList<Department*> listItems = INSTANCE(DepartCtl)->getDeptList();
    ui->cbDepart->addItem(tr("Không xác đinh"), KUidNone);
    foreach(Department* item, listItems){
        ui->cbDepart->addItem(item->name(), item->uid());
    }
}

void DlgPerson::loadCourse()
{
    traced;
    ui->cbCourse->clear();
    QList<Course*> listCourse = INSTANCE(CourseCtl)->getCourseList();
    foreach(Course* item, listCourse){
        ui->cbCourse->addItem(item->name(), item->uid());
    }

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

    DlgCountry* dlg = new DlgCountry();
    if (dlg == nullptr) {
        loge("Open dlg country fail, No memory");
        return;
    }


    if (dlg->exec() == QDialog::Accepted){
        loadCountry();

    }
    delete dlg;
}


void DlgPerson::on_cbCountry_currentIndexChanged(int index)
{
    traced;
    logd("Reload provine of %s", ui->cbCountry->currentText().toStdString().c_str());
    logd("index %d", index);

    loadProvince();
//    if (index >= 0){
//        QString shortName = ui->cbCountry->itemData(index).toString();
//        const QList<Province*>* listProvince = PROVINCE->getProvinceList(shortName);
//        ui->cbProvince->clear();
//        if (listProvince != nullptr){
//            foreach(Province* item, *listProvince){
//                ui->cbProvince->addItem(item->name(), item->uid());
//            }
//        }
//    }

}


void DlgPerson::on_btnAddCommunityHistory_clicked()
{
//    DlgAddCommunityHistory w;
//    //    w.setWindowState(Qt::WindowState::WindowMaximized);
//    w.setModal(true);
//    if (w.exec() == QDialog::Accepted){
//        QTableWidget* tbl = ui->tblCommunityList;
//        qint64 date = w.getDate();
//        int idx = tbl->rowCount();
//        QString name;
//        const Community* comm = w.getCommunity();
//        if (comm != nullptr){
//            name = comm->name();
//        }
//        logd("idx %d", idx);
//        if (date > 0 && !name.isEmpty()){

//            logd("Date %d", date);
//            logd("Name %s", name.toStdString().c_str());
//            tbl->insertRow(idx);
//            qint32 col = 0;
//            tbl->setItem(idx, col++, new QTableWidgetItem(QString("%1").arg(comm->dbId())));
//            tbl->setItem(idx, col++, new QTableWidgetItem(Utils::date2String(date)));
//            tbl->setItem(idx, col++, new QTableWidgetItem(name));
//            tbl->setItem(idx, col++, new QTableWidgetItem(w.getRemark()));

//            //        logd("Date %d", w.getDate());
//        }
//    }
}


void DlgPerson::on_tblClearCommunity_clicked()
{
    traced;
//    ui->tblCommunityList->clearContents();
//    ui->tblCommunityList->model()->removeRows(0, ui->tblCommunityList->rowCount());
}


void DlgPerson::on_btnPreview_clicked()
{
    traced;
    Person* per = buildPerson();
    QString fpath;
    INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
    if (QFile::exists(fpath)){
        dlgHtmlViewer* viewer = new dlgHtmlViewer();
        viewer->setHtmlPath(fpath);
        viewer->setSubject("Person");
        viewer->exec();
    }
}

Person *DlgPerson::person(bool newone)
{
    if (mPerson == nullptr) {
        mPerson = new Person();
    } else if (newone) {
        delete mPerson;
        mPerson = new Person();
    }

    return mPerson;
}


void DlgPerson::on_btnImg_clicked()
{
    traced;
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("PNG Files (*.png);;JPEG (*.jpg *.jpeg)"));

    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QPixmap p(fname);
        ui->lblImg->setPixmap(p.scaledToHeight( ui->lblImg->height()));
        ui->lblImgPath->setText(fname);
    }
}


void DlgPerson::on_btnClearImage_clicked()
{
    traced;
    ui->lblImg->clear();
    ui->lblImgPath->clear();
    ui->lblImg->setText("<Picture>");
}


void DlgPerson::on_btnAddCountry_clicked()
{
    traced;
    DlgCountry * dlg = new DlgCountry();
    if (dlg == nullptr) {
        loge("Open dlg country fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        loadCountry();
    }
    delete dlg;
}


void DlgPerson::on_btnAddEthnic_clicked()
{
    traced;

    DlgEthnic * dlg = new DlgEthnic();
    if (dlg == nullptr) {
        loge("Open dlg ethnic fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        loadEthnic();
    }
    delete dlg;
}


void DlgPerson::on_btnAddEdu_clicked()
{
    traced;
    ErrCode ret = ErrNone;
    bool ok = false;
    QMessageBox msgBox;

    QString eduName = QInputDialog::getText(this, tr("New Education"),
                                         tr("Education name"), QLineEdit::Normal,
                                        "", &ok);
    if (!eduName.isEmpty()){
        Education* edu = new Education();
        edu->setUid(Utils::UidFromName(eduName));
        edu->setName(eduName);
        ret = edu->save();
        delete edu;
    }

    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (ret == ErrNone){
        msgBox.setText(QString("Education '%1' was saved SUCCESSFUL.").arg(eduName));
        loadEdu();

    } else {
        msgBox.setText(QString("FAILED to save edu '%1', err %2")
                           .arg(eduName).arg(ret));
    }
    logd("ret=%d", ret);
    msgBox.exec();

}


void DlgPerson::on_btnAddProvince_clicked()
{
    traced;
    DlgProvince * dlg = new DlgProvince();
    if (dlg == nullptr) {
        loge("Open dlg province fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        loadProvince();

    }
    delete dlg;
}


void DlgPerson::on_btnAddCommunity_clicked()
{
    traced;
}


void DlgPerson::on_btnAddCourse_clicked()
{
    traced;

    DlgCourse * dlg = new DlgCourse();
    if (dlg == nullptr) {
        loge("Open dlg course fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        loadCourse();

    }
    delete dlg;
}



void DlgPerson::on_btnAddWork_clicked()
{
    traced;
    ErrCode ret = ErrNone;
    bool ok = false;
    QMessageBox msgBox;

    QString workName = QInputDialog::getText(this, tr("New Work"),
                                             tr("Work name"), QLineEdit::Normal,
                                             "", &ok);
    if (!workName.isEmpty()){
        Work* work = new Work();
        work->setUid(Utils::UidFromName(workName, UidNameConvertType::NO_VN_MARK_UPPER));
        work->setName(workName);
        ret = work->save();
        delete work;
    }

    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (ret == ErrNone){
        msgBox.setText(QString("Work '%1' was saved SUCCESSFUL.").arg(workName));
        loadWork();

    } else {
        msgBox.setText(QString("FAILED to save edu '%1', err %2")
                           .arg(workName).arg(ret));
    }
    logd("ret=%d", ret);
    msgBox.exec();
}

DlgPerson::Mode DlgPerson::editMode() const
{
    return mEditMode;
}

void DlgPerson::setEditMode(DlgPerson::Mode newEditMode)
{
    mEditMode = newEditMode;
}


void DlgPerson::on_btnAddEvent_clicked()
{
    traced;
    DlgAddPersonEvent * dlg = new DlgAddPersonEvent();
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        PersonEvent* event = dlg->personEvent();
        if (event != nullptr) {
            mListPersonEvent.append(new PersonEvent(event));
        }
        loadEvent();
    }
    delete dlg;
}


void DlgPerson::on_btnDelEvent_clicked()
{
    traced;
    // TODO: validate data
    QTableWidget* tbl = ui->tblEvents;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    // Multiple rows can be selected
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        mListPersonEvent.removeAt(index.row());
    }
    loadEvent();
}


void DlgPerson::on_btnAddDepart_clicked()
{
    traced;
    // TODO: implement add department
}


void DlgPerson::on_btnAddArea_clicked()
{
    traced;
    // TODO: implement add area

}

