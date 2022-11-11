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
 * Filename: dbsqlitespecialist.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#include "dbsqlitespecialist.h"
#include "logger.h"
#include "dbsqlitespecialist.h"
#include "dbsqlite.h"
#include "table/dbsqlitespecialisttbl.h"
#include "table/dbsqlitespecialistpersontbl.h"
#include "defs.h"
#include "dbsqlitedefs.h"

DbSqliteSpecialist* DbSqliteSpecialist::gInstance = nullptr;

DbSqliteSpecialist::DbSqliteSpecialist():
    DbSqliteModelHandler()
{
    traced;
}

DbSqliteSpecialist *DbSqliteSpecialist::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new DbSqliteSpecialist();
    }

    return gInstance;
}

const QString DbSqliteSpecialist::getName()
{
    return KModelHdlSpecialist;
}

QList<DbModel *> DbSqliteSpecialist::getListPerson(const QString &specialistUid)
{
    traced;
    DbSqliteSpecialistPersonTbl* tbl =
        (DbSqliteSpecialistPersonTbl*)DbSqlite::getInstance()->getTable(KTableSpecialistPerson);
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    QList<DbModel *> list = tbl->getListPerson(specialistUid);
    logd("found %lld", list.count());
    tracede;
    return list;
}

DbSqliteTbl *DbSqliteSpecialist::getMainTbl()
{
    return (DbSqliteSpecialistTbl*)DbSqlite::getInstance()->getTable(KTableSpecialist);
}

DbSqliteTbl *DbSqliteSpecialist::getTable(const QString &modelName)
{
    traced;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNameSpecialistPerson) {
        tbl = DbSqlite::table(KTableSpecialistPerson);
    } else {
        tbl = getMainTbl();
    }
    tracede;
    return tbl;
}


//ErrCode DbSqliteSpecialist::addSpecialist(const Specialist *specialist)
//{


//    traced;
//    ErrCode_t err = ErrNone;

//    if (specialist != nullptr){
//        if (!specialistTbl()->isExist(specialist)){
//            err = specialistTbl()->add(specialist);
//        }
//        else{
//            err = ErrExisted;
//            loge("Saint %s already exist", specialist->name().toStdString().c_str());
//        }
//    }
//    else{
//        err = ErrInvalidArg;
//        loge("invalid argument");
//    }



//    return err;
//}

//bool DbSqliteSpecialist::exist(const Specialist *specialist)
//{
//    traced;

//    return specialistTbl()->isExist(specialist);
//}

//QList<Specialist *> DbSqliteSpecialist::getAll()
//{
//    traced;
//    return specialistTbl()->getAll();
//}

//DbSqliteSpecialistTbl *DbSqliteSpecialist::specialistTbl()
//{
//    traced;
//    if (mSpecilistTbl == nullptr){
//        mSpecilistTbl = (DbSqliteSpecialistTbl*)DbSqlite::getInstance()->getTable(KTableSpecialist);
//    }
//    return mSpecilistTbl;
//}
