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
 * Filename: idataexporter.h
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H
#include "errcode.h"
#include "exportfactory.h"
#include <QHash>
#include <QPair>
class FileExporter;
class DbModel;
// TODO: name should be DataExporter???
class DataExporter
{
public:
    DataExporter();

    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;

    /* list of export field, in map: id:name*/
    virtual ErrCode getListTemplateExportKeywords(FileExporter *exporter, QList<QPair<QString,QString>>& outMap) const;

    virtual const QStringList getListExportKeyWord() const;
    virtual ErrCode getExportDataString(const QString& item, QString* data) const;

    /**
     * @brief Get data to be exported
     * @param[in] item keyword/data/item to be exported
     * @param[in] data Model
     * @param[in] exportData data to be exported
     * @return
     */
    virtual ErrCode getExportDataString(const QString& item, const DbModel* data, QString* exportData) const;

    virtual quint64 getExportTypeList();
protected:
    virtual ErrCode parseJsonExportTemplate(const QString& fpath, QList<QPair<QString,QString>>& outMap) const;
    virtual ErrCode parseCsvExportTemplate(const QString& fpath, QList<QPair<QString,QString>>& outMap) const;
};

#endif // DATAEXPORTER_H
