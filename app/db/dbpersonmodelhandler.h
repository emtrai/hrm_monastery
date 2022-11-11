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
 * Filename: dbpersonmodelhandler.h
 * Author: Anh, Ngo Huy
 * Created date:9/10/2022
 * Brief:
 */
#ifndef DBPERSONMODELHANDLER_H
#define DBPERSONMODELHANDLER_H

#include "dbmodelhandler.h"
#include "errcode.h"
#include "personevent.h"

class Specialist;

class DbPersonModelHandler : public virtual DbModelHandler
{
public:
    DbPersonModelHandler();
    virtual ErrCode addEvent(const QString& personUid, const QString& eventUid,
                     qint64 date, const QString& title, const QString& remark) = 0;
    virtual QList<PersonEvent*>* getListEvents( const QString& personUid,
                                                const QString* eventUid = nullptr,
                                                qint64 date = 0) = 0;
    virtual QList<DbModel*> getSpecialistList(const QString& personUid) = 0;
};

#endif // DBPERSONMODELHANDLER_H
