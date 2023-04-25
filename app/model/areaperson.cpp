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
 * Filename: areaperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#include "areaperson.h"

#include "logger.h"
#include "errcode.h"
#include "defs.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "filectl.h"
#include "prebuiltdefs.h"

AreaPerson::AreaPerson():MapDbModel()
{
    tracein;
}

AreaPerson::~AreaPerson()
{
    tracein;
    traceout;
}

DbModel *AreaPerson::build()
{
    AreaPerson* item = new AreaPerson();
    item->init();
    return item;
}

void AreaPerson::clone(const DbModel *model)
{
    tracein;
    if (model) {
        MapDbModel::clone(model);
        copy(*(AreaPerson*)model);
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

DbModelBuilder AreaPerson::getBuilder() const
{
    return &AreaPerson::build;
}


QString AreaPerson::modelName() const
{
    return KModelNameAreaPerson;
}

const QString AreaPerson::exportTemplatePath(FileExporter *exporter, QString *ftype) const
{
    traced;
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_HTML:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltAreaPersonInfoTemplateFileName);
        };
    }
    return QString();
}

void AreaPerson::initExportFields()
{
    tracein;
    MapDbModel::initExportFields();
    mExportCallbacks.insert(KItemPersonNameId, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personNameId();
    });
    mExportCallbacks.insert(KItemArea, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->areaName();
    });
    mExportCallbacks.insert(KItemHollyName, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->hollyName();
    });
    mExportCallbacks.insert(KItemFullName, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personName();
    });
    mExportCallbacks.insert(KItemRole, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->roleName();
    });
    mExportCallbacks.insert(KItemTerm, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->courseName();
    });
    mExportCallbacks.insert(KItemEmail, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personEmail();
    });
    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personTel();
    });
    traceout;
}


DbModelHandler *AreaPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlArea);
}

void AreaPerson::copy(const AreaPerson &model)
{
    tracein;
    mRoleUid = model.mRoleUid;
    mRoleName = model.mRoleName;
    mCourseUid = model.mCourseUid;
    mCourseName = model.mCourseName;
    mPersonUid = model.mPersonUid;
    mPersonName = model.mPersonName;
    mPersonNameId = model.mPersonNameId;
    mHollyName = model.mHollyName;
    mPersonTel = model.mPersonTel;
    mPersonEmail = model.mPersonEmail;
    mAreaUid = model.mAreaUid;
    mAreaName = model.mAreaName;
    mAreaNameId = model.mAreaNameId;

    traceout;
}

const QString &AreaPerson::personNameId() const
{
    return mPersonNameId;
}

void AreaPerson::setPersonNameId(const QString &newPersonNameId)
{
    mPersonNameId = newPersonNameId;
}

const QString &AreaPerson::personEmail() const
{
    return mPersonEmail;
}

void AreaPerson::setPersonEmail(const QString &newPersonEmail)
{
    mPersonEmail = newPersonEmail;
}

const QString &AreaPerson::personTel() const
{
    return mPersonTel;
}

void AreaPerson::setPersonTel(const QString &newPersonTel)
{
    mPersonTel = newPersonTel;
}

const QString &AreaPerson::hollyName() const
{
    return mHollyName;
}

void AreaPerson::setHollyName(const QString &newHollyName)
{
    mHollyName = newHollyName;
}

const QString &AreaPerson::areaNameId() const
{
    return mAreaNameId;
}

void AreaPerson::setAreaNameId(const QString &newAreaNameId)
{
    mAreaNameId = newAreaNameId;
}

const QString &AreaPerson::areaName() const
{
    return mAreaName;
}

void AreaPerson::setAreaName(const QString &newAreaName)
{
    mAreaName = newAreaName;
}

const QString &AreaPerson::areaUid() const
{
    return mAreaUid;
}

void AreaPerson::setAreaUid(const QString &newAreaUid)
{
    mAreaUid = newAreaUid;
    markItemAsModified(KItemArea);
    setUid1(mAreaUid);
}

const QString &AreaPerson::personName() const
{
    return mPersonName;
}

void AreaPerson::setPersonName(const QString &newPersonName)
{
    mPersonName = newPersonName;
}

const QString &AreaPerson::personUid() const
{
    return mPersonUid;
}

void AreaPerson::setPersonUid(const QString &newPersonUid)
{
    mPersonUid = newPersonUid;
    setUid2(mPersonUid);
    markItemAsModified(KItemPerson);
}

const QString &AreaPerson::courseName() const
{
    return mCourseName;
}

void AreaPerson::setCourseName(const QString &newCourseName)
{
    mCourseName = newCourseName;
}

const QString &AreaPerson::courseUid() const
{
    return mCourseUid;
}

void AreaPerson::setCourseUid(const QString &newCourseUid)
{
    mCourseUid = newCourseUid;
    markItemAsModified(KItemCourse);
}

const QString &AreaPerson::roleName()
{
    if (mRoleName.isEmpty() && !mRoleUid.isEmpty()) {
        logd("no role name, but has uid, update it");
        DbModel* model = getDbModelHandler()->getByUid(mRoleUid);
        if (model) {
            mRoleName = model->name();
            delete model;
        } else {
            logw("role uid '%s' not found", STR2CHA(mRoleUid));
        }
    }
    return mRoleName;
}

void AreaPerson::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

const QString &AreaPerson::roleUid() const
{
    return mRoleUid;
}

void AreaPerson::setRoleUid(const QString &newRoleUid)
{
    mRoleUid = newRoleUid;
    markItemAsModified(KItemRole);
}
