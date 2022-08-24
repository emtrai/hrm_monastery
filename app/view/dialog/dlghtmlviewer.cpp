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
#include "filectl.h"

dlgHtmlViewer::dlgHtmlViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHtmlViewer)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowState::WindowMaximized);
}

dlgHtmlViewer::~dlgHtmlViewer()
{
    delete ui;
}

void dlgHtmlViewer::setSubject(const QString &value)
{
    ui->lblSubject->setText(value);
}

void dlgHtmlViewer::setHtmlPath(const QString &fpath)
{
    traced;
    ui->txtContent->clearHistory();
    ui->txtContent->clear();

    ui->txtContent->setHtml(Utils::readAll(fpath));
}
