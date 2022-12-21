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

DbCtl* DbCtl::gInstance = nullptr;

DbCtl::DbCtl():Controller("DbCtl")
{
    traced;
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

    QString dbPath = FileCtl::getAppDataDir("test.db");

    logd("dbPath %s", dbPath.toStdString().c_str());
    dbInfo->setUri(dbPath);

//    mDbSaint = new DbSqliteSaint();
//    mDbCommunity = new DbSqliteCommunity();
//    mDbEdu = new DbSqliteEdu();
//    mDbSpecialist = new DbSqliteSpecialist();
}

IDatabase *DbCtl::database() const
{
    return mDatabase;
}

ErrCode DbCtl::openDb()
{
    traced;
    ErrCode ret = getDb()->openDb();
    tracedr(ret);
    return ret;
}

void DbCtl::closeDb()
{
    traced;
    getDb()->closeDb();
    tracede;
}

DbCtl* DbCtl::getInstance(){
    traced;
    if (gInstance == nullptr){
        gInstance = new DbCtl();
    }

    return gInstance;
}

IDatabase* DbCtl::getDb(){
    return getInstance()->database();
}

void DbCtl::onLoad(){
    traced;
    mDatabase->loadDb(dbInfo);
    tracede;
}
