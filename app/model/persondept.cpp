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
 * Filename: persondept.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/18/2022
 * Brief:
 */
#include "persondept.h"
#include "logger.h"
#include "errcode.h"
#include "dbctl.h"
#include "utils.h"
#include "filectl.h"
#include "iexporter.h"
#include "exportfactory.h"
#include "defs.h"
#include <QMap>
#include "dbpersonmodelhandler.h"
#include "person.h"
#include "department.h"

PersonDept::PersonDept():
    mDepartment(nullptr),
    mPerson(nullptr)
{

}

DbModel *PersonDept::build()
{
    return new PersonDept();
}

const QString &PersonDept::roleUid() const
{
    return mRoleUid;
}

void PersonDept::setRoleUid(const QString &newRoleUid)
{
    mRoleUid = newRoleUid;
}

const QString &PersonDept::roleName() const
{
    return mRoleName;
}

void PersonDept::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

Department *PersonDept::department() const
{
    return mDepartment;
}

void PersonDept::setDepartment(Department *newDepartment)
{
    mDepartment = newDepartment;
}

QString PersonDept::modelName() const
{
    return KModelNamePersonDept;
}

const QString &PersonDept::departUid() const
{
    if (mDepartment != nullptr)
        return mDepartment->uid();
    else
        return mDepartUid;
}

void PersonDept::setDepartUid(const QString &newDepartUid)
{
    mDepartUid = newDepartUid;
}

const QString &PersonDept::courseUid() const
{
    return mCourseUid;
}

void PersonDept::setCourseUid(const QString &newCourseUid)
{
    mCourseUid = newCourseUid;
}

const QString &PersonDept::courseName() const
{
    return mCourseName;
}

void PersonDept::setCourseName(const QString &newCourseName)
{
    mCourseName = newCourseName;
}

Person *PersonDept::person() const
{
    return mPerson;
}

void PersonDept::setPerson(Person *newPerson)
{
    mPerson = newPerson;
}

void PersonDept::dump()
{
    traced;
    DbModel::dump();
    if (mPerson != nullptr) {
        mPerson->dump();
    }
    logd("- CourseUid %s", courseUid().toStdString().c_str());
    logd("- CourseName %s", courseName().toStdString().c_str());
    logd("- RoleUid %s", roleUid().toStdString().c_str());
    logd("- RoleName %s", roleName().toStdString().c_str());
    logd("- departUid %s", departUid().toStdString().c_str());
    if (mDepartment != nullptr) {
        mDepartment->dump();
    }
}

DbModelHandler *PersonDept::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlDept); // TODO: suitable???
}

const QString &PersonDept::remark() const
{
    return mRemark;
}

void PersonDept::setRemark(const QString &newRemark)
{
    mRemark = newRemark;
}

int PersonDept::status() const
{
    return mStatus;
}

void PersonDept::setStatus(int newStatus)
{
    mStatus = newStatus;
}

qint64 PersonDept::endDate() const
{
    return mEndDate;
}

void PersonDept::setEndDate(qint64 newEndDate)
{
    mEndDate = newEndDate;
}

void PersonDept::setEndDate(const QString &newEndDate, const QString &format)
{
    setEndDate(Utils::dateFromString(newEndDate, format));
}

qint64 PersonDept::startDate() const
{
    return mStartDate;
}

void PersonDept::setStartDate(qint64 newStartDate)
{
    mStartDate = newStartDate;
}

void PersonDept::setStartDate(const QString &newStartDate, const QString &format)
{
    setStartDate(Utils::dateFromString(newStartDate, format));
}
