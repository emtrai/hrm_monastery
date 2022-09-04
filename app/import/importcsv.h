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
 * Filename: importcsv.h
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#ifndef IMPORTCSV_H
#define IMPORTCSV_H

#include "importer.h"
#include "errcode.h"
#include <QString>
#include "iimporter.h"
#include "utils.h"

class ImportCSV : public Importer
{
    GET_INSTALCE_DECL(ImportCSV)
public:
    ImportCSV();


    virtual ErrCode importFrom(int importFileType, IImporter* importer, const QString& fpath, void* tag = nullptr);
};

#endif // IMPORTCSV_H
