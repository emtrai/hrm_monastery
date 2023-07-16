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
class Area;
class AreaPerson : public MapDbModel
{
public:
    virtual ~AreaPerson();
    static DbModel* build();

    virtual void clone(const DbModel* model);
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;
    virtual ErrCode exportTemplatePath(FileExporter* exporter,
                                       const QString& name,
                                       QString& fpath,
                                       QString* ftype = nullptr) const;

    virtual QString buildNameId(const QString* seed = nullptr, bool* ok = nullptr);
    virtual void initExportFields();
    virtual void initImportFields();

    virtual const QString &name() const;


    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    const QString &roleName();
    void setRoleName(const QString &newRoleName);
    const QString &courseUid() const;
    void setCourseUid(const QString &newCourseUid);

    const QString &courseName() const;
    void setCourseName(const QString &newCourseName);

    const QString &personUid() const;
    void setPersonUid(const QString &newPersonUid);

    const QString &personName() const;
    void setPersonName(const QString &newPersonName);

    const QString &areaUid() const;
    void setAreaUid(const QString &newAreaUid);

    const QString &areaName() const;
    void setAreaName(const QString &newAreaName);

    const QString &areaNameId() const;
    void setAreaNameId(const QString &newAreaNameId);

    ErrCode setArea(const Area* area);

    const QString &hollyName() const;
    void setHollyName(const QString &newHollyName);

    const QString &personTel() const;
    void setPersonTel(const QString &newPersonTel);

    const QString &personEmail() const;
    void setPersonEmail(const QString &newPersonEmail);

    const QString &personNameId() const;
    void setPersonNameId(const QString &newPersonNameId);

    QString roleNameId() const;
    void setRoleNameId(const QString &newRoleNameId);

    QString courseNameId() const;
    void setCourseNameId(const QString &newCourseNameId);

protected:
    AreaPerson();
    virtual DbModelHandler *getDbModelHandler() const;
    void copy(const AreaPerson& model);
protected:
    QString mPersonUid;
    QString mHollyName; // just for display
    QString mPersonName; // just for display
    QString mPersonNameId; // just for display
    QString mPersonTel; // just for display
    QString mPersonEmail; // just for display
    QString mAreaUid;
    QString mAreaName; // just for display
    QString mAreaNameId; // just for display
    QString mRoleUid;
    QString mRoleNameId;
    QString mRoleName; // just for display
    QString mCourseUid;
    QString mCourseNameId;
    QString mCourseName; // just for display
};

#endif // AREAPERSON_H
