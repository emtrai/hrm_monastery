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
 * Filename: mission.h
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#ifndef MISSION_H
#define MISSION_H


#include <QString>
#include <QList>
#include <QObject>
#include "dbmodel.h"

// Su vu
class Mission : public DbModel
{
public:
    Mission();
    static DbModel *builder();
    virtual QString nameid() const;
    const QString &remark() const;
    void setRemark(const QString &newRemark);

    qint64 countryDbId() const;
    void setCountryDbId(qint64 newCountryDbId);

    const QString &countryShortName() const;
    void setCountryShortName(const QString &newCountryShortName);

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    qint64 mNameid;
    QString mCountryShortName;
    QString mRemark;
};

#endif // MISSION_H
