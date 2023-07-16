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
 * Filename: dbmodelfactory.h
 * Author: Anh, Ngo Huy
 * Created date:7/5/2023
 * Brief:
 */
#ifndef DBMODELFACTORY_H
#define DBMODELFACTORY_H

#include <QString>
#include <QHash>
#include "dbmodel.h"
#include "errcode.h"

class DbModelFactory
{
public:
    static DbModelBuilder getBuilder(const QString& modelName);
private:
    static DbModelFactory* getInstance();
private:
    DbModelFactory();
    ~DbModelFactory();
    DbModelBuilder _getBuilder(const QString& modelName);
    void addBuilders();
    ErrCode _addBuilder(const QString& name, DbModelBuilder builder);
private:
    static DbModelFactory* mInstance;
    QHash<QString, DbModelBuilder> mList;
};

#endif // DBMODELFACTORY_H
