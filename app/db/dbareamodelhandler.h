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
 * Filename: dbareamodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:10/25/2022
 * Brief:
 */
#ifndef DBAREAMODELHANDLER_H
#define DBAREAMODELHANDLER_H

#include "dbmodelhandler.h"
#include <QList>
#include "dbmodel.h"

class Person;
class Community;
class Area;

/**
 * @brief Specific handler for Area
 */
class DbAreaModelHandler
{
public:
    /**
     * @brief Get List of contact people, caller must free resource after use
     * @param areaUid
     * @param status \ref DbModelStatus
     * @return AreaPerson model
     */
    virtual QList<DbModel*> getListContactPeople(const QString& areaUid, int status = 0) = 0;
    /**
     * @brief get list of communities of area
     * @param areaUid
     * @param status
     * @return list of \ref Community
     */
    virtual QList<DbModel*> getListCommunities(const QString& areaUid, int status = 0) = 0;
    /**
     * @brief Add contact person to db
     * @param area Area which persone belong to
     * @param person Person to be added
     * @param status status of contact person, i.e active (can contact), inactive (cannot contact)
     * @param roleUid role of person
     * @param startdate
     * @param enddate
     * @param remark
     * @return
     */
    virtual ErrCode addContactPerson(const DbModel* area,
                                     const DbModel* person,
                                     int status = MODEL_ACTIVE,
                                     const QString& roleUid = nullptr,
                                     qint64 startdate = 0,
                                     qint64 enddate = 0,
                                     const QString& remark = nullptr) = 0;
};

#endif // DBAREAMODELHANDLER_H
