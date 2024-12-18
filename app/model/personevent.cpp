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
#include "utils.h"
#include "datetimeutils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "eventctl.h"
#include "prebuiltdefs.h"
#include "stringdefs.h"
#include "modeldefs.h"

PersonEvent::PersonEvent():DbModel(),
    mDate(0),
    mEndDate(0)
{
    traced;
}

PersonEvent::PersonEvent(const PersonEvent *model):
    DbModel((const DbModel *)model)
{
    tracein;
    setRemark(model->remark());
    setDate(model->date());
    setEndDate(model->endDate());
    setPersonUid(model->personUid());
    setEventUid(model->eventUid());
    setEventName(model->eventName());
    traceout;
}

DbModel *PersonEvent::build()
{
    PersonEvent* model = new PersonEvent();
    model->init();
    return model;
}

QString PersonEvent::makeNameId(const QString &perNameId, const QString &eventNameId, const QString& date)
{
    tracein;
    bool ok = false;
    QString nameId = perNameId.isEmpty()?"KHONG":perNameId;
    nameId += "_";
    nameId += eventNameId.isEmpty()?"KHONG":eventNameId;
    if (!date.isEmpty()) {
        nameId += "_";
        nameId += date;
    }
    logd("nameId '%s'", STR2CHA(nameId));
    nameId = Utils::UidFromName(nameId, NO_VN_MARK_UPPER, &ok);
    if (!ok) {
        nameId.clear();
    }
    logd("nameid '%s'", STR2CHA(nameId));
    traceout;
    return nameId;
}

ErrCode PersonEvent::buildNameIdFromOthersNameId(const QString &perNameId, const QString &eventNameId, const QString &date)
{
    tracein;
    ErrCode err = ErrNone;
    QString dateString = date;
    if (dateString.isEmpty()) {
        dateString = DatetimeUtils::currentTimeToDatestring (QT_DATE_FORMAT_DMYHMS);
    }
    QString nameId = makeNameId(perNameId,
                                eventNameId,
                                dateString);
    if (!nameId.isEmpty()) {
        setNameId(nameId);
    } else {
        err = ErrFailed;
        loge("Failed to build name id for '%s'", MODELSTR2CHA(this));
    }
    traceret(err);
    return err;
}

DbModelBuilder PersonEvent::getBuilder() const
{
    return &PersonEvent::build;
}

QString PersonEvent::modelName() const
{
    return KModelNamePersonEvent;
}

QString PersonEvent::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltPersonEventInfoTemplateFileName;

}

void PersonEvent::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                   PersonEvent,
                                                   KModelNamePersonEvent,
                                                   nameId));
    mExportCallbacks.insert(KItemEvent, EXPORT_CALLBACK_STRING_IMPL(
                                             PersonEvent,
                                             KModelNamePersonEvent,
                                             eventName));

    mExportCallbacks.insert(KItemTitle, EXPORT_CALLBACK_STRING_IMPL(
                                            PersonEvent,
                                            KModelNamePersonEvent,
                                            name));

    mExportCallbacks.insert(KItemStartDate, EXPORT_CALLBACK_STRING_IMPL(
                                                PersonEvent,
                                                KModelNamePersonEvent,
                                                dateString));

    mExportCallbacks.insert(KItemEndDate, EXPORT_CALLBACK_STRING_IMPL(
                                              PersonEvent,
                                              KModelNamePersonEvent,
                                              endDateString));

    traceout;
}

void PersonEvent::_copyData(const DbModel& model)
{
    tracein;
    const PersonEvent* event = static_cast<const PersonEvent*>(&model);
    setDate(event->mDate);
    setEndDate(event->mEndDate);
    setEventUid(event->mEventUid);
    setEventName(event->mEventName);// just for display
    setPersonUid(event->mPersonUid);
    setPersonName(event->mPersonName);
    traceout;
}
void PersonEvent::buildUidIfNotSet()
{
    tracein;
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

DbModelHandler *PersonEvent::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlPersonEvent);
//    return DB->getModelHandler(KModelHdlPerson);
}

QString PersonEvent::personName() const
{
    return mPersonName;
}

void PersonEvent::setPersonName(const QString &newPersonName)
{
    mPersonName = newPersonName;
}

QString PersonEvent::toString() const
{
    return QString("%1:date('%2'):event('%3')").arg(
        DbModel::toString(),DatetimeUtils::date2String(date()), STR2CHA(eventName()));
}

QString PersonEvent::toEventString() const
{
    tracein;
    QString event;
    QString sep = ", ";
    APPEND_STRING(event, dateString(), sep);
    APPEND_STRING(event, endDateString(), "-");
    APPEND_STRING(event, eventName(), ": ");
    APPEND_STRING(event, name(), sep);
    logd("event string '%s'", STR2CHA(event));
    traceout;
    return event;
}

qint64 PersonEvent::endDate() const
{
    return mEndDate;
}

QString PersonEvent::endDateString() const
{
    return DatetimeUtils::date2String(endDate(), DEFAULT_FORMAT_YMD);
}

void PersonEvent::setEndDate(qint64 newEndDate)
{
//    mEndDate = newEndDate;
    CHECK_MODIFIED_THEN_SET(mEndDate, newEndDate, KItemEndDate);
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
//    mEventUid = newEventUid;
    CHECK_MODIFIED_THEN_SET(mEventUid, newEventUid, KItemEvent);
}

ErrCode PersonEvent::setEventNameId(const QString &nameId)
{
    tracein;
    ErrCode err = ErrNone;
    logd("Search event name id '%s'", STR2CHA(nameId));
    DbModel* model = EVENTCTL->getModelByNameId(nameId);
    if (model) {
        setEventUid(model->uid());
        setEventName(model->name());
        delete model;
    } else {
        loge("Not found event name id '%s'", STR2CHA(nameId));
        err = ErrNotFound;
    }
    traceout;
    return err;
}

const QString &PersonEvent::personUid() const
{
    return mPersonUid;
}

void PersonEvent::setPersonUid(const QString &newPersonUid)
{
//    mPersonUid = newPersonUid;
    CHECK_MODIFIED_THEN_SET(mPersonUid, newPersonUid, KItemPerson);
}

qint64 PersonEvent::date() const
{
    return mDate;
}

QString PersonEvent::dateString() const
{
    return DatetimeUtils::date2String(date(), DEFAULT_FORMAT_YMD);
}

void PersonEvent::setDate(qint64 newDate)
{
    //    mDate = newDate;
    CHECK_MODIFIED_THEN_SET(mDate, newDate, KItemDate);
}

void PersonEvent::setDate(const QString &newDate, const QString& format)
{
    setDate(DatetimeUtils::dateFromString(newDate, format));
}

