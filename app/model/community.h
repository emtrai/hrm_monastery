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

// BE WARE, ANY CHANGE TO THIS STATUS WILL IMPACT TO DB
// THIS VALUE IS WRITTEN DIRECTLY TO DB
// TODO: to do what to improve this???
enum CommunityStatus {
    INACTIVE = 0, // inactive, not operating
    ACTIVE, // active, operating
    BUILDING, // building phase, not ready


    MAX
};

class Community: public QObject, public DbModel
{
    Q_OBJECT
public:
    static DbModel *builder();
public:
    Community();
    ~Community();

    const QString &name() const;
    void setName(const QString &newName);

    qint32 level() const;

    void setLevel(qint32 newLevel);

    qint64 createDate() const;
    void setCreateDate(qint64 newCreateDate);

    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    CommunityStatus getStatus() const;
    void setStatus(CommunityStatus newStatus);

//    ErrCode save();
//    void dump();

    qint64 closeDate() const;
    void setCloseDate(qint64 newCloseDate);

protected:
    virtual DbModelHandler* getDbModelHandler();
private:
    QString mName;
    qint32 mLevel; // level 0: root, Level 1, Level 2 (belong to level 1), etc...
    Community* mParent;
    QString mParentUid;

    qint64 mCreateDate;
    qint64 mCloseDate;
    CommunityStatus status;

    QString mBrief;
    QString mFullInfo;
    QString mHistory;
};

#endif // COMMUNITY_H
