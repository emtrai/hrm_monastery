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
 * Filename: importfactory.h
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#ifndef IMPORTFACTORY_H
#define IMPORTFACTORY_H

#include "importer.h"


enum ImportType {
    IMPORT_NONE = 0,
    IMPORT_CSV,
    IMPORT_CSV_LIST,
    IMPORT_DOCX,
    IMPORT_XLSX,
    IMPORT_MAX
};


class ImportFactory
{
public:
    ImportFactory();
    static Importer* getImporter(ImportType type);

    static ErrCode importFrom(IImporter* item, const QString& fpath, ImportType type, void* tag = nullptr);

};

#endif // IMPORTFACTORY_H