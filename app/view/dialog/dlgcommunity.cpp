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
 * Filename: dlgcommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "dlgcommunity.h"
#include "communityctl.h"
#include "communitymanager.h"
#include "person.h"
#include "ui_dlgcommunity.h"
#include "logger.h"
#include "errcode.h"
#include "community.h"
#include "areactl.h"
#include "countryctl.h"
#include "communityctl.h"
#include "dialog/dlgsearchperson.h"
#include "dbmodel.h"
#include "datetimeutils.h"
#include "stringdefs.h"
#include "dialogutils.h"
#include "dlgcommmgr.h"
#include "personctl.h"
#include "rolectl.h"
#include "role.h"
#include "mission.h"
#include "missionctl.h"

DlgCommunity::DlgCommunity(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgCommunity)
{
    ui->setupUi(this);
#ifdef TEST_ENABLE
    ui->frmCommTestBtn->setVisible(true);
#else
    ui->frmCommTestBtn->setVisible(false);
#endif //TEST_ENABLE
    loadData();
}

DlgCommunity::~DlgCommunity()
{
    tracein;
    delete ui;
    traceout;
}


ErrCode DlgCommunity::buildModel(DbModel *model, QString& errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(errMsg);
    Community* comm = static_cast<Community*>(model);
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        comm->setMarkModified(true); // start marking fields which are modified
    }
    if (err == ErrNone) {
        QString name = ui->txtName->text().trimmed();
        if (!name.isEmpty()) {
            comm->setName(name);
        }
    }
    if (err == ErrNone){
        QString name = ui->txtCode->text().trimmed();
        if (!name.isEmpty()) {
            comm->setNameId(name);
        }
    }
    if (err == ErrNone){
        SET_VAL_FROM_VAL_CBOX(ui->cbArea, comm->setAreaUid, comm->setAreaName, err);
    }
    // TODO: we should support multiple missions
    // currently, only one mission uid is supported
    if (err == ErrNone){
        SET_VAL_FROM_VAL_CBOX(ui->cbMission, comm->setMissionUid, comm->setMissionName, err);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtFeaseDay, comm->setFeastDate, DATE_FORMAT_DM);
    }
    if (err == ErrNone){
        comm->setAddr(ui->txtAddr->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setTel(ui->txtTel->text().trimmed());
    }
    if (err == ErrNone){
        comm->setEmail(ui->txtEmail->toPlainText().trimmed());
    }
    if (err == ErrNone){
        SET_VAL_FROM_VAL_CBOX(ui->cbCountry, comm->setCountryUid, comm->setCountryName, err);
    }
    if (err == ErrNone){
        comm->setChurch(ui->txtChurch->toPlainText().trimmed());
    }
    if (err == ErrNone){
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, comm->setModelStatus, comm->setModelStatusName);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtEstablishDate, comm->setCreateDate, DEFAULT_FORMAT_YMD);
    }
    if (err == ErrNone){
        SET_VAL_FROM_VAL_CBOX(ui->cbParentCommunity, comm->setParentUid, comm->setParentName, err);
    }
    if (err == ErrNone){
        SET_VAL_FROM_TEXTBOX(ui->txtCEO, KItemUid, comm->setCurrentCEOUid, comm->setCurrentCEOName);
    }
    if (err == ErrNone){
        comm->setContact(ui->txtContact->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setBrief(ui->txtIntro->toPlainText().trimmed());
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtNote->toPlainText().trimmed());
    }
    dbgv("build model '%s' err %d", MODELSTR2CHA(model), err);
    traceret(err);
    return err;
}

ErrCode DlgCommunity::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    err = DlgCommonEditModel::fromModel(item);
    Community* comm = (Community*)model();
    if (err == ErrNone) {
        dbgd("from model '%s'", MODELSTR2CHA(comm));
        comm->check2LoadAllData();
        ui->txtName->setText(comm->name());
        ui->txtCode->setText(comm->nameId()); // TODO: auto generate???
        Utils::setSelectItemComboxByData(ui->cbArea, comm->areaUid());
        ui->txtEstablishDate->setText(DatetimeUtils::date2String(comm->createDate(), DEFAULT_FORMAT_YMD));
        ui->txtFeaseDay->setText(DatetimeUtils::date2String(comm->feastDate(), DEFAULT_FORMAT_MD));
        ui->txtAddr->setPlainText(comm->addr());
        Utils::setSelectItemComboxByData(ui->cbCountry, comm->countryUid());
        // TODO: ugly hack for single mission by using 1st element of mission uid
        // we should support multiple missions
        Utils::setSelectItemComboxByData(ui->cbMission, comm->mainMissionUid());
        ui->txtTel->setText(comm->tel());
        ui->txtEmail->setPlainText(comm->email());
        ui->txtChurch->setPlainText(comm->church());
        Utils::setSelectItemComboxByData(ui->cbStatus, comm->getStatus());
        // TODO: image???
        Utils::setSelectItemComboxByData(ui->cbParentCommunity, comm->parentUid());
        SET_TEXTBOX_FROM_VALUE(ui->txtCEO, KItemUid,
                               comm->currentCEOUid(), comm->currentCEONameWithUpdate());


        ui->txtIntro->setPlainText(comm->brief());
        ui->txtContact->setPlainText(comm->contact());
        ui->txtNote->setPlainText(comm->remark());
    }
    traceret(err);
    return err;
}

DbModel *DlgCommunity::newModel()
{
    // TODO: check model parameter???
    return Community::build();
}

void DlgCommunity::loadData()
{
    tracein;
    loadList(ui->cbArea, AREACTL, true);
    loadList(ui->cbCountry, COUNTRYCTL, true);
    loadList(ui->cbParentCommunity, COMMUNITYCTL, true);
    loadList(ui->cbMission, MISSIONCTL, true);
    loadStatus();
    traceout;
}

void DlgCommunity::loadStatus()
{
    tracein;
    ui->cbStatus->clear();
    const QHash<int, QString>* statuses = DbModel::getModelStatusIdNameMap();
    logd("the number of status %lld", statuses->count());
    foreach (int key, statuses->keys()) {
        ui->cbStatus->addItem(statuses->value(key), key);
    }
    traceout;
}

bool DlgCommunity::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (IS_MODEL_NAME(mModel, KModelNameCommunity)) {
        Community* comm = static_cast<Community*>(mModel);
        if (comm->name().isEmpty()) {
            msg += STR_LACK_NAME;
            isValid = false;
            logw("lack name");
        }
        if (comm->nameId().isEmpty()) {
            msg += STR_LACK_NAMEID;
            isValid = false;
            logw("lack name id");
        }
    } else {
        msg += STR_INVALID_DATA;
        logw("no model or invalid model '%s' to check", MODELSTR2CHA(mModel));
        isValid = false;
    }
    dbgv("is valid %d", isValid);
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    traceout;
    return isValid;

}

bool DlgCommunity::onConfirmSave(bool isAddnew, DbModel *model, QString& errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    Person* ceo = nullptr;
    DbModel* newCEO = nullptr;
    bool isActiveCEO = false;
    bool accept = false;
    Community* comm = nullptr;
    DlgCommMgr* dlg = nullptr;
    const CommunityManager* mgr = nullptr;
    if (!IS_MODEL_NAME(model, KModelNameCommunity)) {
        err = ErrInvalidArg;
        loge("invalid arg");
        errMsg = STR_INVALID_DATA;
    }
    if (err == ErrNone) {
        accept = DlgCommonEditModel::onConfirmSave(isAddnew, model, errMsg);
        if (!accept) {
            err = ErrCancelled;
            loge("not accept in previous asking");
        }
    }
    if (err == ErrNone) {
        comm = static_cast<Community*>(model);
        // check if need to update CEO
        // 2 cases:
        // - newly add
        // - ceo info is updated
        if ((isNew() || comm->updateCEO()) && !comm->currentCEOUid().isEmpty()) {
            newCEO = PERSONCTL->getModelByUid(comm->currentCEOUid());
            if (!newCEO) {
                loge("not found person with uid '%s'", STR2CHA(comm->currentCEOUid()));
                err = ErrNotFound;
            }
        } else {
            accept = true;
            loge("not update ceo");
        }
    }
    if (err == ErrNone) {
        // new ceo is set, ask user if we need to update CEO info or not
        if (newCEO) {
            ErrCode tmperr = COMMUNITYCTL->getCurrentCEO(comm->uid(), &ceo, &isActiveCEO);
            dbgd("found ceo '%s'", MODELSTR2CHA(ceo));
            QString question;
            if (tmperr == ErrNone || tmperr == ErrNoData) {
                if (ceo) {
                    question = QString("Đổi Phụ trách hiện tại %1 sang Phụ trách %2 cho Cộng đoàn %2?")
                                .arg(ceo->displayName(),
                                        comm->currentCEOName(),
                                        comm->name());
                } else {
                    question = QString("Thiết lập '%1' là Phụ trách cho Cộng đoàn %2?")
                                   .arg(comm->currentCEOName(), comm->name());
                }
                dbgd("confirm dialog '%s'", STR2CHA(question));
                accept = DialogUtils::showConfirmDialog(
                    this, STR_SAVE, question, nullptr);

                if (accept) {
                    // accept to confirm, let's create dialog to add information about comm mgr
                    if (err == ErrNone) {
                        dlg = DlgCommMgr::build(
                            this, false, KModelNameCommManager, nullptr, nullptr);
                        if (!dlg) {
                            loge("failed to build DlgCommMgr, no memory?");
                            err = ErrNoMemory;
                        }
                    }
                    if (err == ErrNone) {
                        logd("Set comm '%s' for person", MODELSTR2CHA(comm));
                        err = dlg->setCommunity(comm);
                    }
                    if (err == ErrNone) {
                        logd("Set newCEO '%s'", MODELSTR2CHA(newCEO));
                        err = dlg->setPerson(static_cast<const Person*>(newCEO));
                    }
                    if (err == ErrNone) {
                        dlg->setModelStatus(MODEL_STATUS_ACTIVE);
                    }

                    logd("get ceo role");
                    Role* role = ROLECTL->getCEORole();
                    if (role) {
                        logd("set role '%s'", MODELSTR2CHA(role));
                        dlg->setRole(role->uid());
                        FREE_PTR(role);
                    }

                    if (err == ErrNone) {
                        if (dlg->exec() == QDialog::Accepted) {
                            const DbModel* mod = dlg->getModel();
                            if (IS_MODEL_NAME(mod, KModelNameCommManager)) {
                                mgr = dynamic_cast<const CommunityManager*>(mod);
                            } else {
                                err = ErrInvalidModel;
                                loge("invalid model '%s', expected '%s'",
                                     MODELSTR2CHA(mod), KModelNameCommManager);
                            }
                        } else {
                            err = ErrCancelled;
                        }
                    }

                    if (err == ErrNone && mgr) {
                        logd("set commmgr '%s'", MODELSTR2CHA(mgr));
                        comm->setNewCommMgr(mgr);
                    }

                }
            } else {
                errMsg = tr("Lỗi tìm Phụ trách cộng đoàn");
                loge("Err get current CEO %d", tmperr);
            }
        } else {
            logi("not change ceo for comm '%s'", MODELSTR2CHA(comm));
            accept = true;
        }
    }
    logife(err, "on confirm failed");
    FREE_PTR(ceo);
    FREE_PTR(newCEO);
    FREE_PTR(dlg);
    traceout;
    return accept && (err == ErrNone);
}

QDialogButtonBox *DlgCommunity::buttonBox()
{
    return ui->buttonBox;
}


void DlgCommunity::on_btnSearchCEO_clicked()
{
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgDeptPerson fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(false);

    if (dlg->exec() == QDialog::Accepted){
        const Person* per = (const Person*)dlg->selectedItem();
        if (per != nullptr) {
            ui->txtCEO->setText(per->fullName());
            logd("setProperty %s", per->uid().toStdString().c_str());
            ui->txtCEO->setProperty(KItemUid, per->uid());
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}


void DlgCommunity::on_txtName_textChanged(const QString &arg1)
{
    tracein;
    onChangeNameIdTxt(ui->txtCode, arg1);
    traceout;
}


void DlgCommunity::on_btnChangeNameId_clicked()
{
    tracein;
    onEditnameId(ui->txtCode);
    if (ui->txtCode->text().length() == 0) { // custome nameid is null, make it auto generate
        onChangeNameIdTxt(ui->txtCode, ui->txtName->text());
    }
    traceout;
}

