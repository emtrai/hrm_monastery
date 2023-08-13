/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: communitydept.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/31/2023
 * Brief:
 */
#include "communitydept.h"

#include "utils.h"
#include "datetimeutils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "utils.h"
#include "person.h"
#include "stringdefs.h"

CommunityDept::CommunityDept():DbModel()
{
    tracein;
    mEstablishDate = 0;
    mStatus = 0;
    mCurrentDirector = nullptr;
    traceout;
}

CommunityDept::~CommunityDept()
{
    tracein;
    FREE_PTR(mCurrentDirector);
    traceout;
}

DbModel *CommunityDept::build()
{
    CommunityDept* model = new CommunityDept();
    model->init();
    return model;
}

DbModelBuilder CommunityDept::getBuilder() const
{
    return &CommunityDept::build;
}

QString CommunityDept::modelName() const
{
    return KModelNameCommDept;
}

void CommunityDept::clone(const DbModel *model)
{
    tracein;
    DbModel::clone(model);
    if (model && model->modelName() == KModelNameCommDept) {
        CommunityDept* comm = (CommunityDept*)model;
        mDepartmentUid = comm->mDepartmentUid;
        mDepartmentDbId = comm->mDepartmentDbId;
        mDepartmentNameId = comm->mDepartmentNameId;
        mDepartmentName = comm->mDepartmentName;
        mCommunityUid = comm->mCommunityUid;
        mCommunityNameId = comm->mCommunityNameId;
        mCommunityDbId = comm->mCommunityDbId;
        mCommunityName = comm->mCommunityName;
        mEstablishDate = comm->mEstablishDate;
        mClosedDate = comm->mClosedDate;
        mEmail = comm->mEmail;
        mAddr = comm->mAddr;
        mTel = comm->mTel;
        mBrief = comm->mBrief;
        mStatus = comm->mStatus;
        mModelStatusName = comm->mModelStatusName;
        FREE_PTR(mCurrentDirector);
        if (comm->currentDirector()) {
            mCurrentDirector = CLONE_MODEL(comm->currentDirector(), Person);
        }
    }
    traceout;
}

DbModel *CommunityDept::clone() const
{
    traced;
    return DbModel::clone();
}
void CommunityDept::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemAddress, EXPORT_CALLBACK_STRING_IMPL(
                                            CommunityDept,
                                            KModelNameCommDept,
                                            addr));
    mExportCallbacks.insert(KItemEmail, EXPORT_CALLBACK_STRING_IMPL(
                                            CommunityDept,
                                            KModelNameCommDept,
                                            email));
    mExportCallbacks.insert(KItemTel, EXPORT_CALLBACK_STRING_IMPL(
                                          CommunityDept,
                                          KModelNameCommDept,
                                          tel));
    mExportCallbacks.insert(KItemBrief, EXPORT_CALLBACK_STRING_IMPL(
                                             CommunityDept,
                                             KModelNameCommDept,
                                             brief));
    mExportCallbacks.insert(KItemRemark, EXPORT_CALLBACK_STRING_IMPL(
                                             CommunityDept,
                                             KModelNameCommDept,
                                             remark));
    mExportCallbacks.insert(KItemEstablishDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                                CommunityDept,
                                                KModelNameCommDept,
                                                establishDate,
                                                DEFAULT_FORMAT_YMD));
    mExportCallbacks.insert(KItemCloseDate, EXPORT_CALLBACK_DATETIME_IMPL(
                                            CommunityDept,
                                            KModelNameCommDept,
                                            closedDate,
                                            DEFAULT_FORMAT_YMD));
    mExportCallbacks.insert(KItemStatus, EXPORT_CALLBACK_STRING_IMPL(
                                            CommunityDept,
                                            KModelNameCommDept,
                                            modelStatusName));
    mExportCallbacks.insert(KItemCommunityName, EXPORT_CALLBACK_STRING_IMPL(
                                             CommunityDept,
                                             KModelNameCommDept,
                                             communityName));
    mExportCallbacks.insert(KItemDepartmentName, EXPORT_CALLBACK_STRING_IMPL(
                                                    CommunityDept,
                                                    KModelNameCommDept,
                                                    departmentName));
    mExportCallbacks.insert(KItemCommunityNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                    CommunityDept,
                                                    KModelNameCommDept,
                                                    communityNameId));
    mExportCallbacks.insert(KItemDepartmentNameId, EXPORT_CALLBACK_STRING_IMPL(
                                                     CommunityDept,
                                                     KModelNameCommDept,
                                                     departmentNameId));
//    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
//        return CHECK_TO_GET_MODEL_DATA_RET(model, CommunityDept, KModelNameCommDept, tel);
//        return ((CommunityDept*)model)->personTel();
//    });
//    mExportCallbacks.insert(KItemRemark, [](const DbModel* model, const QString& item){
//        return ((AreaPerson*)model)->remark();
//    });
//    mExportCallbacks.insert(KItemStartDate, [](const DbModel* model, const QString& item){
//        return DatetimeUtils::date2String(((MapDbModel*)model)->startDate(), DEFAULT_FORMAT_YMD);
//    });
//    mExportCallbacks.insert(KItemEndDate, [](const DbModel* model, const QString& item){
//        return DatetimeUtils::date2String(((MapDbModel*)model)->endDate(), DEFAULT_FORMAT_YMD);
//    });
//    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
//        return ((MapDbModel*)model)->modelStatusName();
//    });
//    mExportCallbacks.insert(KItemStatusId, [](const DbModel* model, const QString& item){
//        return QString("%1").arg(((MapDbModel*)model)->modelStatus());
//    });
    traceout;
}

qint64 CommunityDept::establishDate() const
{
    return mEstablishDate;
}

void CommunityDept::setEstablishDate(qint64 newEstablishDate)
{
    CHECK_MODIFIED_THEN_SET(mEstablishDate, newEstablishDate, KItemEstablishDate);
}

ErrCode CommunityDept::setEstablishDateFromString(const QString &date)
{
    tracein;
    ErrCode err = ErrNone;
    bool isOk = false;
    logd("create date string '%s'", date.toStdString().c_str());
    qint64 dateValue = DatetimeUtils::dateFromString(date, DEFAULT_FORMAT_YMD, &isOk);
    logd("mEstablishDate 0x%llx, isOk %d", dateValue, isOk);
    if (isOk) {
        setEstablishDate(dateValue);
    } else {
        err = ErrInvalidArg;
        loge("invalid establish date time '%s'", STR2CHA(date));
    }

    traceret(err);
    return err;
}

const QString &CommunityDept::email() const
{
    return mEmail;
}

void CommunityDept::setEmail(const QString &newEmail)
{
    CHECK_MODIFIED_THEN_SET(mEmail, newEmail, KItemEmail);
}

const QString &CommunityDept::addr() const
{
    return mAddr;
}

void CommunityDept::setAddr(const QString &newAddr)
{
    CHECK_MODIFIED_THEN_SET(mAddr, newAddr, KItemAddress);
}

const QString &CommunityDept::tel() const
{
    return mTel;
}

void CommunityDept::setTel(const QString &newTel)
{
    CHECK_MODIFIED_THEN_SET(mTel, newTel, KItemTel);
}

const QString &CommunityDept::brief() const
{
    return mBrief;
}

void CommunityDept::setBrief(const QString &newBrief)
{
    CHECK_MODIFIED_THEN_SET(mBrief, newBrief, KItemBrief);
}

qint64 CommunityDept::modelStatus() const
{
    return mStatus;
}

void CommunityDept::setModelStatus(qint64 newStatus)
{
    CHECK_MODIFIED_THEN_SET(mStatus, newStatus, KItemStatus);
}

DbModelHandler *CommunityDept::getDbModelHandler() const
{

    return DB->getModelHandler(KModelHdlCommDept);
}

Person *CommunityDept::currentDirector() const
{
    return mCurrentDirector;
}

QString CommunityDept::currentDirectorName()
{
    QString ret;
    if (mCurrentDirector) {
        ret = mCurrentDirector->displayName();
    }
    return ret;
}

QString CommunityDept::currentDirectorNameId()
{
    QString ret;
    if (mCurrentDirector) {
        ret = mCurrentDirector->nameId();
    }
    return ret;
}

// Just for display
void CommunityDept::setCurrentDirector(const Person *newCurrentDirector)
{
    tracein;
    FREE_PTR(mCurrentDirector);
    if (newCurrentDirector) {
        logd("clone for new director '%s'", MODELSTR2CHA(newCurrentDirector));
        mCurrentDirector = CLONE_MODEL(newCurrentDirector, Person);
    }
    traceout;
}


QString CommunityDept::modelStatusName() const
{
    return mModelStatusName;
}

// Just for display
void CommunityDept::setModelStatusName(const QString &newModelStatusName)
{
    mModelStatusName = newModelStatusName;
}

const QString &CommunityDept::communityNameId() const
{
    return mCommunityNameId;
}

// just for display
void CommunityDept::setCommunityNameId(const QString &newCommunityNameId)
{
    mCommunityNameId = newCommunityNameId;
}

const QString &CommunityDept::departmentNameId() const
{
    return mDepartmentNameId;
}

// just for display
void CommunityDept::setDepartmentNameId(const QString &newDepartmentNameId)
{
    mDepartmentNameId = newDepartmentNameId;
}

qint64 CommunityDept::closedDate() const
{
    return mClosedDate;
}

void CommunityDept::setClosedDate(qint64 newClosedDate)
{
    CHECK_MODIFIED_THEN_SET(mClosedDate, newClosedDate, KItemCloseDate);
}

ErrCode CommunityDept::setClosedDateFromString(const QString &date)
{
    tracein;
    bool isOk = false;
    ErrCode err = ErrNone;
    logd("close date string '%s'", date.toStdString().c_str());
    qint64 dateval = DatetimeUtils::dateFromString(date, DEFAULT_FORMAT_YMD, &isOk);
    logd("mClosedDate 0x%llx, isOk %d", dateval, isOk);
    if (isOk) {
        setClosedDate(dateval);
    } else {
        err = ErrInvalidArg;
        loge("invalid close date time '%s'", STR2CHA(date));
    }

    traceret(err);
    return err;
}


const QString & CommunityDept::communityName() const
{
    return mCommunityName;
}

// just for display
void CommunityDept::setCommunityName(const QString & newCommunityName)
{
    mCommunityName = newCommunityName;
}

const QString &CommunityDept::departmentName() const
{
    return mDepartmentName;
}

// just for display
void CommunityDept::setDepartmentName(const QString &newDepartmentName)
{
    mDepartmentName = newDepartmentName;
}

qint64 CommunityDept::communityDbId() const
{
    return mCommunityDbId;
}

void CommunityDept::setCommunityDbId(qint64 newCommunityDbId)
{
    CHECK_MODIFIED_THEN_SET(mCommunityDbId, newCommunityDbId, KItemCommunityDbId);
}

qint64 CommunityDept::departmentDbId() const
{
    return mDepartmentDbId;
}

void CommunityDept::setDepartmentDbId(qint64 newDepartmentDbId)
{
    CHECK_MODIFIED_THEN_SET(mDepartmentDbId, newDepartmentDbId, KItemDepartmentDbId);
}

const QString &CommunityDept::departmentUid() const
{
    return mDepartmentUid;
}

void CommunityDept::setDepartmentUid(const QString &newDepartmentUid)
{
    CHECK_MODIFIED_THEN_SET(mDepartmentUid, newDepartmentUid, KItemDepartment);
}

const QString &CommunityDept::communityUid() const
{
    return mCommunityUid;
}

void CommunityDept::setCommunityUid(const QString &newCommunityUid)
{
    CHECK_MODIFIED_THEN_SET(mCommunityUid, newCommunityUid, KItemCommunity);
}

