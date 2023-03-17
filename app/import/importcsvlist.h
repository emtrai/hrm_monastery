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
 * Filename: importcsvlist.h
 * Author: Anh, Ngo Huy
 * Created date:9/3/2022
 * Brief:
 */
#ifndef IMPORTCSVLIST_H
#define IMPORTCSVLIST_H

#include "importer.h"

#include "errcode.h"
#include <QString>
#include "idataimporter.h"
#include "utils.h"
class DbModel;

class ImportCSVList : public Importer
{
    GET_INSTANCE_DECL(ImportCSVList)
public:
    ImportCSVList();
    virtual ErrCode importFrom(const QString& importName, int importFileType,
                               IDataImporter* importer, const QString& fpath, QList<DbModel *>* outList = nullptr);
};

#endif // IMPORTCSVLIST_H
