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
 * Filename: iimporter.h
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#ifndef IIMPORTER_H
#define IIMPORTER_H

#include <QString>
#include "errcode.h"


class IImporter
{
public:
    /**
     * @brief Import one field/item
     * @param importFileType file type, like CSV, CSV_LIST. \ref ImportType
     * @param keyword Field/item name, i.e.MA_DINH_DANH
     * @param value Value
     * @param idx
     * @param tag
     * @return ErrNone on success, error code otherwise
     */
    virtual ErrCode onImportItem(int importFileType, const QString& keyword, const QString& value, quint32 idx = 0, void* tag = nullptr);
    virtual ErrCode onImportItem(int importFileType, const QStringList& items, quint32 idx = 0, void* tag = nullptr);
    virtual ErrCode onImportItem(int importFileType, const QHash<QString, QString>& items, quint32 idx = 0, void* tag = nullptr);
};

#endif // IIMPORTER_H
