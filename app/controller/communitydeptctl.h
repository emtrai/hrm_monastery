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
#include "errcode.h"
#include "modelcontroller.h"
#include "modelcontroller.h"
#include "communitydept.h"
#include "utils.h"

#define COMMUNITYDEPTCTL (CommunityDeptCtl::getInstance())

class Person;
class PersonDept;

class CommunityDeptCtl:public ModelController
{
    GET_INSTANCE_DECL(CommunityDeptCtl);
private:
    CommunityDeptCtl(); // not allow to allocate outside of this class
    virtual ~CommunityDeptCtl();
public:

    /**
     * @brief get list of active departments in community, \ref CommunityDept
     * @param communityUid
     * @param ok true if success, false otherwise
     * @return list of department, model CommunityDept. Caller must free after use
     */
    QList<DbModel*> getListDept(const QString& communityUid, bool* ok = nullptr);

    /**
     * @brief Get list of people in department of a community \ref PersonDept
     * @param commDeptUid uid of community/dept
     * @param ok true if success, false otherwise
     * @return list of department, model PersonDept. Caller must free after use
     */
    const QList<DbModel*> getListPerson(const QString& commDeptUid,
                                         int modelStatus = MODEL_STATUS_MAX,
                                         bool* ok = nullptr);

    /**
     * @brief Get list of active people of comm dept
     * @param commDeptUid
     * @param ok
     * @return
     */
    const QList<DbModel*> getListActivePeople(const QString& commDeptUid,
                                         bool* ok = nullptr);


    /**
     * @brief Get list of active people in list with role
     * @param commDeptUid
     * @param ok
     * @return List of people in format: <Name>:<role>
     */
    QString getListActivePeopleInString(const QString& commDeptUid,
                                            const QString &sep = "\n",
                                            bool* ok = nullptr);

protected:
    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
    const QString exportListPrebuiltTemplateName(const QString& modelName) const;

    virtual DbModelBuilder getMainBuilder();

    /**
     * @brief parse one item of prebuilt file
     *        Called when parse prebuilt file
     * @param jobj
     * @param ok true if parsed ok, false otherwise
     * @return
     */
    virtual CommunityDept* onJsonParseOneItem(const QJsonObject& jobj, bool* ok = nullptr);

    /**
     * @brief before import flow
     * @param importName
     * @param importFileType
     * @param fname
     * @return
     */
    virtual ErrCode onImportDataStart(const QString& importName, int importFileType, const QString& fname);

    /**
     * @brief import one item, this called by IDataImport callback when import, mainly when import csv
     * @param importFileType
     * @param items imported data in list, must be in order with headers
     * @param idx data idx, idx 0 is header
     * @return db model, caller must free it after use
     */
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx);
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString>& items, quint32 idx);

protected:

    QList<QString> mImportFields;

};

#endif // COMMUNITYDEPTCTL_H
