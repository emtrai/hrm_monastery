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
 * Filename: dlgimportexportselect.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/19/2023
 * Brief:
 */
#include "dlgimportexportselect.h"
#include "ui_dlgimportexportselect.h"
#include "logger.h"
#include "utils.h"
#include <QFileDialog>
#include "filectl.h"

DlgImportExportSelect::DlgImportExportSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportExportSelect),
    mExportType(0),
    mIsExport(false)
{
    tracein;
    ui->setupUi(this);
    traceout;
}

DlgImportExportSelect::~DlgImportExportSelect()
{
    tracein;
    delete ui;
    traceout;
}

ErrCode DlgImportExportSelect::setExportTypes(quint64 exportTypes)
{
    tracein;
    QHash<int, QString> exportTypeName;
    ErrCode err = ErrNone;
    logd("Export Type 0x%x", exportTypes);
    if (!exportTypes) {
        err = ErrInvalidArg;
        loge("invalid arguments");
    }
    if (err == ErrNone) {
        mExportType = exportTypes;
        err = getExportTypeName(exportTypes, exportTypeName);
    }
    if (err == ErrNone) {
        if (exportTypeName.size() > 0) {
            foreach (int type, exportTypeName.keys()) {
                logd("export type 0x%x", type);
                ui->cbExportType->addItem(exportTypeName.value(type), type);
            }
        } else {
            err = ErrNoData;
            loge("not found any export type name");
        }
    }
    traceret(err);
    return err;
}


void DlgImportExportSelect::setImportExport(bool isExport, const QString &title)
{
    tracein;
    logd("isExport %d", isExport);
    mIsExport = isExport;
    mTitle = isExport?"Xuất dữ liệu":"Nhập dữ liệu";
    if (!title.isEmpty()) {
        mTitle += ": " + title;
    }
    ui->lblTitle->setText(mTitle);
    traceout;
}

const QString &DlgImportExportSelect::path() const
{
    return mPath;
}

ExportType DlgImportExportSelect::selectedExportType() const
{
    return mSelectedExportType;
}

void DlgImportExportSelect::setSelectedExportType(ExportType newSelectedExportType)
{
    mSelectedExportType = newSelectedExportType;
}

void DlgImportExportSelect::accept()
{
    tracein;
    logd("mIsExport %d", mIsExport);
    mPath.clear();
    QVariant currentData = ui->cbExportType->currentData();
    ErrCode err = ErrNone;
    QString ext;
    if (currentData.isValid()) {
        bool ok = false;
        int type = currentData.toInt(&ok);
        if (ok) {
            mSelectedExportType = (ExportType)type;
        } else {
            loge("Selected data is invalid");
            Utils::showErrorBox(tr("Định dạng nhập/xuất không đúng"));
            err = ErrInvalidData;

        }
    } else {
        loge("no selected data");
        Utils::showErrorBox(tr("Định dạng nhập/xuất không có"));
        err = ErrNoData;
    }

    if (err == ErrNone) {
        bool ok = false;
        ext = typeToExt(mSelectedExportType, &ok);
        if (!ok) {
            loge("Not found suitable extension for type %d", mSelectedExportType);
            Utils::showErrorBox(tr("Không tìm thấy định dạng tập tin phù hợp"));
            err = ErrInvalidData;
        }
    }
    if (err == ErrNone) {
        logd("extension %s", STR2CHA(ext));
        if (mIsExport) {
            mPath = Utils::saveFileDialog(this, mTitle,
                                          QString("xuatdulieu.%1").arg(ext),
                                          QString("%1 (*.%2)").arg(ext.toUpper(), ext)
                                          );
        } else {
            mPath = QFileDialog::getOpenFileName(
                this,
                mTitle,
                FileCtl::getAppDataDir(),
                QString("%1 Files (*.%2);").arg(ext.toUpper(), ext)
                );
        }
        QDialog::accept();
    } else {
        loge("Get export path filed, err %d", err);
    }
    traceout;
}
