/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: importlistener.h
 * Author: Anh, Ngo Huy
 * Created date:3/22/2023
 * Brief:
 */
#ifndef IMPORTLISTENER_H
#define IMPORTLISTENER_H

#include "importer.h"
#include <QString>

class ImportListener
{
public:
    virtual QString getName() = 0;
    virtual void onImportStart(const QString& importName, const QString& fpath, ImportType type) = 0;
    virtual void onImportEnd(const QString& importName, ErrCode err, const QString& fpath, ImportType type) = 0;
};

#endif // IMPORTLISTENER_H
