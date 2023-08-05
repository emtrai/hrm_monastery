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
#include "dbdefs.h"
#include "dbsqlite.h"
#include "dbmodelfactory.h"

DbSqliteModelHandler::DbSqliteModelHandler()
{
    tracein;
}

DbSqliteModelHandler::DbSqliteModelHandler(const QString &name)
    :DbSqliteModelHandler()
{
    tracein;
    mName = name;
}

ErrCode DbSqliteModelHandler::add(DbModel *model, bool notify)
{

    tracein;
    ErrCode_t err = ErrNone;

    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getTable(model->modelName());
        if (!tbl->isExist(model)){
            err = tbl->add(model);
        } else{
            err = ErrExisted;
            loge("Model %s already exist", model->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_ADD, err);
    }
    return err;
}

ErrCode DbSqliteModelHandler::update(DbModel *model)
{
    tracein;
    ErrCode_t err = ErrNone;

    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getTable(model->modelName());
        if (tbl->isExist(model)){
            err = tbl->update(model);
        } else {
            err = ErrNotExist;
            loge("model %s not exist", MODELSTR2CHA(model));
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone) {
        notifyDataChange(model, DBMODEL_CHANGE_UPDATE, err);
    }
    return err;
}

ErrCode DbSqliteModelHandler::update(DbModel *model,
                                     const QHash<QString, QString> &inFields,
                                     const QString &tableName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    ErrCode err = ErrNone;
    tbl = DbSqlite::table(tableName);
    if (!tbl) {
        err = ErrNoTable;
        loge("not found table %s", STR2CHA(tableName));
    }
    if (err == ErrNone) {
        err = tbl->update(model->uid(), inFields);
        logd("Update err= %d", err);
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteModelHandler::deleteSoft(DbModel *model)
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getTable(model->modelName());
        if (tbl->isExist(model)){
            err = tbl->deleteSoft(model);
        } else {
            err = ErrNotExist;
            loge("model %s not exist", model->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        notifyDataChange(model, DBMODEL_CHANGE_DELETE, err);
    }
    traceret(err);
    return err;
}
ErrCode DbSqliteModelHandler::deleteHard(DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqliteTbl* tbl = nullptr;
    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it
    if (!model) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone){
        tbl = getTable(model->modelName());
        if (!tbl) {
            err = ErrNoTable;
            loge("Not found table for '%s'", STR2CHA(model->modelName()));
        }
    }
    if (err == ErrNone) {
        err = doDeleteHard(tbl, model, force, msg);
    }
    traceret(err);
    return err;
}

ErrCode DbSqliteModelHandler::deleteHard(DbModel *model, bool force, QString *msg, const QString &tableName)
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it
    DbSqliteTbl* tbl = DbSqlite::table(tableName);
    err = doDeleteHard(tbl, model, force, msg);
    traceret(err);
    return err;
}

ErrCode DbSqliteModelHandler::doDeleteHard(DbSqliteTbl *tbl, DbModel *model, bool force, QString *msg)
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it
    if (!tbl || !model) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone) {
        if (tbl->isExist(model)){
            err = tbl->deleteHard(model);
        } else {
            err = ErrNotExist;
            loge("model %s not exist", model->name().toStdString().c_str());
        }
    }
    if (err == ErrNone) {
        notifyDataChange(model, DBMODEL_CHANGE_DELETE, err);
    }
    traceret(err);
    return err;
}

bool DbSqliteModelHandler::exist(const DbModel *model)
{
    tracein;
    bool exist = false;
    if (model) {
        logd("check exist for model '%s'", STR2CHA(model->toString()));
        DbSqliteTbl* tbl = getTable(model->modelName());
        if (tbl) exist = tbl->isExist(model);
        else loge("not found table for model '%s'", STR2CHA(model->modelName()));
    } else {
        exist = false;
        loge("invalid argument");
    }
    traceout;
    return exist;
}

bool DbSqliteModelHandler::isNameidExist(const QString &nameId, const char *modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    bool exist = false;
    logd("check nameid exist for model '%s'", modelName?modelName:"(null)");
    if (modelName) {
        tbl = getTable(modelName);
    } else {
        logd("get main table");
        tbl = getMainTbl();
    }
    if (tbl){
        exist = tbl->isNameidExist(nameId);
    } else {
        loge("not found suitable table to check name exist");
    }

    logd("exist=%d", exist);
    traceout;
    return exist;
}

QList<DbModel *> DbSqliteModelHandler::getAll(DbModelBuilder builder, qint64 status,
                                              const char* modelName, int from, int noItems, int* total)
{
    tracein;
    return getMainTbl()->getAll(builder, status, from, noItems, total);
}

QHash<QString, DbModel *> DbSqliteModelHandler::getAllInDict(DbModelBuilder builder, qint64 status, const char *modelName)
{
    tracein;
    QList<DbModel *> list = getAll(builder, status, modelName);
    QHash<QString, DbModel *> map;
    logd("found %lld item", list.count());
    foreach (DbModel* item, list) {
        map.insert(item->uid(), item);
    }
    // TODO: cache it??????
    traceout;
    return map;
}

DbModel *DbSqliteModelHandler::getModel(qint64 dbId)
{
    tracein;
    return nullptr;
}

DbModel *DbSqliteModelHandler::getByUid(const QString &uid, const DbModelBuilder &builder)
{
    tracein;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->getByUid(uid, builder);
}

DbModel *DbSqliteModelHandler::getByUid(const QString &uid)
{
    tracein;
    DbModel *model = nullptr;
    DbModelBuilder builder = getMainBuilder();
    if (builder) {
        model = getByUid(uid, builder);
    } else {
        loge("NO BUILDER, SHOULD NOT BE CALLED, MUST BE IMPLEMENTED BY DERIVED CLASS");
    }
    // TODO: throw exception???
    traceout;
    return model;
}

DbModel *DbSqliteModelHandler::getByNameId(const QString &nameId, const DbModelBuilder &builder)
{
    tracein;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->getByNameId(nameId, builder);
}


const QString DbSqliteModelHandler::getName()
{
    return mName;
}

ErrCode DbSqliteModelHandler::search(const QString& keyword,
                                 QList<DbModel*>* outList,
                                 qint64 dbStatus,
                                 int from,
                                 int noItems,
                                 int* total)
{
    tracein;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->search(keyword, outList, dbStatus, from, noItems, total);

}

ErrCode DbSqliteModelHandler::filter(int fieldId,
                                 int operatorId,
                                 const QString &keyword,
                                 const char* targetModelName,
                                 const DbModel* parentModel,
                                 QList<DbModel *> *outList,
                                 qint64 dbStatus,
                                 int from,
                                 int noItems,
                                 int* total)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    DbModelBuilder builder = nullptr;
    if (targetModelName) {
        builder = getBuilder(targetModelName);
        tbl = getTable(targetModelName);
    } else {
        builder = getMainBuilder();
        tbl = getMainTbl();
    }
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    Q_ASSERT(builder != nullptr);

    ErrCode ret = tbl->filter(fieldId, operatorId, keyword, builder,
                              parentModel,
                              outList,
                              dbStatus, from, noItems, total);
    traceret(ret);
    return ret;
}

ErrCode DbSqliteModelHandler::getListItems(const QHash<QString, QString> &inFields,
                                          const QString &tableName,
                                          bool isMatchAllField,
                                          int* total,
                                          QList<DbModel*>* outList,
                                          const DbModelBuilder& builder)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    ErrCode err = ErrNone;
    int cnt = 0;
    logd("get list item for table '%s'", STR2CHA(tableName));
    tbl = DbSqlite::table(tableName);
    if (!tbl) {
        err = ErrNoTable;
        loge("not found table %s", STR2CHA(tableName));
    }
    if (err == ErrNone) {
        err = tbl->getListItems(inFields, builder, outList,
                                isMatchAllField, DB_RECORD_ACTIVE, 0, 0, &cnt);
        logd("found %d items", cnt);
    }
    if (total) *total = cnt;
    traceret(err);
    return err;
}


DbSqliteTbl *DbSqliteModelHandler::getTable(const QString& modelName)
{
    return getMainTbl();
}
DbModelBuilder DbSqliteModelHandler::getBuilder(const QString &modelName)
{
    tracein;
    logd("get builder for model '%s'", STR2CHA(modelName));
    DbModelBuilder builder = nullptr;
    if (!modelName.isEmpty()) {
        builder = DbModelFactory::getBuilder(modelName);
    } else {
        logd("null modelName, try to get defaule onle");
        builder = getMainBuilder();
    }
    traceout;
    return builder;
}

