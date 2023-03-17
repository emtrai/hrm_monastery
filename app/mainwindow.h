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
#include "view/widget/baseview.h"
#include "view/dialog/dlgcommoneditmodel.h"
#include "importfactory.h"
#include <QStack>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define MAIN MainWindow::getInstance()

// TODO: full state machine/state transition check/report incident
enum AppState {
    APP_STATE_NOT_READY = 0,
    APP_STATE_INITING,
    APP_STATE_LOADING,
    APP_STATE_READY,
    APP_STATE_LOAD_ERR,
    APP_STATE_ERROR
};

class Person;
class DlgWait;
class Community;
class ModelController;

    class MainWindow : public QMainWindow, public LoaderListener
{
    Q_OBJECT

        public:
                 MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
            static MainWindow *getInstance();

            static void showAddEditPerson(bool isSelfUpdate = true, Person* per = nullptr, bool isNew = true);
            static void showAddEditCommunity(bool isSelfUpdate = true, Community* com = nullptr,
                                             CommonEditModelListener* listener = nullptr);
            static void showImportDlg(ImportTarget target);
            static void showOnHtmlViewer(DbModel* model, const QString& subject);
            static void showAddEditCommonModel(bool isSelfUpdate = true, DbModel* model = nullptr,
                                             CommonEditModelListener* listener = nullptr);
            ErrCode exportListItems(const QList<DbModel*>* items, ModelController* controller,
                                    const QString& title = nullptr,
                                    quint64 exportTypeList = 0 // List of supported export type, bitwise
                                    );
        protected:
     void showEvent(QShowEvent *ev);
 public:
     void switchView(ViewType type);
     void switchView(QWidget* nextView);
     QWidget* getView(ViewType type);
     AppState appState() const;
     void setAppState(AppState newAppState);

     /**
      * @brief Pop view from stack and show
      * @return true if there is still view exist in stack, false if no more view from stack
      */
     bool popViewFromStackAndShow();
     void pushViewToStack(ViewType type);

 protected:

     virtual void onLoadController (Controller* ctl);
     void doShowAddEditPerson(bool isSelfUpdate = true, Person* per = nullptr, bool isNew = true);
     void doShowAddEditCommunity(bool isSelfUpdate = true, Community* com = nullptr,
                                 CommonEditModelListener* listener = nullptr);
     void doShowImportPerson();
     void doShowImportCommunity();
     void doShowAddEditCommonModel(bool isSelfUpdate = true, DbModel* model = nullptr,
                                        CommonEditModelListener* listener = nullptr);
     ErrCode doExportListItems(const QList<DbModel*>* items, ModelController* controller,
                               const QString& title = nullptr, quint64 exportTypeList = 0);
 private:
     void loadHomePageFile();
     void loadOtherMenu();
     void loadOtherAddMenu();
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
    UITableView* mCourseView;
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
    AppState mAppState;

    /* stack of table view
     * it just used for view created at run time,
     * not fix view which can be shown by toolbar
     * I don't use QStackedWidget here, because we have same panel, but display
     * 2 kinds of table view:
     * - fix one (can open/show via button on toolbar, like community, etc.)
     * - dynamic one, open/create in sub-menu item, etc...
     * We can not push fix one to stack, to avoid case that user click/toggle to change
     * view continuously, causing stack overflow
     * Or another consider point is that: it can include all type of tablve view, including fix and dynamic
     * mainapp will have a hash map, which containt list of view type + its pointer
     * when thereis request to show view, get it from this hash map, if not exist, create new one, else
     * get one from hash map, then update data (or override value).
     * ViewStack will contain a vie type only, not pointer to widget, pointer point to widget is just contain in hash map
     * it seems better one, consider it again....
     * Re-design is not good, but follow Agile, we need to make it run first, then improve later,
     * so accept re-design risk
     */
    QStack<QWidget*> mViewStack;

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
     void on_actionCommunity_triggered();
     void on_actionSummarize_triggered();
     void on_actionSaints_2_triggered();
     void on_actionHome_triggered();
     void on_actionPerson_triggered();
     void on_actionSearch_triggered();
     void on_actionArea_triggered();
     void on_actionRole_triggered();
     void on_actionDept_triggered();
     void on_actionCourse_triggered();

     void on_action_ExportPersonList_triggered();
     void on_action_ExportCommunityList_triggered();

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

     void on_actionBack_triggered();


 private:
     static MainWindow* gInstance;
};
#endif // MAINWINDOW_H
