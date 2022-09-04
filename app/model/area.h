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
 * Filename: area.h
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#ifndef AREA_H
#define AREA_H

#include <dbmodel.h>
#include <person.h>
#include <QHash>
#include <QList>
#include "country.h"

class Area : public DbModel
{
public:
    Area();
    static DbModel *builder();
    Country *getCountry() const;
    void setCountry(Country *newCountry);

    qint64 countryDbId() const;
    void setCountryDbId(qint64 newCountryDbId);

    qint64 personDbId() const;
    void setPersonDbId(qint64 newPersonDbId);

    const QString &remark() const;
    void setRemark(const QString &newRemark);


    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

protected:
    virtual DbModelHandler* getDbModelHandler();
private:
    Person* mManager;
    QHash<qint32, QList<Person*>> mMember; // role, list of member
    Country* country;
    qint64 mCountryDbId;
    qint64 mPersonDbId;
    QString mRemark;
    QString mCountryUid;

};

#endif // AREA_H