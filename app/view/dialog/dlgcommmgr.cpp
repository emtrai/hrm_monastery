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
 * Filename: dlgcommmgr.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#include "dlgcommmgr.h"
#include "ui_dlgcommmgr.h"
#include "communitymanager.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "person.h"
#include "coursectl.h"
#include "rolectl.h"
#include "dialog/dlgsearchperson.h"
#include "stringdefs.h"
#include "community.h"
#include "dbctl.h"
#include "modeldefs.h"

DlgCommMgr::DlgCommMgr(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgCommMgr),
    mCommunity(nullptr),
    mPerson(nullptr)
{
    tracein;
    ui->setupUi(this);
    loadCourse();
    loadRole();
    loadStatus();
    traceout;
}

DlgCommMgr::~DlgCommMgr()
{
    tracein;
    delete ui;
    FREE_PTR(mCommunity);
    FREE_PTR(mPerson);
    traceout;
}

ErrCode DlgCommMgr::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    CommunityManager* comm = nullptr;
    UNUSED(errMsg);
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid argument, null model");
    }

    if (err == ErrNone && !mCommunity) {
        err = ErrNoData;
        loge("No mCommunity object to be set");
    }
    if (err == ErrNone && ui->txtSearch->text().isEmpty()) {
        err = ErrNoData;
        loge("No person is set");
    }
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone) {
        comm = (CommunityManager*) model;
        comm->setMarkModified(true); // start marking fields which are modified
        comm->setModelStatus(MODEL_STATUS_ACTIVE);
        comm->setCommunity(mCommunity);
        SET_MODEL_FROM_TEXTBOX(ui->txtSearch, KItemUid, comm->setPerson, PERSONCTL, Person, err);
        SET_MODEL_FROM_VAL_CBOX(ui->cbRole, comm->setRole, ROLECTL, Role, err);
        SET_MODEL_FROM_VAL_CBOX(ui->cbTerm, comm->setCourse, COURSECTL, Course, err);
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, comm->setModelStatus, comm->setModelStatusName);
        SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, comm->setStartDate);
        SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, comm->setEndDate);
        CHECK_SET_NAMEID(err, comm, ui->txtNameId->text().trimmed());
    }
    traceret(err);
    return err;
}

ErrCode DlgCommMgr::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNameCommManager) {
        err = DlgCommonEditModel::fromModel(item);
        CommunityManager* comm = (CommunityManager*)model();
        if (err == ErrNone) {
            Utils::setSelectItemComboxByData(ui->cbTerm, comm->courseUid());
            Utils::setSelectItemComboxByData(ui->cbRole, comm->roleUid());
            Utils::setSelectItemComboxByData(ui->cbStatus, comm->modelStatus());
            ui->txtStartDate->setText(DatetimeUtils::date2String(comm->startDate(), DEFAULT_FORMAT_YMD));
            ui->txtEndDate->setText(DatetimeUtils::date2String(comm->endDate(), DEFAULT_FORMAT_YMD));
            ui->txtRemark->setPlainText(comm->remark());
            ui->btnSearch->setEnabled(false); // not allow to change person
            QString nameid = comm->nameId();
            if (!comm->personUid().isEmpty()) {
                logd("Search person uid '%s'", STR2CHA(comm->personUid()));
                Person* per = (Person*)PERSONCTL->getModelByUid(comm->personUid());
                if (per) {
                    SET_TEXTBOX_FROM_VALUE(ui->txtSearch, KItemUid,
                                           per->uid(), per->fullName());
                    logd("found person '%s'", STR2CHA(per->toString()));
                    if (nameid.isEmpty()) {
                        nameid = QString("%1_%2").arg(comm->communityNameId(), per->nameId());
                        comm->setNameId(nameid);
                    }

                    delete per;
                } else {
                    loge("Not found person uid '%s'", STR2CHA(comm->personUid()));
                }
            } else {
                loge("No person uid");
            }
            ui->txtNameId->setText(comm->nameId());
        }
    } else {
        err = ErrInvalidArg;
        loge("null item or invalid model '%s'", MODELNAME2CHA(item));
    }
    traceret(err);
    return err;
}

void DlgCommMgr::loadCourse()
{
    tracein;
    ui->cbTerm->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItems(); // TODO: should call getAllItem???
    ui->cbTerm->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCourse){
        ui->cbTerm->addItem(item->name(), item->uid());
    }

}

void DlgCommMgr::loadRole()
{
    ui->cbRole->clear();
    QList<DbModel*> list = INSTANCE(RoleCtl)->getAllItemsFromDb(); // TODO: getAllItem???
    foreach(DbModel* item, list){
        ui->cbRole->addItem(item->name(), item->uid());
    }
}

void DlgCommMgr::loadStatus()
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

void DlgCommMgr::updatePeson(const Person *per)
{
    if (per != nullptr) {
        ui->txtSearch->setText(per->fullName());
        logd("setProperty %s", per->uid().toStdString().c_str());
        ui->txtSearch->setProperty(KItemUid, per->uid());
        QString nameid;
        if (mCommunity) {
            nameid = QString("%1_%2").arg(mCommunity->nameId(), per->nameId());
        }
        ui->txtNameId->setText(nameid);
    } else {
        logi("No person to set");
    }
}

void DlgCommMgr::on_btnSearch_clicked()
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
        updatePeson(per);
    }
    delete dlg;
    traceout;
}

ErrCode DlgCommMgr::setPerson(const Person *newPerson)
{
    logd("set person '%s'", MODELSTR2CHA(newPerson));
    FREE_PTR(mPerson);
    mPerson = CLONE_MODEL_CONST1(newPerson, Person);
    if (mPerson) {
        updatePeson(mPerson);
    }
    return ErrNone;
}

void DlgCommMgr::setModelStatus(qint64 status)
{
    logd("set status '%lld'", status);
    Utils::setSelectItemComboxByData(ui->cbStatus, status);
    ui->cbStatus->setEditable(false);
    ui->cbStatus->setDisabled(true);
}

void DlgCommMgr::setRole(const QString &roleUid)
{
    logd("set roleUid '%s'", STR2CHA(roleUid));
    Utils::setSelectItemComboxByData(ui->cbRole, roleUid);
    ui->cbRole->setEditable(false);
    ui->cbRole->setDisabled(true);

}

ErrCode DlgCommMgr::setCommunity(const Community *comm)
{
    tracein;
    ErrCode err = ErrNone;
    FREE_PTR(mCommunity);
    if (comm) {
        logd("clone new comm");
        mCommunity = CLONE_MODEL_CONST1(comm, Community);
        if (!mCommunity) {
            err = ErrNoMemory;
            loge("Clone new comm fail, no memory?");
        }
    }
    traceret(err);
    return err;
}

QDialogButtonBox *DlgCommMgr::buttonBox()
{
    return ui->buttonBox;
}

DbModel *DlgCommMgr::newModel()
{
    return CommunityManager::build();
}

bool DlgCommMgr::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        if (mModel->nameId().isEmpty()) {
            msg += tr("Thiếu mã định danh.");
            isValid = false;
            logw("lack name id");
        }
    } else {
        logw("no model to check");
        isValid = false;
    }
    logd("is valid %d", isValid);
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    traceout;
    return isValid;
}
