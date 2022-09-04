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

ImportFactory::ImportFactory()
{
    traced;
}

Importer *ImportFactory::getImporter(ImportType type)
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

ErrCode ImportFactory::importFrom(IImporter *item, const QString &fpath, ImportType type, void* tag)
{
    traced;
    ErrCode ret = ErrNone;
    logi("Import from %d", type);
    Importer* importer = getImporter(type);
    if (importer != nullptr)
        ret = importer->importFrom((int) type, item, fpath, tag);
    else {
        ret = ErrNotSupport;
        loge("Importer %d not support", type);
    }
    return ret;
}
