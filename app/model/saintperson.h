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
 * Filename: saintperson.h
 * Author: Anh, Ngo Huy
 * Created date:10/4/2022
 * Brief:
 */
#ifndef SAINTPERSON_H
#define SAINTPERSON_H

#include <dbmodel.h>
#include <QHash>

// Internal model
class SaintPerson : public DbModel
{
public:
    SaintPerson();
    static DbModel* build();
    virtual DbModelBuilder getBuilder() const;
    virtual QString buildUid(const QString* seed = nullptr);

    virtual QString modelName() const;

    const QString &personUid() const;
    void setPersonUid(const QString &newPersonUid);

    qint64 personDbId() const;
    void setPersonDbId(qint64 newPersonDbId);



    const QString &saintUid() const;
    void setSaintUid(const QString &newSaintUid);

    qint64 saintDbId() const;
    void setSaintDbId(qint64 newSaintDbId);

protected:
    virtual DbModelHandler *getDbModelHandler() const;
private:
    QString mPersonUid;
    qint64 mPersonDbId;
    QString mSaintUid;
    qint64 mSaintDbId;
};

#endif // SAINTPERSON_H
