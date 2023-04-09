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
 * Filename: dbmodelhandler.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "dbmodelhandler.h"
#include "logger.h"
#include "errcode.h"

void DbModelHandler::cleanUpModelList(QList<DbModel *> &list)
{
    tracein;
    foreach (DbModel* model, list) {
        if (model) delete model;
    }
    list.clear();
    traceout;
}

DbModelHandler::DbModelHandler()
{
    tracein;
}

QList<DbModel *> DbModelHandler::getAll(const char *modelName, qint64 status, int from, int noItems, int *total)
{
    return getAll(getMainBuilder(), status, modelName, from, noItems, total);
}

DbModel *DbModelHandler::getItem(const QString &uid, DbModelBuilder builder, const char* modelName)
{
    tracein;
    logd("uid %s", uid.toStdString().c_str());
    QHash<QString, DbModel*> list = getAllInDict(builder, DB_RECORD_ALL, modelName); // TODO: all status or action one only???
    DbModel* model = nullptr;
    if (list.count() > 0) {
        if (list.contains(uid)){
            model = list.value(uid);
        } else {
            loge("Not found uid '%s'", uid.toStdString().c_str());
        }
    } else {
        logi("Not found any item");
    }
    traceout;
    return model;
}

DbModelBuilder DbModelHandler::getMainBuilder()
{
    FAIL("DEFAULT getMainBuilder, should not be called");
    return nullptr;
}

DbModelBuilder DbModelHandler::getBuilder(const QString &modelName)
{
    if (modelName.isEmpty()){
        return getMainBuilder();
    }
    return nullptr;
}

DbModel *DbModelHandler::getByUid(const QString &uid, const DbModelBuilder &builder)
{

    tracein;
    loge("Default one, do nothing");
    return nullptr;// TODO: throw exception????
}

DbModel *DbModelHandler::getByUid(const QString &uid)
{
    tracein;
    loge("Default one, do nothing");
    return nullptr; // TODO: throw exception????
}

DbModel *DbModelHandler::getByNameId(const QString &nameId, const DbModelBuilder &builder)
{
    tracein;
    loge("Default one, do nothing");
    return nullptr; // TODO: throw exception????
}

DbModel *DbModelHandler::getByNameId(const QString &nameId)
{
    tracein;
    DbModel *model = nullptr;
    DbModelBuilder builder = getMainBuilder();
    if (builder) {
        model = getByNameId(nameId, builder);
    } else {
        loge("NO BUILDER, SHOULD NOT BE CALLED, MUST BE IMPLEMENTED BY DERIVED CLASS");
    }
    // TODO: throw exception???
    traceout;
    return model;
}


void DbModelHandler::addListener(onDbModelHandlerListener *listener)
{
    tracein;
    if (listener) {
        if (!mListeners.contains(listener)) {
            logd("Add listener '%s'", STR2CHA(listener->getName()));
            mListeners.append(listener);
        } else {
            loge("Listener '%s' already existed", STR2CHA(listener->getName()));
        }
    } else {
        loge("Invalid listener");
    }
    traceout;
}

void DbModelHandler::delListener(onDbModelHandlerListener *listener)
{
    tracein;
    if (listener) {
        logd("Remove listener '%s'", STR2CHA(listener->getName()));
        mListeners.removeOne(listener);
    } else {
        loge("Invalid listener");
    }
    traceout;
}

void DbModelHandler::notifyDataChange(DbModel *model, int type, ErrCode err)
{
    tracein;
    foreach (onDbModelHandlerListener* listener, mListeners) {
        if (listener) {
            logi("Call listener '%s'", STR2CHA(listener->getName()));
            listener->onDbModelHandlerDataUpdate(model, type, err);
        }
    }
    traceout;
}
