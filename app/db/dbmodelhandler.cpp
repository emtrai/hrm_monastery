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
DbModelHandler::DbModelHandler()
{

}

DbModel *DbModelHandler::getItem(const QString &uid, DbModelBuilder builder, const char* modelName)
{
    traced;
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
    tracede;
    return model;
}

int DbModelHandler::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    loge("Default search one, do nothing");
    return 0;
}

int DbModelHandler::searchAll(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    loge("Default search one, do nothing");
    return 0;
}

int DbModelHandler::filter(int fieldId, int operatorId, const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    loge("Default filter one, do nothing");
    return 0;

}

DbModelBuilder DbModelHandler::getMainBuilder()
{
    FAIL("DEFAULT getMainBuilder, should not be called");
    return nullptr;
}

DbModel *DbModelHandler::getByName(const QString &name, const DbModelBuilder &builder)
{
    traced;
    loge("Default one, do nothing");
    return nullptr;// TODO: throw exception????
}

DbModel *DbModelHandler::getByName(const QString &name)
{
    traced;
    loge("Default one, do nothing");
    return nullptr;// TODO: throw exception????
}

DbModel *DbModelHandler::getByUid(const QString &uid, const DbModelBuilder &builder)
{

    traced;
    loge("Default one, do nothing");
    return nullptr;// TODO: throw exception????
}

DbModel *DbModelHandler::getByUid(const QString &uid)
{
    traced;
    loge("Default one, do nothing");
    return nullptr; // TODO: throw exception????
}

DbModel *DbModelHandler::getByNameId(const QString &nameId, const DbModelBuilder &builder)
{
    traced;
    loge("Default one, do nothing");
    return nullptr; // TODO: throw exception????
}

DbModel *DbModelHandler::getByNameId(const QString &nameId)
{
    traced;
    DbModel *model = nullptr;
    DbModelBuilder builder = getMainBuilder();
    if (builder) {
        model = getByNameId(nameId, builder);
    } else {
        loge("NO BUILDER, SHOULD NOT BE CALLED, MUST BE IMPLEMENTED BY DERIVED CLASS");
    }
    // TODO: throw exception???
    tracede;
    return model;
}

