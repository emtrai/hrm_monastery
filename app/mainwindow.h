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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

        public:
                 MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 protected:
     void showEvent(QShowEvent *ev);
     static void onFinishLoading(int ret, void* data);
 private:
    Ui::MainWindow *ui;

 signals:
    void load();

 private slots:
     void on_action_New_triggered();
     void on_actionNew_Community_triggered();
     void on_actionImportComm_triggered();
};
#endif // MAINWINDOW_H
