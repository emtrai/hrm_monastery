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

#include "exporter.h"

enum ExportType {
    EXPORT_NONE = 0,
    EXPORT_HTML,
    EXPORT_CSV,
    EXPORT_DOCX,
    EXPORT_XLSX,
    EXPORT_MAX
};


class ExportFactory
{
public:
    ExportFactory();

    static Exporter* getExporter(ExportType type);

    static ErrCode exportTo(const IExporter* item, const QString& fpath, ExportType type);

};

#endif // EXPORTFACTORY_H
