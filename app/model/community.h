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
 * Filename: community.h
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <QObject>
#include <QString>
#include "dbmodel.h"
#include "errcode.h"
#include "address.h"
#include "person.h"
#include "department.h"
#include "iexporter.h"

class Area;
// BE WARE, ANY CHANGE TO THIS STATUS WILL IMPACT TO DB
// THIS VALUE IS WRITTEN DIRECTLY TO DB
// TODO: to do what to improve this???
// TODO: change to StatusModel?????
enum CommunityStatus {
    INACTIVE = 0, // inactive, not operating
    ACTIVE, // active, operating
    BUILDING, // building phase, not ready


    COMMUNITY_STATUS_MAX
};

class Community: public DbModel
{
public:
    static DbModel *build();
public:
    Community();
    Community(const Community& obj);
    ~Community();

    virtual void clone(const DbModel* per);
    virtual void initExportFields();
    virtual void initImportFields();

    qint32 level() const;

    void setLevel(qint32 newLevel);

    qint64 createDate() const;
    void setCreateDate(qint64 newCreateDate);
    void setCreateDateFromString(const QString& date, const QString& format="D.M.Y");

    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    CommunityStatus getStatus() const;
    void setStatus(CommunityStatus newStatus);
    void setStatus(int newStatus);

//    ErrCode save();
//    void dump();

    qint64 closeDate() const;
    void setCloseDate(qint64 newCloseDate);

    const QString &church() const;
    void setChurch(const QString &newChurch);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    const QString &province() const;
    void setProvince(const QString &newProvince);

    const QString &country() const;
    void setCountry(const QString &newCountry);

    const QString &tel() const;
    void setTel(const QString &newTel);

    const QString &email() const;
    void setEmail(const QString &newEmail);

    qint64 feastDate() const;
    void setFeastDate(qint64 newFeastDate);
    void setFeastDateFromString(const QString& date, const QString& format="D.M");

    virtual bool isValid();
    virtual void dump();
    const QString &imgPath() const;
    void setImgPath(const QString &newImgPath);

    const QString &areaUid() const;
    void setAreaUid(const QString &newAreaUid);


    qint64 areaDbId() const;
    void setAreaDbId(qint64 newAreaDbId);

    const QString &areaName() const;
    void setAreaName(const QString &newAreaName);

    const QString &communityCode() const;
    void setCommunityCode(const QString &newCommunityCode);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);


    const QString &statusName() const;
    void setStatusName(const QString &newStatusName);

    const QString &parentName() const;
    void setParentName(const QString &newParentName);

    const QString &currentCEO() const;
    void setCurrentCEO(const QString &newCurrentCEO);

    const QString &currentCEOUid() const;
    void setCurrentCEOUid(const QString &newCurrentCEOUid);

    const QString &intro() const;
    void setIntro(const QString &newIntro);

    const QString &contact() const;
    void setContact(const QString &newContact);

    const QString &areaCode() const;
    void setAreaCode(const QString &newAreaCode);

    const QString &parentCode() const;
    void setParentCode(const QString &newParentCode);

    const QString &currentCEOCode() const;
    void setCurrentCEOCode(const QString &newCurrentCEOCode);
    virtual const QString exportTemplatePath(Exporter* exporter) const;
protected:
    virtual DbModelHandler* getDbModelHandler();
private:
    QString mImgPath;
    QString mCommunityCode;    // TODO: community code: to be use later???
    QString mAddr;
    QString mProvince;
    QString mCountry;
    QString mCountryUid;
    QString mChurch;
    QString mTel;
    QString mEmail;
    qint32 mLevel; // level 0: root, Level 1, Level 2 (belong to level 1), etc...
    Community* mParent;
    QString mParentName;
    QString mParentUid;
    QString mParentCode;

    qint64 mCreateDate;
    qint64 mCloseDate;
    qint64 mFeastDate;
    CommunityStatus status;
    QString mStatusName;

    QString mBrief;
    QString mFullInfo;
    QString mHistory;
    Person* mPIC;
    QHash<qint32, QList<Person*>> mManagement; // role, list of person
    Area* mArea;
    // Board of management?
    QList<Department*> mDepList;

    QString mAreaUid;
    qint64 mAreaDbId;
    QString mAreaName; // just for display, not store in db of community
    QString mAreaCode; // Area code

    QString mCurrentCEO; // full name
    QString mCurrentCEOUid; // uid
    QString mCurrentCEOCode; // code

    QString mIntro;
    QString mContact; // Other detail contact
};

#endif // COMMUNITY_H
