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
#include "utils.h"
#include <QList>
#include "importlistener.h"
#include "importer.h"
#include "importtype.h"

class DbModel;

class ImportFactory
{
    GET_INSTANCE_DECL(ImportFactory)
public:
    ImportFactory();
    static Importer* getImporter(ImportType type);
    static ImportType importTypeFromExt(const QString& extension, bool list = false);

    static ErrCode importFrom(const QString& importName, IDataImporter* item,
                              const QString& fpath, ImportType type,
                              QList<DbModel *>* outList = nullptr);

    static void addListener(ImportListener* listener);
    static void removeListener(ImportListener* listener);
protected:
    Importer* doGetImporter(ImportType type);
    ErrCode doImportFrom(const QString& importName, IDataImporter* item,
                              const QString& fpath, ImportType type,
                              QList<DbModel *>* outList = nullptr);

    void doAddListener(ImportListener* listener);
    void doRemoveListener(ImportListener* listener);
    void notifyListenerStart(const QString& importName, const QString& fpath, ImportType type);
    void notifyListenerEnd(const QString& importName, ErrCode err, const QString& fpath, ImportType type);
protected:
    QList<ImportListener*> mImportListener;

};

#endif // IMPORTFACTORY_H
