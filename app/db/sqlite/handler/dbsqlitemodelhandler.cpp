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

ErrCode DbSqliteModelHandler::add(DbModel *model)
{

    traced;
    ErrCode_t err = ErrNone;

    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getTable(model->modelName());
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

QList<DbModel *> DbSqliteModelHandler::getAll(DbModelBuilder builder, const char* modelName)
{
    traced;
    return getMainTbl()->getAll(builder);
}

QHash<QString, DbModel *> DbSqliteModelHandler::getAllInDict(DbModelBuilder builder, const char *modelName)
{
    traced;
    QList<DbModel *> list = getAll(builder, modelName);
    QHash<QString, DbModel *> map;
    logd("found %lld item", list.count());
    foreach (DbModel* item, list) {
        map.insert(item->uid(), item);
    }
    // TODO: cache it??????
    tracede;
    return map;
}

DbModel *DbSqliteModelHandler::getModel(qint64 dbId)
{
    traced;
    return nullptr;
}

DbModel *DbSqliteModelHandler::getByName(const QString &name, const DbModelBuilder &builder)
{
    traced;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->getByName(name, builder);

}

DbModel *DbSqliteModelHandler::getByName(const QString &name)
{
    traced;
    loge("SHOULD NOT BE CALLED, MUST BE IMPLEMENTED BY DERIVED CLASS");
    // TODO: throw exception???
    return nullptr;
}

const QString DbSqliteModelHandler::getName()
{
    return "sqlite";
}

int DbSqliteModelHandler::search(const QString& keyword, QList<DbModel*>* outList)
{
    traced;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->search(keyword, outList);

}

DbSqliteTbl *DbSqliteModelHandler::getTable(const QString& modelName)
{
    return getMainTbl();
}
