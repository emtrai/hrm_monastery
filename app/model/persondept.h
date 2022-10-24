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

class Community;
class Department;

// Department managers
class PersonDept : public DbModel
{
public:
    PersonDept();
    static DbModel* build();

    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

    Department *department() const;
    void setDepartment(Department *newDepartment);

    virtual QString modelName() const;


    const QString &departUid() const;
    void setDepartUid(const QString &newDepartUid);

    const QString &courseUid() const;
    void setCourseUid(const QString &newCourseUid);

    const QString &courseName() const;
    void setCourseName(const QString &newCourseName);

    Person *person() const;
    void setPerson(Person *newPerson);

    virtual void dump();

    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);
    void setStartDate(const QString&  newStartDate,
                     const QString& format = DATE_FORMAT_DMY);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);
    void setEndDate(const QString&  newEndDate,
                      const QString& format = DATE_FORMAT_DMY);

    int status() const;
    void setStatus(int newStatus);

    const QString &remark() const;
    void setRemark(const QString &newRemark);

protected:
    virtual DbModelHandler *getDbModelHandler();
protected:
    QString mRoleUid;
    QString mRoleName;
    QString mCourseUid;
    QString mCourseName;
    QString mDepartUid;
    qint64 mStartDate;
    qint64 mEndDate;
    Department* mDepartment;
    Person* mPerson;
    int mStatus;
    QString mRemark;
};

#endif // PERSONDEPT_H
