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
 * Filename: viewutils.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/26/2023
 * Brief:
 */
#include "viewutils.h"
#include "logger.h"
#include <QTableWidget.h>
#include <QTableWidgetItem>
#include "utils.h"

void ViewUtils::cleanupTableItems(QTableWidget *tbl)
{
    tracein;
    if (tbl) {
        int rowCnt = tbl->rowCount();
        int colCnt = tbl->columnCount();
        dbgd("rowCnt %d colCnt %d", rowCnt, colCnt);
        for (int i = 0; i < rowCnt; i++) {
            for (int j = 0; j < colCnt; j++) {
                QTableWidgetItem* item = tbl->takeItem(i, j);
                FREE_PTR(item);
            }
        }
        tbl->clearContents();
        tbl->model()->removeRows(0, tbl->rowCount());
    } else {
        logw("not clean table, invalid arg");
    }
    traceout;
}
