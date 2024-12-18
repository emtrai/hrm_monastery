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
 * Filename: dbcommunitymodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:10/10/2022
 * Brief:
 */
#ifndef DBCOMMUNITYMODELHANDLER_H
#define DBCOMMUNITYMODELHANDLER_H

#include <QList>
#include "dbmodel.h"

class Person;
class Community;
class CommunityPerson;

class DbCommunityModelHandler
{
public:

    virtual QList<Person*> getListPerson(const QString& uid, int modelStatus = MODEL_STATUS_MAX, const QString* perStatusUid = nullptr) = 0;
    /**
     * @brief getListCommunityPerson
     * @param commUid
     * @param modelStatus
     * @param perStatusUid
     * @return List of CommunityPerson objects
     */
    virtual QList<DbModel*> getListCommunityPerson(const QString& commUid, int modelStatus = MODEL_STATUS_MAX, const QString* perStatusUid = nullptr) = 0;
    virtual ErrCode addPerson2Community(const Community *comm,
                                        const Person* per,
                                        bool updateCommPer = true, // update mapping comm & per
                                      int status = MODEL_STATUS_ACTIVE,
                                      qint64 startdate = 0,
                                      qint64 enddate = 0,
                                      const QString& remark = nullptr,
                                      bool notifyDbChange = true) = 0;
    virtual QList<DbModel*> getListCommunityPersonOfPerson(const QString& perUid,
                                                            int modelStatus = MODEL_STATUS_MAX) = 0;
    virtual QList<DbModel*> getListActiveCommunityPersonOfPerson(const QString& perUid) = 0;
    /**
     * @brief Root/Top level community
     * @return Community model, null if not exist
     */
    virtual const Community* getRootCommunity() = 0;

    /**
     * @brief Get manager list of community
     * @param communityUid
     * @param outList list of CommunityManager model
     * @param modelStatus
     * @return
     */
    virtual ErrCode getManagersList(const QString &communityUid, QList<DbModel *> &outList,
                                          qint64 modelStatus) = 0;
    virtual ErrCode getManagersListWithRole(const QString &communityUid,
                                            QList<DbModel *> &outList,
                                            const QString& roleUid,
                                            qint64 modelStatus = MODEL_STATUS_MAX) = 0;
    virtual ErrCode getAllManagersList(QList<DbModel *> &outList,
                               qint64 modelStatus = MODEL_STATUS_MAX) = 0;
    virtual ErrCode getCEOList(const QString &communityUid, QList<DbModel *> &outList,
                               qint64 modelStatus = MODEL_STATUS_MAX) = 0;
    /**
     * @brief getCurrentCEO
     * @param communityUid
     * @param ceo
     * @param isActiveCEO
     * @return  ErrNoData if not found CEO, ErrNone if found, else other error code
     */
    virtual ErrCode getCurrentCEO(const QString &communityUid,
                                  Person** ceo = nullptr,
                                  bool* isActiveCEO = nullptr) = 0;
};

#endif // DBCOMMUNITYMODELHANDLER_H
