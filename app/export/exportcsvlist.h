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
 * Filename: exportcsvlist.h
 * Author: Anh, Ngo Huy
 * Created date:1/27/2023
 * Brief:
 */
#ifndef EXPORTCSVLIST_H
#define EXPORTCSVLIST_H

#include "fileexporter.h"
#include "errcode.h"
#include <QString>
#include "dataexporter.h"
#include "utils.h"

class ExportCSVList : public FileExporter
{
    GET_INSTANCE_DECL(ExportCSVList)
public:
    ExportCSVList();
//    virtual ErrCode saveTo(const DataExporter* item, const QString& fpath);
    virtual ErrCode saveTo(const DataExporter* exporter, const QString& datatype, const QList<DbModel*> data, const QString& fpath);
    virtual ExportType getExportType();
};

#endif // EXPORTCSVLIST_H
