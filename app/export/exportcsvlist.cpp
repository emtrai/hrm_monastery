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
 * Filename: exportcsvlist.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/27/2023
 * Brief:
 */
#include "exportcsvlist.h"
#include "logger.h"
#include "defs.h"
#include "utils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "filectl.h"

GET_INSTANCE_IMPL(ExportCSVList)

ExportCSVList::ExportCSVList()
{
    tracein;
}

ErrCode ExportCSVList::saveTo(const DataExporter* exporter, const QString& datatype, const QList<DbModel*> listData, const QString &fpath)
{
    tracein;
    ErrCode ret = ErrNone;
    qint32 cnt = 0;
    QStringList items;
    QString finalData;
    QList<QPair<QString,QString>> keywordMap;

    ret = exporter->getListTemplateExportKeywords(this, datatype, keywordMap);

    if (ret == ErrNone) {
        logd("get header");
        foreach (auto item, keywordMap) {
            if (finalData.length() > 0) {
                finalData += CSV_LIST_ITEM_SPLIT;
            }
            finalData += item.first;
        }
        finalData = "#" + finalData + "\n";
        logd("get data, no item: %lld", listData.length());
        foreach (DbModel* dataExport, listData) {
            QString dataLine;
            foreach (QString item, items) {
                QString data;
                logd("item '%s'", STR2CHA(item));
                ret = exporter->getExportDataString(item.trimmed(), this, datatype, dataExport, &data);
                if (ret == ErrNone) {
                    logd("data '%s'", STR2CHA(data));
                    if (dataLine.length() > 0) {
                        dataLine += CSV_LIST_ITEM_SPLIT;
                    }
                    dataLine += QString("\"%1\"").arg(data);
                } else {
                    loge("get export data string for item '%s' failed", STR2CHA(item));
                    break;
                }
            }

            if (ret == ErrNone) {
                logd("dataLine %s", STR2CHA(dataLine));
                finalData += dataLine + "\n";
            } else {
                break;
            }
        }
    }
    if (ret == ErrNone) {
        logd("Write %d finadata to file %s", finalData.length(), fpath.toStdString().c_str());
        ret = FileCtl::writeStringToFile(finalData, fpath);
    }
    traceret(ret);
    return ret;
}

ExportType ExportCSVList::getExportType() const
{
    return EXPORT_CSV_LIST;
}

