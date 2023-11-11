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
 * Filename: community.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#include "community.h"

#include "dbctl.h"
#include "logger.h"
#include "utils.h"
#include "datetimeutils.h"
#include "filectl.h"
#include "communityctl.h"
#include "dbmodel.h"
#include "areactl.h"
#include "countryctl.h"
#include "personctl.h"
#include "prebuiltdefs.h"
#include "stringdefs.h"
#include "stringdefs.h"
#include "communitymanager.h"
#include "person.h"

DbModel *Community::build()
{
    tracein;
    Community* item = new Community();
    item->init();
    return item;
}

Community::Community():DbModel()
{
    tracein;
    resetResource();
    traceout;
}

Community::Community(const Community &obj):DbModel(obj)
{
    tracein;
    resetResource();
    copy(obj);
    traceout;
}

Community::~Community()
{
    tracein;
    FREE_PTR(mNewCommMgr);
    resetResource();
    tracein;
}

void Community::clone(const DbModel *model)
{
    tracein;
    if (model && model->modelName() == KModelNameCommunity) {
        DbModel::clone(model);
        copy(*(Community*)model);
    } else {
        loge("clone invalid model '%s'", MODELSTR2CHA(model));
    }
    traceout;
}

DbModelBuilder Community::getBuilder() const
{
    return &Community::build;
}

void Community::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemCountry, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->countryName();
    });
    mExportCallbacks.insert(KItemCountryNameId, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->countryNameId();
    });
    mExportCallbacks.insert(KItemAddress, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->addr();
    });
    mExportCallbacks.insert(KItemEmail, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->email();
    });
    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->tel();
    });
    mExportCallbacks.insert(KItemEstablishDate, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return DatetimeUtils::date2String(((Community*)model)->createDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemCloseDate, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return DatetimeUtils::date2String(((Community*)model)->closeDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemFeastDay, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return DatetimeUtils::date2String(((Community*)model)->feastDate(), DEFAULT_FORMAT_MD);
    });
    mExportCallbacks.insert(KItemOtherContact, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->contact();
    });
    mExportCallbacks.insert(KItemArea, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->areaName();
    });
    mExportCallbacks.insert(KItemAreaNameId, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->areaNameId();
    });
    mExportCallbacks.insert(KItemParentCommunity, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->parentName();
    });
    mExportCallbacks.insert(KItemParentCommunityNameId, [](const DbModel* model,
                                                           const QString& item){
        UNUSED(item);
        return ((Community*)model)->parentNameId();
    });
    mExportCallbacks.insert(KItemBrief, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->brief();
    });
    mExportCallbacks.insert(KItemFullIntro, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->fullInfo();
    });
    mExportCallbacks.insert(KItemCEO, [](const DbModel* model, const QString& item){
        UNUSED(item);
        QString ret;
        const Community* comm = static_cast<const Community*>(model);
        if (comm) {
            const_cast<Community*>(comm)->check2LoadAllData();
            ret = comm->currentCEOName();
        } else {
            ret = STR_INVALID_DATA;
            loge("community is null");
        }
        return ret;
    });
    mExportCallbacks.insert(KItemCEONameId, [](const DbModel* model, const QString& item){
        UNUSED(item);
        QString ret;
        const Community* comm = static_cast<const Community*>(model);
        if (model) {
            const_cast<Community*>(comm)->check2LoadAllData();
            ret = comm->currentCEONameId();
        } else {
            ret = STR_INVALID_DATA;
            loge("community is null");
        }
        return ret;
    });
    mExportCallbacks.insert(KItemChurchAddress, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->church();
    });
    mExportCallbacks.insert(KItemHistory, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->history();
    });
    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->statusName();
    });
    mExportCallbacks.insert(KItemMission, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->missionNameString();
    });
    mExportCallbacks.insert(KItemMissionNameId, [](const DbModel* model, const QString& item){
        UNUSED(item);
        return ((Community*)model)->missionNameIdString();
    });
    mExportCallbacks.insert(KItemManagers, [](const DbModel* model, const QString& item){
        UNUSED(item);
        QString managers;
        ErrCode err = ErrNone;
        if (!model) {
            err = ErrInvalidModel;
        }
        if (err == ErrNone) {
            err = COMMUNITYCTL->getManagersListInString(model->uid(), "\n", managers);
            logife(err, "failed to get managers list for ''%s'", MODELSTR2CHA(model));
        }
        if (err != ErrNone && managers.isEmpty()) {
            managers = QString(STR_DATA_ERROR_CODE).arg(err);
        }
        return managers;
    });
    // TODO: implement more
    traceout;
}

void Community::initImportFields()
{
    tracein;
    DbModel::initImportFields();
    mImportCallbacks.insert(KItemStatus, [this](const QString& value){
        this->setModelStatus(value.toInt()); // TODO: handle error case when convert to Int
        return ErrNone;
    });
    mImportCallbacks.insert(KItemCountryNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        if (!value.isEmpty()) {
            DbModel* model = COUNTRYCTL->getModelByNameId(value);
            if (model) {
                this->setCountryName(model->name());
                this->setCountryUid(model->uid());
                delete model;
            } else {
                err = ErrNotFound;
                loge("not found %s:'%s'", KItemCountryNameId, STR2CHA(value));
            }
        }
        return err;
    });
    mImportCallbacks.insert(KItemAddress, [this](const QString& value){
        this->setAddr(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemEmail, [this](const QString& value){
        this->setEmail(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemTel, [this](const QString& value){
        this->setTel(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemEstablishDate, [this](const QString& value){
        if (!value.isEmpty())
            return this->setCreateDateFromString(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemCloseDate, [this](const QString& value){
        if (!value.isEmpty())
            return this->setCloseDateFromString(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemFeastDay, [this](const QString& value){
        if (!value.isEmpty())
            return this->setFeastDateFromString(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemOtherContact, [this](const QString& value){
        this->setContact(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemAreaNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        logd("Search area nameid '%s'", STR2CHA(value));
        CHECK_SET_IMPORT_NAME_ID(err, value, AREACTL, this->setAreaUid, this->setAreaName);
        return err;
    });
    mImportCallbacks.insert(KItemParentCommunityNameId, [this](const QString& value){
        ErrCode err = ErrNone;
        CHECK_SET_IMPORT_NAME_ID(err, value, COMMUNITYCTL, (this->setParentUid), (this->setParentName));
        return err;
    });
    mImportCallbacks.insert(KItemBrief, [this](const QString& value){
        this->setBrief(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemCEONameId, [this](const QString& value){
        ErrCode err = ErrNone;
        CHECK_SET_IMPORT_NAME_ID(err, value, PERSONCTL, (this->setCurrentCEONameId), (this->setParentName));
        return err;
    });
    // TODO: implement more
    traceout;
}

QString Community::modelName() const
{
    return KModelNameCommunity;
}

qint32 Community::level() const
{
    return mLevel;
}

void Community::setLevel(qint32 newLevel)
{
    mLevel = newLevel;
    markItemAsModified(KItemLevel);
    // TODO: level should be update automatically, by counting parent uid chain
    // if parent uid is null, mean root community. There must be only one community with level 0 (root community)
    // by default, all community must has root community as parent uid, until it's update
}

qint64 Community::createDate() const
{
    return mCreateDate;
}

void Community::setCreateDate(qint64 newCreateDate)
{
    CHECK_MODIFIED_THEN_SET(mCreateDate, newCreateDate, KItemCreateTime);
}

ErrCode Community::setCreateDateFromString(const QString &date, const QString &format)
{
    tracein;
    logd("create date string '%s'", date.toStdString().c_str());
    bool ok = false;
    qint64 dateval = DatetimeUtils::dateFromString(date, format, &ok);
    logd("dateval %lld", dateval);
    if (ok) {
        setCreateDate(dateval);
    } else {
        loge("Invalid date '%s', format '%s'", STR2CHA(date), STR2CHA(format));
    }
    traceout;
    return ok?ErrNone:ErrInvalidArg;
}

const QString &Community::parentUid() const
{
    return mParentUid;
}

void Community::setParentUid(const QString &newParentUid)
{
    if (newParentUid.isEmpty() || newParentUid != uid()) {
        CHECK_MODIFIED_THEN_SET(mParentUid, newParentUid, KItemParentCommunity);
    } else {
        loge("Cannot set parent to itself");
    }
}

DbModelStatus Community::getStatus() const
{
    return mStatus;
}

void Community::setModelStatus(int newStatus)
{
    if ((newStatus & MODEL_STATUS_MAX) != 0) {
        mStatus = (DbModelStatus)newStatus;
        mStatusName = DbModel::modelStatus2Name(mStatus);
    }
    logd("new status = %d", mStatus);
    logd("new mStatusName = %s", STR2CHA(mStatusName));
    markItemAsModified(KItemStatus);
}

qint64 Community::closeDate() const
{
    return mCloseDate;
}

void Community::setCloseDate(qint64 newCloseDate)
{
    CHECK_MODIFIED_THEN_SET(mCloseDate, newCloseDate, KItemCloseDate);
}

ErrCode Community::setCloseDateFromString(const QString &date, const QString &format)
{
    tracein;
    bool ok = false;
    logd("close date string '%s'", date.toStdString().c_str());
    qint64 dateVal = DatetimeUtils::dateFromString(date, format, &ok);
    logd("dateVal %ll", dateVal);
    if (!ok) {
        setCloseDate(dateVal);
    } else {
        loge("Invalid date '%s', format '%s'", STR2CHA(date), STR2CHA(format));
    }
    traceout;
    return ok?ErrNone:ErrInvalidArg;
}


DbModelHandler *Community::getDbModelHandler() const
{
    return DbCtl::getDb()->getCommunityModelHandler();
}

bool Community::allowRemove(QString *msg)
{
    tracein;
    bool allow = ((level() != 0) || !mParentUid.isEmpty()) && (nameId() != KModelNameIdRootCommunity);
    logd("level %d, parent uid '%s', allow %d", level(), STR2CHA(mParentUid), allow); // TODO: level only is enought???
    if (msg) {
        if (level() == 0 && !mParentUid.isEmpty()) {
            loge("Not allow to delete root community");
            *msg = QObject::tr("Không được phép xóa cộng đoàn gốc");
        }
    }
    if (!allow) {
        loge("not allow to delete '%s'", MODELSTR2CHA(this));
    }
    traceout;
    return allow; // not allow to deleve root community, only update is allowed
}

void Community::copy(const Community &model)
{
    tracein;
    mImgPath = model.mImgPath;
    mAddr = model.mAddr;

    mProvince = model.mProvince;

    mCountryUid = model.mCountryUid;
    mCountryName = model.mCountryName;
    mCountryNameId = model.mCountryNameId;

    mChurch = model.mChurch;

    mTel = model.mTel;
    mEmail = model.mEmail;

    mLevel = model.mLevel;

    mParentName = model.mParentName;
    mParentUid = model.mParentUid;
    mParentNameId = model.mParentNameId;

    mCreateDate = model.mCreateDate;
    mCloseDate = model.mCloseDate;
    mFeastDate = model.mFeastDate;

    mStatus = model.mStatus;
    mStatusName = model.mStatusName;

    mBrief = model.mBrief;
    mFullInfo = model.mFullInfo;
    mHistory = model.mHistory;

    mAreaUid = model.mAreaUid;
    mAreaDbId = model.mAreaDbId;
    mAreaName = model.mAreaName;
    mAreaNameId = model.mAreaNameId;

    mCurrentCEOName = model.mCurrentCEOName;
    mCurrentCEOUid = model.mCurrentCEOUid;
    mCurrentCEONameId = model.mCurrentCEONameId;
    mContact = model.mContact;

    mMissionUid = model.mMissionUid;
    mMissionName = model.mMissionName;
    FREE_PTR(mNewCommMgr);
    mNewCommMgr = CLONE_MODEL(model.newCommMgr(), CommunityManager);
    mLoadedCEO = model.mLoadedCEO;
    traceout;
}

void Community::resetResource()
{
    mLevel = -1;
    mCreateDate = 0;
    mCloseDate = 0;
    mFeastDate = 0;
    mStatus = MODEL_STATUS_NOT_READY;
    mAreaDbId = 0;
    mMissionUid.clear();
    mMissionName.clear();
    mNewCommMgr = nullptr;
    mLoadedCEO = false;
}

ErrCode Community::loadAllData()
{
    tracein;
    Person* ceo = nullptr;
    bool isActiveCEO = false;
    dbgd("load CEO for community '%s'", STR2CHA(uid()));
    ErrCode err = COMMUNITYCTL->getCurrentCEO(uid(), &ceo, &isActiveCEO);
    dbgd("found ceo '%s', isActiveCEO %d err %d",
         MODELSTR2CHA(ceo), isActiveCEO, err);
    if (err == ErrNone && ceo && isActiveCEO) {
        dbgd("current CEO for community '%s'", MODELSTR2CHA(ceo));
        setCurrentCEOUid(ceo->uid());
        setCurrentCEOName(ceo->displayName());
        setCurrentCEONameId(ceo->nameId());
    }
    FREE_PTR(ceo);
    traceret(err);
    return err;
}

CommunityManager *Community::newCommMgr() const
{
    return mNewCommMgr;
}

void Community::setNewCommMgr(const CommunityManager *newCommMgr)
{
    FREE_PTR(mNewCommMgr);
    mNewCommMgr = CLONE_MODEL(newCommMgr, CommunityManager);
}

bool Community::updateCEO() const
{
    return isFieldUpdated(KItemCEO);
}

QString Community::toString() const
{
    QString str = DbModel::toString();
    str += QString(":ceouid('%1')").arg(currentCEOUid());
    str += QString(":status('%1')").arg(getStatus());
    str += QString(":areauid('%1')").arg(areaUid());
    str += QString(":missionuid('%1')").arg(missionUidString());
    str += QString(":parentuid('%1')").arg(parentUid());
    return str;
}

const QString &Community::history() const
{
    return mHistory;
}

void Community::setHistory(const QString &newHistory)
{
    CHECK_MODIFIED_THEN_SET(mHistory, newHistory, KItemHistory);
}

const QStringList &Community::missionNameId() const
{
    return mMissionNameId;
}

void Community::setMissionNameId(const QStringList &newMissionNameId)
{
    CHECK_MODIFIED_THEN_SET(mMissionNameId, newMissionNameId, KItemMission);
}

void Community::addMissionNameId(const QString &newMissionNameId)
{
    if (!mMissionNameId.contains(newMissionNameId)) {
        mMissionNameId.append(newMissionNameId);
        markItemAsModified(KItemMission);
    }
}

QString Community::missionNameIdString() const
{
    return mMissionNameId.isEmpty()?"":mMissionNameId.join(MISSION_DELIM);
}

const QString &Community::fullInfo() const
{
    return mFullInfo;
}

void Community::setFullInfo(const QString &newFullInfo)
{
    CHECK_MODIFIED_THEN_SET(mFullInfo, newFullInfo, KItemFullIntro);
}

const QString &Community::countryNameId()
{
    CHECK_UID_TO_UPDATE_VALUE(mCountryUid, mCountryNameId, KModelHdlCountry, nameId);
    return mCountryNameId;
}

void Community::setCountryNameId(const QString &newCountryNameId)
{
    CHECK_MODIFIED_THEN_SET(mCountryNameId, newCountryNameId, KItemCountry);
}

const QString &Community::areaNameId()
{
    CHECK_UID_TO_UPDATE_VALUE(mAreaUid, mAreaNameId, KModelHdlArea, nameId);
    return mAreaNameId;
}

void Community::setAreaNameId(const QString &newAreaNameId)
{
    CHECK_MODIFIED_THEN_SET(mAreaNameId, newAreaNameId, KItemArea);
}

const QString &Community::countryName()
{
    CHECK_UID_TO_UPDATE_VALUE(mCountryUid, mCountryName, KModelHdlCountry, name);
    return mCountryName;
}

void Community::setCountryName(const QString &newCountryName)
{
    mCountryName = newCountryName;
}


const QStringList &Community::missionName() const
{
    return mMissionName;
}

QString Community::missionNameString() const
{
    return mMissionName.isEmpty()?"":mMissionName.join(MISSION_DELIM);

}

void Community::setMissionName(const QStringList &newMissionName)
{
    mMissionName = newMissionName;
}

void Community::addMissionName(const QString &newMissionName)
{
    mMissionName.push_back(newMissionName);
}

const QStringList &Community::missionUid() const
{
    return mMissionUid;
}

QString Community::missionUidString() const
{
    return mMissionUid.isEmpty()?"":mMissionUid.join(MISSION_DELIM);
}

void Community::setMissionUid(const QStringList &newMissionUid)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mMissionUid, newMissionUid, KItemMission);
}

void Community::setMissionUid(const QString &newMissionUid)
{
    QStringList list;
    if (!newMissionUid.isEmpty()) {
        list = newMissionUid.split(MISSION_DELIM);
    }
    setMissionUid(list);
}

void Community::addMissionUid(const QString &newMissionUid)
{
    mMissionUid.push_back(newMissionUid);
    markItemAsModified(KItemMission);
}

const QString &Community::brief() const
{
    return mBrief;
}

void Community::setBrief(const QString &newBrief)
{
    CHECK_MODIFIED_THEN_SET(mBrief, newBrief, KItemBrief);
}

const QString &Community::currentCEONameId() const
{
//    CHECK_UID_TO_UPDATE_VALUE(mCurrentCEOUid, mCurrentCEONameId, KModelHdlPerson, nameId);
    return mCurrentCEONameId;
}

void Community::setCurrentCEONameId(const QString &newCurrentCEOCode)
{
    // TODO: query uid from code
    CHECK_MODIFIED_THEN_SET(mCurrentCEONameId, newCurrentCEOCode, KItemCEO);
}

ErrCode Community::exportTemplatePath(FileExporter* exporter,
                                      const QString& name,
                                      QString& fpath,
                                      QString* ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    if (exporter) {
        ExportType type = exporter->getExportType();
        switch (type) {
        case EXPORT_HTML:
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltCommunityInfoTemplateFileName);
            break;
        default:
            err = ErrNotSupport;
            loge("export type %d not support", type);
            break;
        };
    } else {
        err = ErrInvalidArg;
        loge("invalid exporter");
    }
    traceret(err);
    return err;
}

const QString &Community::parentNameId()
{
    CHECK_UID_TO_UPDATE_VALUE(mParentUid, mParentNameId, KModelHdlCommunity, nameId);
    return mParentNameId;
}

void Community::setParentNameId(const QString &newParentNameId)
{
    // TODO: query parent community uid from parent community code
    CHECK_MODIFIED_THEN_SET(mParentNameId, newParentNameId, KItemParentCommunity);
}


const QString &Community::contact() const
{
    return mContact;
}

void Community::setContact(const QString &newContact)
{
    CHECK_MODIFIED_THEN_SET(mContact, newContact, KItemContact);
}

const QString &Community::currentCEOUid() const
{
    return mCurrentCEOUid;
}

void Community::setCurrentCEOUid(const QString &newCurrentCEOUid)
{
    CHECK_MODIFIED_THEN_SET(mCurrentCEOUid, newCurrentCEOUid, KItemCEO);
}

const QString &Community::currentCEOName() const
{
    return mCurrentCEOName;
}

const QString &Community::currentCEONameWithUpdate()
{
    CHECK_UID_TO_UPDATE_VALUE(mCurrentCEOUid, mCurrentCEOName, KModelHdlPerson, name);
    return currentCEOName();
}

void Community::setCurrentCEOName(const QString &newCurrentCEO)
{
    mCurrentCEOName = newCurrentCEO;
}

const QString &Community::parentName()
{
    CHECK_UID_TO_UPDATE_VALUE(mParentUid, mParentName, KModelHdlCommunity, name);
    return mParentName;
}

void Community::setParentName(const QString &newParentName)
{
    mParentName = newParentName;
}

const QString &Community::statusName() const
{
    return mStatusName;
}

void Community::setModelStatusName(const QString &newStatusName)
{
    mStatusName = newStatusName;
}

const QString &Community::countryUid() const
{
    return mCountryUid;
}

void Community::setCountryUid(const QString &newCountryUid)
{
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountry);
}

const QString &Community::areaName()
{
    CHECK_UID_TO_UPDATE_VALUE(mAreaUid, mAreaName, KModelHdlArea, name);
    return mAreaName;
}

void Community::setAreaName(const QString &newAreaName)
{
    mAreaName = newAreaName;
    markItemAsModified(KItemArea);
}

qint64 Community::areaDbId() const
{
    return mAreaDbId;
}

void Community::setAreaDbId(qint64 newAreaDbId)
{
    CHECK_MODIFIED_THEN_SET(mAreaDbId, newAreaDbId, KItemArea);
}

const QString &Community::areaUid() const
{
    return mAreaUid;
}

void Community::setAreaUid(const QString &newAreaUid)
{
    CHECK_MODIFIED_THEN_SET(mAreaUid, newAreaUid, KItemArea);
}

const QString &Community::imgPath() const
{
    return mImgPath;
}

void Community::setImgPath(const QString &newImgPath)
{
    mImgPath = newImgPath;
}

qint64 Community::feastDate() const
{
    return mFeastDate;
}

void Community::setFeastDate(qint64 newFeastDate)
{
    CHECK_MODIFIED_THEN_SET(mFeastDate, newFeastDate, KItemFeastDay);
}

ErrCode Community::setFeastDateFromString(const QString &date, const QString &format)
{
    tracein;
    bool ok = false;
    logd("mFeastDate date string '%s'", date.toStdString().c_str());
    mFeastDate = DatetimeUtils::dateFromString(date, format, &ok);
    logd("mFeastDate %ll", mFeastDate);
    if (!ok) {
        loge("Invalid date '%s', format '%s'", STR2CHA(date), STR2CHA(format));
    }
    markItemAsModified(KItemFeastDay);
    traceout;
    return ok?ErrNone:ErrInvalidArg;

}

bool Community::isValid()
{
    // TODO: check more and more;
    return !name().isEmpty() && !nameId().isEmpty();
}

void Community::dump() const
{
    tracein;
#ifdef DEBUG_TRACE

    logd("DUMP:");
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- Addr %s", addr().toStdString().c_str());
    logd("- Feastday %s", DatetimeUtils::date2String(mFeastDate).toStdString().c_str());
    logd("- CEO Uid %s", currentCEOUid().toStdString().c_str());
    logd("- Aread uid %s", areaUid().toStdString().c_str());
//    logd("- Aread name %s", areaName().toStdString().c_str());
    logd("- Status %d", getStatus());
#endif //DEBUG_TRACE
}

const QString &Community::email() const
{
    return mEmail;
}

void Community::setEmail(const QString &newEmail)
{
    CHECK_MODIFIED_THEN_SET(mEmail, newEmail, KItemEmail);
}

const QString &Community::tel() const
{
    return mTel;
}

void Community::setTel(const QString &newTel)
{
    CHECK_MODIFIED_THEN_SET(mTel, newTel, KItemTel);
}

const QString &Community::province() const
{
    return mProvince;
}

void Community::setProvince(const QString &newProvince)
{
    CHECK_MODIFIED_THEN_SET(mProvince, newProvince, KItemProvince);
}

const QString &Community::addr() const
{
    return mAddr;
}

void Community::setAddr(const QString &newAddr)
{
    CHECK_MODIFIED_THEN_SET(mAddr, newAddr, KItemAddress);
}

const QString &Community::church() const
{
    return mChurch;
}

void Community::setChurch(const QString &newChurch)
{
    CHECK_MODIFIED_THEN_SET(mChurch, newChurch, KItemChurchAddress);
}
