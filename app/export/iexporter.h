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
 * Filename: iexporter.h
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#ifndef IEXPORTER_H
#define IEXPORTER_H
#include "errcode.h"
#include "exportfactory.h"

class Exporter;
class DbModel;
// TODO: name should be IDataExporter???
class IExporter
{
public:
    IExporter();

    virtual const QString exportTemplatePath(Exporter* exporter) const;

    virtual const QStringList getListExportKeyWord() const;
    virtual ErrCode getExportDataString(const QString& keyword, QString* data) const;
    virtual ErrCode getExportDataString(const QString& keyword, const DbModel* data, QString* exportData) const;

    virtual quint64 getExportTypeList();
    virtual ErrCode getListExportKeywords(Exporter* exporter,
                                        QHash<QString, QString>& outMap) const;
};

#endif // IEXPORTER_H
