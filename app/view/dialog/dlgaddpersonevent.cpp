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
 * Filename: dlgaddpersonevent.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/15/2022
 * Brief:
 */
#include "dlgaddpersonevent.h"
#include "ui_dlgaddpersonevent.h"
#include "personctl.h"
#include "dbmodel.h"
#include "logger.h"
#include "errcode.h"
#include "personevent.h"
#include "utils.h"
#include "datetimeutils.h"
#include "eventctl.h"
#include "event.h"
#include "person.h"
#include "dlgsearchperson.h"

DlgAddPersonEvent::DlgAddPersonEvent(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgAddPersonEvent),
    mPerson(nullptr),
    mEvent(nullptr),
    mEvenInfoOnly(false)
{
    tracein;
    ui->setupUi(this);
    loadEvent();
}

DlgAddPersonEvent::~DlgAddPersonEvent()
{
    tracein;
    delete ui;
    if (mPerson != nullptr) {
        delete mPerson;
        mPerson = nullptr;
    }
    if (mEvent != nullptr) {
        delete mEvent;
        mEvent = nullptr;
    }
    RELEASE_LIST_DBMODEL(mEventList);
    traceout;
}

ErrCode DlgAddPersonEvent::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid argument, null model");
    }

//    if (err == ErrNone && !mPerson) {
//        err = ErrNoData;
//        loge("No person object to be set");
//    }
    if (err == ErrNone) {
        PersonEvent * per = (PersonEvent*) model;
        if (err == ErrNone){
            per->setMarkModified(true); // start marking fields which are modified
        }
        per->setName(ui->txtTitle->text().trimmed());
        SET_VAL_FROM_CBOX(ui->cbEvent, per->setEventUid, per->setEventName, err);
        SET_DATE_VAL_FROM_WIDGET(ui->txtDate, per->setDate);
        SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, per->setEndDate);
        if (!mEvenInfoOnly) {
            SET_VAL_FROM_EDITBOX(ui->txtPersonName, KItemPerson, per->setPersonUid, per->setPersonName);
            QString nameid = ui->txtNameId->text().trimmed();
            if (!nameid.isEmpty()) {
                per->setNameId(nameid);
            } else {
                err = ErrNoData;
                loge("Lack of nameId");
            }
        } else {
            logd("Event info only, skip person & name id setting");
        }
        QString remark = ui->txtRemark->toPlainText().trimmed();
        if (!remark.isEmpty()) {
            per->setRemark(remark);
        }
    }
    traceret(err);
    return err;

}

ErrCode DlgAddPersonEvent::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNamePersonEvent) {
        err = DlgCommonEditModel::fromModel(item);
        PersonEvent* comm = (PersonEvent*)model();
        if (err == ErrNone) {
            if (!comm->eventUid().isEmpty()) {
                setSelectedEvent(comm->eventUid());
            }
            Utils::setSelectItemComboxByData(ui->cbEvent, comm->eventUid());
            ui->txtTitle->setText(comm->name());
            ui->txtDate->setText(comm->dateString());
            ui->txtEndDate->setText(comm->endDateString());
            ui->txtRemark->setPlainText(comm->remark());
            ui->btnSearch->setEnabled(false); // not allow to change person
            if (!mEvenInfoOnly) {
                QString nameid = comm->nameId();
                if (!comm->personUid().isEmpty()) {
                    logd("Search person uid '%s'", STR2CHA(comm->personUid()));
                    Person* per = (Person*)PERSONCTL->getModelByUid(comm->personUid());
                    if (per) {
                        setPerson(per);
                        delete per;
                    } else {
                        loge("Not found person uid '%s'", STR2CHA(comm->personUid()));
                    }
                } else {
                    loge("No person uid");
                }
                if (nameid.isEmpty()) {
                    updateNameId();
                }
                ui->txtNameId->setText(nameid);
            } else {
                logd("event info only, no name id nor person");
            }
        }
    } else {
        err = ErrInvalidArg;
        loge("null item or invalid model '%s'", MODELNAME2CHA(item));
    }
    traceret(err);
    return err;

}

void DlgAddPersonEvent::setSelectedEvent(int index)
{
    tracein;
    logd("set selected idx %d", index);
    if (index >= 0 && index < mEventList.size()) {
        QVariant value = ui->cbEvent->itemData(index);
        if (!value.isNull()) {
            DbModel* event = mEventList.at(index);
            if (event != nullptr) {
                if (event->uid() == value.toString()) {
                    if (mEvent) delete mEvent;
                    mEvent = (Event*)event->clone();
                    updateNameId();
                } else {
                    loge("something went wrong, list uid '%s' not match with cb uid '%s'",
                         STR2CHA(event->toString()),
                         STR2CHA(value.toString())
                         );
                }
            }
        }
    } else {
        loge("invalid index %d", index);
    }
    traceout;
}

void DlgAddPersonEvent::updateNameId()
{
    tracein;
    if (!mEvenInfoOnly) {
        QString nameId = PersonEvent::makeNameId(
            mPerson?mPerson->nameId():"KHONG",
            mEvent?mEvent->nameId():"KHONG",
            ui->txtDate->text());
//        QString nameId = mPerson?mPerson->nameId():"KHONG";
//        nameId += "_";
//        nameId += mEvent?mEvent->nameId():"KHONG";
//        nameId += "_";
//        nameId += ui->txtDate->text();
        DlgCommonEditModel::onChangeNameIdTxt(ui->txtNameId, nameId, true);
    } else {
        logd("event info only, skip setting name id");
        DlgCommonEditModel::onChangeNameIdTxt(ui->txtNameId, "");
    }
    traceout;
}

bool DlgAddPersonEvent::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        PersonEvent* event = (PersonEvent*)mModel;
        if (event->nameId().isEmpty() && !mEvenInfoOnly) {
            msg += tr("Thiếu mã định danh.");
            isValid = false;
            logw("lack name id");
        }
        if (event->date() == 0) {
            msg += tr("Thiếu ngày/tháng");
            isValid = false;
            logw("lack date");
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

void DlgAddPersonEvent::loadEvent()
{
    tracein;
    RELEASE_LIST_DBMODEL(mEventList);
    mEventList = INSTANCE(EventCtl)->getAllItems(); // TODO: getAllItem???
    logd("the number of eventList %lld", mEventList.count());
    ui->cbEvent->clear();

    if (mEventList.size() > 0) {
        foreach(DbModel* item, mEventList){
            ui->cbEvent->addItem(item->name(), item->uid());
            // TODO: data is sorted???
        }
        ui->cbEvent->setCurrentIndex(0);
        setSelectedEvent(0);
    } else {
        logi("Not data to load");
    }
    traceout;
}

void DlgAddPersonEvent::setSelectedEvent(const QString &eventUid)
{
    tracein;
    int index = -1;
    ErrCode err = Utils::setSelectItemComboxByData(ui->cbEvent, eventUid, &index);
    if (err == ErrNone && index >= 0) {
        logd("event idx = %d", index);
        setSelectedEvent(index);
    } else {
        loge("set event failed, err = %d", err);
    }
    traceout;
}

QDialogButtonBox *DlgAddPersonEvent::buttonBox()
{
    return ui->buttonBox;
}

DbModel *DlgAddPersonEvent::newModel()
{
    return PersonEvent::build();
}

void DlgAddPersonEvent::on_btnSearch_clicked()
{
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(false);
    // TODO: support multi selection for person

    if (dlg->exec() == QDialog::Accepted){
        const Person* per = (const Person*)dlg->selectedItem();
        if (per != nullptr) {
            setPerson(per);
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}


void DlgAddPersonEvent::on_btnChangeNameId_clicked()
{
    if (!mEvenInfoOnly) {
        DlgCommonEditModel::onEditnameId(ui->txtNameId);
        if (ui->txtNameId->text().length() == 0) { // custome nameid is null, make it auto generate
            updateNameId();
        }
    } else {
        logw("event infor only, this event should not occur");
    }
}

void DlgAddPersonEvent::on_txtDate_textChanged(const QString &arg1)
{
    if (!mEvenInfoOnly) {
        updateNameId();
    }
}


void DlgAddPersonEvent::on_cbEvent_currentIndexChanged(int index)
{
    tracein;
    if (!mEvenInfoOnly) {
        setSelectedEvent(index);
    }
    traceout;
}

void DlgAddPersonEvent::setEvenInfoOnly(const QList<DbModel*>* listPerson)
{
    tracein;
    mEvenInfoOnly = true;
    logd("event info only, skip change name and search person");
    ui->btnChangeNameId->setEnabled(false);
    ui->btnSearch->setEnabled(false);
    ui->txtNameId->setText("");
    ui->txtNameId->setEnabled(false);
    if (listPerson && listPerson->size() > 0) {
        logd("set list person");
        foreach (DbModel* item, *listPerson) {
            Person* per = (Person*) item;
            if (per) {
                ui->txtPersonName->appendPlainText(per->displayName());
            } else {
                logw("something went wrong, person should not null here!!");
            }
        }
    }
    traceout;
}

const Event *DlgAddPersonEvent::getSelectedEvent() const
{
    return mEvent;
}

ErrCode DlgAddPersonEvent::setPerson(const Person *newPerson)
{
    tracein;
    ErrCode err = ErrNone;
    if (!mEvenInfoOnly) {
        if (mPerson) {
            delete mPerson;
            mPerson = nullptr;
        }
        if (newPerson) {
            logd("clone new person");
            mPerson = (Person*)(((DbModel*)newPerson)->clone());
            ui->txtPersonName->setPlainText(mPerson->displayName());
    //        ui->lblNameId->setText(mPerson->nameId());
            logd("setProperty %s", mPerson->uid().toStdString().c_str());
            ui->txtPersonName->setProperty(KItemPerson, mPerson->uid());
            ui->btnSearch->setEnabled(false); // not allow to change person info
            updateNameId();
        } else {
            logw("no person is set");
        }
    } else {
        err = ErrNotSupport;
        loge("Event info only, no person set");
    }
    traceout;
    return err;
}

