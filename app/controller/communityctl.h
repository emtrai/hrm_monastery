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
 * Filename: communityctl.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef COMMUNITYCTL_H
#define COMMUNITYCTL_H

#include <QString>
#include "logger.h"
#include "errcode.h"
#include "controller.h"
#include "commonctl.h"

#define COMMUNITYCTL (CommunityCtl::getInstance())

class Community;
class Person;

class CommunityCtl: public CommonCtl
{

public:
    // Load Community from file
    ErrCode loadFromFile(const QString& path);

//    ErrCode loadFromDb();

//    const QList<Community*> getCommunityList(bool reload = false);
    const QList<DbModel*> getPersonList(const QString& communityUid);
    ErrCode addPerson2Community(const Community* comm,
                      const Person* per,
                      int status = 0,
                      qint64 startdate = 0,
                      qint64 enddate = 0,
                      const QString& remark = nullptr);

    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
    virtual QList<DbModel*> getItemFromDb();
    virtual DbModelHandler* getModelHandler();
    virtual ErrCode addPerson(Community* comm, Person* per);
    virtual QHash<int, QString> getStatusIdNameMap();
private:
    CommunityCtl();

public:
    static CommunityCtl* getInstance();
protected:
    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
    virtual Community* parseOneItem(const QJsonObject& jobj);

private:
    static CommunityCtl* gInstance;

//    QList<Community*> mListCommunity;

//public slots:
//    virtual void onLoad();
};

#endif // COMMUNITYCTL_H
