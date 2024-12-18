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
 * Filename: exporter.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "fileexporter.h"
#include "logger.h"
#include "dataexporter.h"
#include "utils.h"
#include "dbmodel.h"

FileExporter::FileExporter()
{
    tracein;
}

ErrCode FileExporter::saveTo(const DataExporter* exporter,
                             const QString& datatype,
                             const QString &fpath)
{
    tracein;
    loge("Not support export here");
    ASSERT(false, "CAN NOT EXPORT HERE, MUST BE IMPLEMENTED BY DERIVED CLASS");
    return ErrNotSupport;
}

ErrCode FileExporter::saveTo(const DataExporter* exporter,
                             const QString& datatype,
                             const QList<DbModel*> data,
                             const QString &fpath)
{
    tracein;
    loge("Not support export list here");
    ASSERT(false, "CAN NOT EXPORT LIST HERE, MUST BE IMPLEMENTED BY DERIVED CLASS");
    return ErrNotSupport;
}
