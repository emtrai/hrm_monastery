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
 * Filename: dialogutils.cpp
 * Author: Anh, Ngo Huy
 * Created date:6/18/2023
 * Brief:
 */
#include "dialogutils.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "utils.h"
#include "logger.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include "dlgmsg.h"

void DialogUtils::showMsgBox(const QString &msg, QWidget* parent)
{
    tracein;
    logd("Msg box %s", msg.toStdString().c_str());
    DlgMsg::showMessage(msg, parent);
    // cannot use QMessageBox here, as it's auto cancel, still don't understand why
    // TODO: check why cannot use QMessageBox here
    traceout;
}

void DialogUtils::showErrorBox(const QString &msg, QWidget* parent)
{
    tracein;
    logd("Error box %s", msg.toStdString().c_str());
    DlgMsg::showErrMessage(msg, parent);
    // cannot use QMessageBox here, as it's auto cancel, still don't understand why
    // TODO: check why cannot use QMessageBox here
    traceout;
}

void DialogUtils::showErrorBox(int ret, const QString& msg)
{
    tracein;
    QString errMsg;
    if (ret != ErrNone) {
        errMsg.append(QString("Lỗi ! Mã lỗi %1\n").arg(ret)); // TODO: translation
    }
    if (!msg.isEmpty()) {
        errMsg.append(msg);
    }
    if (errMsg.isEmpty()) {
        errMsg = STR_ERROR;
    }
    showErrorBox(errMsg);
}

bool DialogUtils::showConfirmDialog(QWidget *parent, const QString &title,
                              const QString &message,
                              std::function<void(int)> onFinished)
{
    tracein;
    bool ok = false;
    if (!parent) parent = MAINWIN;
    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
//    msgBox.setParent(parent);
    int reply = msgBox.exec();
//    reply = QMessageBox::question(parent, title, message,
//                                  QMessageBox::Yes|QMessageBox::No);
    if (onFinished) onFinished(reply);
    if (reply == QMessageBox::Yes) {
        ok = true;
    } else {
        logd("selected no");
        ok = false;
    }
    traceout;
    return ok;
}

QString DialogUtils::showInputDialog(QWidget *parent, const QString &title, const QString &message, const QString& initInput, bool *isOk)
{
    tracein;
    QString text = QInputDialog::getText(parent, title,
                                         message, QLineEdit::Normal,
                                         initInput, isOk);
    logd("input text: '%s'", STR2CHA(text));
    traceout;
    return text;
}


void DialogUtils::showDlgUnderDev(const QString &info)
{
    QMessageBox msgBox;
    tracein;
    QString msg = QObject::tr("Tính năng đang phát triển");
    if (!info.isEmpty()) {
        msg += QObject::tr("\n* "); // TODO: translation???
        msg += info;
    }
    logd("Info%s", msg.toStdString().c_str());
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
    traceout;
}

QString DialogUtils::saveFileDialog(QWidget *parent,
                              const QString& title,
                              const QString& initFileName,
                              const QString& filter,
                              const QString& initDir)
{
    tracein;
    QString dir = !(initDir.isEmpty())?initDir:QDir::homePath();
    QString name = !(initFileName.isEmpty())?initFileName:"";
    QString fileFilter = !(filter.isEmpty())?filter:"Tất cả loại tập tin (*.*)";
    QString dlgTitle = !(title.isEmpty())?title:"Lưu tập tin";
    logd("dir %s", STR2CHA(dir));
    logd("name %s", STR2CHA(name));
    logd("filter %s", STR2CHA(fileFilter));
    logd("title %s", STR2CHA(dlgTitle));
    QString fpath = QFileDialog::getSaveFileName(parent,
                                                 dlgTitle,
                                                 dir + "/" + name,
                                                 fileFilter);
    logd("save to file '%s'", STR2CHA(fpath));
    traceout;
    return fpath;
}

