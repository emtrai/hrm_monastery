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
 * Filename: personevent.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef PERSONEVENT_H
#define PERSONEVENT_H

#include <QObject>
#include <dbmodel.h>

class PersonEvent : public DbModel
{

public:
    PersonEvent();
    PersonEvent(const PersonEvent* model);
    static DbModel *build();
    static QString makeNameId(const QString& perNameId, const QString& eventNameId, const QString& date);
    ErrCode buildNameId(const QString& perNameId, const QString& eventNameId, const QString& date = nullptr);
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;
    virtual void clone(const DbModel* model);
    /**
     * @brief Copy data only, except identity such as uid, nameid, dbid
     * @param model
     */
    virtual ErrCode copyData(const DbModel* model);
    virtual void buildUidIfNotSet();

    qint64 date() const;
    void setDate(qint64 newDate);

    const QString &personUid() const;
    void setPersonUid(const QString &newPersonUid);

    const QString &eventUid() const;
    void setEventUid(const QString &newEventUid);

    const QString &eventName() const;
    void setEventName(const QString &newEventName);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);

    QString personName() const;
    void setPersonName(const QString &newPersonName);
    virtual QString toString() const;

protected:
    virtual DbModelHandler *getDbModelHandler() const;
private:
    qint64 mDate;
    qint64 mEndDate;
    QString mEventUid;
    QString mEventName; // for display only
    QString mPersonUid;
    QString mPersonName; // for display only
};

#endif // PERSONEVENT_H
