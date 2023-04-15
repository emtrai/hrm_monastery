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
#include "dataexporter.h"
#include "exportfactory.h"
#include "defs.h"
#include <QMap>
#include "dbpersonmodelhandler.h"
#include "person.h"
#include "department.h"

PersonDept::PersonDept():mModelStatus(0)
{
    tracein;
}

DbModel *PersonDept::build()
{
    PersonDept* model = new PersonDept();
    model->init();
    return model;
}

DbModelBuilder PersonDept::getBuilder() const
{
    return &PersonDept::build;
}

void PersonDept::clone(const DbModel *model)
{
    tracein;
    DbModel::clone(model);
    if (model && model->modelName() == KModelNamePersonDept) {
        copy(*((PersonDept*)model));
    } else {
        loge("cloned failed, model is null or invalid name '%s'", model?STR2CHA(model->toString()):"null");
    }
    traceout;
}

QString PersonDept::toString() const
{
    QString name = DbModel::toString();
    name.append(":" + personUid());
    name.append(":" + commDeptUid());
    return name;
}

const QString &PersonDept::roleUid() const
{
    return mRoleUid;
}

void PersonDept::setRoleUid(const QString &newRoleUid)
{
    CHECK_MODIFIED_THEN_SET(mRoleUid, newRoleUid, KItemRole);
//    mRoleUid = newRoleUid;
}

const QString &PersonDept::roleName() const
{
    return mRoleName;
}

void PersonDept::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

QString PersonDept::modelName() const
{
    return KModelNamePersonDept;
}


const QString &PersonDept::courseUid() const
{
    return mCourseUid;
}

void PersonDept::setCourseUid(const QString &newCourseUid)
{
    CHECK_MODIFIED_THEN_SET(mCourseUid, newCourseUid, KItemCourse);
//    mCourseUid = newCourseUid;
}

const QString &PersonDept::courseName() const
{
    return mCourseName;
}

void PersonDept::setCourseName(const QString &newCourseName)
{
    mCourseName = newCourseName;
}


void PersonDept::dump()
{
    tracein;
    DbModel::dump();
    logd("- Comm Dept Uid %s", mCommDeptUid.toStdString().c_str());
    logd("- Person Uid %s", mPersonUid.toStdString().c_str());
    logd("- CourseUid %s", courseUid().toStdString().c_str());
    logd("- CourseName %s", courseName().toStdString().c_str());
    logd("- RoleUid %s", roleUid().toStdString().c_str());
    logd("- RoleName %s", roleName().toStdString().c_str());

}

DbModelHandler *PersonDept::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCommDept); // TODO: suitable???
}

void PersonDept::copy(const PersonDept &model)
{
    tracein;
    mRoleUid = model.mRoleUid;
    mRoleName = model.mRoleName;
    mCourseUid = model.mCourseUid;
    mCourseName = model.mCourseName;
    mStartDate = model.mStartDate;
    mEndDate = model.mEndDate;
    mModelStatusName = model.mModelStatusName;
    mModelStatus = model.mModelStatus;
    mCommDeptUid = model.mCommDeptUid;
    mCommDeptNameId = model.mCommDeptNameId;
    mPersonUid = model.mPersonUid;
    mPersonName = model.mPersonName;
    mPersonNameId = model.mPersonNameId;
    traceout;
}

const QString &PersonDept::commDeptNameId() const
{
    return mCommDeptNameId;
}

void PersonDept::setCommDeptNameId(const QString &newCommDeptNameId)
{
    mCommDeptNameId = newCommDeptNameId;
}

const QString &PersonDept::personNameId() const
{
    return mPersonNameId;
}

void PersonDept::setPersonNameId(const QString &newPersonNameId)
{
    mPersonNameId = newPersonNameId;
}

const QString &PersonDept::modelStatusName() const
{
    return mModelStatusName;
}

void PersonDept::setModelStatusName(const QString &newModelStatusName)
{
    mModelStatusName = newModelStatusName;
}

const QString &PersonDept::personName() const
{
    return mPersonName;
}

void PersonDept::setPersonName(const QString &newPersonName)
{
    mPersonName = newPersonName;
}

const QString &PersonDept::changeHistory() const
{
    return mChangeHistory;
}

void PersonDept::setChangeHistory(const QString &newChangeHistory)
{
    CHECK_MODIFIED_THEN_SET(mChangeHistory, newChangeHistory, KItemChangeHistory);
}

const QString &PersonDept::personUid() const
{
    return mPersonUid;
}

void PersonDept::setPersonUid(const QString &newPersonUid)
{
    CHECK_MODIFIED_THEN_SET(mPersonUid, newPersonUid, KItemPerson);
}

const QString &PersonDept::commDeptUid() const
{
    return mCommDeptUid;
}

void PersonDept::setCommDeptUid(const QString &newCommDeptUid)
{
    CHECK_MODIFIED_THEN_SET(mCommDeptUid, newCommDeptUid, KItemCommunityDept);
}

int PersonDept::modelStatus() const
{
    return mModelStatus;
}

void PersonDept::setModelStatus(int newStatus)
{
    CHECK_MODIFIED_THEN_SET(mModelStatus, newStatus, KItemStatus);
}

qint64 PersonDept::endDate() const
{
    return mEndDate;
}

void PersonDept::setEndDate(qint64 newEndDate)
{
    CHECK_MODIFIED_THEN_SET(mEndDate, newEndDate, KItemEndDate);
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
    CHECK_MODIFIED_THEN_SET(mStartDate, newStartDate, KItemStartDate);
}

void PersonDept::setStartDate(const QString &newStartDate, const QString &format)
{
    setStartDate(Utils::dateFromString(newStartDate, format));
}
