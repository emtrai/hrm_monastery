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
 * Filename: dlgmsg.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/29/2023
 * Brief:
 */
#include "dlgmsg.h"
#include "ui_dlgmsg.h"
#include "logger.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include <QPushButton>

DlgMsg::DlgMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMsg)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_CANCEL);
}

DlgMsg::~DlgMsg()
{
    delete ui;
}

void DlgMsg::setMsg(const QString& title, const QString &msg, QStyle::StandardPixmap style)
{
    tracein;
    ui->txtText->setPlainText(msg);
    QIcon icon = MAINWIN->style()->standardIcon(style); //or
    //whatever icon you choose
    QPixmap pixmap = icon.pixmap(QSize(60, 60));
    ui->lblIcon->setPixmap(pixmap);
    setWindowTitle(title);
    traceout;
}

void DlgMsg::showMessage(const QString &msg, QWidget *parent)
{
    traced;
    DlgMsg dlg(parent);
    dlg.setMsg(STR_INFO, msg, QStyle::SP_MessageBoxInformation);
    dlg.exec();
}

void DlgMsg::showErrMessage(const QString &msg, QWidget *parent)
{
    traced;
    DlgMsg dlg(parent);
    dlg.setMsg(STR_ERROR, msg, QStyle::SP_MessageBoxCritical);
    dlg.exec();
}
