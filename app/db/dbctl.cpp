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
 * Filename: dbctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "dbctl.h"
#include "logger.h"
#include "errcode.h"
#include "sqlite/dbsqlite.h"
#include <QStandardPaths>
#include <QDir>
#include "dbsqlitesaint.h"
#include "dbsqlitecommunity.h"
#include "dbsqliteedu.h"
#include "dbsqlitespecialist.h"
#include "filectl.h"
#include "defs.h"
#include "controllerdefs.h"

DbCtl* DbCtl::gInstance = nullptr;

DbCtl::DbCtl()
{
    tracein;
    mDatabase = DbSqlite::getInstance();
    dbInfo = new DbInfo();
    // TODO: refactor this code

    QString dbPath = FileCtl::getAppWorkingDataDir(KDatabasename);
    QString dbMetaPath = FileCtl::getAppWorkingDataDir(KDatabaseMetaName);

    logd("dbPath %s", dbPath.toStdString().c_str());
    dbInfo->setUri(dbPath);
    dbInfo->setMetaUri(dbMetaPath);

    traceout;
}

IDatabase *DbCtl::database() const
{
    return mDatabase;
}

QString DbCtl::getName() const
{
    return KControllerDb;
}


DbCtl* DbCtl::getInstance(){
    if (gInstance == nullptr){
        gInstance = new DbCtl();
    }
    return gInstance;
}

IDatabase* DbCtl::getDb(){
    return getInstance()->database();
}

ErrCode DbCtl::onLoad(){
    tracein;
    dbgtrace;
    ErrCode err = mDatabase->validateDbInfo(dbInfo);
    if (err == ErrNone) {
        err = mDatabase->loadDb(dbInfo);
    } else {
        loge("validate db info failed, err=%d", err);
    }
    traceret(err);
    return err;
}

void DbCtl::onUnload()
{
    traced;
    dbgtrace;
    // TODO: unload db controller??
}
