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
    virtual void clone(const DbModel* per);
    virtual DbModelBuilder getBuilder();
    virtual void initExportFields();
    virtual void initImportFields();

    virtual QString modelName() const;


    qint32 level() const;

    void setLevel(qint32 newLevel);

    qint64 createDate() const;
    void setCreateDate(qint64 newCreateDate);
    void setCreateDateFromString(const QString& date, const QString& format="D.M.Y");

    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    DbModelStatus getStatus() const;
    void setStatus(DbModelStatus newStatus);
    void setStatus(int newStatus);

//    ErrCode save();
//    void dump();

    qint64 closeDate() const;
    void setCloseDate(qint64 newCloseDate);
    void setCloseDateFromString(const QString& date, const QString& format="D.M.Y");

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

    const QString &parentNameId() const;
    void setParentNameId(const QString &newParentNameId);

    const QString &currentCEOCode() const;
    void setCurrentCEOCode(const QString &newCurrentCEOCode);
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

    const QString &countryName() const;
    void setCountryName(const QString &newCountryName);

protected:
    virtual DbModelHandler* getDbModelHandler();

    /**
     * @brief return if model is deleted or not
     * @param msg: informative message
     * @return true: allow to delete, false otherwise
     */
    virtual bool allowRemove(QString* msg = nullptr);
private:
    QString mImgPath;
    QString mAddr;
    QString mProvince;
    QString mCountry;
    QString mCountryUid;
    QString mCountryName;
    QString mChurch;
    QString mTel;
    QString mEmail;
    qint32 mLevel; // level 0: root, Level 1, Level 2 (belong to level 1), etc...
    Community* mParent;
    QString mParentName;
    QString mParentUid;
    QString mParentNameId;

    qint64 mCreateDate;
    qint64 mCloseDate;
    qint64 mFeastDate;
    DbModelStatus status;
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

    QStringList mMissionUid;
    QStringList mMissionName;
};

#endif // COMMUNITY_H
