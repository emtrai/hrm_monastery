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
 * Filename: dbsqlitemodelhandler.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "dbsqlitemodelhandler.h"
#include "logger.h"
#include "dbmodel.h"

DbSqliteModelHandler::DbSqliteModelHandler()
{

}

ErrCode DbSqliteModelHandler::add(const DbModel *model)
{

    traced;
    ErrCode_t err = ErrNone;

    if (model != nullptr){
        DbSqliteTbl* tbl = getMainTbl();
        if (!tbl->isExist(model)){
            err = tbl->add(model);
        }
        else{
            err = ErrExisted;
            loge("Saint %s already exist", model->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }



    return err;
}

bool DbSqliteModelHandler::exist(const DbModel *edu)
{
    traced;

    return getMainTbl()->isExist(edu);;
}

QList<DbModel *> DbSqliteModelHandler::getAll(DbModelBuilder builder)
{
    traced;
    return getMainTbl()->getAll(builder);
}

DbModel *DbSqliteModelHandler::getModel(qint64 dbId)
{
    traced;
    return nullptr;
}

const QString DbSqliteModelHandler::getName()
{
    return "sqlite";
}
