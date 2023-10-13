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
 * Filename: dlgabout.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#include "dlgabout.h"
#include "ui_dlgabout.h"
#include "logger.h"
#include <QDesktopServices>
#include <QUrl>
#include "filectl.h"
#include "logger.h"
#include "utils.h"
#include "defs.h"
#include "config.h"


DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
    int h = 0;
    int w = 0;
    Utils::screenSize(&w, &h);
    resize(w/2, h/2);
    this->setWindowTitle(tr("Thông tin ứng dụng"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Thoát"));

    ui->lblAppName->setText(tr("Quản lý Hội Dòng"));
    QString appInfo;
    // TODO: read somewhere????
    appInfo += QString(tr("\n------ Thông tin phiên bản ------\n"));
    appInfo += QString(tr("- Phiên bản: %1 (0x%2)\n").arg(APP_VERSION).arg(APP_VERSION_CODE, 16));
    appInfo += QString(tr("- Ngày tạo: %1\n").arg(__DATE__));
    appInfo += QString(tr("Phiên bản dùng thử\n")); // TODO: load release info from file???
    appInfo += QString(tr("\n\n------ Thư mục dữ liệu ------\n"));
    appInfo += QString(tr("- Thư mục dữ liệu cài đặt ứng dụng: \n%1\n").arg(FileCtl::getAppInstallDir()));
    appInfo += QString(tr("- Thư mục dữ liệu ứng dụng: \n%1\n").arg(FileCtl::getAppWorkingDataDir()));
    appInfo += QString(tr("- Thư mục log: \n%1\n").arg(Logger::logDirPath()));
    appInfo += QString(tr("- Thư mục tạm: \n%1\n").arg(FileCtl::getTmpDataDir()));
    appInfo += QString(tr("- Thư mục sao lưu: \n%1\n").arg(CONFIG->getAutoBackupDirPath()));
    ui->txtAppInfo->setText(appInfo);
}

DlgAbout::~DlgAbout()
{
    delete ui;
}

void DlgAbout::on_txtLog_clicked()
{

    QString logDir = Logger::logDirPath();
    logd("Open log dir=%s", STR2CHA(logDir));
    QDesktopServices::openUrl(QUrl::fromLocalFile(logDir));
}

