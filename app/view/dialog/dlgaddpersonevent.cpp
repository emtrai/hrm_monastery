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
#include "eventctl.h"
#include "event.h"

DlgAddPersonEvent::DlgAddPersonEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddPersonEvent),
    mPersonEvent(nullptr)
{
    tracein;
    ui->setupUi(this);

    QList<DbModel*> eventList = INSTANCE(EventCtl)->getAllItemsFromDb(); // TODO: getAllItem???

    ui->cbEvent->clear();
    foreach(DbModel* item, eventList){
        ui->cbEvent->addItem(item->name(), item->uid());
    }

}

DlgAddPersonEvent::~DlgAddPersonEvent()
{
    tracein;
    delete ui;

    if (mPersonEvent != nullptr) {
        delete mPersonEvent;
        mPersonEvent = nullptr;
    }
}


void DlgAddPersonEvent::accept()
{
    tracein;
    ErrCode ret = ErrNone;
    QString name = ui->txtTitle->text().trimmed();
    QString date = ui->txtDate->text().trimmed();
    QString endDate = ui->txtEndDate->text().trimmed();
    QString remark = ui->txtRemark->toPlainText().trimmed();
    QString eventName;
    QString eventUid;
    ret = Utils::getCurrentComboxDataString(ui->cbEvent, &eventUid, &eventName);
    logd("get combox data ret %d", ret);
    if ((ret == ErrNone) && !name.isEmpty() && !eventUid.isEmpty()){
        logd("Setup Person");
        if (mPersonEvent == nullptr)
            mPersonEvent = new PersonEvent();
        if (mPersonEvent != nullptr){
            mPersonEvent->setName(name); // TODO: validate info

            if (!eventUid.isEmpty())
                mPersonEvent->setEventUid(eventUid);
            if (!date.isEmpty())
                mPersonEvent->setDate(Utils::dateFromString(date));

            if (!endDate.isEmpty())
                mPersonEvent->setEndDate(Utils::dateFromString(endDate));

            mPersonEvent->setRemark(remark);

            if (!eventName.isEmpty())
                mPersonEvent->setEventName(eventName);

        } else {
            ret = ErrNoMemory;
            loge("No memory");
        }
    } else {
        if (ret == ErrNone) ret = ErrInvalidData;
        loge("Title or event field is empty, or event error return %d", ret);
    }

    logi("Make PersonEvent, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(ret);
}

PersonEvent *DlgAddPersonEvent::personEvent() const
{
    return mPersonEvent;
}
