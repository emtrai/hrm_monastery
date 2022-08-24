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

ExportFactory::ExportFactory()
{
    traced;
}

Exporter *ExportFactory::getExporter(ExportType type)
{
    Exporter* ret = nullptr;
    traced;
    logd("get export type %d", type);
    switch (type) {
    case EXPORT_HTML:
        ret = INSTANCE(ExportHtml);
        break;
    default:
        loge("Export type %d not support", type);
        ret = nullptr;
        break;
    }
    return ret;
}

ErrCode ExportFactory::exportTo(const IExporter *item, const QString &fpath, ExportType type)
{
    traced;
    ErrCode ret = ErrNone;
    logi("Export to %d", type);
    Exporter* exporter = getExporter(type);
    if (exporter != nullptr)
        ret = exporter->saveTo(item, fpath);
    else {
        ret = ErrNotSupport;
        loge("Exporter %d not support", type);
    }
    return ret;
}
