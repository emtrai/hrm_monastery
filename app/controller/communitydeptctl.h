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
 * Filename: communitydeptctl.h
 * Author: Anh, Ngo Huy
 * Created date:2/1/2023
 * Brief:
 */
#ifndef COMMUNITYDEPTCTL_H
#define COMMUNITYDEPTCTL_H
#include <QString>
#include "logger.h"
#include "errcode.h"
#include "controller.h"
#include "commonctl.h"
#include "communitydept.h"

#define COMMUNITYDEPTCTL (CommunityDeptCtl::getInstance())

class Person;
class PersonDept;

class CommunityDeptCtl:public CommonCtl
{

public:
    virtual ErrCode addNew(DbModel* model);
    ErrCode loadFromFile(const QString& path);

    QList<DbModel*> getDepartList(const QString& communityUid);
    ErrCode addPerson2CommunityDept(PersonDept* perdept);

    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
    virtual QList<DbModel*> getItemFromDb();
    virtual DbModelHandler* getModelHandler();
    virtual ErrCode markModelDelete(DbModel* model);
    virtual ErrCode deleteModel(DbModel* model);

    virtual DbModel* doImportOneItem(int importFileType, const QStringList& items, quint32 idx);
    virtual DbModel* doImportOneItem(int importFileType, const QHash<QString, QString>& items, quint32 idx);

    const QList<DbModel*> getPersonList(const QString& deptUid);
    const QList<DbModel*> getDeptList(const QString& communityUid = nullptr);
private:
    CommunityDeptCtl();

public:
    static CommunityDeptCtl* getInstance();
protected:
    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
    virtual CommunityDept* parseOneItem(const QJsonObject& jobj);

protected:

    QList<QString> mImportFields;
private:
    static CommunityDeptCtl* gInstance;
protected:

};

#endif // COMMUNITYDEPTCTL_H
