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
 * Filename: importcsv.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/27/2022
 * Brief:
 */
#include "importcsv.h"
#include "logger.h"
#include "defs.h"
#include "utils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "filectl.h"
#include <QHash>

GET_INSTANCE_IMPL(ImportCSV)

ImportCSV::ImportCSV()
{
    traced;
}

ErrCode ImportCSV::importFrom(IImporter *importer, const QString &fpath)
{
    traced;
    QHash<QString, QString> item;
    ErrCode ret = ErrNone;

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
        ret = Utils::parseDataFromCSVFile(fpath, &item);
    }
    if (ret == ErrNone){
        int cnt = item.count();
        logd("Parsed %d key", cnt);
        if (cnt > 0) {
            foreach (QString key, item.keys()) {
                logd("key %s", key.toStdString().c_str());
                logd("val %s", item.value(key).toStdString().c_str());
                importer->onImportItem(key, item.value(key));
            }
        }
    }

    tracedr(ret);
    return ret;

}
