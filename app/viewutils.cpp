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

void ViewUtils::updatePageInfo(QComboBox *cbPage, QLabel *lblPage,
                               qint32 page, qint32 total, qint32 itemPerPage)
{
    tracein;
    if (page > 0 && total > 0) {
        qint32 perPage = ((itemPerPage != 0) && (itemPerPage < total))?itemPerPage:total;
        qint32 totalPages = total/perPage + ((total%perPage == 0)?0:1);
        dbgd("page %d, total %d, total page %d, perPage = %d",
             page, total, totalPages, perPage);

        cbPage->blockSignals(true); // TODO: store old state then restore?
        cbPage->clear();
        lblPage->setText("");
        for (int pageidx = 0; pageidx < totalPages; pageidx++) {
            cbPage->addItem(QString::number(pageidx+1), pageidx+1);
        }
        qint32 startIdx = (page-1)*perPage;
        qint32 endIdx = (startIdx + perPage);
        if (endIdx > total) endIdx = total;
        logd("startIdx %d, endIdx = %d", startIdx, endIdx);
        lblPage->setText(QString("%1-%2/%3").arg(startIdx+1).arg(endIdx).arg(total));
        cbPage->setCurrentIndex(page-1);
        cbPage->blockSignals(false);
    } else {
        loge("invalid page %d or total %d", page, total);
        cbPage->blockSignals(true); // TODO: store old state and restore?
        cbPage->clear();
        lblPage->setText("");
        cbPage->blockSignals(false);
    }
    traceout;
}
