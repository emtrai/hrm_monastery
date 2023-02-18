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
    virtual QString modelName() const;

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

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    qint64 mDate;
    qint64 mEndDate;
    QString mEventUid;
    QString mEventName;
    QString mPersonUid;
};

#endif // PERSONEVENT_H
