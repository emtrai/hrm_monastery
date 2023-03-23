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
 * Filename: importfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#include "importfactory.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "importcsv.h"
#include "importcsvlist.h"

GET_INSTANCE_IMPL(ImportFactory)

ImportFactory::ImportFactory()
{
    traced;
}

Importer *ImportFactory::getImporter(ImportType type)
{
    return INSTANCE(ImportFactory)->doGetImporter(type);
}

Importer *ImportFactory::doGetImporter(ImportType type)
{
    Importer* ret = nullptr;
    traced;
    logd("get import type %d", type);
    switch (type) {
    case IMPORT_CSV:
        ret = INSTANCE(ImportCSV);
        break;
    case IMPORT_CSV_LIST:
        ret = INSTANCE(ImportCSVList);
        break;
    default:
        loge("Import type %d not support", type);
        ret = nullptr;
        break;
    }
    return ret;
}

ErrCode ImportFactory::importFrom(const QString& importName, IDataImporter *item,
                                  const QString &fpath, ImportType type,
                                  QList<DbModel *>* outList)
{
    return INSTANCE(ImportFactory)->doImportFrom(importName, item, fpath, type, outList);
}

void ImportFactory::addListener(ImportListener *listener)
{
    return INSTANCE(ImportFactory)->doAddListener(listener);
}

void ImportFactory::doAddListener(ImportListener *listener)
{
    traced;
    if (!mImportListener.contains(listener)) {
        logd("add listener '%s'", STR2CHA(listener->getName()));
        mImportListener.append(listener);
    }
    tracede;
}


void ImportFactory::removeListener(ImportListener *listener)
{
    return INSTANCE(ImportFactory)->doRemoveListener(listener);

}

void ImportFactory::doRemoveListener(ImportListener *listener)
{
    traced;
    if (mImportListener.contains(listener)) {
        logd("remove listener '%s'", STR2CHA(listener->getName()));
        mImportListener.removeOne(listener);
    }
    tracede;
}

void ImportFactory::notifyListenerStart(const QString &importName, const QString &fpath, ImportType type)
{
    traced;
    foreach(ImportListener* listener, mImportListener) {
        if (listener != nullptr) {
            logd("Call import listener onImportStart '%s'", STR2CHA(listener->getName()));
            listener->onImportStart(importName, fpath, type);
        }
    }
    traced;
}

void ImportFactory::notifyListenerEnd(const QString &importName, ErrCode err, const QString &fpath, ImportType type)
{
    traced;
    foreach(ImportListener* listener, mImportListener) {
        if (listener != nullptr) {
            logd("Call import listener onImportEnd '%s'", STR2CHA(listener->getName()));
            listener->onImportEnd(importName, err, fpath, type);
        }
    }
    traced;
}

ErrCode ImportFactory::doImportFrom(const QString& importName, IDataImporter *item,
                                  const QString &fpath, ImportType type,
                                  QList<DbModel *>* outList)
{
    traced;
    ErrCode ret = ErrNone;
    logi("Import from %d", type);
    Importer* importer = getImporter(type);
    if (importer != nullptr) {
        notifyListenerStart(importName, fpath, type);
        ret = importer->importFrom(importName, (int) type, item, fpath, outList);
        notifyListenerEnd(importName, ret, fpath, type);
    } else {
        ret = ErrNotSupport;
        loge("Importer %d not support", type);
    }
    return ret;
}

