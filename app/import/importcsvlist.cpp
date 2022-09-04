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
 * Filename: importcsvlist.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/3/2022
 * Brief:
 */
#include "importcsvlist.h"
#include "logger.h"
#include "defs.h"
#include "utils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "filectl.h"
#include <QHash>

GET_INSTANCE_IMPL(ImportCSVList)

ImportCSVList::ImportCSVList()
{
    traced;
}

ErrCode ImportCSVList::importFrom(int importFileType, IImporter *importer, const QString &fpath, void* tag)
{
    traced;
//    QHash<QString, QString> item;
    ErrCode ret = ErrNone;
    qint32 cnt = 0;

    if (importer == nullptr){
        ret = ErrInvalidArg;
        loge("import failed, importer null");
    }

    if (ret == ErrNone) {
        logd("parse csv file %s", fpath.toStdString().c_str());
        // TODO: parse and put all item in hash, consume memory, should use callback?
        // Intend to use callback, but considering the case that error happend when
        // parsing --> date stored by callback may invalid
        // parse and check whole will ensure parsing finished ok
        ret = Utils::parseCSVFile(fpath,
                [importer, importFileType](const QStringList& items, void* caller, void* param, quint32 idx){
                traced;
                logd("called from lambda of importFrom");
//                IImporter* importer = (IImporter)*param;
                ErrCode ret2 = ErrNone;
                if (importer != nullptr){
                    ret2 = importer->onImportItem(importFileType, items, idx, param);
                }
                tracedr(ret2);
                return ret2;
                },
            this, tag, CSV_ITEM_SPLIT, &cnt);
    }
    logi("Parsed %d item", cnt);
    logd("Parse result %d", ret);
    // TODO: handle error case, some items may added to system, some error, should continue???

    tracedr(ret);
    return ret;
}