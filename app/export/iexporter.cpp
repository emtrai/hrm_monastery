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
 * Filename: iexporter.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "iexporter.h"
#include "logger.h"
#include "defs.h"
#include "exportfactory.h"
#include "utils.h"

IExporter::IExporter()
{
    traced;
}

const QString IExporter::exportTemplatePath(Exporter* exporter) const
{
    traced;
    return QString();
}

const QStringList IExporter::getListExportKeyWord() const
{
    traced;
    return QStringList();
}

ErrCode IExporter::getExportDataString(const QString &keyword, QString* data) const
{
    traced;
    loge("Not support here");
    return ErrNotSupport;
}

ErrCode IExporter::getExportDataString(const QString &keyword, const DbModel *data, QString *exportData) const
{
    traced;
    loge("Not support here");
    ASSERT(false, "CAN NOT GET EXPORT DATA HERE, MUST BE IMPLEMENTED BY DERIVED CLASS");
    return ErrNotSupport;
}

quint64 IExporter::getExportTypeList()
{
    loge("get eport type list from base class, SHOULD be implemented in derived class");
    return ExportType::EXPORT_NONE;
}

ErrCode IExporter::getListExportKeywords(Exporter *exporter, QHash<QString, QString> &outMap) const
{
    traced;
    loge("MUST BE IMPLEMENTED IN DERIVED CLASS OF IExporter");
    return ErrNotImpl;
}
