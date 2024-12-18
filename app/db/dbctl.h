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
 * Filename: dbctl.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef DBCTL_H
#define DBCTL_H

#include <QObject>
#include "controller.h"
#include "idatabase.h"
#include "dbinfo.h"


#define DB DbCtl::getDb()

class DbCtl: public Controller
{

public:
    static DbCtl* getInstance();
    static IDatabase* getDb();
    IDatabase *database() const;
    virtual QString getName() const;
    bool isDbExist();

private:
    DbCtl();
private:
    static DbCtl* gInstance;

    IDatabase* mDatabase;
    DbInfo* dbInfo;

    // event
public:
    virtual ErrCode onLoad();
    virtual void onUnload();
};

#endif // DBCTL_H
