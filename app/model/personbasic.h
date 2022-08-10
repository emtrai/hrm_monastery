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
 * Filename: personbasic.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef PERSONBASIC_H
#define PERSONBASIC_H

#include <QString>
#include <QObject>
#include "errcode.h"

class Address;

class PersonBasic: public QObject
{
public:
    PersonBasic();
    const QString &firstName() const;
    void setFirstName(const QString &newFirstName);

    Address *permanentAddr() const;
    void setPermanentAddr(Address *newPermanentAddr);

    Address *livingAddr() const;
    void setLivingAddr(Address *newLivingAddr);

    const QString &birthPlace() const;
    void setBirthPlace(const QString &newBirthPlace);

    qint64 birthday() const;
    void setBirthday(qint64 newBirthday);

    const QString &lastName() const;
    void setLastName(const QString &newLastName);


    ErrCode setNameFromFullName(const QString& name);

    QString getFullName();
private:

        QString mFirstName;
        QString mLastName;
        qint64 mBirthday;
        QString mBirthPlace;
        Address* mPermanentAddr;
        Address* mLivingAddr;
};

#endif // PERSONBASIC_H
