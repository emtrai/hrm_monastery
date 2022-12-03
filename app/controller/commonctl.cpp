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
 * Filename: commonctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#include "commonctl.h"

#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "dbmodel.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "filectl.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



QList<DbModel *> CommonCtl::getItemFromDb()
{
    traced;
    return QList<DbModel *>();
}


void CommonCtl::clearItemList(QList<DbModel *> *list)
{
    traced;
    if (list != nullptr) {
        list->clear(); // TODO: clear each item???
    }
    tracede;
}


ErrCode CommonCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}


ErrCode CommonCtl::loadFromDb()
{
    traced;
    ErrCode err = ErrNone;
    clearItemList(&mItemList);
    mItemList = getItemFromDb();
    foreach (DbModel* model, mItemList){
        model->dump();
    }
    logd("load %d item from db", mItemList.count());
    tracedr(err);
    return err;
}


const QList<DbModel *> CommonCtl::getAllItems(bool reload)
{
    traced;
    logd("reload %d", reload);
    if (reload) {
        loadFromDb();
    }
    return mItemList;
}

CommonCtl::CommonCtl()
{
    traced;
}
