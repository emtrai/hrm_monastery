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
 * Filename: exporttype.h
 * Author: Anh, Ngo Huy
 * Created date:1/27/2023
 * Brief:
 */
#ifndef EXPORTTYPE_H
#define EXPORTTYPE_H

// TODO: should we separate into Export format (csv, xlsx, etc.) and export type (list, item, etc.)?
enum ExportType {
    EXPORT_NONE = 0,
    EXPORT_HTML,
    EXPORT_CSV,
    EXPORT_CSV_LIST,
    EXPORT_DOCX,
    EXPORT_XLSX,
    EXPORT_MAX
};

#endif // EXPORTTYPE_H
