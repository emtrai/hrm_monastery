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
 * Filename: importxlsx.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/25/2023
 * Brief:
 */
#include "importxlsx.h"

#include "logger.h"
#include "defs.h"
#include "utils.h"
#include "datetimeutils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "filectl.h"
#include <QHash>
#include "dbmodel.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

#include "importexportdefs.h"


GET_INSTANCE_IMPL(ImportXlsx)

ImportXlsx::ImportXlsx():Importer()
{
    tracein;
}

ErrCode ImportXlsx::importFrom(const QString &importName, int importFileType,
                               IDataImporter *importer, const QString &fpath,
                               QList<DbModel *> *outList)
{
    tracein;
    //    QHash<QString, QString> item;
    ErrCode ret = ErrNone;
    qint32 cnt = 0;

    if (importer == nullptr){
        ret = ErrInvalidArg;
        loge("import failed, importer null");
    }

    if (ret == ErrNone) {
        ret = importer->onImportDataStart(importName, importFileType, fpath);
    }
    if (ret == ErrNone) {
        logi("Read data from xlsx file %s", STR2CHA(fpath));
        QXlsx::Document xlsxR(fpath);
        if (xlsxR.load()) {
            xlsxR.workbook()->setDate1904(false);
            int headCol = 1;
            int headRow = 1;
            #define STATE_FIND_ID  (0)
            #define STATE_READ_ID  (1)
            #define STATE_READ_DATA  (2)
            int state = STATE_FIND_ID;
            QList<QString> fields;
            QHash<QString, QString> fieldValues;
            int fieldIdx = 0;
            bool finished = false;
            int nameIdIdx = 0;
            for (int row = 1; row < MAX_ROW && !finished; row++) {
                tracelog("Read xlsx at row=%d, state=%d", row, state);
                for (int col = headCol; col < MAX_COL && row < MAX_ROW && !finished; ) {
                    QXlsx::Cell* cell = xlsxR.cellAt(row, col); // get cell pointer.

                    QString value;
                    if (cell != nullptr) {
                        QVariant var = cell->readValue(); // read cell value (number(double), QDateTime, QString ...)
                        if (var.isValid()) {
                            value = var.toString().trimmed();
                        }
                        if (cell->isDateTime()) { // QXlsx read in format of YYYY/MM/DD
                            qint32 time = DatetimeUtils::dateFromString(value, "YYYY/MM/DD");
                            value = DatetimeUtils::date2String(time, DEFAULT_FORMAT_YMD);
                        }
                    }
                    logd("value='%s'", STR2CHA(value));
                    switch (state) {
                    case STATE_FIND_ID:
                        if (cell && !value.isEmpty() && value == MARK_START_CHAR) {
                            headRow = row;
                            headCol = col;
                            state = STATE_READ_ID;
                            tracelog("Found neccessary mark '%s' at row=%d, col=%d", MARK_START_CHAR, row, col);
                        }
                        break;
                    case STATE_READ_ID:
                        if (cell && !value.isEmpty()) {
                            tracelog("Add field '%s'", STR2CHA(value));
                            fields.append(value);
                        } else {
                            state = STATE_READ_DATA;
                            row++;
                            col = headCol;
                            tracelog("restart to read rata at row=%d, col=%d", row, col);
                        }
                        break;
                    case STATE_READ_DATA:
                        if (col == headCol) {
                            if (!cell || value.toLower() == MARK_END_CHAR) {
                                finished = true;
                            }
                        } else {
                            fieldIdx = col - headCol - 1;
                            if (fieldIdx < fields.size()) {
                                tracelog("Got '%s'[%d]:'%s'", STR2CHA(fields[fieldIdx]), fieldIdx, STR2CHA(value));
                                fieldValues[fields[fieldIdx]] = value;
                            } else {
                                row++;
                                col = headCol;
                                logd("reach end field, start parsing");
                                ret = importer->onImportParseDataItem(importName, importFileType, fieldValues, cnt, outList);
                                fieldValues.clear();
                                cnt++;
                                continue;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                    col++;
                }
            }
        } else {
            loge("Load xlsx failed, file '%s'", STR2CHA(fpath));
        }
        importer->onImportDataEnd(importName, importFileType, fpath, ret);
    }
    logi("Parsed %d item", cnt);
    logd("Parse result %d", ret);
    // TODO: handle error case, some items may added to system, some error, should continue???

    traceret(ret);
    return ret;
}
