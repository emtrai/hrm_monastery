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
#include <QTextBrowser>

#include "view/widget/uisummarizeview.h"
#include "view/widget/uitableview.h"
#include "view/widget/uitableviewfactory.h"
#include <QAction>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

        public:
                 MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
            static MainWindow *getInstance();

        protected:
     void showEvent(QShowEvent *ev);
     static void onFinishLoading(int ret, void* data);
 public:
     void switchView(ViewType type);
     void switchView(QWidget* nextView);
     QWidget* getView(ViewType type);
 private:
     void loadHomePageFile();
     void loadOtherMenu();
     void loadImportMenu();

 private:
    Ui::MainWindow *ui;
    UISummarizeView* mSummarizeView;
    UITableView* mCommunityView;
    UITableView* mSaintsView;
    UITableView* mPersonView;
    UITableView* mAreaView;
    UITableView* mDepartView;
    QList<QWidget*> mMainViews;
    QTextBrowser* mHomeView;
    QWidget* mCurrentView;
    QAction* mActionImportPersonList;
    QAction* mActionImportPerson;
    QAction* mActionImportCommunityList;
 signals:
    void load();

 private slots:
     void on_action_ImportPersonList_triggered();
     void on_action_New_triggered();
     void on_actionNew_Community_triggered();
     void on_actionImportComm_triggered();
     void on_actionCommunity_triggered();
     void on_actionSummarize_triggered();
     void on_actionSaints_2_triggered();
     void on_actionHome_triggered();
     void on_actionPerson_triggered();
     void on_actionSearch_triggered();
     void on_actionArea_triggered();
     void on_actionDepart_triggered();
 private:
     static MainWindow* gInstance;
};
#endif // MAINWINDOW_H
