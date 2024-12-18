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
 * Filename: dlgmsg.h
 * Author: Anh, Ngo Huy
 * Created date:10/29/2023
 * Brief:
 */
#ifndef DLGMSG_H
#define DLGMSG_H

#include <QDialog>
#include <QStyle>

namespace Ui {
class DlgMsg;
}

class DlgMsg : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMsg(QWidget *parent = nullptr);
    ~DlgMsg();
    void setMsg(const QString& title, const QString& msg, QStyle::StandardPixmap style);
    static void showMessage(const QString& msg, QWidget *parent = nullptr);
    static void showErrMessage(const QString& msg, QWidget *parent = nullptr);
private:
    Ui::DlgMsg *ui;
};

#endif // DLGMSG_H
