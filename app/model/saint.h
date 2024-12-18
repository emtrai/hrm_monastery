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
 * Filename: saint.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef SAINT_H
#define SAINT_H

#include <QObject>
#include <QString>

#include <QList>
#include <QHash>
#include <QMap>
#include <functional>
#include <iostream>

#include "defs.h"
#include "errcode.h"
#include "dbmodel.h"
#include "dataexporter.h"
#include "idataimporter.h"



class Saint: public DbModel
{
public:
    static DbModel* build();
public:
    Saint();
    virtual DbModelBuilder getBuilder() const;

    void initImportFields();
    virtual ErrCode onImportParseDataItem(const QString& importName, int importFileType,
                                 const QString& keyword, const QString& value,
                                 quint32 idx = 0, QList<DbModel *>* outList = nullptr);

    virtual QString modelName() const;

    qint64 feastDay() const;
    void setFeastDay(qint64 newFeastDay);
    void setFeastDay(const QString& newFeastDay, const QString& f = "D-M");

    Gender gender() const;
    void setGender(Gender newGender);
    void setGender(const QString& gender);

    const QString &country() const;
    void setCountry(const QString &newCountry);

    virtual QString fullName() const;
    void setFullName(const QString &newFullName);

    virtual void dump() const;

public:
    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

    const QString &originName() const;
    void setOriginName(const QString &newOriginName);


protected:
    virtual DbModelHandler *getDbModelHandler() const;
    virtual void _copyData(const DbModel& model);

private:
    QString mFullName;
    QString mOriginName;
    Gender mGender;
    qint64 mFeastDay; // ngay bon mang
    QString mCountry;
    QString mCountryUid;
};


#endif // SAINT_H
