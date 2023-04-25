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


DbCtl* DbCtl::gInstance = nullptr;

DbCtl::DbCtl()
{
    tracein;
    mDatabase = DbSqlite::getInstance();
    dbInfo = new DbInfo();
    // TODO: refactor this code
//    QString appPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    QDir appDirPath(appPath);
//    logd("App path %s", appPath.toStdString().c_str());

//    if (!appDirPath.exists()){
//        logi("App path not exist, create new one");
//        appDirPath.mkpath(appDirPath.absolutePath());
//    }

    QString dbPath = FileCtl::getAppWorkingDataDir(KDatabasename);
    QString dbMetaPath = FileCtl::getAppWorkingDataDir(KDatabaseMetaName);

    logd("dbPath %s", dbPath.toStdString().c_str());
    dbInfo->setUri(dbPath);
    dbInfo->setMetaUri(dbMetaPath);

//    mDbSaint = new DbSqliteSaint();
//    mDbCommunity = new DbSqliteCommunity();
//    mDbEdu = new DbSqliteEdu();
//    mDbSpecialist = new DbSqliteSpecialist();
}

IDatabase *DbCtl::database() const
{
    return mDatabase;
}

QString DbCtl::getName()
{
    return "DbCtl";
}


DbCtl* DbCtl::getInstance(){
    tracein;
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
    // TODO: unload db controller??
}
