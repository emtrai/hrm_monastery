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

#include "exporter.h"
#include "errcode.h"
#include <QString>
#include "iexporter.h"
#include "utils.h"

class ExportHtml : public Exporter
{
public:
    ExportHtml();
    virtual ErrCode saveTo(const IExporter* item, const QString& fpath);


    GET_INSTALCE_DECL(ExportHtml)
};

#endif // EXPORTHTML_H