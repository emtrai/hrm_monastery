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
 * Filename: dlghtmlviewer.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/21/2022
 * Brief:
 */
#include "dlghtmlviewer.h"
#include "ui_dlghtmlviewer.h"
#include "utils.h"
#include <QPushButton>
#include "dialogutils.h"
#include "stringdefs.h"

dlgHtmlViewer::dlgHtmlViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHtmlViewer)
{
    ui->setupUi(this);
    DIALOG_SIZE_SHOW(this);

    if (ui->buttonBox->button(QDialogButtonBox::Cancel))
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_EXIT);
}

dlgHtmlViewer::~dlgHtmlViewer()
{
    delete ui;
}

void dlgHtmlViewer::setSubject(const QString &value)
{
    setWindowTitle(value);
}

void dlgHtmlViewer::setHtmlPath(const QString &fpath)
{
    tracein;
    ui->txtContent->clearHistory();
    ui->txtContent->clear();

    logi("html path '%s'", STR2CHA(fpath));
    if (QFile::exists(fpath)) {
        ui->txtContent->setHtml(Utils::readAll(fpath));
        mHtmlPath = fpath;
    } else {
        loge("file '%s' not exist", STR2CHA(fpath));
    }

    traceout;
}

void dlgHtmlViewer::on_btnExport_clicked()
{
    // Export information to file
    tracein;
    ErrCode err = ErrNone;

    if (mHtmlPath.isEmpty() || !QFile::exists(mHtmlPath)) {
        err = ErrNotFound;
        loge("'%s' is empty or not found", STR2CHA(mHtmlPath));
    }

    if (err == ErrNone) {
        err = Utils::saveHtmlToPdf(mHtmlPath,
                                   Utils::UidFromName(this->windowTitle(),
                                        UidNameConvertType::NO_VN_MARK_UPPER),
                                   this);
    }
    if (err == ErrNone) {
        logi("Saved html path '%s' to pdf succeed", STR2CHA(mHtmlPath));
    } else {
        loge("Saved html path '%s' to pdf failed, err=%d", STR2CHA(mHtmlPath), err);
        DialogUtils::showErrorBox(err, QString(tr("Xuất dữ liệu lỗi.")));
    }
    traceout;
}

