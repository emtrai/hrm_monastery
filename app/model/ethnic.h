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
 * Filename: ethnic.h
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#ifndef ETHNIC_H
#define ETHNIC_H

#include <QString>
#include <QList>
#include <QObject>
#include "dbmodel.h"

class Ethnic : public DbModel
{
public:
    Ethnic();
    static DbModel *build();
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;

    virtual void initExportFields();
    virtual QString exportHtmlTemplateFile(const QString& name) const;

    qint64 countryDbId() const;
    void setCountryDbId(qint64 newCountryDbId);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

    const QString &countryName() const;
    void setCountryName(const QString &newCountryName);
    virtual QString toString() const;

protected:
    virtual void _copyData(const DbModel& model);
    virtual DbModelHandler *getDbModelHandler() const;
private:
    qint64 mCountryDbId;
    QString mCountryUid;
    QString mCountryName; // just for informative only, not save to db
};

#endif // ETHNIC_H
