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
 * Filename: dbmodelutils.h
 * Author: Anh, Ngo Huy
 * Created date:6/29/2023
 * Brief:
 */
#ifndef DBMODELUTILS_H
#define DBMODELUTILS_H
#include <QString>
#include "errcode.h"
class DbModel;
class DbSqliteUpdateBuilder;
class DbModelUtils
{
public:
    /**
     * @brief get model by in order uid -> name id -> name
     *        Caller will take over returned obj, so need to make sure it'll be released
     *        after use
     * @param modelHdlName
     * @param nameId
     * @param name
     * @param ok
     * @return DbModel object
     */
    static DbModel* getDbModel(const QString& modelHdlName,
                        const QString& uid = nullptr,
                        const QString& nameId = nullptr,
                        const QString& name = nullptr,
                        bool* ok = nullptr);

    static ErrCode updateField(DbSqliteUpdateBuilder *builder,
                                const QString& uidFieldname,
                                const QString& hdlName,
                                const QString& uid = nullptr,
                                const QString& nameid = nullptr,
                                const QString& name = nullptr);
};

#endif // DBMODELUTILS_H
