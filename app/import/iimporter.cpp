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
 * Filename: iimporter.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#include "iimporter.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"

ErrCode IImporter::onImportItem(int importFileType, const QString &keyword, const QString &value, quint32 idx, void* tag)
{
    traced;
    loge("DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    ASSERT(false, "DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return ErrNotSupport;
}

ErrCode IImporter::onImportItem(int importFileType, const QStringList &items, quint32 idx, void* tag) {
    traced;
    loge("DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    ASSERT(false, "DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return ErrNotSupport;
}

ErrCode IImporter::onImportItem(int importFileType, const QHash<QString, QString> &items, quint32 idx, void *tag)
{
    traced;
    loge("DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    ASSERT(false, "DEFAULT onImportItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return ErrNotSupport;
}
