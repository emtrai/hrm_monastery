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
protected:
    Area();
public:
    virtual ~Area();
    static DbModel *build();
    virtual void clone(const DbModel* model);

    virtual QString modelName() const;

    qint64 countryDbId() const;
    void setCountryDbId(qint64 newCountryDbId);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

    QString countryName() const;
    void setCountryName(QString newCountryName);

    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);

    qint32 modelStatus() const;
    void setModelStatus(qint32 newModelStatus);

    const QString &changeHistory() const;
    void setChangeHistory(const QString &newChangeHistory);

    const QString &tel() const;
    void setTel(const QString &newTel);

    const QString &email() const;
    void setEmail(const QString &newEmail);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    QString modelStatusName() const;
    void setModelStatusName(QString newModelStatusName);

    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;

    virtual void initExportFields();
protected:
    virtual DbModelHandler* getDbModelHandler() const;
    virtual DbModelBuilder getBuilder() const;
    void copy(const Area& model);
private:
    QString mCountryName; // just for display, not store to db
    qint64 mCountryDbId;
    QString mCountryUid;

    QString mAddr;
    QString mTel;
    QString mEmail;
    qint64 mStartDate;
    qint64 mEndDate;
    qint32 mModelStatus;
    QString mModelStatusName; // just for display, not store to db
    QString mChangeHistory;

};

#endif // AREA_H
