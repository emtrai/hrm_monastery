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

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define VERSION STRINGIFY(_VERSION)

#define _VERSION VER_MAJOR.VER_MINOR.VER_PATCH


DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);

    ui->lblAppName->setText(tr("Quản lý Hội Dòng"));
    QString appInfo;
    // TODO: read somewhere????
    appInfo += QString(tr("Phiên bản: %1\n").arg(VERSION));
    appInfo += QString(tr("Ngày tạo: %1\n").arg(__DATE__));
    ui->txtAppInfo->setText(appInfo);
}

DlgAbout::~DlgAbout()
{
    delete ui;
}
