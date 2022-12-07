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
#include <QToolButton>
#include "view/widget/uisummarizeview.h"
#include "view/widget/uitableview.h"
#include "view/widget/uitableviewfactory.h"
#include <QAction>
#include "loader/loaderctl.h"
QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Person;
class DlgWait;

    class MainWindow : public QMainWindow, public LoaderListener
{
    Q_OBJECT

        public:
                 MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
            static MainWindow *getInstance();

            static void showAddEditPerson(bool isSelfUpdate = true, Person* per = nullptr);
        protected:
     void showEvent(QShowEvent *ev);
     static void onFinishLoading(int ret, void* data);
 public:
     void switchView(ViewType type);
     void switchView(QWidget* nextView);
     QWidget* getView(ViewType type);
    protected:

     virtual void onLoadController (Controller* ctl);
     void doShowAddEditPerson(bool isSelfUpdate = true, Person* per = nullptr);
 private:
     void loadHomePageFile();
     void loadOtherMenu();
     void loadImportMenu();
     void loadExportMenu();

 private:
    Ui::MainWindow *ui;
    UISummarizeView* mSummarizeView;
    UITableView* mCommunityView;
    UITableView* mSaintsView;
    UITableView* mPersonView;
    UITableView* mAreaView;
    UITableView* mDepartView;
    UITableView* mRoleView;
    QList<QWidget*> mMainViews;
    QTextBrowser* mHomeView;
    QWidget* mCurrentView;

    QToolButton *mImportButton;
    QAction* mActionImportPersonList;
    QAction* mActionImportPerson;
    QAction* mActionImportCommunityList;

    QToolButton *mExportButton;
    QAction* mActionExportPersonList;
    DlgWait* mWaitDlg;

 signals:
    void load();

 private slots:
    void onLoad();
     void on_action_ImportPerson_triggered();
     void on_action_ImportPersonList_triggered();
     void on_action_ImportCommunityList_triggered();
     void on_action_ImportCommunity_triggered();
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
     void on_actionRole_triggered();
     void on_actionDept_triggered();

     void on_action_ExportPersonList_triggered();

     void on_action_About_triggered();

     void on_action_Backup_triggered();

     void on_actionRestore_triggered();

     void on_actionRevert_triggered();

     void on_action_Help_triggered();


     void on_actionEthnic_triggered();
     void on_actionWork_triggered();
     void on_actionEducation_triggered();
     void on_actionSpeclialist_triggered();
     void on_actionMisson_triggered();
     void on_actionProvince_triggered();
     void on_actionCountry_triggered();

 private:
     static MainWindow* gInstance;
};
#endif // MAINWINDOW_H
