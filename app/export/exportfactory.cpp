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
 * Filename: exportfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "exportfactory.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "exporthtml.h"
#include "exportcsvlist.h"
#include "exportxlsx.h"

ExportFactory::ExportFactory()
{
    tracein;
}

FileExporter *ExportFactory::getExporter(ExportType type)
{
    FileExporter* ret = nullptr;
    tracein;
    logd("get export type %d", type);
    switch (type) {
    case EXPORT_HTML:
        ret = INSTANCE(ExportHtml);
        break;
    case EXPORT_CSV_LIST:
        ret = INSTANCE(ExportCSVList);
        break;
    case EXPORT_XLSX:
        ret = INSTANCE(ExportXlsx);
        break;
    default:
        loge("Export type %d not support", type);
        ret = nullptr;
        break;
    }
    return ret;
}

ErrCode ExportFactory::exportTo(const DataExporter *item,
                                const QString& datatype,
                                const QString &fpath,
                                ExportType type)
{
    tracein;
    ErrCode ret = ErrNone;
    logi("Export to %d", type);
    FileExporter* exporter = getExporter(type);
    if (exporter != nullptr)
        ret = exporter->saveTo(item, datatype, fpath);
    else {
        ret = ErrNotSupport;
        loge("FileExporter %d not support", type);
    }
    return ret;
}

ErrCode ExportFactory::exportTo(const DataExporter* item,
                                const QString& datatype,
                                QList<DbModel*> data,
                                const QString &fpath,
                                ExportType type)
{
    tracein;
    // TODO: add "tag" parameter here to input to getExporter/saveTo???
    // so that derived class can have more information to judge which data should be used.
    // TODO: add title? (title for export info)
    ErrCode ret = ErrNone;
    logi("Export to %d, datatype '%s'", type, STR2CHA(datatype));
    FileExporter* exporter = getExporter(type);
    if (exporter != nullptr)
        ret = exporter->saveTo(item, datatype, data, fpath);
    else {
        ret = ErrNotSupport;
        loge("FileExporter %d not support", type);
    }
    return ret;
}

