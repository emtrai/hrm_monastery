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
    traced;
}

ErrCode ExportCSVList::saveTo(const IDataExporter* exporter, const QList<DbModel*> listData, const QString &fpath)
{
    traced;
    ErrCode ret = ErrNone;
    const QString templatePath = exporter->exportTemplatePath(this);
    qint32 cnt = 0;
    QStringList items;
    QString finalData;
    logd("templatePath %s", templatePath.toStdString().c_str());

    if (templatePath.isEmpty()) {
        ret = ErrInvalidData;
        loge("Export HTML failed, not template found");
    }


    if (ret == ErrNone) {
        logd("parse csv file %s", templatePath.toStdString().c_str());
        ret = Utils::parseCSVFile(templatePath,
            [this](const QStringList& items, void* caller, void* param, quint32 idx){
                traced;
                logd("callback from lamda when parse csv file for export csv list, idx=%d", idx);
                return ErrCancelled; // we assume 1st line is headers, so don't need to read more
            },
            this, nullptr, CSV_LIST_ITEM_SPLIT, &cnt, &items);
    }
    if (ret == ErrNone || ret == ErrCancelled) {
        if (items.length() == 0) {
            ret = ErrNoData;
            loge("No header found");
        } else {
            ret = ErrNone; // if ErrCancelled is intended, to stop reading, focus on 1st line only
        }
    }
    if (ret == ErrNone) {
        logd("get header");
        foreach (QString item, items) {
            if (finalData.length() > 0) {
                finalData += CSV_LIST_ITEM_SPLIT;
            }
            finalData += item.trimmed();
        }
        finalData += "\n";
        logd("get data, no item: %lld", listData.length());
        foreach (DbModel* dataExport, listData) {
            QString dataLine;
            foreach (QString item, items) {
                QString data;
                logd("keyword '%s'", STR2CHA(item));
                ret = exporter->getExportDataString(item.trimmed(), dataExport, &data);
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
    tracedr(ret);
    return ret;
}

ExportType ExportCSVList::getExportType()
{
    return EXPORT_CSV_LIST;
}

