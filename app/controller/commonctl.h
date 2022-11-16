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
 * Filename: commonctl.h
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#ifndef COMMONCTL_H
#define COMMONCTL_H

#include "controller.h"
#include <QList>

class DbModel;


class CommonCtl : public Controller
{

public:
    // Load Community from file
    ErrCode loadFromFile(const QString& path);

    virtual ErrCode loadFromDb();
    const QList<DbModel*> getAllItems(bool reload = false);

    /* Derived class should implement below functions
     * virtual const char* getPrebuiltFileName();
     * virtual const char* getPrebuiltFileType();
     * virtual QList<DbModel*> getItemFromDb();
     */

protected:
    CommonCtl();

    virtual QList<DbModel*> getItemFromDb();

    virtual void clearItemList(QList<DbModel*>* list);

protected:

    QList<DbModel*> mItemList;

};

#endif // COMMONCTL_H