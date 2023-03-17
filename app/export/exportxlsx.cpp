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
 * Filename: exportxlsx.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/19/2023
 * Brief:
 */
#include "exportxlsx.h"
#include "logger.h"
#include "defs.h"
#include "utils.h"
#include "dataexporter.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


GET_INSTANCE_IMPL(ExportXlsx)

ExportXlsx::ExportXlsx()
{
    traced;

}

ErrCode ExportXlsx::saveTo(const DataExporter *item, const QString &fpath)
{
    traced;
    QXlsx::Document xlsx;
    xlsx.write("A1", "Hello Qt!"); // write "Hello Qt!" to cell(A,1). it's shared string.
    xlsx.saveAs(fpath); // save the document as 'Test.xlsx'
    // TODO: implement it carefully!
    tracede;
    return ErrNone;
}

ErrCode ExportXlsx::saveTo(const DataExporter *exporter, const QList<DbModel *> listData, const QString &fpath)
{
//    traced;
//    QXlsx::Document xlsx;
//    xlsx.write("A1", "Hello Qt!"); // write "Hello Qt!" to cell(A,1). it's shared string.
//    xlsx.saveAs(fpath); // save the document as 'Test.xlsx'
//    // TODO: implement it carefully!
//    tracede;
//    return ErrNone;
    traced;
    ErrCode ret = ErrNone;
    QList<QPair<QString,QString>> keywordMap;
    qint32 cnt = 0;
    QStringList items;
    QString finalData;
    QXlsx::Document xlsx;
    ret = exporter->getListTemplateExportKeywords(this, keywordMap);
    if (keywordMap.isEmpty()) {
        ret = ErrInvalidData;
        loge("Export HTML failed, not template found");
    }

    if (ret == ErrNone) {
        logd("get header");
        int col = 1;
        int row = 1;
        foreach (auto item, keywordMap) {
            col++;
            xlsx.write(row, col, item.second);
            xlsx.write(row+1, col, item.first);
        }
        row++;
        int idx = 0;

        logd("get data, no item: %lld", listData.length());
        foreach (DbModel* dataExport, listData) {
            col = 1;
            row++;
            idx++;
            xlsx.write(row, col++, QString("%1").arg(idx));
            foreach (auto item, keywordMap) {
                QString data;
                logd("keyword '%s'", STR2CHA(item.first));
                ret = exporter->getExportDataString(item.first, dataExport, &data);
                if (ret == ErrNone) {
                    xlsx.write(row, col++, data);
                } else {
                    loge("get export data string for item '%s' failed", STR2CHA(item.first));
                    break;
                }
            }

            if (ret != ErrNone) {
                break;
            }
        }
    }
    if (ret == ErrNone) {
        logd("Write to file %s", fpath.toStdString().c_str());
        xlsx.saveAs(fpath);
    }
    tracedr(ret);
    return ret;
}

ExportType ExportXlsx::getExportType()
{
    return EXPORT_XLSX;
}
