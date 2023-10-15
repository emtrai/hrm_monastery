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
 * Filename: exporthtml.h
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include "fileexporter.h"
#include "errcode.h"
#include <QString>
#include "dataexporter.h"
#include "utils.h"
#include "exporttype.h"
class ExportHtml : public FileExporter
{
    GET_INSTANCE_DECL(ExportHtml)
public:
    ExportHtml();
    virtual ErrCode saveTo(const DataExporter* item, const QString& datatype, const QString& fpath);
    virtual ExportType getExportType() const ;


};

#endif // EXPORTHTML_H
