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
 * Filename: communitydept.h
 * Author: Anh, Ngo Huy
 * Created date:1/31/2023
 * Brief:
 */
#ifndef COMMUNITYDEPT_H
#define COMMUNITYDEPT_H

#include <dbmodel.h>

class Person;
class CommunityDept : public DbModel
{
public:
    explicit CommunityDept();
    virtual ~CommunityDept();

    static DbModel *build();
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;
    virtual void clone(const DbModel* model);
    virtual DbModel* clone() const;

    virtual void initExportFields();
    qint64 establishDate() const;
    void setEstablishDate(qint64 newEstablishDate);
    ErrCode setEstablishDateFromString(const QString &date);

    const QString &email() const;
    void setEmail(const QString &newEmail);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    const QString &tel() const;
    void setTel(const QString &newTel);

    const QString &brief() const;
    void setBrief(const QString &newBrief);

    qint64 modelStatus() const;
    void setModelStatus(qint64 newStatus);

    const QString &communityUid() const;
    void setCommunityUid(const QString &newCommunityUid);

    const QString &departmentUid() const;
    void setDepartmentUid(const QString &newDepartmentUid);

    qint64 departmentDbId() const;
    void setDepartmentDbId(qint64 newDepartmentDbId);

    qint64 communityDbId() const;
    void setCommunityDbId(qint64 newCommunityDbId);

    const QString &departmentName() const;
    void setDepartmentName(const QString &newDepartmentName);

    const QString & communityName() const;
    void setCommunityName(const QString & newCommunityName);

    qint64 closedDate() const;
    void setClosedDate(qint64 newClosedDate);
    ErrCode setClosedDateFromString(const QString &date);


    const QString &departmentNameId() const;
    void setDepartmentNameId(const QString &newDepartmentNameId);

    const QString &communityNameId() const;
    void setCommunityNameId(const QString &newCommunityNameId);

    QString modelStatusName() const;
    void setModelStatusName(const QString &newModelStatusName);

    /**
     * @brief Current director of department
     *        just for display
     * @return
     */
    Person *currentDirector() const;
    QString currentDirectorName();
    QString currentDirectorNameId();
    /**
     * @brief set current director of department
     *        just for display
     * @param newCurrentDirector
     */
    void setCurrentDirector(const Person *newCurrentDirector);

protected:
    virtual DbModelHandler* getDbModelHandler() const;
private:

    QString mDepartmentUid;
    qint64 mDepartmentDbId;
    QString mDepartmentNameId;
    QString mDepartmentName; // just for caching
    QString mCommunityUid;
    QString mCommunityNameId;
    qint64 mCommunityDbId;
    QString mCommunityName; // just for caching
    qint64 mEstablishDate;
    qint64 mClosedDate;
    QString mEmail;
    QString mAddr;
    QString mTel;
    QString mBrief;
    qint64 mStatus;
    QString mModelStatusName;

    Person* mCurrentDirector;
};

#endif // COMMUNITYDEPT_H
