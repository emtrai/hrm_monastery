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
#include "filectl.h"
#include "communityctl.h"

DbModel *Community::build()
{
    traced;
    Community* item = new Community();
    item->init();
    return item;
}

Community::Community():DbModel()
{
    traced;
}

Community::Community(const Community &obj):DbModel(obj)
{
    traced;
    setCommunityCode(obj.communityCode());
    setContact(obj.contact());

    setImgPath(obj.imgPath());
    setParentUid(obj.parentUid());
    setParentName(obj.parentName());
    setAreaName(obj.areaName());
    setAreaUid(obj.areaUid());
    setAreaDbId(obj.areaDbId());
    setCountryUid(obj.countryUid());
    setChurch(obj.church());
    setAddr(obj.addr());
    setTel(obj.tel());
    setEmail(obj.email());
    setFeastDate(obj.feastDate());
    setCreateDate(obj.createDate());
    setStatus(obj.getStatus());
    setCurrentCEOUid(obj.currentCEOUid());
    setCurrentCEO(obj.currentCEO());
    setIntro(obj.intro());
    setContact(obj.contact());
    setLevel(obj.level());
    setRemark(obj.remark());
    setCommunityCode(obj.communityCode());
    setAreaCode(obj.areaCode());
    setCurrentCEOCode(obj.currentCEOCode());
    tracede;
}

Community::~Community()
{
    traced;
}

void Community::clone(const DbModel *model)
{
    traced;
    DbModel::clone(model);
    Community* comm = (Community*) model;
    setCommunityCode(comm->communityCode());
    setContact(comm->contact());

    setImgPath(comm->imgPath());
    setParentUid(comm->parentUid());
    setParentName(comm->parentName());
    setAreaName(comm->areaName());
    setAreaUid(comm->areaUid());
    setAreaDbId(comm->areaDbId());
    setCountryUid(comm->countryUid());
    setChurch(comm->church());
    setAddr(comm->addr());
    setTel(comm->tel());
    setEmail(comm->email());
    setFeastDate(comm->feastDate());
    setCreateDate(comm->createDate());
    setStatus(comm->getStatus());
    setCurrentCEOUid(comm->currentCEOUid());
    setCurrentCEO(comm->currentCEO());
    setIntro(comm->intro());
    setContact(comm->contact());
    setLevel(comm->level());
    setRemark(comm->remark());
    setCommunityCode(comm->communityCode());
    setAreaCode(comm->areaCode());
    setCurrentCEOCode(comm->currentCEOCode());
    tracede;
}

void Community::initExportFields()
{
    traced;
    DbModel::initExportFields();
    mExportFields.insert(KItemCommunityCode, [this](const QString& item){
        return this->communityCode();

    });
    mExportFields.insert(KItemCountry, [this](const QString& item){
        return this->country(); // TODO: get country id from country name???
    });
    mExportFields.insert(KItemAddress, [this](const QString& item){
        return this->addr();
    });
    mExportFields.insert(KItemEmail, [this](const QString& item){
        return this->email();
    });
    mExportFields.insert(KItemTel, [this](const QString& item){
        return this->tel();
    });
    mExportFields.insert(KItemEstablishDate, [this](const QString& item){
        return Utils::date2String(this->createDate(), DATE_FORMAT_YMD);
    });
    mExportFields.insert(KItemFeastDay, [this](const QString& item){
        return Utils::date2String(this->feastDate(), DATE_FORMAT_MD);
    });
    mExportFields.insert(KItemOtherContact, [this](const QString& item){
        return this->contact();
    });
    mExportFields.insert(KItemAreaCode, [this](const QString& item){
        return this->areaName();// TODO: need to search Area Uid from Area Code
    });
    mExportFields.insert(KItemParentCommunityCode, [this](const QString& item){
        return this->parentName();// TODO: need to search  Uid from  Code
    });
    mExportFields.insert(KItemIntro, [this](const QString& item){
        return this->intro();
    });
    mExportFields.insert(KItemCEOCode, [this](const QString& item){
        return this->currentCEO();
    });
    // TODO: implement more
    tracede;
}

void Community::initImportFields()
{
    traced;
    DbModel::initImportFields();
    mImportFields.insert(KItemCommunityCode, [this](const QString& value){
        this->setCommunityCode(value);
    });
    mImportFields.insert(KItemStatus, [this](const QString& value){
        this->setStatus(value.toInt()); // TODO: handle error case when convert to Int
    });
    mImportFields.insert(KItemCountry, [this](const QString& value){
        this->setCountry(value); // TODO: get country id from country name???
    });
    mImportFields.insert(KItemAddress, [this](const QString& value){
        this->setAddr(value);
    });
    mImportFields.insert(KItemEmail, [this](const QString& value){
        this->setEmail(value);
    });
    mImportFields.insert(KItemTel, [this](const QString& value){
        this->setTel(value);
    });
    mImportFields.insert(KItemEstablishDate, [this](const QString& value){
        this->setCreateDateFromString(value);
    });
    mImportFields.insert(KItemFeastDay, [this](const QString& value){
        this->setFeastDateFromString(value);
    });
    mImportFields.insert(KItemOtherContact, [this](const QString& value){
        this->setContact(value);
    });
    mImportFields.insert(KItemAreaCode, [this](const QString& value){
        this->setAreaCode(value);// TODO: need to search Area Uid from Area Code
    });
    mImportFields.insert(KItemParentCommunityCode, [this](const QString& value){
        this->setParentCode(value);// TODO: need to search  Uid from  Code
    });
    mImportFields.insert(KItemIntro, [this](const QString& value){
        this->setIntro(value);
    });
    mImportFields.insert(KItemCEOCode, [this](const QString& value){
        this->setCurrentCEOCode(value);
    });
    // TODO: implement more
    tracede;
}

qint32 Community::level() const
{
    return mLevel;
}

void Community::setLevel(qint32 newLevel)
{
    mLevel = newLevel;
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
    mCreateDate = newCreateDate;
}

void Community::setCreateDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("create date string '%s'", date.toStdString().c_str());
    mCreateDate = Utils::dateFromString(date, format);
    logd("mCreateDate %ll", mCreateDate);
}

const QString &Community::parentUid() const
{
    return mParentUid;
}

void Community::setParentUid(const QString &newParentUid)
{
    mParentUid = newParentUid;
}

CommunityStatus Community::getStatus() const
{
    return status;
}

void Community::setStatus(CommunityStatus newStatus)
{
    status = newStatus;
}

void Community::setStatus(int newStatus)
{
    if (newStatus < COMMUNITY_STATUS_MAX) {
        status = (CommunityStatus)newStatus;
        mStatusName = COMMUNITYCTL->status2Name(status);
    }
}

//ErrCode_t Community::save()
//{
//    return DbCtl::getInstance()->dbCommunity()->add(this);
//}

//void Community::dump()
//{
//    traced;
//    QString log = QStringLiteral(
//                      "name '%1'\n"
//                      )
//                      .arg(name());
//    logd("%s", log.toStdString().c_str());
//}

qint64 Community::closeDate() const
{
    return mCloseDate;
}

void Community::setCloseDate(qint64 newCloseDate)
{
    mCloseDate = newCloseDate;
}

DbModelHandler *Community::getDbModelHandler()
{
    return DbCtl::getDb()->getCommunityModelHandler();
}

const QString &Community::currentCEOCode() const
{
    return mCurrentCEOCode;
}

void Community::setCurrentCEOCode(const QString &newCurrentCEOCode)
{
    mCurrentCEOCode = newCurrentCEOCode;
    // TODO: query uid from code
}

const QString Community::exportTemplatePath(Exporter* exporter) const
{
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_HTML:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltCommunityInfoTemplateFileName);
        };
    }
    return QString();
}

const QString &Community::parentCode() const
{
    return mParentCode;
}

void Community::setParentCode(const QString &newParentCode)
{
    mParentCode = newParentCode;
    // TODO: query parent community uid from parent community code
}

const QString &Community::areaCode() const
{
    return mAreaCode;
}

void Community::setAreaCode(const QString &newAreaCode)
{
    mAreaCode = newAreaCode;
    // TODO: query area uid from area code
    // Community table only store Area Uid, not code, as code can be changed/update
    // so must search code to get uid
}

const QString &Community::contact() const
{
    return mContact;
}

void Community::setContact(const QString &newContact)
{
    mContact = newContact;
}

const QString &Community::intro() const
{
    return mIntro;
}

void Community::setIntro(const QString &newIntro)
{
    mIntro = newIntro;
}

const QString &Community::currentCEOUid() const
{
    return mCurrentCEOUid;
}

void Community::setCurrentCEOUid(const QString &newCurrentCEOUid)
{
    mCurrentCEOUid = newCurrentCEOUid;
}

const QString &Community::currentCEO() const
{
    return mCurrentCEO;
}

void Community::setCurrentCEO(const QString &newCurrentCEO)
{
    mCurrentCEO = newCurrentCEO;
}

const QString &Community::parentName() const
{
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

void Community::setStatusName(const QString &newStatusName)
{
    mStatusName = newStatusName;
}

const QString &Community::countryUid() const
{
    return mCountryUid;
}

void Community::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
}

const QString &Community::communityCode() const
{
    return mCommunityCode;
}

void Community::setCommunityCode(const QString &newCommunityCode)
{
    mCommunityCode = newCommunityCode;
}

const QString &Community::areaName() const
{
    return mAreaName;
}

void Community::setAreaName(const QString &newAreaName)
{
    mAreaName = newAreaName;
}

qint64 Community::areaDbId() const
{
    return mAreaDbId;
}

void Community::setAreaDbId(qint64 newAreaDbId)
{
    mAreaDbId = newAreaDbId;
}

const QString &Community::areaUid() const
{
    return mAreaUid;
}

void Community::setAreaUid(const QString &newAreaUid)
{
    mAreaUid = newAreaUid;
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
    mFeastDate = newFeastDate;
}

void Community::setFeastDateFromString(const QString &date, const QString &format)
{
    traced;
    logd("mFeastDate date string '%s'", date.toStdString().c_str());
    mFeastDate = Utils::dateFromString(date, format);
    logd("mFeastDate %ll", mFeastDate);

}

bool Community::isValid()
{
    // TODO: check more and more;
    return !name().isEmpty() && !uid().isEmpty();
}

void Community::dump()
{
    traced;
#ifdef DEBUG_TRACE

    logd("DUMP:");
    logd("- Uid %s", uid().toStdString().c_str());
    logd("- Name %s", name().toStdString().c_str());
    logd("- Addr %s", addr().toStdString().c_str());
    logd("- Feastday %s", Utils::date2String(mFeastDate).toStdString().c_str());
    logd("- CEO Uid %s", currentCEOUid().toStdString().c_str());
    logd("- Aread uid %s", areaUid().toStdString().c_str());
    logd("- Aread name %s", areaName().toStdString().c_str());
#endif //DEBUG_TRACE
}

const QString &Community::email() const
{
    return mEmail;
}

void Community::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
}

const QString &Community::tel() const
{
    return mTel;
}

void Community::setTel(const QString &newTel)
{
    mTel = newTel;
}

const QString &Community::country() const
{
    return mCountry;
}

void Community::setCountry(const QString &newCountry)
{
    mCountry = newCountry;
}

const QString &Community::province() const
{
    return mProvince;
}

void Community::setProvince(const QString &newProvince)
{
    mProvince = newProvince;
}

const QString &Community::addr() const
{
    return mAddr;
}

void Community::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
}

const QString &Community::church() const
{
    return mChurch;
}

void Community::setChurch(const QString &newChurch)
{
    mChurch = newChurch;
}
