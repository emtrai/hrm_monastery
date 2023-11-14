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
 * Filename: dlgdeptperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "dlgdeptperson.h"
#include "persondept.h"
#include "ui_dlgdeptperson.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "person.h"
#include "coursectl.h"
#include "rolectl.h"
#include "dialog/dlgsearchperson.h"
#include "stringdefs.h"
#include "modeldefs.h"

DlgDeptPerson::DlgDeptPerson(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgDeptPerson)
{
    tracein;
    ui->setupUi(this);
    loadCourse();
    loadRole();
    loadStatus();
    traceout;
}

DlgDeptPerson::~DlgDeptPerson()
{
    delete ui;
}

ErrCode DlgDeptPerson::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    PersonDept* perdep = (PersonDept*) model;
    if (!perdep){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        perdep->setMarkModified(true); // start marking fields which are modified
        perdep->setModelStatus(MODEL_STATUS_ACTIVE);
        SET_VAL_FROM_TEXTBOX(ui->txtSearch, KItemUid, perdep->setPersonUid, perdep->setPersonName);
        SET_VAL_FROM_VAL_CBOX(ui->cbRole, perdep->setRoleUid, perdep->setRoleName, err);
        SET_VAL_FROM_VAL_CBOX(ui->cbTerm, perdep->setCourseUid, perdep->setCourseName, err);
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, perdep->setModelStatus, perdep->setModelStatusName);
        SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, perdep->setStartDate);
        SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, perdep->setEndDate);
        QString nameid = ui->txtNameId->text().trimmed();
        if (!nameid.isEmpty()) {
            perdep->setNameId(nameid);
        }
        if (!mCommDeptUid.isEmpty()) {
            perdep->setCommDeptUid(mCommDeptUid);
        }
    }
    dbgv("build model '%s' err %d", MODELSTR2CHA(perdep), err);
    traceret(err);
    return err;
}

ErrCode DlgDeptPerson::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    PersonDept* comm = nullptr;
    err = DlgCommonEditModel::fromModel(item);
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNamePersonDept)) {
        err = ErrInvalidArg;
        loge("null item or invalid type '%s'", MODELSTR2CHA(item));
    }
    if (err == ErrNone) {
        comm = static_cast<PersonDept*>(model());
        if (!comm) {
            err = ErrInvalidModel;
            loge("not found suitable model");
        }
    }
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
                    nameid = QString("%1_%2").arg(comm->commDeptNameId(), per->nameId());
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
    traceret(err);
    return err;
}

void DlgDeptPerson::loadCourse()
{
    tracein;
    ui->cbTerm->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItems(); // TODO: should call getAllItem???
    ui->cbTerm->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCourse){
        ui->cbTerm->addItem(item->name(), item->uid());
    }

}

void DlgDeptPerson::loadRole()
{
    ui->cbRole->clear();
    QList<DbModel*> list = INSTANCE(RoleCtl)->getAllItems();
    // not allow none/empty one
    foreach(DbModel* item, list){
        ui->cbRole->addItem(item->name(), item->uid());
    }
}

void DlgDeptPerson::loadStatus()
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

void DlgDeptPerson::on_btnSearch_clicked()
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
            ui->txtSearch->setText(per->fullName());
            logd("setProperty %s", STR2CHA(per->uid()));
            ui->txtSearch->setProperty(KItemUid, per->uid());
            QString nameid = QString("%1_%2").arg(mCommDeptNameId, per->nameId());
            ui->txtNameId->setText(nameid);
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}

void DlgDeptPerson::setCommDeptNameId(const QString &newCommDeptNameId)
{
    mCommDeptNameId = newCommDeptNameId;
}

QDialogButtonBox *DlgDeptPerson::buttonBox()
{
    return ui->buttonBox;
}

void DlgDeptPerson::setCommDeptUid(const QString &newCommDeptUid)
{
    mCommDeptUid = newCommDeptUid;
}

DbModel *DlgDeptPerson::newModel()
{
    return PersonDept::build();
}

bool DlgDeptPerson::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        if (mModel->nameId().isEmpty()) {
            msg += STR_LACK_NAMEID;
            isValid = false;
            logw("lack name id");
        }
    } else {
        logw("no model to check");
        isValid = false;
    }
    logi("is valid %d", isValid);
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    traceout;
    return isValid;
}
