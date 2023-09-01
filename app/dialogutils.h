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
 * Filename: dialogutils.h
 * Author: Anh, Ngo Huy
 * Created date:6/18/2023
 * Brief:
 */
#ifndef DIALOGUTILS_H
#define DIALOGUTILS_H

#include <QString>
#include <QWidget>

#define UNDER_DEV(...) DialogUtils::showDlgUnderDev(__VA_ARGS__)


class DialogUtils
{
public:
    static void showMsgBox(const QString& msg, QWidget* parent = nullptr);
    static void showErrorBox(const QString& msg);
    static void showErrorBox(int ret, const QString& msg = nullptr);
    static bool showConfirmDialog(QWidget *parent, const QString& title,
                                  const QString& message,
                                  std::function<void(int)> onFinished = nullptr);
    static QString showInputDialog(QWidget *parent, const QString& title, const QString& message, const QString& initInput = "", bool* isOk = nullptr);

    static void showDlgUnderDev(const QString& info = nullptr);
    static QString saveFileDialog(QWidget *parent = nullptr,
                                  const QString& title = QString(),
                                  const QString& initFileName = QString(),
                                  const QString& filter = QString(),
                                  const QString& initDir = QString());

};

#endif // DIALOGUTILS_H
