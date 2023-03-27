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
 * Filename: idataimporter.h
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#ifndef IDATAIMPORTER_H
#define IDATAIMPORTER_H

#include <QString>
#include "errcode.h"

class DbModel;

class IDataImporter
{
public:
    virtual ErrCode onImportDataStart(const QString& importName, int importFileType, const QString& fname);
    /**
     * @brief Import one field/item
     * @param importFileType file type, like CSV, CSV_LIST. \ref ImportType
     * @param keyword Field/item name, i.e.MA_DINH_DANH
     * @param value Value
     * @param idx
     * @param tag
     * @return ErrNone on success, error code otherwise
     */
    virtual ErrCode onImportParseDataItem(const QString& importName, int importFileType,
                                 const QString& keyword, const QString& value,
                                 quint32 idx = 0, QList<DbModel *>* outList = nullptr);
    virtual ErrCode onImportParseDataItem(const QString& importName, int importFileType,
                                 const QStringList& items, quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);
    virtual ErrCode onImportParseDataItem(const QString& importName, int importFileType,
                                 const QHash<QString, QString>& items, quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);
    virtual void onImportDataEnd(const QString& importName, int importFileType,
                                const QString& fname, ErrCode result);
};

#endif // IDATAIMPORTER_H
