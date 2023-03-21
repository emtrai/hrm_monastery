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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef PEOPLE_H
#define PEOPLE_H

#include <QObject>
#include <QString>
#include <QSet>
#include "address.h"
#include "errcode.h"

#include <QList>
#include <QHash>
#include <QMap>
#include "dbmodel.h"
#include "dataexporter.h"
#include "idataimporter.h"
#include "utils.h"
#include <functional>
#include <iostream>

class Education;
class Work;
class Person;
class ModelController;

typedef QString *GET_DATA_FUNC();

class Person: public DbModel
{
public:
    static DbModel* build();
    virtual DbModelBuilder getBuilder();
    virtual void initExportFields();
    virtual void initImportFields();
    /**
     * @brief validate if data is all valid
     * @param result of validate for each field Field:ErrCode
     * @return ErrNone on ok, ErrInvalidData if data is invalid, other error code otherwhise
     */
    virtual ErrCode validateAllFields();
private:
    ErrCode commonCheckField(QString& name,
                             QString& uid,
                             ModelController* controller,
                             const char* const itemName,
                             int& invalidField);
public:
    Person();
    virtual ~Person();
    virtual void clone(const DbModel* per);
    virtual void buildUidIfNotSet();
    virtual QString buildUid(const QString* seed = nullptr);
    virtual QString modelName() const;

    const QString &firstName() const;
    void setFirstName(const QString &newFirstName);
    const QString &lastName() const;
    void setLastName(const QString &newLastName);


    const QString &birthPlace() const;
    void setBirthPlace(const QString &newBirthPlace);

    qint64 birthday() const;
    void setBirthday(qint64 newBirthday);
    void setBirthday(const QString& newBirthday);

    ErrCode setNameFromFullName(const QString& name);

    QString getFullName() const;
    ErrCode fromCSVFile(const QString& fname);

    virtual DataExporter* getExporter();
    const QString &personCode() const;
    void setPersonCode(const QString &newPersonCode);

    qint64 christenDate() const;
    void setChristenDate(qint64 newChristenDate);
    void setChristenDate(const QString& newChristenDate,
                         const QString& format = DATE_FORMAT_DMY);

    virtual ErrCode exportTo(const QString &fpath, ExportType type);

    const QString &imgPath() const;
    void setImgPath(const QString &newImgPath);

    qint64 trainJoinDate() const;
    void setTrainJoinDate(qint64 newTrainJoinDate);
    void setTrainJoinDate(const QString& newTrainJoinDate,
                          const QString& format = DATE_FORMAT_DMY);

    Person *trainPIC() const;
    void setTrainPIC(Person *newTrainPIC);

    const QStringList &saintUidList() const;
    QString saintUidListInString();
    void setSaintUidList(const QStringList &newSaintUidList);
    void setSaintUidList(const QString &newSaintUidList);
    void clearSaintUid();
    void addSaintUid(const QString& saint);

    const QString &hollyName() const;
    QStringList hollyNameInList();
    void setHollyName(const QString &newHollyName, bool parseUid = false);
//    ErrCode checkAndUpdateSaintListFromHollyName();

    const QString &nationalityUid() const;
    void setNationalityUid(const QString &newNationalityUid);

    const QString &nationalityName() const;
    void setNationalityName(const QString &newNationalityName);


    virtual void dump();

    const QString &ethnicUid() const;
    void setEthnicUid(const QString &newEthnicUid);

    const QString &ethnicName() const;
    void setEthnicName(const QString &newEthnicName);

    const QString &idCard() const;
    void setIdCard(const QString &newIdCard);

    qint64 idCardIssueDate() const;
    void setIdCardIssueDate(qint64 newIdCardIssueDate);
    void setIdCardIssueDate(const QString& newIdCardIssueDate);

    const QString &idCardIssuePlace() const;
    void setIdCardIssuePlace(const QString &newIdCardIssuePlace);

    const QString &eduUid() const;
    void setEduUid(const QString &newEduUid);

    const QString &eduName() const;
    void setEduName(const QString &newEduName);

    const QStringList &specialistUidList() const;
    void setSpecialistUidList(const QStringList &newSpecialistUidList);
    void setSpecialistUidList(const QString &newSpecialistUidList);
    void setSpecialistNames(const QString &newSpecialists, bool parseUid = false);
    void clearSpecialistUid();
    void addSpecialistUid(const QString& uid);
    QList<DbModel *> getSpecialistList();

    const QStringList &specialistNameList() const;
    void setSpecialistNameList(const QStringList &newSpecialistNameList);

    const QString &course() const;
    void setCourse(const QString &newCourse);

    const QString &courseUid() const;
    void setCourseUid(const QString &newCourseUid);

    const QString &countryUid() const;
    void setCountryUid(const QString &newCountryUid);

    const QString &countryName() const;
    void setCountryName(const QString &newCountryName);

    const QString &provinceUid() const;
    void setProvinceUid(const QString &newProvinceUid);

    const QString &provinceName() const;
    void setProvinceName(const QString &newProvinceName);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    const QString &churchAddr() const;
    void setChurchAddr(const QString &newChurchAddr);

    const QStringList &email() const;
    void setEmail(const QStringList &newEmail);
    void setEmail(const QString &newEmail);

    const QStringList &tel() const;
    void setTel(const QStringList &newTel);
    void setTel(const QString &newTel);

    const QString &dadName() const;
    void setDadName(const QString &newDadName);

    qint64 dadBirthday() const;
    void setDadBirthday(qint64 newDadBirthday);
    void setDadBirthday(const QString& newDadBirthday,
                        const QString& format = DATE_FORMAT_DMY);

    const QString &dadAddr() const;
    void setDadAddr(const QString &newDadAddr);

    const QString &momName() const;
    void setMomName(const QString &newMomName);

    qint64 momBirthday() const;
    void setMomBirthday(qint64 newMomBirthday);
    void setMomBirthday(const QString& newMomBirthday,
                        const QString& format = DATE_FORMAT_DMY);

    const QString &momAddr() const;
    void setMomAddr(const QString &newMomAddr);


    const QString &familyHistory() const;
    void setFamilyHistory(const QString &newFamilyHistory);

    const QString &christenPlace() const;
    void setChristenPlace(const QString &newChristenPlace);

    const QString &hollyPlace() const;
    void setHollyPlace(const QString &newHollyPlace);

    qint64 hollyDate() const;
    void setHollyDate(qint64 newHollyDate);
    void setHollyDate(const QString& newHollyDate,
                      const QString& format = DATE_FORMAT_DMY);

    const QString &familyContact() const;
    void setFamilyContact(const QString &newFamilyContact);

    qint64 joinDate() const;
    void setJoinDate(qint64 newJoinDate);
    void setJoinDate(const QString& newJoinDate,
                     const QString& format = DATE_FORMAT_DMY);

    const QString &joinPICUid() const;
    void setJoinPICUid(const QString &newJoinPICUid);

    const QString &joinPICName() const;
    void setJoinPICName(const QString &newJoinPICName);

    qint64 preTrainJoinDate() const;
    void setPreTrainJoinDate(qint64 newPreTrainJoinDate);
    void setPreTrainJoinDate(const QString& newPreTrainJoinDate,
                             const QString& format = DATE_FORMAT_DMY);

    const QString &preTrainPICUid() const;
    void setPreTrainPICUid(const QString &newPreTrainPICUid);

    const QString &preTrainPICName() const;
    void setPreTrainPICName(const QString &newPreTrainPICName);

    const QString &trainPICUid() const;
    void setTrainPICUid(const QString &newTrainPICUid);

    const QString &trainPICName() const;
    void setTrainPICName(const QString &newTrainPICName);

    const QString &vowsCEOUid() const;
    void setVowsCEOUid(const QString &newVowsCEOUid);

    const QString &vowsCEOName() const;
    void setVowsCEOName(const QString &newVowsCEOName);

    qint64 vowsDate() const;
    void setVowsDate(qint64 newVowsDate);
    void setVowsDate(const QString& newVowsDate,
                     const QString& format = DATE_FORMAT_DMY);

    qint64 eternalVowsDate() const;
    void setEternalVowsDate(qint64 newEternalVowsDate);
    void setEternalVowsDate(const QString& newEternalVowsDate,
                            const QString& format = DATE_FORMAT_DMY);

    const QString &eternalVowsCEOUid() const;
    void setEternalVowsCEOUid(const QString &newEternalVowsCEOUid);

    const QString &eternalVowsCEOName() const;
    void setEternalVowsCEOName(const QString &newEternalVowsCEOName);

    const QString &eternalVowsPICUid() const;
    void setEternalVowsPICUid(const QString &newEternalVowsPICUid);

    const QString &eternalVowsPICName() const;
    void setEternalVowsPICName(const QString &newEternalVowsName);

    qint64 bankDate() const;
    void setBankDate(qint64 newBankDate);
    void setBankDate(const QString& newBankDate,
                     const QString& format = DATE_FORMAT_DMY);

    const QString &bankPlace() const;
    void setBankPlace(const QString &newBankPlace);

    qint64 goldenDate() const;
    void setGoldenDate(qint64 newGoldenDate);
    void setGoldenDate(const QString& newGoldenDate,
                       const QString& format = DATE_FORMAT_DMY);

    const QString &goldenPlace() const;
    void setGoldenPlace(const QString &newGoldenPlace);

    qint64 eternalDate() const;
    void setEternalDate(qint64 newEternalDate);
    void setEternalDate(const QString& newEternalDate,
                       const QString& format = DATE_FORMAT_DMY);

    const QString &eternalPlace() const;
    void setEternalPlace(const QString &newEternalPlace);

    const QString &statusUid() const;
    void setStatusUid(const QString &newStatusUid);

    const QString &statusName() const;
    void setStatusName(const QString &newStatusName);

    qint64 retireDate() const;
    void setRetireDate(qint64 newRetireDate);
    void setRetireDate(const QString & newRetireDate,
                       const QString& format = DATE_FORMAT_DMY);

    const QString &retirePlace() const;
    void setRetirePlace(const QString &newRetirePlace);

    qint64 deadDate() const;
    void setDeadDate(qint64 newDeadDate);
    void setDeadDate(const QString&  newDeadDate,
                     const QString& format = DATE_FORMAT_DMY);

    const QString &deadPlace() const;
    void setDeadPlace(const QString &newDeadPlace);

    const QString &imgId() const;
    void setImgId(const QString &newImgId);

    const QStringList &eventUidList() const;
    void setEventUidList(const QStringList &newEventUidList);

    const QList<Education *> &educationList() const;
    void setEducationList(const QList<Education *> &newEducationList);

    const QList<Work *> &workList() const;
    void setWorkList(const QList<Work *> &newWorkList);


    const QHash<QString, QString> &educationUidList() const;
    void setEducationUidList(const QHash<QString, QString> &newEducationUidList);

    const QHash<QString, QString> &workUidList() const;
    void setWorkUidList(const QHash<QString, QString> &newWorkUidList);

    const QString &otherContact() const;
    void setOtherContact(const QString &newOtherContact);


    qint64 eucharistDate() const;
    void setEucharistDate(qint64 newEucharistDate);
    void setEucharistDate(const QString& newEucharistDate,
                          const QString& format = DATE_FORMAT_DMY);

    const QString &eucharistPlace() const;
    void setEucharistPlace(const QString &newEucharistPlace);

    qint64 feastDay() const;
    void setFeastDay(qint64 newFeastDay);
    void setFeastDay(const QString& newFeastDay,
                     const QString& format = DATE_FORMAT_MD);

    virtual ErrCode onImportItem(const QString& importName,
                                 int importFileType,
                                 const QString& keyword,
                                 const QString& value,
                                 quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);

    const QString &areaUid() const;
    void setAreaUid(const QString &newAreaUid);

    const QString &areaName() const;
    void setAreaName(const QString &newAreaName);

    const QString &departUid() const;
    void setDepartUid(const QString &newDepartUid);

    const QString &departName() const;
    void setDepartName(const QString &newDepartName);

    const QString &communityUid() const;
    void setCommunityUid(const QString &newCommunityUid);

    const QString &communityName() const;
    void setCommunityName(const QString &newCommunityName);

    const QString &eduDetail() const;
    void setEduDetail(const QString &newEduDetail);

    const QString &specialistInfo() const;
    void setSpecialistInfo(const QString &newSpecialistInfo);

    const QHash<QString, QString> &saintUidNameMap() const;
    void setSaintUidNameMap(const QHash<QString, QString> &newSaintUidNameMap);


    const QString &currentWorkUid() const;
    void setCurrentWorkUid(const QString &newCurrentWorkUid);

    const QString &currentWorkName() const;
    void setCurrentWorkName(const QString &newCurrentWorkName);

    const QHash<QString, ExportCallbackFunc > &exportFields() const;

protected:
    virtual DbModelHandler *getDbModelHandler() const;
    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;

    virtual ErrCode prepare2Save();

protected:
    QString mPersonCode; // TODO: reconsider again if we need this, as we have common "nameid"

    QString mFirstName;
    QString mLastName;

    qint64 mBirthday;
    QString mBirthPlace;

    QString mHollyName;
    QStringList mSaintUidList;
    QHash<QString, QString> mSaintUidNameMap; // TODO: duplicate with mSaintUidList??

    qint64 mFeastDay;

    QString mImgId;
    QString mImgPath;

    // nationality
    QString mNationalityUid;
    QString mNationalityName;


    QString mEthnicUid;
    QString mEthnicName;

    // ID card
    QString mIdCard;
    QString mIdCardIssuePlace;
    qint64 mIdCardIssueDate;

    // education
    QString mEduUid;
    QString mEduName;
    QString mEduDetail;


    QStringList mSpecialistUidList;
    QStringList mSpecialistNameList;
    QHash<QString, QString> mSpecialistUidNameMap; // TODO: duplicate with mSpecialistUidList??
    QString mSpecialistInfo;

    //course
    QString mCourseUid;
    QString mCourse;

    // country
    QString mCountryUid;
    QString mCountryName;

    // province
    QString mProvinceUid;
    QString mProvinceName;

    // address
    QString mAddr;

    // church
    QString mChurchAddr;

    // contact
    QStringList mEmail;
    QStringList mTel;
    QString mOtherContact;

    //area
    QString mAreaUid;
    QString mAreaName;

    //department
    QString mDepartUid;
    QString mDepartName;


    //community (current)
    QString mCommunityUid;
    QString mCommunityName;

    // dad
    QString mDadName;
    qint64 mDadBirthday;
    QString mDadAddr;

    // mom
    QString mMomName;
    qint64 mMomBirthday;
    QString mMomAddr;

    // family info, includes: name of dad, mom, address, the number of brothers/sisters, etc.
    QString mFamilyHistory;

    QString mFamilyContact;

    // Rua toi
    qint64 mChristenDate;
    QString mChristenPlace;

    // Thanh the
    qint64 mEucharistDate;
    QString mEucharistPlace;

    // Them suc
    qint64 mHollyDate;
    QString mHollyPlace;


    QHash<QString,QString> mEducationUidList;
    QHash<QString,QString> mWorkUidList;

    QList<Education*> mEducationList;
    QList<Work*> mWorkList;

    // cong viec hien tai
    QString mCurrentWorkUid;
    QString mCurrentWorkName;

    qint64 mJoinDate; // ngay nhap tu
    QString mJoinPICUid;
    QString mJoinPICName;

    qint64 mPreTrainJoinDate; // ngay gia nhap Tien Tap Vien
    QString mPreTrainPICUid; // nguoi dac trac
    QString mPreTrainPICName; // nguoi dac trac

    qint64 mTrainJoinDate; // ngay gia nhap Tap Vien
    QString mTrainPICUid; // nguoi dac trac
    QString mTrainPICName; // nguoi dac trac

    qint64 mVowsDate; // ngay tien khan
    QString mVowsCEOUid; // chi tong phu trach
    QString mVowsCEOName; // chi tong phu trach


    qint64 mEternalVowsDate; // ngay vinh khan
    QString mEternalVowsCEOUid; // chi tong phu trach
    QString mEternalVowsCEOName;
    QString mEternalVowsPICUid; // nguoi dac trac
    QString mEternalVowsPICName; // nguoi dac trac

    qint64 mBankDate; // ngan khanh
    QString mBankPlace;
    qint64 mGoldenDate; // kim khanh
    QString mGoldenPlace;
    qint64 mEternalDate; // vinh khanh
    QString mEternalPlace;

    QString mStatusUid;
    QString mStatusName;
    qint64 mRetireDate;
    QString mRetirePlace;
    qint64 mDeadDate;
    QString mDeadPlace; // noi chon cat

    QStringList mEventUidList;
};

#endif // PEOPLE_H
