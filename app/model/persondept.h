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
 * Filename: persondept.h
 * Author: Anh, Ngo Huy
 * Created date:10/18/2022
 * Brief:
 */
#ifndef PERSONDEPT_H
#define PERSONDEPT_H

#include "person.h"
#include "dbmodel.h"
#include "datetimeutils.h"

class Community;
class Department;

// Department managers
class PersonDept : public DbModel
{
public:
    PersonDept();
    static DbModel* build();
    virtual DbModelBuilder getBuilder() const;

    virtual QString toString() const;
    virtual QString exportHtmlTemplateFile(const QString& name) const;
    virtual void initExportFields();

    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

    virtual QString modelName() const;


    const QString &courseUid() const;
    void setCourseUid(const QString &newCourseUid);

    const QString &courseName() const;
    void setCourseName(const QString &newCourseName);

    virtual void dump() const;

    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);
    void setStartDate(const QString&  newStartDate,
                     const QString& format = DEFAULT_FORMAT_YMD);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);
    void setEndDate(const QString&  newEndDate,
                      const QString& format = DEFAULT_FORMAT_YMD);

    int modelStatus() const;
    void setModelStatus(int newStatus);

    const QString &commDeptUid() const;
    void setCommDeptUid(const QString &newCommDeptUid);

    const QString &personUid() const;
    void setPersonUid(const QString &newPersonUid);

    const QString &changeHistory() const;
    void setChangeHistory(const QString &newChangeHistory);

    const QString &personName() const;
    void setPersonName(const QString &newPersonName);

    const QString &modelStatusName() const;
    void setModelStatusName(const QString &newModelStatusName);

    const QString &personNameId() const;
    void setPersonNameId(const QString &newPersonNameId);

    const QString &commDeptNameId() const;
    void setCommDeptNameId(const QString &newCommDeptNameId);

    QString personHollyName() const;
    void setPersonHollyName(const QString &newPersonHollyName);

    QString commDeptName() const;
    void setCommDeptName(const QString &newCommDeptName);

    QString roleNameId() const;
    void setRoleNameId(const QString &newRoleNameId);

    QString courseNameId() const;
    void setCourseNameId(const QString &newCourseNameId);

    QString personEmail() const;
    void setPersonEmail(const QString &newPersonEmail);

    QString personTel() const;
    void setPersonTel(const QString &newPersonTel);

protected:
    virtual DbModelHandler *getDbModelHandler() const;
    virtual void _copyData(const DbModel& model);
protected:
    QString mRoleUid;
    QString mRoleName; // not store to db
    QString mRoleNameId; // not store to db
    QString mCourseUid;
    QString mCourseName; // not store to db
    QString mCourseNameId; // not store to db
    qint64 mStartDate;
    qint64 mEndDate;
    int mModelStatus;
    QString mModelStatusName; // not store to db
    QString mCommDeptUid;
    QString mCommDeptNameId; // not store to db
    QString mCommDeptName; // not store to db
    QString mPersonUid;
    QString mPersonName; // not store to db
    QString mPersonNameId; // not store to db
    QString mPersonHollyName; // not store to db
    QString mPersonEmail; // not store to db
    QString mPersonTel; // not store to db
    QString mChangeHistory;

};

#endif // PERSONDEPT_H
