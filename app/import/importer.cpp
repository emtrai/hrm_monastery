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
 * Filename: importer.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#include "importer.h"
#include "logger.h"
#include "dbmodel.h"

Importer::Importer()
{

}

ErrCode Importer::importFrom(const QString& importName, int importFileType,
                             IDataImporter *importer, const QString &fpath, QList<DbModel *>* outList)
{
    traced;
    logd("Not support here");
    return ErrNotSupport;
}
