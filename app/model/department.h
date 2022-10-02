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
 * Filename: department.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <QObject>
#include <QString>
#include "dbmodel.h"

class Department:public QObject, public DbModel
{
    Q_OBJECT
        public:
                 Department();

            static DbModel *builder();

                 qint64 parentDbId() const;
                 void setParentDbId(qint64 newParentDbId);

                 const QString &parentUid() const;
                 void setParentUid(const QString &newParentUid);

                 qint64 communityDbId() const;
                 void setCommunityDbId(qint64 newCommunityDbId);

                 const QString &remark() const;
                 void setRemark(const QString &newRemark);

                 qint64 hoDPersonId() const;
                 void setHoDPersonId(qint64 newHoDPersonId);

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

                 const QString &shortName() const;
                 void setShortName(const QString &newShortName);

                 const QString &communityUid() const;
                 void setCommunityUid(const QString &newCommunityUid);

             protected:
                 virtual DbModelHandler* getDbModelHandler();
             private:

            QString mShortName;
            qint64 mParentDbId;
            QString mParentUid;
            qint64 mCommunityDbId;
            QString mCommunityUid;
            QString mRemark;
            qint64 mHoDPersonId;
            qint64 mEstablishDate;
            QString mEmail;
            QString mAddr;
            QString mTel;
            QString mBrief;
            qint64 mStatus;
};

#endif // DEPARTMENT_H
