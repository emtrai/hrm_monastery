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
 * Filename: personevent.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#include "personevent.h"
#include "logger.h"
#include "errcode.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

PersonEvent::PersonEvent():
    mDate(0),
    mEndDate(0)
{
    traced;
}

PersonEvent::PersonEvent(const PersonEvent *model):
    DbModel((const DbModel *)model)
{
    traced;
    setRemark(model->remark());
    setDate(model->date());
    setEndDate(model->endDate());
    setPersonUid(model->personUid());
    setEventUid(model->eventUid());
    setEventName(model->eventName());
}

DbModel *PersonEvent::build()
{
    PersonEvent* model = new PersonEvent();
    model->init();
    return model;
}

DbModelBuilder PersonEvent::getBuilder()
{
    return &PersonEvent::build;
}

QString PersonEvent::modelName() const
{
    return KModelNamePersonEvent;
}
void PersonEvent::buildUidIfNotSet()
{
    traced;
    if (uid().isEmpty()){
        bool isOk = false;
        QString uid = Utils::UidFromName(
            QString("%1%2%3%4").arg(personUid()).arg(eventUid()).arg(date()).arg(name().simplified().toLower()),
            UidNameConvertType::HASH_NAME, &isOk);
        // TODO: check isOk
        logd("Uuid %s", uid.toStdString().c_str());
        setUid(uid);
    }
}

DbModelHandler *PersonEvent::getDbModelHandler()
{
//    return DB->getModelHandler(KModelHdlPersonEvent);
    return DB->getModelHandler(KModelHdlPerson);
}

qint64 PersonEvent::endDate() const
{
    return mEndDate;
}

void PersonEvent::setEndDate(qint64 newEndDate)
{
    mEndDate = newEndDate;
}

const QString &PersonEvent::eventName() const
{
    return mEventName;
}

void PersonEvent::setEventName(const QString &newEventName)
{
    mEventName = newEventName;
}

const QString &PersonEvent::eventUid() const
{
    return mEventUid;
}

void PersonEvent::setEventUid(const QString &newEventUid)
{
    mEventUid = newEventUid;
}

const QString &PersonEvent::personUid() const
{
    return mPersonUid;
}

void PersonEvent::setPersonUid(const QString &newPersonUid)
{
    mPersonUid = newPersonUid;
}

qint64 PersonEvent::date() const
{
    return mDate;
}

void PersonEvent::setDate(qint64 newDate)
{
    mDate = newDate;
}

