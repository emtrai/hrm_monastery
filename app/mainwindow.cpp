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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logger.h"

#include "loader/loaderctl.h"
#include "dlgcommunity.h"
#include "config.h"
#include "communityctl.h"
#include "dlgperson.h"
#include "filectl.h"
#include "view/widget/uitableviewfactory.h"
#include "utils.h"
#include <QToolButton>
#include <QFileDialog>
#include <QThread>

#include "view/dialog/dlgimportpersonlistresult.h"
#include "view/dialog/dlgimportcommunitylistresult.h"
#include "view/dialog/dlghtmlviewer.h"
#include "view/dialog/dlgeditmodel.h"
#include "personctl.h"
#include "dialog/dlgabout.h"
#include "dlgwait.h"
#include "dialog/dlgcommunity.h"
#include "view/widget/uitableviewfactory.h"
#include "modelcontroller.h"
#include "personctl.h"
#include "dlgimportexportselect.h"

#define ADD_MENU_ITEM(menu, func, name, iconPath) \
do { \
        QAction* act = new QAction(this);\
        act->setText(tr(name));\
        QIcon icon;\
        icon.addFile(QString::fromUtf8(iconPath), QSize(), QIcon::Normal, QIcon::On);\
        act->setIcon(icon);\
        QObject::connect(act, SIGNAL(triggered()), this, SLOT(func()));\
        menu->addAction(act);\
} while (0)



MainWindow* MainWindow::gInstance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSummarizeView(nullptr)
    , mCommunityView(nullptr)
    , mPersonView(nullptr)
    , mAreaView(nullptr)
    , mDepartView(nullptr)
    , mRoleView(nullptr)
    , mCurrentView(nullptr)
    , mWaitDlg(nullptr)
    , mAppState(APP_STATE_NOT_READY)
{

    Logger::init();
    mAppState = APP_STATE_INITING;
    gInstance = this;
    ui->setupUi(this);

    LOADERCTL->setOnFinishLoadListener(this, this);
    LOADERCTL->preLoad();
    // TODO: sync data?
    // TODO: register data sync account? (i.e sync/backup to google account

    // TODO: separted software into multiple parts/components???
    // App for main GUI
    // libraries: import, export, sqlite database, etc.
    // Purpose: easy to update software
    // TODO: verson control for each component???


    QCoreApplication::setApplicationName( QString(tr("Quản lý hội dòng")) );
    setWindowTitle( QCoreApplication::applicationName() );


    loadImportMenu();
    loadOtherMenu();
    loadExportMenu();
    loadOtherAddMenu();

    mSummarizeView = new UISummarizeView();

    mCommunityView = UITableViewFactory::getView(ViewType::VIEW_COMMUNITY);
    mSaintsView = UITableViewFactory::getView(ViewType::VIEW_SAINT);
    mPersonView = UITableViewFactory::getView(ViewType::VIEW_PERSON);
    mAreaView = UITableViewFactory::getView(ViewType::VIEW_AREA);
    mDepartView = UITableViewFactory::getView(ViewType::VIEW_DEPARTMENT);
    mRoleView = UITableViewFactory::getView(ViewType::VIEW_ROLE);
    mCourseView = UITableViewFactory::getView(ViewType::VIEW_COURSE);

    mHomeView = new QTextBrowser(this);
    mHomeView->clearHistory();
    mHomeView->clear();

    loadHomePageFile();
    // TODO: load all memory, consume much memory and performance
    mHomeView->setSearchPaths(QStringList(FileCtl::getOrCreatePrebuiltDataDir()));
    mHomeView->setHtml(Utils::readAll(
        FileCtl::getUpdatePrebuiltDataFilePath(KPrebuiltHomeHtmlFileName, false)));


    mMainViews.append((QWidget*)mSummarizeView);
    mMainViews.append((QWidget*)mSaintsView);
    mMainViews.append((QWidget*)mPersonView);
    mMainViews.append((QWidget*)mAreaView);
    mMainViews.append((QWidget*)mHomeView);
    mMainViews.append((QWidget*)mCommunityView);
    mMainViews.append((QWidget*)mDepartView);
    mMainViews.append((QWidget*)mRoleView);
    mMainViews.append((QWidget*)mCourseView);

    switchView(mHomeView);

    logd("init config");
    Config::init();

    logd("connect");
//    QObject::connect(this, SIGNAL(load()), loader, SLOT(onLoad()));
    QObject::connect(this, SIGNAL(load()), this, SLOT(onLoad()));
}


void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    logd("On load");

    logd("emit load, curr state %d", appState());
    if (appState() == APP_STATE_INITING)
        emit load();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showAddEditPerson(bool isSelfUpdate, Person *per, bool isNew)
{
    traced;
    getInstance()->doShowAddEditPerson(isSelfUpdate, per, isNew);
    tracede;
}

void MainWindow::showAddEditCommunity(bool isSelfUpdate, Community *com, CommonEditModelListener* listener)
{
    traced;
    getInstance()->doShowAddEditCommunity(isSelfUpdate, com, listener);
    tracede;
}

void MainWindow::showImportDlg(ImportTarget target)
{
    traced;
    logd("import target %d", target);
    switch (target) {
    case IMPORT_TARGET_PERSON:
        getInstance()->doShowImportPerson();
        break;
    case IMPORT_TARGET_COMMUNITY:
        getInstance()->doShowImportCommunity();
        break;
    }
    // TODO: return value to handle error case???
    // TODO: use function pointer instead?
    tracede;
}

void MainWindow::showOnHtmlViewer(DbModel *model, const QString& subject)
{
    QString fpath;
    ErrCode ret = model->exportToFile(ExportType::EXPORT_HTML, &fpath);
    if (QFile::exists(fpath)){
        dlgHtmlViewer* viewer = new dlgHtmlViewer();
        viewer->setHtmlPath(fpath);
        viewer->setSubject(subject);
        viewer->exec();
    }
}

void MainWindow::showAddEditCommonModel(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    traced;
    getInstance()->doShowAddEditCommonModel(isSelfUpdate, model, listener);
    tracede;

}

ErrCode MainWindow::exportListItems(const QList<DbModel *>* items,
                                    ModelController* controller,
                                    const QString& title, quint64 exportTypeList)
{
    traced;
    ErrCode err = ErrNone;
    err = getInstance()->doExportListItems(items, controller, title, exportTypeList);
    tracedr(err);
    return err;
}

void MainWindow::switchView(ViewType type)
{
    traced;
    QWidget *nextView = getView(type);
    if (nextView != nullptr) {
        switchView(nextView);
    }
    tracede;
}

void MainWindow::switchView(QWidget *nextView)
{
    traced;
    if (mCurrentView != nullptr) {
        logd("hide currentl widget");
        mCurrentView->hide();
        ui->centralLayout->replaceWidget(mCurrentView, nextView);
//        ui->centralwidget->layout()->replaceWidget(mCurrentView, nextView);
        // TODO: make this as stack????
        if (!mMainViews.contains(mCurrentView)) {
            logd("Not in cached view, remove");
//            delete mCurrentView;
            // TODO: delete here cause something terrible, i.e UICommDeptListView --> uidepartmentpersonlistview.cpp
            // UICommDeptListView is deleted, but its function is still called, because
            // uidepartmentpersonlistview is created in menu UICommDeptListView::onMenuActionListPerson
            // cause use-after-free issue
            // Need to rethink this again!!!!!
            mCurrentView = nullptr;
        } else {
            logd("In cache view, keep current, just hide");
        }
    }
    else{
//        ui->centralwidget->layout()->addWidget(nextView);
        ui->centralLayout->addWidget(nextView);

    }
    logd("show next");
    mCurrentView = nextView;
    mCurrentView->show();
    tracede;
}

QWidget *MainWindow::getView(ViewType type)
{
    traced;
    QWidget *nextView = nullptr;
    logd("type %d", type);
    switch (type) {
    case ViewType::VIEW_DEPARTMENT:
        nextView = mDepartView;
        break;
    case ViewType::VIEW_PERSON:
        nextView = mPersonView;
        break;
    default:
        {
            UITableView* view = UITableViewFactory::getView(type);
            if (view) {
                nextView = (QWidget*) view;
            } else {
                loge("Unknown type %d", type);
            }
        }
        break;
    }
    tracede;
    return nextView;
}

void MainWindow::onLoadController (Controller* ctl)
{
    traced;
    logd("on load ctrl %s", ctl->getName().toStdString().c_str());
    if (mWaitDlg != nullptr) {
        mWaitDlg->setMessage(ctl->getName());
    }
//    QThread::msleep(500);
    tracede;
}

void MainWindow::doShowAddEditPerson(bool isSelfUpdate, Person *per, bool isNew)
{
    traced;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgPerson* dlg = DlgPerson::buildDlg(this, per, isNew);
    dlg->setIsSelfSave(isSelfUpdate);
    dlg->exec();
    delete dlg;
    tracede;
}

void MainWindow::doShowAddEditCommunity(bool isSelfUpdate, Community *com, CommonEditModelListener* listener)
{
    traced;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCommunity* dlg = DlgCommunity::build(this, isSelfUpdate, (DbModel*)com);
    dlg->setListener(listener);
    dlg->exec();
    delete dlg;
    tracede;
}

void MainWindow::doShowImportPerson()
{
    traced;
    ErrCode err = ErrNone;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QList<DbModel*> list;
        logd("Import from file %s", fname.toStdString().c_str());
        ErrCode ret = INSTANCE(PersonCtl)->importFromFile(KModelHdlPerson, ImportType::IMPORT_CSV, fname, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
        dlg->setup(list);
        dlg->exec();
        delete dlg;
    }
    tracede;
}

void MainWindow::doShowImportCommunity()
{
    traced;
    ErrCode err = ErrNone;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QList<DbModel*> list;
        logd("Import from file %s", fname.toStdString().c_str());
        ErrCode ret = COMMUNITYCTL->importFromFile(KModelHdlCommunity, ImportType::IMPORT_CSV, fname, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        DlgImportCommunityListResult* dlg = new DlgImportCommunityListResult();
        dlg->setup(list);
        dlg->exec();
        delete dlg;
    }
    tracede;
}

void MainWindow::doShowAddEditCommonModel(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    traced;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgEditModel* dlg = DlgEditModel::build(this, isSelfUpdate, model);
    dlg->setListener(listener);
    dlg->exec();
    delete dlg;
    tracede;
}

ErrCode MainWindow::doExportListItems(const QList<DbModel *> *items, ModelController *controller,
                                      const QString& title, quint64 exportTypeList)
{
    traced;
    ErrCode err = ErrNone;
    QString fpath;
    ExportType type;
    DlgImportExportSelect* dlg = new DlgImportExportSelect(this);
    dlg->setImportExport(true, title);
    dlg->setExportTypes(exportTypeList);
    dlg->exec();
    fpath = dlg->path();
    type = dlg->selectedExportType();
    if (type && !fpath.isEmpty()) {
        // TODO: show dialog???
        // TODO: should get export type list basing on controller???
        err = controller->exportToFile(items, type, &fpath);
    } else {
        loge("Not selected type nor fpath");
    }
    delete dlg;
    tracedr(err);
    return err;
}



void MainWindow::loadHomePageFile()
{
    traced;
    ErrCode ret = ErrNone;
    ret = FileCtl::checkAndUpdatePrebuiltFile(KPrebuiltHomeHtmlFileName, true);
    if (ret == ErrNone)
        ret = FileCtl::checkAndUpdatePrebuiltFile(KPrebuiltLogoFileName, true);

    logd("Load home page file ret %d", ret);
    // TODO: should log to file???
}

#define ADD_ACTION_ITEM(menu, func, name, iconPath) \
    do { \
        QAction* act = nullptr;\
        act = menu->addAction(QIcon(QString::fromUtf8(iconPath)), \
                                   tr(name)); \
        QObject::connect(act, SIGNAL(triggered()), this, SLOT(func())); \
    } while (0)


void MainWindow::loadOtherAddMenu()
{
    traced;

    QToolButton *otherButton = new QToolButton(this);
    // TODO: when other button be clear? check it
    otherButton->setText(tr("&Khác"));
    otherButton->setPopupMode(QToolButton::InstantPopup);
    otherButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/icon/icons8-add-64"), QSize(), QIcon::Normal, QIcon::On);
    otherButton->setIcon(icon);

    QMenu *otherMenu = new QMenu(otherButton);

    otherButton->setMenu(otherMenu);
    // actionDummy? stupid? but it works
    ui->toolBar->insertWidget(ui->actionDummyAdd, otherButton);

    ADD_ACTION_ITEM(otherMenu,
                    on_actionNew_Community_triggered,
                    "Cộng đoàn",
                    ICON_PATH("icons8-community-64"));



}

void MainWindow::loadOtherMenu()
{
    traced;

    QToolButton *otherButton = new QToolButton(this);
    // TODO: when other button be clear? check it
    otherButton->setText(tr("&Khác"));
        otherButton->setPopupMode(QToolButton::InstantPopup);
    otherButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/icon/icons8-diversity-64"), QSize(), QIcon::Normal, QIcon::On);
    otherButton->setIcon(icon);

    QMenu *otherMenu = new QMenu(otherButton);

    otherButton->setMenu(otherMenu);
    // actionDummy? stupid? but it works
    ui->toolBar->insertWidget(ui->actionDummy, otherButton);

    ADD_ACTION_ITEM(otherMenu,
                    on_actionArea_triggered,
                    "Khu vực/Vùng",
                    ICON_PATH("icons8-earth-planet-80"));

    ADD_ACTION_ITEM(otherMenu,
                    on_actionSaints_2_triggered,
                    "Thánh",
                    ICON_PATH("icons8-saint-64"));


    ADD_ACTION_ITEM(otherMenu,
                    on_actionRole_triggered,
                    "Vị trí/vai trò",
                    ICON_PATH("icons8-unit-80"));


    ADD_ACTION_ITEM(otherMenu,
                    on_actionCountry_triggered,
                    "Quốc gia",
                    ICON_PATH("icons8-catholic-64"));


#ifndef SKIP_PERSON_PROVINE
    ADD_ACTION_ITEM(otherMenu,
                    on_actionProvince_triggered,
                    "Tỉnh/Thành phố/Bang",
                    ICON_PATH("icons8-catholic-64"));
#endif
    ADD_ACTION_ITEM(otherMenu,
                    on_actionMisson_triggered,
                    "Nhiệm vụ xã hội",
                    ICON_PATH("icons8-catholic-64"));


    ADD_ACTION_ITEM(otherMenu,
                    on_actionSpeclialist_triggered,
                    "Chuyên môn",
                    ICON_PATH("icons8-catholic-64"));

    ADD_ACTION_ITEM(otherMenu,
                    on_actionEducation_triggered,
                    "Giáo dục",
                    ICON_PATH("icons8-catholic-64"));

    ADD_ACTION_ITEM(otherMenu,
                    on_actionWork_triggered,
                    "Công việc",
                    ICON_PATH("icons8-catholic-64"));

    ADD_ACTION_ITEM(otherMenu,
                    on_actionEthnic_triggered,
                    "Dân tộc",
                    ICON_PATH("icons8-catholic-64"));

    ADD_ACTION_ITEM(otherMenu,
                    on_actionCourse_triggered,
                    "Khóa",
                    ICON_PATH("icons8-unit-80"));

    //    QAction* act = nullptr;

    //    act = otherMenu->addAction(QIcon(QString::fromUtf8(":/icon/icon/icons8-earth-planet-80")),
    //                               tr("Khu vực"));
    //    QObject::connect(act, SIGNAL(triggered()), this, SLOT(on_actionArea_triggered()));

    // TODO: when other menu clear? check it
    //    act = otherMenu->addAction(QIcon(QString::fromUtf8(":/icon/icon/icons8-saint-64")),
    //                                        tr("Thánh"));
    //                   QObject::connect(act, SIGNAL(triggered()), this, SLOT(on_actionSaints_2_triggered()));


    //    act = otherMenu->addAction(QIcon(QString::fromUtf8(":/icon/icon/icons8-unit-80.png")),
    //                               tr("Vị trí/vai trò"));
    //    QObject::connect(act, SIGNAL(triggered()), this, SLOT(on_actionRole_triggered()));
}

void MainWindow::loadImportMenu()
{
    traced;

    mImportButton = new QToolButton(this);
    // TODO: when import button be clear? check it
    mImportButton->setText(tr("&Nhập dữ liệu"));
    mImportButton->setPopupMode(QToolButton::InstantPopup);
    mImportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/icon/icons8-import-64"), QSize(), QIcon::Normal, QIcon::On);
    mImportButton->setIcon(icon);

    QMenu *importMenu = new QMenu(mImportButton);

    // TODO: when menu is clear???? check it careffully
    mImportButton->setMenu(importMenu);
    ui->toolBar->insertWidget(ui->action_New, mImportButton);

//    ADD_MENU_ITEM(importMenu,
//                  on_action_ImportPersonList_triggered,
//                  "Danh sách Nữ Tu",
//                  ":/icon/icon/icons8-nun-64.png");

    ADD_MENU_ITEM(importMenu,
                  on_action_ImportPerson_triggered,
                  "Nữ Tu",
                  ":/icon/icon/icons8-nun-64.png");

    ADD_MENU_ITEM(importMenu,
                  on_action_ImportCommunityList_triggered,
                  "Danh sách Cộng đoàn",
                  ":/icon/icon/icons8-community-64.png");

    ADD_MENU_ITEM(importMenu,
                  on_action_ImportCommunity_triggered,
                  "Cộng đoàn",
                  ":/icon/icon/icons8-community-64.png");

//    mActionImportPersonList = new QAction(this);
//    mActionImportPersonList->setObjectName(QString::fromUtf8("action_ImportPersonList"));
//    mActionImportPersonList->setText(tr("Danh sách Nữ tu"));
//    QIcon mImportPersonListIcon;
//    mImportPersonListIcon.addFile(QString::fromUtf8(":/icon/icon/icons8-nun-64.png"), QSize(), QIcon::Normal, QIcon::On);
//    mActionImportPersonList->setIcon(mImportPersonListIcon);
//    QObject::connect(mActionImportPersonList, SIGNAL(triggered()), this, SLOT(on_action_ImportPersonList_triggered()));
//    importMenu->addAction(mActionImportPersonList);

//    mActionImportPerson = new QAction(this);
//    mActionImportPerson->setObjectName(QString::fromUtf8("action_ImportPerson"));
//    mActionImportPerson->setText(tr("Nữ tu"));
//    mActionImportPerson->setIcon(mImportPersonListIcon);
////    QObject::connect(mActionImportPerson, SIGNAL(triggered()), this, SLOT(on_action_ImportPersonList_triggered()));
//    importMenu->addAction(mActionImportPerson);

//    mActionImportCommunityList = new QAction(this);
//    mActionImportCommunityList->setObjectName(QString::fromUtf8("action_ImportCommunityList"));
//    mActionImportCommunityList->setText(tr("Danh sách Cộng đoàn"));
//    QIcon mImportCommunityIcon;
//    mImportCommunityIcon.addFile(QString::fromUtf8(":/icon/icon/icons8-community-64 (1).png"), QSize(), QIcon::Normal, QIcon::On);
//    mActionImportCommunityList->setIcon(mImportCommunityIcon);
//    importMenu->addAction(mActionImportCommunityList);

    tracede;
}

void MainWindow::loadExportMenu()
{
    traced;

    mExportButton = new QToolButton(this);
    // TODO: when import button be clear? check it
    mExportButton->setText(tr("&Xuất dữ liệu"));
    mExportButton->setPopupMode(QToolButton::InstantPopup);
    mExportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/icon/icons8-export-100"), QSize(), QIcon::Normal, QIcon::On);
    mExportButton->setIcon(icon);

    QMenu *exportMenu = new QMenu(mExportButton);
    // TODO: when menu is clear???? check it careffully
    mExportButton->setMenu(exportMenu);
    ui->toolBar->insertWidget(ui->action_New, mExportButton);

    ADD_MENU_ITEM(exportMenu,
                  on_action_ExportPersonList_triggered,
                  "Danh sách Nữ tu",
                  ICON_PATH("icons8-nun-64.png"));
    ADD_MENU_ITEM(exportMenu,
                  on_action_ExportCommunityList_triggered,
                  "Danh sách cộng đoàn",
                  ICON_PATH("icons8-community-64.png"));


    tracede;

}

AppState MainWindow::appState() const
{
    return mAppState;
}

void MainWindow::setAppState(AppState newAppState)
{
    logd("set app state %d --> %d", mAppState, newAppState);
    mAppState = newAppState;
}

bool MainWindow::popViewFromStackAndShow()
{
    // TODO: implement his
    traced;
}

void MainWindow::pushViewToStack(ViewType type)
{
    // TODO: implement this
    // TODO: should limit max view, auto clean up old view
}

void MainWindow::onLoad()
{
    traced;
    mAppState = APP_STATE_LOADING;
    if (!mWaitDlg)
        mWaitDlg = new DlgWait(this);
    mWaitDlg->setMessage("Loadding");
    mWaitDlg->setAllowCancel(false);
    mWaitDlg->show(nullptr,
          nullptr,
          [this](ErrCode* err, void* data, DlgWait* dlg) {
            logd("Call loader on load");
            LoaderCtl::getInstance()->onLoad();
            return nullptr;
          },
        [this](ErrCode err, void* data, void* result, DlgWait* dlg) {
            logd("Finished, close wait dlg");
            dlg->forceClose();
            this->setAppState(APP_STATE_READY);
            return ErrNone;
        }
        );
//    LoaderCtl::getInstance()->onLoad();
//    this->setAppState(APP_STATE_READY);
    tracede;
}

void MainWindow::on_action_ImportPerson_triggered()
{
    traced;
    doShowImportPerson();
    tracede;
}

void MainWindow::on_action_ImportPersonList_triggered()
{
    traced;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QList<DbModel*> list;
        logd("Import from file %s", fname.toStdString().c_str());
        ErrCode ret = INSTANCE(PersonCtl)->importFromFile(KModelHdlPerson, ImportType::IMPORT_CSV_LIST, fname, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
        dlg->setup(list);
        dlg->exec();
        delete dlg;
    }

}

void MainWindow::on_action_ImportCommunityList_triggered()
{
    traced;
    UNDER_DEV(tr("Nhập danh sách Cộng Đoàn từ tập tin"));
    // TODO: implement it
}

void MainWindow::on_action_ImportCommunity_triggered()
{
    traced;
//    UNDER_DEV(tr("Nhập Cộng Đoàn từ tập tin"));
    MainWindow::showImportDlg(ImportTarget::IMPORT_TARGET_COMMUNITY);
    // TODO: handle error case??

}

void MainWindow::on_action_New_triggered()
{
    traced;
//    Person* person = TestCtl::genRandomPerson();
//    person->save();

//    delete person;
    DlgPerson *dlg = DlgPerson::buildDlg(this);
    //    w.setWindowState(Qt::WindowState::WindowMaximized);
    dlg->exec();
    delete dlg;
}


void MainWindow::on_actionNew_Community_triggered()
{
    traced;
//    Community* comm = TestCtl::genRandomCommunity();
//    comm->save();

//    delete comm;
    DlgCommunity w;
//    w.setWindowState(Qt::WindowState::WindowMaximized);
    w.exec();
}


void MainWindow::on_actionCommunity_triggered()
{
    traced;
    switchView(mCommunityView);
    tracede;
}


void MainWindow::on_actionSummarize_triggered()
{
    traced;
    switchView(mSummarizeView);
    tracede;
}


void MainWindow::on_actionSaints_2_triggered()
{
    traced;
    switchView(mSaintsView);
    tracede;

}


void MainWindow::on_actionHome_triggered()
{
    traced;
    switchView(mHomeView);
    tracede;
}


void MainWindow::on_actionPerson_triggered()
{
    traced;
    switchView(mPersonView);
    tracede;
}


void MainWindow::on_actionSearch_triggered()
{
    traced;
    UNDER_DEV(tr("Tìm kiếm thông tin"));
}

void MainWindow::on_actionArea_triggered()
{
    traced;
    switchView(mAreaView);
    tracede;
}


void MainWindow::on_actionRole_triggered()
{
    traced;
    switchView(mRoleView);
    tracede;
}

void MainWindow::on_actionCourse_triggered()
{
    traced;
    switchView(mCourseView);
    tracede;
}


MainWindow *MainWindow::getInstance()
{
    return gInstance;
}


void MainWindow::on_actionDept_triggered()
{
    traced;
    switchView(mDepartView);
    tracede;

}

void MainWindow::on_action_ExportPersonList_triggered()
{
    traced;
    QList<DbModel*> list;
//    UNDER_DEV(tr("Xuất danh sách nữ tu ra tập tin"));
    doExportListItems(&list, PERSONCTL, "Danh sách nữ tu", ExportType::EXPORT_XLSX | ExportType::EXPORT_CSV_LIST);
}

void MainWindow::on_action_ExportCommunityList_triggered()
{
    traced;
    QList<DbModel*> list = COMMUNITYCTL->getAllItemsFromDb();
//    UNDER_DEV(tr("Xuất danh sách nữ tu ra tập tin"));
    doExportListItems(&list, COMMUNITYCTL, "Danh sách cộng đoàn", ExportType::EXPORT_XLSX);
}


void MainWindow::on_action_About_triggered()
{
    DlgAbout* dlg = new DlgAbout(this);
    dlg->exec();
    delete dlg;
}


void MainWindow::on_action_Backup_triggered()
{
    traced;
    UNDER_DEV(tr("Sao lưu dữ liệu"));
    // TODO: implement it

}


void MainWindow::on_actionRestore_triggered()
{
    traced;
    UNDER_DEV(tr("Phục hồi dữ liệu đã sao lưu"));
    // TODO: implement it

}


void MainWindow::on_actionRevert_triggered()
{
    traced;
    UNDER_DEV(tr("Khôi phục dữ liệu (khôi phục dữ liệu cũ, trong trường hợp dữ liệu hiện tại bị lỗi)"));
    // TODO: implement it

}


void MainWindow::on_action_Help_triggered()
{
    traced;
    UNDER_DEV(tr("Hướng dẫn sử dụng phần mềm"));
    // TODO: implement it

}

void MainWindow::on_actionEthnic_triggered()
{
    traced;
//    UNDER_DEV(tr("Dân tộc"));
    // TODO: implement it
    switchView(VIEW_ETHNIC);
}

void MainWindow::on_actionWork_triggered()
{

    traced;
    switchView(VIEW_WORK);
    // TODO: implement it

}

void MainWindow::on_actionEducation_triggered()
{

    traced;
//    UNDER_DEV(tr("Giáo dục"));
    // TODO: implement it
    switchView(VIEW_EDUCATION);
}

void MainWindow::on_actionSpeclialist_triggered()
{
    traced;
//    UNDER_DEV(tr("Chuyên môn"));

    switchView(VIEW_SPECIALIST);
}

void MainWindow::on_actionMisson_triggered()
{
    traced;
//    UNDER_DEV(tr("Nhiệm vụ"));
    // TODO: implement it
    switchView(VIEW_MISSION);
    tracede;
}

void MainWindow::on_actionProvince_triggered()
{
    traced;
    UNDER_DEV(tr("Tỉnh/Thành phố/Bang"));
    // TODO: implement it
}

void MainWindow::on_actionCountry_triggered()
{
    traced;
//    UNDER_DEV(tr("Quốc gia"));
    // TODO: implement it

    switchView(VIEW_COUNTRY);
}


void MainWindow::on_actionBack_triggered()
{
    traced;
    // TODO: implemen this
    // TODO: how about forward/redo???? show we support it?
//    popViewFromStackAndShow();
    UNDER_DEV(tr("Quay lại màn hình trước"));
}

