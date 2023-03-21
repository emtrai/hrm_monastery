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
 * Filename: areaperson.h
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#ifndef AREAPERSON_H
#define AREAPERSON_H

#include "mapdbmodel.h"

class AreaPerson : public MapDbModel
{
public:
    virtual ~AreaPerson();
    static DbModel* build();

    virtual void clone(const DbModel* model);
    virtual DbModelBuilder getBuilder();
    virtual QString modelName() const;
    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    DbModel *person() const;
    void setPerson(DbModel *newPerson);

    DbModel *area() const;
    void setArea(DbModel *newArea);

    const QString &roleName();
    void setRoleName(const QString &newRoleName);

protected:
    AreaPerson();
    virtual DbModelHandler *getDbModelHandler() const;
    void copy(const AreaPerson& model);
protected:
    QString mRoleUid;
    QString mRoleName;
    DbModel* mPerson;
    DbModel* mArea;
};

#endif // AREAPERSON_H
