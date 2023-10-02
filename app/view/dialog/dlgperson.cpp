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
#include "datetimeutils.h"
#include "saintctl.h"
#include "personstatusctl.h"

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
#include "view/dialog/dlgsearchperson.h"

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
#include "personstatus.h"
#include "personstatusctl.h"
#include "personevent.h"
#include "areactl.h"
#include "area.h"
#include "departctl.h"
#include "department.h"

#include "stringdefs.h"
#include "image.h"
#include "imagedefs.h"
#include "dialogutils.h"
#include "modeldefs.h"

#define SPLIT_EMAIL_PHONE ";"

#define PERSON_DISPLAY_NAME_SPLIT "-"
#define PERSON_DISPLAY_NAME(per) QString("%1" PERSON_DISPLAY_NAME_SPLIT "%2").arg(per->nameId(), per->getFullName())

const char* const KUiMultiComboxNameSaint = "saint";
const char* const KUiMultiComboxNameSpecialist = "specialist";

DlgPerson::DlgPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPerson),
    mPerson(nullptr),
    cbSaints(nullptr),
    cbSpecialist(nullptr),
    mEditMode(Mode::NEW),
    mIsNew(true),
    mIsSelfSave(true),
    mInitDone(false)
{
    tracein;
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
    RELEASE_LIST_DBMODEL(mListPersonEvent);
    delete ui;
}

void DlgPerson::on_btnImport_clicked()
{
    tracein;
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
        ret = ImportFactory::importFrom(KModelHdlPerson, per, fname, IMPORT_CSV);
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
    tracein;
    /*
     * SET UP UI
     */
    ui->setupUi(this);

    DIALOG_SIZE_SHOW(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Thoát"));
    ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Lưu"));


    QStringList eventListHdr;
    // TODO: translation
    eventListHdr.append(STR_NAMEID);
    eventListHdr.append(STR_EVENT);
    eventListHdr.append(STR_TITLE);
    eventListHdr.append(STR_DATE);
    eventListHdr.append(STR_ENDDATE);
    eventListHdr.append(STR_NOTE);
    ui->tblEvents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblEvents->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblEvents->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    ui->tblEvents->setShowGrid(true);
    ui->tblEvents->setColumnCount(eventListHdr.count());
//    ui->tblCommunityList->setMinimumWidth(500);
    ui->tblEvents->setMinimumHeight(200);

    ui->tblEvents->setHorizontalHeaderLabels(eventListHdr);

    // Call connect here may cause call is called twice, it's because
    // Qt check func name, if it's in format on_buttonBox_clicked --> auto register slot
    //    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
    //            this, SLOT(on_buttonBox_clicked(QAbstractButton*)));

    /*
     * LOAD DATA
     */
    loadPersonCode();

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

#ifndef SKIP_PERSON_PROVINE
    //TODO: I supported this by history, but not completed it yet
    // main issue is mapping between province and country, causing build dialog from
    // person info be complicated. It's possible, but need time to do
    // Will do it, if have time

    // State/Province
    logd("Load state/province");
    loadProvince();
#else
    ui->cbProvince->setVisible(false);
    ui->lblProvince->setVisible(false);
//    ui->btnAddProvince->setVisible(false);
#endif // SKIP_PERSON_PROVINE

    // Community
    loadCommunity();

    // Course
    loadCourse();

    // work
    loadWork();

    // load Event
//    loadEvent();

    mInitDone = true;
}

Person *DlgPerson::getPerson()
{
    tracein;
    Person* per = person();
    buildPerson(per);
    traceout;
    return per;
}

ErrCode DlgPerson::buildPerson(Person* per)
{
    tracein;
    // TODO: backup data to restore later??
    ErrCode err = ErrNone;
    QString imgPath = ui->lblImgPath->text().trimmed();
    per->setMarkModified(true); // start marking fields which are modified
    // Image
    if (imgPath.isEmpty()) {
        per->setImgId("");
        per->markImageDelete();
    } else {
        per->setImgPath(imgPath);
        // FIXME: image is not updated in person list view after changing image
    }
    // person code
    per->setNameId(ui->txtCode->text().trimmed());

    // holly uid
    QList<QVariant> saints = cbSaints->valueItems();
    per->clearSaintUid();
    foreach (QVariant id, saints){
        per->addSaintUid(id.toString());
    }

    // holly name
    per->setHollyName(ui->txtSaint->text().trimmed());
    SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtFeastDay, per->setFeastDay, DEFAULT_FORMAT_MD);

    // set name from full name
    per->setNameFromFullName(ui->txtName->text().trimmed());

    // birthday
    SET_DATE_VAL_FROM_WIDGET(ui->txtBirthday, per->setBirthday);

    // birthplace
    per->setBirthPlace(ui->txtBirthplace->text().trimmed());

    // nationality
    logd("set nationality");
    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbNationality, per->setNationalityUid, per->setNationalityName, err);

    // ethnic name
    logd("set Ethnic");
    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbEthic, per->setEthnicUid, per->setEthnicName, err);

    // id card
    per->setIdCard(ui->txtIDCard->text().trimmed());

    // id card issue date
    SET_DATE_VAL_FROM_WIDGET(ui->txtIdCardDate, per->setIdCardIssueDate);

    per->setIdCardIssuePlace(ui->txtIdCardPlace->text().trimmed());


    // edu name
    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbEdu, per->setEduUid, per->setEduName, err);
    per->setEduDetail(ui->txtEduDetail->toPlainText().trimmed());

    //specialist
    QList<QVariant> specialist = cbSpecialist->valueItems();
    per->clearSpecialistUid();
    foreach (QVariant id, specialist){
        per->addSpecialistUid(id.toString());
    }
    per->setSpecialistInfo(ui->txtSpecialistInfo->toPlainText().trimmed());

    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbCourse, per->setCourseUid, per->setCourseName, err);
    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbCountry, per->setCountryUid, per->setCountryName, err);
#ifndef SKIP_PERSON_PROVINE
    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbProvince, per->setProvinceUid, per->setProvinceName, err);
#endif
    per->setAddr(ui->txtAddr->toPlainText().trimmed());
    per->setChurchAddr(ui->txtChurch->toPlainText().trimmed());
    per->setEmail(ui->txtEmail->text().split(SPLIT_EMAIL_PHONE));
    per->setTel(ui->txtPhone->text().split(SPLIT_EMAIL_PHONE));
    per->setOtherContact(ui->txtOtherContact->toPlainText().trimmed());

    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbCommunity, per->setCommunityUid, per->setCommunityName, err);


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

    logd("set join PIC");
//    SET_VAL_FROM_CBOX(ui->cbJoinPIC, per->setJoinPICUid, per->setJoinPICName);
    SET_VAL_FROM_TEXTBOX(ui->txtJoinPIC, KItemUid, per->setJoinPICUid, per->setJoinPICName);


    SET_DATE_VAL_FROM_WIDGET(ui->txtPreTrainJoinDate, per->setPreTrainJoinDate);
    SET_VAL_FROM_TEXTBOX(ui->txtPreTrainJoinPIC, KItemUid, per->setPreTrainPICUid, per->setPreTrainPICName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtTrainJoinDate, per->setTrainJoinDate);
    SET_VAL_FROM_TEXTBOX(ui->txtTrainPIC, KItemUid, per->setTrainPICUid, per->setTrainPICName);


    SET_DATE_VAL_FROM_WIDGET(ui->txtVowsDate, per->setVowsDate);
    SET_VAL_FROM_TEXTBOX(ui->txtVowsCEO, KItemUid, per->setVowsCEOUid, per->setVowsCEOName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtEternalVowsDate, per->setEternalVowsDate);
//    SET_VAL_FROM_TEXTBOX(ui->txtEternalVowsPIC, KItemUid, per->setEternalVowsPICUid, per->setEternalVowsPICName);
    SET_VAL_FROM_TEXTBOX(ui->txtEternalVowsCEO, KItemUid, per->setEternalVowsCEOUid, per->setEternalVowsCEOName);

    SET_DATE_VAL_FROM_WIDGET(ui->txtBankDate, per->setBankDate);
    per->setBankPlace(ui->txtBankPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtGoldenDate, per->setGoldenDate);
    per->setGoldenPlace(ui->txtGoldenPlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtEternalDate, per->setEternalDate);
    per->setEternalPlace(ui->txtEternalPlace->text().trimmed());

    if (err == ErrNone) SET_VAL_FROM_CBOX(ui->cbStatus, per->setPersonStatusUid, per->setPersonStatusName, err);

    SET_DATE_VAL_FROM_WIDGET(ui->txtRetireDate, per->setRetireDate);
    per->setRetirePlace(ui->txtRetirePlace->text().trimmed());

    SET_DATE_VAL_FROM_WIDGET(ui->txtDeadDate, per->setDeadDate);
    per->setDeadPlace(ui->txtDeadPlace->text().trimmed());

    // work
    logd("set work");
    SET_VAL_FROM_CBOX(ui->cbWork, per->setCurrentWorkUid, per->setCurrentWorkName, err);
    per->setWorkHistory(ui->txtWorkHistory->toPlainText().trimmed());

    //event
//    QList<QVariant> specialist = ui->tblEvents->item
//    per->clearSpecialistUid();
//    foreach (QVariant id, specialist){
//        per->addSpecialistUid(id.toString());
//    }
    if (mListPersonEvent.size() > 0) {
        per->setPersonEventList(mListPersonEvent);
    }
    logd("build person '%s'", STR2CHA(per->toString()));
    per->dump();
    traceout;
    return err;

}

ErrCode DlgPerson::fromPerson(const Person *model)
{
    tracein;
    ErrCode ret = ErrNone;
    QList<DbModel*> events;
    QList<DbModel*> specialistList;
    Person* per = person();

    mInitDone = false;
    if (model == nullptr) {
        loge("Invalid person info to clone");
        ret = ErrInvalidData;
        goto out;
    }
    if (per == nullptr){
        ret = ErrInvalidArg; // TODO: should raise assert instead???
        goto out;
    }
    per->clone(model);
    logd("fromPerson '%s'", MODELSTR2CHA(per));
    per->validateAllFields(); // TODO: should call validate here???
    // TODO: check and valid data??
    if (!per->imgPath().isEmpty()) {
        // TODO: validate image path
        QPixmap p(per->imgPath());
        ui->lblImg->setPixmap(p.scaledToHeight( ui->lblImg->height()));
        ui->lblImgPath->setText(per->imgPath());
    }

    ui->txtCode->setText(per->nameId());
    ui->txtName->setText(per->fullName());
    ui->txtBirthday->setText(DatetimeUtils::date2String(per->birthday()));

    // cbSaints
    // TODO: fix issue of show saint adding dialog if name is not exist in list
    foreach (QString saintName, per->hollyNameInList()) {
        logd("saintName %s", STR2CHA(saintName));
        cbSaints->addSelectedItemByName(saintName);
    }
    ui->txtSaint->setText(per->hollyName());
    // ngay bon mang
    ui->txtFeastDay->setText(DatetimeUtils::date2String(per->feastDay(), DEFAULT_FORMAT_MD));
    // birthday
    ui->txtBirthplace->setText(per->birthPlace());
    // TODO: country, ethinic, nationality, etc.
    // cbNationality
    Utils::setSelectItemComboxByData(ui->cbNationality, per->nationalityUid());
    // cbEthic
    Utils::setSelectItemComboxByData(ui->cbEthic, per->ethnicUid());

    // id card
    ui->txtIDCard->setText(per->idCard());
    ui->txtIdCardDate->setText(DatetimeUtils::date2String(per->idCardIssueDate()));
    ui->txtIdCardPlace->setText(per->idCardIssuePlace());
    // cbEdu
    Utils::setSelectItemComboxByData(ui->cbEdu, per->eduUid());

    // cbSpecialist
    specialistList = per->getSpecialistList();
    logd("set specialist, no. item %d", specialistList.count());
    foreach (DbModel* item, specialistList) {
        cbSpecialist->addSelectedItemByData(item->uid());
    }

    // cbCourse
    Utils::setSelectItemComboxByData(ui->cbCourse, per->courseUid());
    // cbCountry
    Utils::setSelectItemComboxByData(ui->cbCountry, per->countryUid());
    // cbProvince
#ifndef SKIP_PERSON_PROVINE
    Utils::setSelectItemComboxByData(ui->cbProvince, per->provinceUid());
#endif
    // dia chi
    ui->txtAddr->setPlainText(per->addr());
    // giao xu
    ui->txtChurch->setPlainText(per->churchAddr());
    ui->txtEmail->setText(per->email().join(SPLIT_EMAIL_PHONE));
    ui->txtPhone->setText(per->tel().join(SPLIT_EMAIL_PHONE));
    ui->txtOtherContact->setPlainText(per->otherContact());

    ui->txtDad->setText(per->dadName());
    ui->txtDadBirth->setText(DatetimeUtils::date2String(per->dadBirthday()));
    ui->txtDadAddr->setText(per->dadAddr());

    ui->txtMom->setText(per->momName());
    ui->txtMonBirth->setText(DatetimeUtils::date2String(per->momBirthday()));
    ui->txtMonAddr->setText(per->momAddr());

    ui->txtFamilyHistory->setPlainText(per->familyHistory());
    ui->txtFamilyContact->setPlainText(per->familyContact());

    // ngay rua toi
    ui->txtChristenDate->setText(DatetimeUtils::date2String(per->christenDate()));
    ui->txtChristenPlace->setText(per->christenPlace());

    // bi tich thanh the
    ui->txtEucharisDate->setText(DatetimeUtils::date2String(per->eucharistDate()));
    ui->txtEucharistPlace->setText(per->eucharistPlace());

    // bi tich them suc
    ui->txtHollyDate->setText(DatetimeUtils::date2String(per->hollyDate()));
    ui->txtHollyPlace->setText(per->hollyPlace());

    // ngay nhap tu
    ui->txtJoinDate->setText(DatetimeUtils::date2String(per->joinDate()));
    SET_TEXTBOX_FROM_VALUE(ui->txtJoinPIC, KItemUid, per->joinPICUid(), per->joinPICName());
    //cbJoinPIC (nguoi dac trach)

    // ngay gia nhap tien tap vien
    ui->txtPreTrainJoinDate->setText(DatetimeUtils::date2String(per->preTrainJoinDate()));
    //cbPreTrainJoinPIC
    SET_TEXTBOX_FROM_VALUE(ui->txtPreTrainJoinPIC, KItemUid,
                           per->preTrainPICUid(), per->preTrainPICName());

    ui->txtTrainJoinDate->setText(DatetimeUtils::date2String(per->trainJoinDate()));
    //cbTrainPIC
    SET_TEXTBOX_FROM_VALUE(ui->txtTrainPIC, KItemUid,
                           per->trainPICUid(), per->trainPICName());

    ui->txtVowsDate->setText(DatetimeUtils::date2String(per->vowsDate()));
    //cbVowsCEO
    SET_TEXTBOX_FROM_VALUE(ui->txtVowsCEO, KItemUid,
                           per->vowsCEOUid(), per->vowsCEOName());

    ui->txtEternalVowsDate->setText(DatetimeUtils::date2String(per->eternalVowsDate()));
    //cbEternalVowsPIC
//    SET_TEXTBOX_FROM_VALUE(ui->txtEternalVowsPIC, KItemUid,
//                           per->eternalVowsPICUid(), per->eternalVowsPICName());
    //cbEternalVowsCEO
    SET_TEXTBOX_FROM_VALUE(ui->txtEternalVowsCEO, KItemUid,
                           per->eternalVowsCEOUid(), per->eternalVowsCEOName());

    ui->txtBankDate->setText(DatetimeUtils::date2String(per->bankDate()));
    ui->txtBankPlace->setText(per->bankPlace());

    ui->txtGoldenDate->setText(DatetimeUtils::date2String(per->goldenDate()));
    ui->txtGoldenPlace->setText(per->goldenPlace());

    ui->txtEternalDate->setText(DatetimeUtils::date2String(per->eternalDate()));
    ui->txtEternalPlace->setText(per->eternalPlace());

    // cbStatus

    ui->txtRetireDate->setText(DatetimeUtils::date2String(per->retireDate()));
    ui->txtRetirePlace->setText(per->retirePlace());

    ui->txtDeadDate->setText(DatetimeUtils::date2String(per->deadDate()));
    ui->txtDeadPlace->setText(per->deadPlace());

    // current work
    logd("load work");
    Utils::setSelectItemComboxByData(ui->cbWork, per->currentWorkUid());
    ui->txtWorkHistory->setPlainText(per->workHistory());

    logd("load community");
    // TODO: handle the case that adding community dialog is shown when no community found
    // before showing person add dialog
    Utils::setSelectItemComboxByData(ui->cbCommunity, per->communityUid());

    logd("load person event");
    events = per->personEventList();
    if (!events.empty()) {
        RELEASE_LIST_DBMODEL(mListPersonEvent);
        foreach (DbModel* item, events) {
            if (item) {
                DbModel* event = item->clone();
                if (event) {
                    mListPersonEvent.append(event);
                } else {
                    logw("failed to clone event item, no memory");
                }
            } else {
                logw("invalid event items");
            }
        }
    }
    if (!mListPersonEvent.empty()) {
        loadEvent();
    } else {
        logd("no event to load");
    }
out:
    traceret(ret);
    mInitDone = true; // TODO: check setting mInitDone again, should offer api to mark init done?
    return ret;
}


ErrCode DlgPerson::onComboxNewItem(UIMultiComboxView *ui, const QString &value, bool silent)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("do silent %d", silent);
    logd("name %s", ui->name().toStdString().c_str());

    if (!mInitDone)
        return ErrNone; // not finish init yet, out;

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

void DlgPerson::multiComboxItemUpdate(UIMultiComboxView *cb, QLineEdit* txt)
{
    tracein;
    QHash<QString, QVariant> items = cb->items();
    QString str;
    foreach (QString value, items.keys()) {
        if (!str.isEmpty()) str.append(NAME_ITEM_SPLIT);
        str.append(value);
    }
    if (txt) txt->setText(str);
    traceout;
}


void DlgPerson::onComboxItemAdded(UIMultiComboxView *cb, const QString &name, const QVariant &value)
{
    tracein;
    logd("name %s", cb->name().toStdString().c_str());


    // TODO: separate processing to multi functions????
    if (cb->name() == KUiMultiComboxNameSaint) {
        multiComboxItemUpdate(cb, ui->txtSaint);
    }

    traceout;
}

void DlgPerson::onComboxItemDeleted(UIMultiComboxView *cb, const QString &name, const QVariant &value)
{
    tracein;
    logd("name %s", cb->name().toStdString().c_str());


    // TODO: separate processing to multi functions????
    if (cb->name() == KUiMultiComboxNameSaint) {
        multiComboxItemUpdate(cb, ui->txtSaint);
    }
    traceout;
}

void DlgPerson::onComboxClearAll()
{
    tracein;
}

void DlgPerson::loadEdu()
{
    tracein;
    logd("Load Education");
    QList<DbModel*> list = EDUCTL->getAllItemsFromDb();
    ui->cbEdu->clear();
    ui->cbEdu->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* edu, list){
        ui->cbEdu->addItem(edu->name(), edu->uid());
    }
}

void DlgPerson::loadSaints()
{
    tracein;
    logd("Load Saint");
    if (cbSaints == nullptr) {

        cbSaints = new UIMultiComboxView(KUiMultiComboxNameSaint, ui->wgSaint);
        cbSaints->setListener(this);

        ui->wgSaint->layout()->addWidget(cbSaints);
    }
    cbSaints->clearAll();
    QList<DbModel*> saints = SAINTCTL->getAllItems();

    foreach (DbModel* saint, saints) {
        logd(">> Saint %s", saint->name().toStdString().c_str());
        cbSaints->addItem(saint->name(), saint->uid());
    }

}

void DlgPerson::loadSpecialist()
{
    tracein;
    logd("Load specialist");
    if (cbSpecialist == nullptr){
        cbSpecialist = new UIMultiComboxView(KUiMultiComboxNameSpecialist, ui->wgSaint);
        cbSpecialist->setListener(this);
        ui->wgSpecialist->layout()->addWidget(cbSpecialist);
    }
    cbSpecialist->clearAll();
    QList<DbModel*> specialists = SPECIALISTCTL->getAllItemsFromDb();
    foreach (DbModel* specialist, specialists) {
        //        logd(">> specialist %s", name.toStdString().c_str());
        cbSpecialist->addItem(specialist->name(), specialist->uid());
    }

}


void DlgPerson::loadEthnic()
{
    tracein;

    // Someone may have US nationality, but Ethic is Kinh, as their original is VN
    logd("Reload course");
    ui->cbEthic->clear();
    QList<DbModel*> list = ETHNIC->getAllItemsFromDb();
    ui->cbEthic->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, list){
        ui->cbEthic->addItem(item->name(), item->uid()); // TODO: name include country?
    }
}


void DlgPerson::loadCountry()
{
    tracein;

    logd("Load country");
    QList<DbModel*> listCountry = COUNTRYCTL->getAllItemsFromDb();
    ui->cbNationality->clear();
    ui->cbCountry->clear();
    ui->cbNationality->addItem(STR_UNKNOWN, KUidNone);
    ui->cbCountry->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCountry){

        ui->cbNationality->addItem(item->name(), item->uid());
        ui->cbCountry->addItem(item->name(), item->uid());
    }
}

#ifndef SKIP_PERSON_PROVINE
void DlgPerson::loadProvince()
{
    tracein;
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
#endif

void DlgPerson::loadWork()
{
    tracein;
    ui->cbWork->clear();
    QList<DbModel*> list = INSTANCE(WorkCtl)->getAllItemsFromDb();
    ui->cbWork->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, list){
        ui->cbWork->addItem(item->name(), item->uid());
    }

}

void DlgPerson::loadEvent(bool reloadAll)
{
    tracein;
    QTableWidget* tbl = ui->tblEvents;
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    logd("reloadAll %d", reloadAll);
    if (reloadAll) {
        cleanEvent();
        Person* per = person();
        if (per != nullptr) {
            QList<DbModel*> list;
            RELEASE_LIST_DBMODEL(mListPersonEvent);
            ErrCode err = INSTANCE(PersonCtl)->getListEvents(per->uid(), list);
            if (err == ErrNone) {
                foreach(DbModel* item, list){
                    mListPersonEvent.append(item);
                }
            } else {
                loge("Get list event of person uid '%s' failed, err", STR2CHA(per->uid()), err);
                // TODO: show error dialog???
            }
        }
    }

    qint32 col = 0;
    qint32 idx = tbl->rowCount();
    logd("idx=%d", idx);
    logd("mListPersonEvent cnt=%d", mListPersonEvent.count());
    foreach (DbModel* model, mListPersonEvent) {
        PersonEvent* event = (PersonEvent*)model;
        col = 0;
        logd("idx=%d", idx);
        tbl->insertRow(idx);
        tbl->setItem(idx, col++, new QTableWidgetItem(event->nameId()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->eventName()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->name()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->dateString()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->endDateString()));
        tbl->setItem(idx, col++, new QTableWidgetItem(event->remark()));
        idx++;
    }

}

void DlgPerson::cleanEvent()
{
    tracein;
    RELEASE_LIST_DBMODEL(mListPersonEvent);
    traceout;
}

void DlgPerson::searchPerson(QLineEdit *wget)
{
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(false);

    if (dlg->exec() == QDialog::Accepted){
        const Person* per = (const Person*)dlg->selectedItem();
        if (per != nullptr) {
            wget->setText(per->fullName());
            logd("setProperty %s", per->uid().toStdString().c_str());
            wget->setProperty(KItemUid, per->uid());
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}

void DlgPerson::loadCommunity()
{
    tracein;
    logd("Load community");
    QList<DbModel*> listCommunity = COMMUNITYCTL->getAllItems();
    ui->cbCommunity->clear();
    ui->cbCommunity->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCommunity){
        ui->cbCommunity->addItem(item->name(), item->uid());
    }
    traceout;
}

void DlgPerson::loadStatus()
{
    tracein;
    ui->cbStatus->clear();
    QList<DbModel*> listItems = INSTANCE(PersonStatusCtl)->getAllItemsFromDb(); // TODO: getAllItem??
    ui->cbStatus->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listItems){
        ui->cbStatus->addItem(item->name(), item->uid());
    }

}


void DlgPerson::loadCourse()
{
    tracein;
    ui->cbCourse->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItems(); // TODO: should call getAllItem???
    ui->cbCourse->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCourse){
        ui->cbCourse->addItem(item->name(), item->uid());
    }

}

//void DlgPerson::accept()
//{
//    tracein;
//}

void DlgPerson::on_buttonBox_clicked( QAbstractButton * button )
{
    tracein;
    bool ok2Save = false;
    ErrCode ret = ErrNone;
    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton( button );
    logd("btn: %d", btn);
    if (btn == QDialogButtonBox::StandardButton::Save)
    {
        if (!mIsSelfSave) {
            logd("not mIsSelfSave, just call accept");
            QDialog::accept();
            return;
        }
        logd("mIsSelfSave, do save");
        QRegularExpression re("(\\d{4}[\\/\\.\\-](0?[1-9]|1[012])[\\/\\.\\-](0?[1-9]|[12][0-9]|3[01]))|\\d{4}");
        ok2Save = true;
        QString birthday = ui->txtBirthday->text();
        logd("check match birthday %s", birthday.toStdString().c_str());
        QRegularExpressionMatch match = re.match(birthday);

        if (!match.hasMatch())
        {
            ok2Save = false;
            QMessageBox msgBox;
            msgBox.setText("Ngày sinh không đúng");
            msgBox.setStandardButtons(QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            msgBox.exec();
        }
        logd("ok2save %d", ok2Save);
        if (ok2Save) {
            // TODO: check should we save here? or let caller save???
            // TODO: dialg with mode: add new, update, store directly, etc.
            Person* per = person();
            ret = buildPerson(per);
            if (ret == ErrNone) {
                if (mIsNew) {
                    logd("Save it");
                    ret = per->save();
                } else {
                    logd("Update it");
                    ret = per->update();
                }
                logi("Save/Update person result %d", ret);
            } else {
                loge("Build person err=%d", ret);
            }

            if (ret == ErrNone) {
                logd("Save/update ok, close dialog");
                QDialog::accept();
            } else {
                DialogUtils::showErrorBox(QString("Lỗi, không thể lưu thông tin, Mã lỗi %1").arg(ret));
            }
        }

    } else {
        logd("Do nothing, cancel");
        QDialog::close();
    }
    traceout;
}


void DlgPerson::on_btnEditNation_clicked()
{
    tracein;

    DlgCountry* dlg = new DlgCountry();
    if (dlg == nullptr) {
        loge("Open dlg country fail, No memory");
        return;
    }
    dlg->setIsSelfSave(true);

    if (dlg->exec() == QDialog::Accepted){
        loadCountry();

    }
    delete dlg;
    traceout;
}


void DlgPerson::on_cbCountry_currentIndexChanged(int index)
{
    tracein;
    logd("Reload provine of %s", ui->cbCountry->currentText().toStdString().c_str());
    logd("index %d", index);
#ifndef SKIP_PERSON_PROVINE
    loadProvince();
#endif
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
//            tbl->setItem(idx, col++, new QTableWidgetItem(DatetimeUtils::date2String(date)));
//            tbl->setItem(idx, col++, new QTableWidgetItem(name));
//            tbl->setItem(idx, col++, new QTableWidgetItem(w.getRemark()));

//            //        logd("Date %d", w.getDate());
//        }
//    }
}


void DlgPerson::on_tblClearCommunity_clicked()
{
    tracein;
//    ui->tblCommunityList->clearContents();
//    ui->tblCommunityList->model()->removeRows(0, ui->tblCommunityList->rowCount());
}


void DlgPerson::on_btnPreview_clicked()
{
    tracein;
    Person* per = person();
    ErrCode ret = buildPerson(per);
    if (ret == ErrNone) {
        QString fpath;
        INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
        if (QFile::exists(fpath)){
            dlgHtmlViewer* viewer = new dlgHtmlViewer();
            viewer->setHtmlPath(fpath);
            viewer->setSubject("Person");
            viewer->exec();
        }
    } else {
        loge("Bild person failed, err=%d", ret);
        DialogUtils::showErrorBox(QString("Có lỗi xảy ra, mã lỗi %1").arg(ret));
    }
    traceout;
}

Person *DlgPerson::person(bool newone)
{
    if (mPerson == nullptr) {
        mPerson = (Person*) Person::build();
    } else if (newone) {
        delete mPerson;
        mPerson = (Person*) Person::build();
    }

    return mPerson;
}

DlgPerson *DlgPerson::buildDlg(QWidget *parent, const Person *per, bool isNew)
{
    tracein;
    DlgPerson* dlg = nullptr;
    dlg = new DlgPerson(parent);
    dlg->setIsSelfSave(true);
    dlg->setIsNew(isNew);
    logd("isNew %d", isNew);
    if (per != nullptr) {
        logd("setup person");
        dlg->fromPerson(per);
    }

    traceout;
    return dlg;
}

QString DlgPerson::getName()
{
    return "DlgPerson";
}

void DlgPerson::onDbModelReady(ErrCode ret, DbModel *model, DlgCommonEditModel *dlg)
{
    tracein;
    logd("ret=%d", ret);
    if (ret == ErrNone && model) {
        if (model->modelName() == KModelNameEthnic) {
            logd("Reload ethnic");
            loadEthnic();
        } else {
            logw("not support model '%s'", STR2CHA(model->modelName()));
        }
    } else {
        logw("ret=%d model=%d", ret, (model != nullptr));
    }
    traceout;
}

DbModel *DlgPerson::onCreateDbModelObj(const QString &modelName)
{
    DbModel* model = nullptr;
    tracein;
    if (!modelName.isEmpty() && (modelName == KModelNameEthnic)) {
        model = Ethnic::build();
    } else {
        logw("invalid or not support modelName '%s'", STR2CHA(modelName));
    }
    traceout;
    return model;
}


void DlgPerson::on_btnImg_clicked()
{
    tracein;
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("Image Files (*.png *.jpg *.bmp *.jpeg)")); // TODO: make it common?

    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QPixmap p(fname);
        ui->lblImg->setPixmap(p.scaledToHeight( ui->lblImg->height()));
        ui->lblImgPath->setText(fname);
        Image img;
        img.loadImage(fname, IMG_TAG_PEOPLE);
    }
}


void DlgPerson::on_btnClearImage_clicked()
{
    tracein;
    ui->lblImg->clear();
    ui->lblImgPath->clear();
    ui->lblImg->setText("<Picture>");
}


void DlgPerson::on_btnAddCountry_clicked()
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
    }
    delete dlg;
    traceout;
}


void DlgPerson::on_btnAddEthnic_clicked()
{
    tracein;

    DlgEthnic* dlg = DlgEthnic::build(this, true, KModelNameEthnic);
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
    tracein;
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
    tracein;
#ifndef SKIP_PERSON_PROVINE
    DlgProvince * dlg = new DlgProvince();
    if (dlg == nullptr) {
        loge("Open dlg province fail, No memory");
        return;
    }

    if (dlg->exec() == QDialog::Accepted){
        loadProvince();

    }
    delete dlg;
#else
    loge("PROVINE IS DISABLED!!! SHOULD NOT CALL HERE!!!");
#endif
}


void DlgPerson::on_btnAddCommunity_clicked()
{
    tracein;
}


void DlgPerson::on_btnAddCourse_clicked()
{
    tracein;

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
    tracein;
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
    tracein;
    DlgAddPersonEvent * dlg = DlgAddPersonEvent::build(this, false, nullptr);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return;
    }

    dlg->setPerson(getPerson());
    if (dlg->exec() == QDialog::Accepted){
        const DbModel* event = dlg->getModel();
        if (event != nullptr) {
            logd("Add event to list '%s'", STR2CHA(event->toString()));
            mListPersonEvent.append(event->clone());
        }
        loadEvent();
    }
    delete dlg;
    traceout;
}


void DlgPerson::on_btnDelEvent_clicked()
{
    tracein;
    // TODO: validate data
    QTableWidget* tbl = ui->tblEvents;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    // Multiple rows can be selected
    QList<DbModel*> toDel;
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        DbModel* model = mListPersonEvent.at(index.row());
        if (model) {
            logd("Delete event '%s'", STR2CHA(model->toString()));
            toDel.append(model);
        }
        mListPersonEvent.removeAt(index.row());
    }
    RELEASE_LIST_DBMODEL(toDel);
    loadEvent();
}


bool DlgPerson::isSelfSave() const
{
    return mIsSelfSave;
}

void DlgPerson::setIsSelfSave(bool newIsSelfSave)
{
    mIsSelfSave = newIsSelfSave;
}

void DlgPerson::loadPersonCode()
{
    tracein;

    ui->txtCode->setText(Config::getNextPersonalCode());

    traceout;
}

bool DlgPerson::isNew() const
{
    return mIsNew;
}

void DlgPerson::setIsNew(bool newIsNew)
{
    mIsNew = newIsNew;
}


void DlgPerson::on_btnSearchJoinPIC_clicked()
{
    tracein;
    searchPerson(ui->txtJoinPIC);
    traceout;
}


void DlgPerson::on_btnSearchPreTrainJoinPIC_clicked()
{
    tracein;
    searchPerson(ui->txtPreTrainJoinPIC);
    traceout;

}


void DlgPerson::on_btnSearchTrainPIC_clicked()
{
    tracein;
    searchPerson(ui->txtTrainPIC);
    traceout;

}


void DlgPerson::on_btnSearchVowsCEO_clicked()
{
    tracein;
    searchPerson(ui->txtVowsCEO);
    traceout;

}


void DlgPerson::on_btnSearchEternalVowsPIC_clicked()
{
    tracein;
//    searchPerson(ui->txtEternalVowsPIC);
    traceout;

}


void DlgPerson::on_btnSearchEternalVowsCEO_clicked()
{
    tracein;
    searchPerson(ui->txtEternalVowsCEO);
    traceout;

}




void DlgPerson::on_btnModifyEvent_clicked()
{
    tracein;
    // TODO: validate data
    QTableWidget* tbl = ui->tblEvents;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    // Multiple rows can be selected
    DbModel* model = nullptr;
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        model = mListPersonEvent.at(index.row());
        if (model) {
            break;
        }
    }

    if (model) {
        DlgAddPersonEvent * dlg = DlgAddPersonEvent::build(this, false, model->modelName(), model);
        if (dlg == nullptr) {
            loge("Open dlg DlgAddPersonEvent fail, No memory");
            return;
        }

        dlg->setPerson(getPerson());
        if (dlg->exec() == QDialog::Accepted){
            const DbModel* event = dlg->getModel();
            if (event != nullptr) {
                logd("copy data from '%s'", STR2CHA(event->toString()));
                model->setMarkModified(true);
                model->copyData(event);
            } else {
                logw("something was wrong, invalid data");
            }
            loadEvent();
        }
        delete dlg;
    } else {
        loge("no person event model to modify");
    }
    traceout;
}

