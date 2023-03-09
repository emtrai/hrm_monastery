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

class CommunityDept : public DbModel
{
public:
    CommunityDept();

    static DbModel *build();
    virtual DbModelBuilder getBuilder();
    virtual QString modelName() const;

    virtual const QString &name() const;

    qint64 establishDate() const;
    void setEstablishDate(qint64 newEstablishDate);
    void setEstablishDateFromString(const QString &date, const QString &format="D.M.Y");

    const QString &email() const;
    void setEmail(const QString &newEmail);

    const QString &addr() const;
    void setAddr(const QString &newAddr);

    const QString &tel() const;
    void setTel(const QString &newTel);

    const QString &brief() const;
    void setBrief(const QString &newBrief);

    qint64 status() const;
    void setStatus(qint64 newStatus);

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
    void setClosedDateFromString(const QString &date, const QString &format="D.M.Y");

protected:
    virtual DbModelHandler* getDbModelHandler();
private:

    QString mDepartmentUid;
    qint64 mDepartmentDbId;
    QString mDepartmentName; // just for caching
    QString mCommunityUid;
    qint64 mCommunityDbId;
    QString mCommunityName; // just for caching
    qint64 mEstablishDate;
    qint64 mClosedDate;
    QString mEmail;
    QString mAddr;
    QString mTel;
    QString mBrief;
    qint64 mStatus;
};

#endif // COMMUNITYDEPT_H
