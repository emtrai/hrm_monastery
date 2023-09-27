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
#include "stringdefs.h"
#include "utils.h"
#include "datetimeutils.h"
#include "filectl.h"
#include "dataexporter.h"
#include "exportfactory.h"
#include "defs.h"
#include <QMap>
#include "dbpersonmodelhandler.h"
#include "person.h"
#include "department.h"
#include "prebuiltdefs.h"

PersonDept::PersonDept():DbModel(),
    mModelStatus(0)
{
    traced;
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

QString PersonDept::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltCommunityDeptPersonInfoTemplateName;

}

void PersonDept::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemPersonNameId, EXPORT_CALLBACK_STRING_IMPL(
                                              PersonDept,
                                              KModelNamePersonDept,
                                              personNameId));

    mExportCallbacks.insert(KItemDepartmentName, EXPORT_CALLBACK_STRING_IMPL(
                                                   PersonDept,
                                                   KModelNamePersonDept,
                                                   commDeptName));

    mExportCallbacks.insert(KItemDepartmentNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                     PersonDept,
                                                     KModelNamePersonDept,
                                                     commDeptNameId));

    mExportCallbacks.insert(KItemPersonName, EXPORT_CALLBACK_STRING_IMPL(
                                                       PersonDept,
                                                       KModelNamePersonDept,
                                                       personName));

    mExportCallbacks.insert(KItemPersonNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                 PersonDept,
                                                 KModelNamePersonDept,
                                                 personNameId));

    mExportCallbacks.insert(KItemHollyName, EXPORT_CALLBACK_STRING_IMPL(
                                                   PersonDept,
                                                   KModelNamePersonDept,
                                                   personHollyName));

    mExportCallbacks.insert(KItemRole, EXPORT_CALLBACK_STRING_IMPL(
                                                PersonDept,
                                                KModelNamePersonDept,
                                                roleName));


    mExportCallbacks.insert(KItemRoleNameId, EXPORT_CALLBACK_STRING_IMPL(
                                           PersonDept,
                                           KModelNamePersonDept,
                                           roleNameId));


    mExportCallbacks.insert(KItemTerm, EXPORT_CALLBACK_STRING_IMPL(
                                                 PersonDept,
                                                 KModelNamePersonDept,
                                                 courseName));


    mExportCallbacks.insert(KItemTermNameId, EXPORT_CALLBACK_STRING_IMPL(
                                           PersonDept,
                                           KModelNamePersonDept,
                                           courseNameId));

    mExportCallbacks.insert(KItemEmail, EXPORT_CALLBACK_STRING_IMPL(
                                                 PersonDept,
                                                 KModelNamePersonDept,
                                                 personEmail));

    mExportCallbacks.insert(KItemTel, EXPORT_CALLBACK_STRING_IMPL(
                                            PersonDept,
                                            KModelNamePersonDept,
                                            personTel));

    mExportCallbacks.insert(KItemStartDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                          PersonDept,
                                          KModelNamePersonDept,
                                          startDate,
                                          DEFAULT_FORMAT_YMD));

    mExportCallbacks.insert(KItemEndDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                                PersonDept,
                                                KModelNamePersonDept,
                                                endDate,
                                                DEFAULT_FORMAT_YMD));


    mExportCallbacks.insert(KItemStatus, EXPORT_CALLBACK_STRING_IMPL(
                                              PersonDept,
                                              KModelNamePersonDept,
                                              modelStatusName));

    mExportCallbacks.insert(KItemStatusId, EXPORT_CALLBACK_INT_IMPL(
                                             PersonDept,
                                             KModelNamePersonDept,
                                             modelStatus));


    mExportCallbacks.insert(KItemChangeHistory, EXPORT_CALLBACK_STRING_IMPL(
                                             PersonDept,
                                             KModelNamePersonDept,
                                             changeHistory));
    traceout;
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


void PersonDept::dump() const
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
    mRoleNameId = model.mRoleNameId;
    mCourseUid = model.mCourseUid;
    mCourseName = model.mCourseName;
    mCourseNameId = model.mCourseNameId;
    mStartDate = model.mStartDate;
    mEndDate = model.mEndDate;
    mModelStatusName = model.mModelStatusName;
    mModelStatus = model.mModelStatus;
    mCommDeptUid = model.mCommDeptUid;
    mCommDeptNameId = model.mCommDeptNameId;
    mCommDeptName = model.mCommDeptName;
    mPersonUid = model.mPersonUid;
    mPersonName = model.mPersonName;
    mPersonNameId = model.mPersonNameId;
    mPersonEmail = model.mPersonEmail;
    mPersonTel = model.mPersonTel;
    mChangeHistory = model.mChangeHistory;
    traceout;
}

QString PersonDept::personTel() const
{
    return mPersonTel;
}

void PersonDept::setPersonTel(const QString &newPersonTel)
{
    mPersonTel = newPersonTel;
}

QString PersonDept::personEmail() const
{
    return mPersonEmail;
}

void PersonDept::setPersonEmail(const QString &newPersonEmail)
{
    mPersonEmail = newPersonEmail;
}

QString PersonDept::courseNameId() const
{
    return mCourseNameId;
}

void PersonDept::setCourseNameId(const QString &newCourseNameId)
{
    mCourseNameId = newCourseNameId;
}

QString PersonDept::roleNameId() const
{
    return mRoleNameId;
}

void PersonDept::setRoleNameId(const QString &newRoleNameId)
{
    mRoleNameId = newRoleNameId;
}

QString PersonDept::commDeptName() const
{
    return mCommDeptName;
}

void PersonDept::setCommDeptName(const QString &newCommDeptName)
{
    mCommDeptName = newCommDeptName;
}

QString PersonDept::personHollyName() const
{
    return mPersonHollyName;
}

void PersonDept::setPersonHollyName(const QString &newPersonHollyName)
{
    mPersonHollyName = newPersonHollyName;
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
    setEndDate(DatetimeUtils::dateFromString(newEndDate, format));
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
    setStartDate(DatetimeUtils::dateFromString(newStartDate, format));
}
