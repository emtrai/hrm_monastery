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
#include "errcode.h"
#include "modelcontroller.h"
#include "modelcontroller.h"
#include "community.h"
#include "dbmodel.h"

#define COMMUNITYCTL (CommunityCtl::getInstance())

class Person;
class CommunityPerson;

class CommunityCtl: public ModelController
{
    GET_INSTANCE_DECL(CommunityCtl);
private:
    CommunityCtl(); // not allow to allocate outside of this class
    virtual ~CommunityCtl();
public:

    /**
     * @brief get list of active people with communityUid
     *        Caller must free resource after use
     * @param communityUid
     * @return List of items
     */
    ErrCode getActivePersonList(const QString& communityUid, QList<Person*>& outList);
    ErrCode getPersonList(const QString& communityUid, QList<Person*>& outList, qint64 modelStatus = MODEL_STATUS_MAX);
    /**
     * @brief getListCommunityPerson
     * @param communityUid
     * @param outList List of CommunityPerson objects
     * @param modelStatus
     * @return
     */
    ErrCode getListCommunityPerson(const QString& communityUid, QList<DbModel*>& outList,
                                   qint64 modelStatus = MODEL_STATUS_MAX);

    /**
     * @brief get list of active community-person mapping of person
     * @param perUid
     * @param outList list of CommunityPerson objects
     * @return
     */
    ErrCode getListActiveCommunityPersonOfPerson(const QString& perUid, QList<DbModel*>& outList);

    /**
     * @brief Add person to community
     * @param comm community
     * @param per person to be added
     * @param status status of person in community \ref DbModelStatus
     * @param startdate start date of person in community, in format of DDMMYYY
     * @param enddate end date of person in community if any, in format of DDMMYYY
     * @param remark
     * @return error code
     */
    ErrCode addPerson2Community(const Community* comm,
                      const Person* per,
                      bool updateCommPer = true, // update mapping comm & per
                      int status = MODEL_STATUS_ACTIVE, // it's DbModelStatus
                      qint64 startdate = 0,
                      qint64 enddate = 0,
                      const QString& remark = nullptr);
    /**
     * @brief get root community, i.e. HOI_DONG
     * @return
     */
    const Community* getRootCommunity();
    /**
     * @brief Get manager list of community
     * @param communityUid
     * @param outList list of CommunityManager model
     * @param modelStatus
     * @return
     */
    ErrCode getManagersList(const QString &communityUid, QList<DbModel *> &outList,
                            qint64 modelStatus = MODEL_STATUS_MAX);
    ErrCode getAllManagersList(QList<DbModel *> &outList,
                            qint64 modelStatus = MODEL_STATUS_MAX);
    ErrCode getManagersListInString(const QString &communityUid, const QString& sep,
                            QString &outString,
                            qint64 modelStatus = MODEL_STATUS_MAX);
protected:

    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
    /**
     * @brief default/main builder
     * @return
     */
    virtual DbModelBuilder getMainBuilder();

    /**
     * @brief parse one item of prebuilt file
     *        Called when parse prebuilt file
     * @param jobj
     * @param ok true if parsed ok, false otherwise
     * @return
     */
    virtual DbModel* onJsonParseOneItem(const QJsonObject& jobj, bool* ok = nullptr);

    virtual const QString exportListPrebuiltTemplateName(const QString& modelName = nullptr) const;

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

protected:
    QList<QString> mImportHeaderFields;// import fields/headers, main used in CSV importing
};

#endif // COMMUNITYCTL_H
