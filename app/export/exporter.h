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
 * Filename: exporter.h
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#ifndef EXPORTER_H
#define EXPORTER_H

#include "errcode.h"
#include <QString>
#include "exporttype.h"
#include <QList>

class IDataExporter;
class DbModel;

class Exporter
{
public:
    Exporter();

    // Exporter may contains data for export, or get data from list of data to export
    // TODO: is there any better way????
    virtual ErrCode saveTo(const IDataExporter* exporter, const QString& fpath);
    virtual ErrCode saveTo(const IDataExporter* exporter, const QList<DbModel*> data, const QString& fpath);
    virtual ExportType getExportType() = 0;


};

#endif // EXPORTER_H
