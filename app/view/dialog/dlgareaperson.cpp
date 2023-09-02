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
 * Filename: DlgAreaPerson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "dlgareaperson.h".h"
#include "areaperson.h"
#include "ui_DlgAreaPerson.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "person.h"
#include "coursectl.h"
#include "rolectl.h"
#include "dialog/dlgsearchperson.h"
#include "area.h"
#include "stringdefs.h"

DlgAreaPerson::DlgAreaPerson(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgAreaPerson),
    mArea(nullptr)
{
    tracein;
    ui->setupUi(this);
    loadCourse();
    loadRole();
    loadStatus();
}

DlgAreaPerson::~DlgAreaPerson()
{
    delete ui;
    if (mArea) {
        delete mArea;
        mArea = nullptr;
    }
}

ErrCode DlgAreaPerson::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid argument, null model");
    }

    if (err == ErrNone && !mArea) {
        err = ErrNoData;
        loge("No area object to be set");
    }
    if (err == ErrNone && ui->txtSearch->text().isEmpty()) {
        err = ErrNoData;
        loge("No person is set");
    }
    if (err == ErrNone) {
        AreaPerson* per = (AreaPerson*) model;
        if (!model){
            err = ErrInvalidArg;
            loge("Invalid arg");
        }
        if (err == ErrNone){
            per->setMarkModified(true); // start marking fields which are modified
        }
        per->setModelStatus(MODEL_STATUS_ACTIVE);
        per->setAreaUid(mArea->uid());
        SET_VAL_FROM_TEXTBOX(ui->txtSearch, KItemUid, per->setPersonUid, per->setPersonName);
        SET_VAL_FROM_CBOX(ui->cbRole, per->setRoleUid, per->setRoleName, err);
        SET_VAL_FROM_CBOX(ui->cbTerm, per->setCourseUid, per->setCourseName, err);
        SET_INT_VAL_FROM_CBOX(ui->cbStatus, per->setModelStatus, per->setModelStatusName);
        SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, per->setStartDate);
        SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, per->setEndDate);
        QString nameid = ui->txtNameId->text().trimmed();
        if (!nameid.isEmpty()) {
            per->setNameId(nameid);
        } else {
            err = ErrNoData;
            loge("Lack of nameId");
        }
    } else {
    }
    traceret(err);
    return err;
}

ErrCode DlgAreaPerson::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNameAreaPerson) {
        err = DlgCommonEditModel::fromModel(item);
        AreaPerson* comm = (AreaPerson*)model();
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
                                           per->uid(), per->getFullName());
                    logd("found person '%s'", STR2CHA(per->toString()));
                    if (nameid.isEmpty()) {
                        nameid = QString("%1_%2").arg(comm->areaNameId(), per->nameId());
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

void DlgAreaPerson::loadCourse()
{
    tracein;
    ui->cbTerm->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItems(); // TODO: should call getAllItem???
    ui->cbTerm->addItem(STR_UNKNOWN, KUidNone);
    foreach(DbModel* item, listCourse){
        ui->cbTerm->addItem(item->name(), item->uid());
    }

}

void DlgAreaPerson::loadRole()
{
    ui->cbRole->clear();
    QList<DbModel*> list = INSTANCE(RoleCtl)->getAllItemsFromDb(); // TODO: getAllItem???
    foreach(DbModel* item, list){
        ui->cbRole->addItem(item->name(), item->uid());
    }
}

void DlgAreaPerson::loadStatus()
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

void DlgAreaPerson::on_btnSearch_clicked()
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
            ui->txtSearch->setText(per->getFullName());
            logd("setProperty %s", per->uid().toStdString().c_str());
            ui->txtSearch->setProperty(KItemUid, per->uid());
            QString nameid;
            if (mArea) {
                nameid = QString("%1_%2").arg(mArea->nameId(), per->nameId());
            }
            ui->txtNameId->setText(nameid);
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}

ErrCode DlgAreaPerson::setArea(const Area *newArea)
{
    tracein;
    ErrCode err = ErrNone;
    FREE_PTR(mArea);
    if (newArea) {
        logd("clone new area");
        mArea = CLONE_MODEL(newArea, Area);
        if (!mArea) {
            err = ErrNoMemory;
            loge("Clone new area fail, no memory?");
        }
    }
    traceret(err);
    return err;
}

void DlgAreaPerson::setAreaUid(const QString &id)
{
    mAreaUid = id;
}

void DlgAreaPerson::setAreaNameId(const QString &id)
{
    mAreaNameId = id;
}

QDialogButtonBox *DlgAreaPerson::buttonBox()
{
    return ui->buttonBox;
}

DbModel *DlgAreaPerson::newModel()
{
    return AreaPerson::build();
}

bool DlgAreaPerson::onValidateData(QString &msg)
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
