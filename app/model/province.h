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
 * Filename: province.h
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#ifndef PROVINCE_H
#define PROVINCE_H

#include <QString>
#include <QList>
#include <QObject>
#include "dbmodel.h"

enum ProvinceType{
    PROVINCE = 0,
    CITY,
    STATE
};

class Province: public QObject, public DbModel
{
public:
    Province();
    static DbModel *builder();
//    virtual QString nameid() const;

    qint64 parentDbId() const;
    void setParentDbId(qint64 newParentDbId);



    qint64 countryDbId() const;
    void setCountryDbId(qint64 newCountryDbId);

    const QString &remark() const;
    void setRemark(const QString &newRemark);





    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    Province* parent;
    qint64 mParentDbId;
    qint64 mCountryDbId;
    QString mCountryUid;
    QString mParentUid;
    QString mRemark;
    QList<Province> mChildProvince; // City belongs to Province, Province belongs to state
};

#endif // PROVINCE_H
