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
 * Filename: exportfactory.h
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#ifndef EXPORTFACTORY_H
#define EXPORTFACTORY_H

#include "fileexporter.h"
#include "exporttype.h"

enum ExportType;
struct ExportTemplate;
class ExportFactory
{
public:
    ExportFactory();

    static FileExporter* getExporter(ExportType type);

    static ErrCode exportTo(const DataExporter* item,
                            const QString& datatype,
                            const QString& fpath,
                            ExportType type);
    static ErrCode exportTo(const DataExporter* item,
                            const QString& datatype,
                            QList<DbModel*> data,
                            const QString& fpath,
                            ExportType type);

    static QString getExportTemplatePath(const QString& modelName, ExportType type, bool* ok = nullptr);
private:
    static void initExportTemplate();
    static void addExportTemplate(ExportType type, const QString& modelName, QString fpath);
private:
    static QHash<QString, ExportTemplate*> s_modelNameExportTemplateMap;
};

#endif // EXPORTFACTORY_H
