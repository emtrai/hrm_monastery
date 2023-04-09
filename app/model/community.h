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
#include "dataexporter.h"
#include "dbmodel.h"

#define MISSION_DELIM ","

class Area;

// BE WARE, ANY CHANGE TO THIS STATUS WILL IMPACT TO DB
// THIS VALUE IS WRITTEN DIRECTLY TO DB
// TODO: to do what to improve this???
// TODO: change to DbModelStatus?????
enum CommunityStatus {
    COMMUNITY_INACTIVE = 0, // inactive, not operating
    COMMUNITY_ACTIVE, // active, operating
    COMMUNITY_BUILDING, // building phase, not ready


    COMMUNITY_STATUS_MAX
};

class Community: public DbModel
{
public:
    static DbModel *build();
protected:
    Community();
    Community(const Community& obj);
public:
    virtual ~Community();
    virtual void clone(const DbModel* model);

    virtual DbModelBuilder getBuilder() const;
    virtual void initExportFields();
    virtual void initImportFields();

    virtual QString modelName() const;

    qint32 level() const;

    void setLevel(qint32 newLevel);

    qint64 createDate() const;
    void setCreateDate(qint64 newCreateDate);
    ErrCode setCreateDateFromString(const QString& date, const QString& format="D.M.Y");

    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    DbModelStatus getStatus() const;
    void setModelStatus(int newStatus);

    qint64 closeDate() const;
    void setCloseDate(qint64 newCloseDate);
    ErrCode setCloseDateFromString(const QString& date, const QString& format="D.M.Y");

    const QString &church() const;
    void setChurch(const QString &newChurch);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    const QString &province() const;
    void setProvince(const QString &newProvince);

    const QString &tel() const;
    void setTel(const QString &newTel);

    const QString &email() const;
    void setEmail(const QString &newEmail);

    qint64 feastDate() const;
    void setFeastDate(qint64 newFeastDate);
    ErrCode setFeastDateFromString(const QString& date, const QString& format="D.M");

    virtual bool isValid();
    virtual void dump();
    const QString &imgPath() const;
    void setImgPath(const QString &newImgPath);

    const QString &areaUid() const;
    void setAreaUid(const QString &newAreaUid);


    qint64 areaDbId() const;
    void setAreaDbId(qint64 newAreaDbId);

    const QString &areaName();
    void setAreaName(const QString &newAreaName);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);


    const QString &statusName() const;
    void setModelStatusName(const QString &newStatusName);

    const QString &parentName();
    void setParentName(const QString &newParentName);

    const QString &currentCEOName() ;
    void setCurrentCEOName(const QString &newCurrentCEOName);

    const QString &currentCEOUid() const;
    void setCurrentCEOUid(const QString &newCurrentCEOUid);

    const QString &contact() const;
    void setContact(const QString &newContact);

    const QString &parentNameId();
    void setParentNameId(const QString &newParentNameId);

    const QString &currentCEONameId();
    void setCurrentCEONameId(const QString &newCurrentCEOCode);
    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;
    const QString &brief() const;
    void setBrief(const QString &newBrief);

    const QStringList &missionUid() const;
    QString missionUidString() const;
    void setMissionUid(const QStringList &newMissionUid);
    void setMissionUid(const QString &newMissionUid);
    void addMissionUid(const QString &newMissionUid);

    const QStringList &missionName() const;
    QString missionNameString() const;
    void setMissionName(const QStringList &newMissionName);
    void addMissionName(const QString &newMissionName);

    const QString &countryName();
    void setCountryName(const QString &newCountryName);

    const QString &areaNameId();
    void setAreaNameId(const QString &newAreaNameId);

    const QString &countryNameId();
    void setCountryNameId(const QString &newCountryNameId);

    const QString &fullInfo() const;
    void setFullInfo(const QString &newFullInfo);

    const QStringList &missionNameId() const;
    void setMissionNameId(const QStringList &newMissionNameId);
    void addMissionNameId(const QString &newMissionNameId);
    QString missionNameIdString() const;
    const QString &history() const;
    void setHistory(const QString &newHistory);

protected:
    virtual DbModelHandler* getDbModelHandler() const;

    /**
     * @brief return if model is deleted or not
     * @param msg: informative message
     * @return true: allow to delete, false otherwise
     */
    virtual bool allowRemove(QString* msg = nullptr);
    void copy(const Community& model);
    void resetResource();
private:
    QString mImgPath;
    QString mAddr;
    QString mProvince;
    QString mCountryUid;
    QString mCountryName;
    QString mCountryNameId;
    QString mChurch;
    QString mTel;
    QString mEmail;
    qint32 mLevel; // level 0: root, Level 1, Level 2 (belong to level 1), etc...
    QString mParentName;
    QString mParentUid;
    QString mParentNameId;

    qint64 mCreateDate;
    qint64 mCloseDate;
    qint64 mFeastDate;
    DbModelStatus mStatus;
    QString mStatusName;

    QString mBrief;
    QString mFullInfo;
    QString mHistory;

    QString mAreaUid;
    qint64 mAreaDbId;
    QString mAreaNameId; // just for display, not store in db of community
    QString mAreaName; // just for display, not store in db of community

    QString mCurrentCEOName; // full name
    QString mCurrentCEOUid; // uid
    QString mCurrentCEONameId; // code

    QString mContact; // Other detail contact

    // Nhiem vu xa hoi cua cong doan
    QStringList mMissionUid;
    QStringList mMissionName;
    QStringList mMissionNameId;
};

#endif // COMMUNITY_H
