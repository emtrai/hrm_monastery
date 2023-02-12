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
 * Filename: exporttype.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/31/2023
 * Brief:
 */

#include "exporttype.h"
#include "logger.h"
#include "utils.h"

static void getListExportTypeName(QHash<int, QString>* list)
{
    static bool initialized = false;
    static QHash<int, QString> s_ExportTypeName;
    traced;
    logd("initialized = %d", initialized);
    if (!initialized) {
        logd("not init initExportTypeName, init it");

        s_ExportTypeName.insert(EXPORT_HTML, QObject::tr("HTML"));
        s_ExportTypeName.insert(EXPORT_CSV, QObject::tr("CSV"));
        s_ExportTypeName.insert(EXPORT_CSV_LIST, QObject::tr("CSV_LIST"));
        s_ExportTypeName.insert(EXPORT_DOCX, QObject::tr("DOCX"));
        s_ExportTypeName.insert(EXPORT_XLSX, QObject::tr("XLSX"));

        initialized = true;
    }
    list = &s_ExportTypeName;
    tracede;
}

ErrCode getExportTypeName(uint32_t exportTypes, QHash<int, QString>& exportTypeName)
{
    traced;
    ErrCode err = ErrNone;
    QHash<int, QString> list;
    getListExportTypeName(&list);
    logd("exportTypes 0x%x", exportTypes);
    if (exportTypes) {
        foreach (int type, list.keys()) {
            if (type & exportTypes) {
                QString name = list.value(type);
                logd("found type 0x%x=%s", type, STR2CHA(name));
                exportTypeName.insert(type, name);
            }
        }
    } else {
        logi("empty exportTypes"); // TODO: assert it to make if failed, as it's abnormal case
    }
    logd("exportTypeName cnt %d", exportTypeName.count());
    tracedr(err);
    return err;
}
