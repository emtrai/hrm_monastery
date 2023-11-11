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
#include "personctl.h"
#include "areactl.h"
#include "rolectl.h"
#include "coursectl.h"
#include "person.h"
#include "utils.h"
#include "area.h"

AreaPerson::AreaPerson():MapDbModel()
{
    traced;
}

AreaPerson::~AreaPerson()
{
    traced;
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

ErrCode AreaPerson::exportTemplatePath(FileExporter *exporter,
                                       const QString& name,
                                             QString& fpath, QString *ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    if (exporter) {
        ExportType type = exporter->getExportType();
        switch (type) {
        case EXPORT_HTML:
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltAreaPersonInfoTemplateFileName);
            break;
        default:
            loge("export type %d not support", type);
            err = ErrNotSupport;
        };
    } else {
        err = ErrInvalidArg;
        loge("invalid exporter");
    }
    traceret(err);
    return err;
}

QString AreaPerson::buildNameId(const QString *seed, bool *ok)
{
    QString nameid;
    tracein;
    if (!Utils::appendString(nameid, areaNameId())) {
        logw("no area nameid to build nameid");
    }
    if (!Utils::appendString(nameid, personNameId())) {
        logw("no person nameid to build nameid");
    }
    if (!Utils::appendString(nameid, roleNameId())) {
        logw("no role nameid to build nameid");
    }
    if (!Utils::appendString(nameid, courseNameId())) {
        logw("no course nameid to build nameid");
    }
    if (seed) {
        if (!Utils::appendString(nameid, Utils::UidFromName(*seed, NO_VN_MARK_UPPER))) {
            logw("no seed to build nameid");
        }
    }
    if (ok) *ok = !nameid.isEmpty();
    logd("built nameid %s", STR2CHA(nameid));
    traceout;
    return nameid;
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
    mExportCallbacks.insert(KItemAreaNameId, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->areaNameId();
    });
    mExportCallbacks.insert(KItemPersonName, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personName();
    });
    mExportCallbacks.insert(KItemRole, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->roleName();
    });
    mExportCallbacks.insert(KItemRoleNameId, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->roleNameId();
    });
    mExportCallbacks.insert(KItemTerm, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->courseName();
    });
    mExportCallbacks.insert(KItemTermNameId, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->courseNameId();
    });
    mExportCallbacks.insert(KItemEmail, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personEmail();
    });
    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->personTel();
    });
    mExportCallbacks.insert(KItemRemark, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->remark();
    });
    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
        return ((AreaPerson*)model)->modelStatusName();
    });
    traceout;
}
void AreaPerson::initImportFields()
{
    tracein;
    MapDbModel::initImportFields();
    // person
    mImportCallbacks.insert(KItemPersonName, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->personName().isEmpty()) {
            this->setPersonName(value);
        }
        return err;
    });
    mImportCallbacks.insert(KItemPersonNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        this->setPersonNameId(value);
        DbModel* model = PERSONCTL->getModelByNameId(value);
        if (model) {
            if (IS_MODEL_NAME(model, KModelNamePerson)) {
                Person* per = (Person*) model;
                this->setPersonName(per->displayName());
                this->setPersonEmail(per->emailString());
                this->setPersonTel(per->telString());
                this->setPersonUid(per->uid());
            } else {
                loge("invalid model name '%s'", MODELSTR2CHA(model));
            }
        } else {
            loge("not found name id '%s' for field '%s'",
                 STR2CHA(value), KItemPersonNameId);
            err = ErrNotFound;
        }
        FREE_PTR(model);
        return err;
    });
    // area
    mImportCallbacks.insert(KItemArea, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->areaName().isEmpty()) {
            this->setAreaName(value);
        }
        return err;
    });
    mImportCallbacks.insert(KItemAreaNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        this->setAreaNameId(value);
        DbModel* model = AREACTL->getModelByNameId(value);
        if (IS_MODEL_NAME(model, KModelNameArea)) {
            this->setArea((Area*)model);
        } else {
            loge("not found name id '%s' for field '%s'",
                 STR2CHA(value), KItemAreaNameId);
            err = ErrNotFound;
        }
        FREE_PTR(model);
        return err;
    });
    // Role
    mImportCallbacks.insert(KItemRole, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->roleName().isEmpty()) {
            this->setRoleName(value);
        }
        return err;
    });
    mImportCallbacks.insert(KItemRoleNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        this->setRoleNameId(value);
        DbModel* model = ROLECTL->getModelByNameId(value);
        if (model) {
            this->setRoleName(model->name());
            this->setRoleUid(model->uid());
        } else {
            loge("not found name id '%s' for field '%s'",
                 STR2CHA(value), KItemRoleNameId);
            err = ErrNotFound;
        }
        FREE_PTR(model);
        return err;
    });
    // Term/Course
    mImportCallbacks.insert(KItemTerm, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->courseName().isEmpty()) {
            this->setCourseName(value);
        }
        return err;
    });

    mImportCallbacks.insert(KItemTermNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        this->setCourseNameId(value);
        DbModel* model = COURSECTL->getModelByNameId(value);
        if (model) {
            this->setCourseName(model->name());
            this->setCourseUid(model->uid());
        } else {
            loge("not found name id '%s' for field '%s'",
                 STR2CHA(value), KItemTermNameId);
            err = ErrNotFound;
        }
        FREE_PTR(model);
        return err;
    });
    // contact
    mImportCallbacks.insert(KItemEmail, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->personEmail().isEmpty()) {
            this->setPersonEmail(value);
        }
        return err;
    });
    mImportCallbacks.insert(KItemTel, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("set value '%s'", STR2CHA(value));
        if (this->personTel().isEmpty()) {
            this->setPersonTel(value);
        }
        return err;
    });
    mImportCallbacks.insert(KItemRemark, [this](const QString& value){
        logd("set value '%s'", STR2CHA(value));
        this->setRemark(value);
        return ErrNone;
    });

    mImportItemsType.insert(KItemPersonNameId,
                            (IMPORT_ITEM_TYPE_MATCH_DB | IMPORT_ITEM_TYPE_MANDATORY));
    mImportItemsType.insert(KItemAreaNameId,
                            (IMPORT_ITEM_TYPE_MATCH_DB | IMPORT_ITEM_TYPE_MANDATORY));
    mImportItemsType.insert(KItemRoleNameId,
                            (IMPORT_ITEM_TYPE_MATCH_DB | IMPORT_ITEM_TYPE_MANDATORY));
    mImportItemsType.insert(KItemRemark,
                            (IMPORT_ITEM_TYPE_UPDATE_DB));
    mImportItemsType.insert(KItemTermNameId,
                            (IMPORT_ITEM_TYPE_MATCH_DB));
    traceout;
}

const QString &AreaPerson::name() const
{
    return personName();
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
    mRoleNameId = model.mRoleNameId;

    mCourseUid = model.mCourseUid;
    mCourseName = model.mCourseName;
    mCourseNameId = model.mCourseNameId;

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

QString AreaPerson::courseNameId() const
{
    return mCourseNameId;
}

void AreaPerson::setCourseNameId(const QString &newCourseNameId)
{
    CHECK_MODIFIED_THEN_SET(mCourseNameId, newCourseNameId, KItemCourse);
}

QString AreaPerson::roleNameId() const
{
    return mRoleNameId;
}

void AreaPerson::setRoleNameId(const QString &newRoleNameId)
{
    CHECK_MODIFIED_THEN_SET(mRoleNameId, newRoleNameId, KItemRole);
}

const QString &AreaPerson::personNameId() const
{
    return mPersonNameId;
}

void AreaPerson::setPersonNameId(const QString &newPersonNameId)
{
    CHECK_MODIFIED_THEN_SET(mPersonNameId, newPersonNameId, KItemPerson);
}

const QString &AreaPerson::personEmail() const
{
    return mPersonEmail;
}

void AreaPerson::setPersonEmail(const QString &newPersonEmail)
{
    mPersonEmail = newPersonEmail;
    // don't mark modified, as it's info of person
}

const QString &AreaPerson::personTel() const
{
    return mPersonTel;
}

void AreaPerson::setPersonTel(const QString &newPersonTel)
{
    mPersonTel = newPersonTel;
    // don't mark modified, as it's info of person
}

const QString &AreaPerson::hollyName() const
{
    return mHollyName;
}

void AreaPerson::setHollyName(const QString &newHollyName)
{
    mHollyName = newHollyName;
    // don't mark modified, as it's info of person
}

const QString &AreaPerson::areaNameId() const
{
    return mAreaNameId;
}

void AreaPerson::setAreaNameId(const QString &newAreaNameId)
{
    CHECK_MODIFIED_THEN_SET(mAreaNameId, newAreaNameId, KItemArea);
}

ErrCode AreaPerson::setArea(const Area *area)
{
    tracein;
    ErrCode err = ErrNone;
    if (!area) {
        err = ErrInvalidArg;
        loge("invalid area");
    }
    if (err == ErrNone) {
        setAreaName(area->name());
        setAreaNameId(area->nameId());
        setAreaUid(area->uid());
    }
    traceout;
    return err;
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
    if (mAreaUid != newAreaUid) {
        mAreaUid = newAreaUid;
        markItemAsModified(KItemArea);
        setUid1(mAreaUid);
    }
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
    if (mPersonUid != newPersonUid) {
        mPersonUid = newPersonUid;
        setUid2(mPersonUid);
        markItemAsModified(KItemPerson);
    }
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
    CHECK_MODIFIED_THEN_SET(mCourseUid, newCourseUid, KItemCourse);
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
    CHECK_MODIFIED_THEN_SET(mRoleUid, newRoleUid, KItemRole);
}
