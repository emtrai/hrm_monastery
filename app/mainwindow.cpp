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
#include <QDesktopServices>

#include "view/dialog/dlgimportpersonlistresult.h"
#include "view/dialog/dlghtmlviewer.h"
#include "view/dialog/dlgeditmodel.h"
#include "view/dialog/dlgcourse.h"
#include "view/dialog/dlgcommdept.h"
#include "view/dialog/dlgarea.h"
#include "view/dialog/dlgethnic.h"
#include "view/dialog/dlgaddpersonevent.h"
#include "personctl.h"
#include "dialog/dlgabout.h"
#include "dlgwait.h"
#include "dialog/dlgcommunity.h"
#include "view/widget/uitableviewfactory.h"
#include "modelcontroller.h"
#include "personctl.h"
#include "dlgimportexportselect.h"
#include "backup/backupctl.h"
#include "prebuiltdefs.h"
#include "dialogutils.h"
#include "dlgimportlistresultfactory.h"
#include "stringdefs.h"
#include "imagedefs.h"
#include "statistic/statistic.h"
#include "dbctl.h"
#include "modeldefs.h"

#define ADD_MENU_ITEM(menu, func, name, iconPath) \
do { \
        QAction* act = new QAction(this);\
        act->setText(name);\
        QIcon icon;\
        icon.addFile(QString::fromUtf8(iconPath), QSize(), QIcon::Normal, QIcon::On);\
        act->setIcon(icon);\
        QObject::connect(act, SIGNAL(triggered()), this, SLOT(func()));\
        menu->addAction(act);\
} while (0)


#define ADD_ACTION_ITEM(menu, func, name, iconPath) \
do { \
        QAction* act = nullptr;\
        act = menu->addAction(QIcon(QString::fromUtf8(iconPath)), \
                          name); \
        QObject::connect(act, SIGNAL(triggered()), this, SLOT(func())); \
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
    , isFirstInit(false)
{
    tracein;
    // currently, judge app is first inited or not by checking if db file is existed.
    // this is not really good solution, as app may be stopped while initializing, but
    // still be marked as initialized later on.
    // TODO: any more other checking method to judge if app first inited?
    // i.e. create file like firstinit to mark initalization stage (store init stage)?
    isFirstInit = !DB->isDbExist();
    QCoreApplication::setApplicationName(STR_APP_NAME);
    setWindowTitle(STR_APP_TITLE);

    // init order is important, take care of this

    // init log to get log as soon as possible
    Logger::init();

    setAppState(APP_STATE_INITING);
    gInstance = this;
    ui->setupUi(this);
    if (isFirstInit) {
        logi("First init time");
    }
    dbg(LOG_VERBOSE, "init config");
    Config::initConfig();

    LOADERCTL->setOnFinishLoadListener(this, this);
    LOADERCTL->preLoad();
    // TODO: sync data?
    // TODO: register data sync account? (i.e sync/backup to google account

    // TODO: separted software into multiple parts/components???
    // App for main GUI
    // libraries: import, export, sqlite database, etc.
    // Purpose: easy to update software
    // TODO: verson control for each component???

    dbg(LOG_DEBUG, "init menu");
    loadImportMenu();
    loadOtherMenu();
    loadExportMenu();
    loadOtherAddMenu();


    dbg(LOG_DEBUG, "init views");
    mCommunityView = UITableViewFactory::getView(ViewType::VIEW_COMMUNITY);
    mSaintsView = UITableViewFactory::getView(ViewType::VIEW_SAINT);
    mPersonView = UITableViewFactory::getView(ViewType::VIEW_PERSON);
    mAreaView = UITableViewFactory::getView(ViewType::VIEW_AREA);
    mDepartView = UITableViewFactory::getView(ViewType::VIEW_DEPARTMENT);
    mRoleView = UITableViewFactory::getView(ViewType::VIEW_ROLE);
    mCourseView = UITableViewFactory::getView(ViewType::VIEW_COURSE);
    mPersonStatusView = UITableViewFactory::getView(ViewType::VIEW_PERSON_STATUS);
//    mSummarizeView = UITableViewFactory::getView(ViewType::VIEW_SUMMARY);


    dbg(LOG_DEBUG, "setup home view");
    mHomeView = new UITextBrowser(this);
    mHomeView->clearHistory();
    mHomeView->clear();

    loadHomePageFile();
    // TODO: load all memory, consume much memory and performance
    mHomeView->setSearchPaths(QStringList(FileCtl::getOrCreatePrebuiltDataDir()));

    // get home.html file from install dir first, if not exist, load from prebuilt folder
    // which is copied from app's resource
    QString homePath;
    homePath = FileCtl::getAppInstallDir(KPrebuiltHomeHtmlFileName);
    logd("install dir homePath '%s'", STR2CHA(homePath));
    if (homePath.isEmpty() || !QFileInfo::exists(homePath)) {
        logd("homePath '%s' not exist", STR2CHA(homePath));
        homePath = FileCtl::getUpdatePrebuiltDataFilePath(KPrebuiltHomeHtmlFileName, false);
    }
    dbg(LOG_VERBOSE, "homePath '%s'", STR2CHA(homePath));
    mHomeView->setHtml(Utils::readAll(homePath));


    mViewList.insert(VIEW_SUMMARY, (BaseView*)mSummarizeView);
    mViewList.insert(VIEW_SAINT, (BaseView*)mSaintsView);
    mViewList.insert(VIEW_PERSON, (BaseView*)mPersonView);
    mViewList.insert(VIEW_AREA, (BaseView*)mAreaView);
    mViewList.insert(VIEW_TEXT_BROWSER, (BaseView*)mHomeView);
    mViewList.insert(VIEW_COMMUNITY, (BaseView*)mCommunityView);
    mViewList.insert(VIEW_DEPARTMENT, (BaseView*)mDepartView);
    mViewList.insert(VIEW_ROLE, (BaseView*)mRoleView);
    mViewList.insert(VIEW_COURSE, (BaseView*)mCourseView);
//    mViewList.insert(VIEW_PERSON_STATUS, (BaseView*)mPersonStatusView);

    switchView(mHomeView);


    dbg(LOG_DEBUG, "setup signal/slot connect");
    if (!QObject::connect(this, &MainWindow::load, this, &MainWindow::onLoad, Qt::QueuedConnection)) {
        loge("Failed to connect load to onLoad");
    }
    if (!QObject::connect(this, &MainWindow::unload, this, &MainWindow::onUnload, Qt::QueuedConnection)){
        loge("Failed to connect unload to onUnload");
    }
    if (!QObject::connect(this,
                          &MainWindow::showMsgDlgSignal,
                          this,
                          &MainWindow::onShowMsgDlg, Qt::QueuedConnection)) {
        loge("Failed to connect showMsgDlgSignal to onShowMsgDlg");
    }
    if (!QObject::connect(this,
                          &MainWindow::showErrorDlgSignal,
                          this,
                          &MainWindow::onShowErrorMsgDlg, Qt::QueuedConnection)) {
        loge("Failed to connect showErrorDlgSignal to onShowErrorMsgDlg");
    }

    if (!QObject::connect(this, &MainWindow::importPeople,
                          this, &MainWindow::onImportPeople, Qt::QueuedConnection)){
        loge("Failed to connect importPeople to onImportPeople");
    }
    setAppState(APP_STATE_INITED);
    dbg(LOG_VERBOSE, "init app done");
    traceout;
}


void MainWindow::showEvent(QShowEvent *ev)
{
    tracein;
    QMainWindow::showEvent(ev);
    dbg(LOG_VERBOSE, "On show window, start loading at app state '%d'", appState());

    logd("emit load, curr state %d", appState());
    if (appState() == APP_STATE_INITED) {
        emit load();
    } else {
        loge("window not finish initialize yet!, current state '%d'", appState());
    }
    traceout;
}

MainWindow::~MainWindow()
{
    // TODO: handle anything when window is freed?
    logi("on deleted window");
    delete ui;
}


void MainWindow::showAddEditPerson(bool isSelfUpdate, Person *per, bool isNew)
{
    tracein;
    getInstance()->doShowAddEditPerson(isSelfUpdate, per, isNew);
    traceout;
}

void MainWindow::showAddEditCommunity(bool isSelfUpdate, const Community *com, CommonEditModelListener* listener)
{
    tracein;
    getInstance()->doShowAddEditCommunity(isSelfUpdate, com, listener);
    traceout;
}

ErrCode MainWindow::showImportDlg(ImportTarget target,
                                  ModelController* controller,
                                  const QString& modelName,
                                  const DbModel* targetModel)
{
    tracein;
    ErrCode err = ErrNone;
    dbg(LOG_DEBUG, "show import target %d", target);
    switch (target) {
    case IMPORT_TARGET_PERSON:
        err = getInstance()->doShowImportPerson();
        break;
    case IMPORT_TARGET_COMMUNITY:
        err = getInstance()->doShowImportCommunity();
        break;
    default:
        logw("import target %d not support, use commone one", target);
        err = getInstance()->doShowCommonImport(target, controller, modelName, targetModel);
        break;
    }
    logife(err, "Show import dialog failed");
    // TODO: return value to handle error case???
    // TODO: use function pointer instead?
    traceret(err);
    return err;
}

ErrCode MainWindow::showOnHtmlViewer(const DbModel *model, const QString& subject)
{
    tracein;
    QString fpath;
    ErrCode ret = ErrNone;
    dlgHtmlViewer* viewer = nullptr;
    if (!model) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }

    if (ret == ErrNone) {
        dbg(LOG_DEBUG, "export temporary to html file");
        ret = model->exportToFile(ExportType::EXPORT_HTML, &fpath);
        dbg(LOG_INFO, "exported file '%s'", STR2CHA(fpath));
    }

    if (ret == ErrNone && !QFile::exists(fpath)){
        loge("Export html file %s not exist", STR2CHA(fpath));
        ret = ErrNotExist;
    }

    if (ret == ErrNone && ((viewer = new dlgHtmlViewer()) == nullptr)) {
        loge("faie to allocate viewer dialog");
        ret = ErrNoMemory;
    }
    if (ret == ErrNone) {
        dbg(LOG_INFO, "show html viewer");
        viewer->setHtmlPath(fpath);
        viewer->setSubject(subject);
        viewer->exec();
    }
    FREE_PTR(viewer);
    traceret(ret);
    return ret;
}

void MainWindow::showAddEditCommonModel(bool isSelfUpdate, const DbModel *model,
                                        CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCommonModel(isSelfUpdate, model, listener);
    traceout;
}

void MainWindow::showAddEditCourse(bool isSelfUpdate, const DbModel *com, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCourse(isSelfUpdate, com, listener);
    traceout;

}

void MainWindow::showAddEditCommDept(bool isSelfUpdate, const DbModel* comm,
                                     const DbModel *dept, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCommDept(isSelfUpdate, comm, dept, listener);
    traceout;

}

void MainWindow::showAddEditArea(bool isSelfUpdate, const DbModel *com, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditArea(isSelfUpdate, com, listener);
    traceout;
}

void MainWindow::showAddEditEthnic(bool isSelfUpdate, const DbModel *com, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditEthnic(isSelfUpdate, com, listener);
    traceout;

}

ErrCode MainWindow::exportListItems(const QList<DbModel *>* items,
                                    const QString& datatype,
                                    ModelController* controller,
                                    const QString& title, quint64 exportTypeList,
                                    QString* fpath)
{
    tracein;
    ErrCode err = ErrNone;
    err = getInstance()->doExportListItems(items, datatype, controller, title, exportTypeList, fpath);
    traceret(err);
    return err;
}

ErrCode MainWindow::showProcessingDialog(const QString& title,
                                         WaitPrepare_t prepare,
                                         WaitRunt_t run,
                                         WaitFinished_t finish,
                                         void *data)
{
    return getInstance()->doShowProcessingDialog(title, prepare, run, finish, data);
}

void MainWindow::addMainWindownImportListener(MainWindownImportListener *listener)
{
    return getInstance()->doAddMainWindownImportListener(listener);
}

void MainWindow::removeMainWindownImportListener(MainWindownImportListener *listener)
{
    return getInstance()->doRemoveMainWindownImportListener(listener);
}

void MainWindow::showMessageBox(const QString &msg)
{
    emit showMsgDlgSignal(msg);
}

void MainWindow::showErrorBox(const QString &msg, ErrCode err)
{
    logd("emit error dlg with err=%d", err);
    emit showErrorDlgSignal(err, msg);
}

void MainWindow::showIfErrorBox(const QString &msg, ErrCode err)
{
    tracein;
    if(err != ErrNone) {
        showErrorBox(msg, err);
    } else {
        logd("not error, skip");
    }
    traceout;
}

ErrCode MainWindow::doShowProcessingDialog(const QString& title,
                               WaitPrepare_t prepare,
                               WaitRunt_t run,
                               WaitFinished_t finish,
                               void *data)
{
    tracein;
    ErrCode err = ErrNone;
    DlgWait waitdlg(this);
    waitdlg.setMessage(title);
    waitdlg.setAllowCancel(false);
    err = waitdlg.show(data, prepare, run, finish);
    logife(err, "show waiting dialg failed");
    traceout;
    return err;
}

ErrCode MainWindow::doShowAddPersonEvent(bool isSelfUpdate, DbModel* person,
                                         DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    ErrCode err = ErrNone;
    dbg(LOG_VERBOSE, "show add person event for model '%s'", MODELSTR2CHA(model));
    logd("isSelfUpdate %d", isSelfUpdate);
    logd("Set person '%s'", MODELSTR2CHA(person));
    logd("Set model '%s'", MODELSTR2CHA(model));
    DlgAddPersonEvent* dlg = DlgAddPersonEvent::build(this, isSelfUpdate,
                                                      KModelNamePersonEvent, model, listener);
    if (!dlg) {
        err = ErrNoMemory;
        loge("not memory to create add person event dialg");
    }
    if (err == ErrNone && person) {
        dlg->setPerson((Person*)person);
    }
    if (err == ErrNone) {
        dlg->exec();
    }
    logife(err, "failed to show add person event dlg");
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

void MainWindow::switchView(ViewType type, void* data)
{
    tracein;
    dbg(LOG_DEBUG, "swith to view '%d'", type);
    BaseView *nextView = getView(type);
    if (nextView != nullptr) {
        nextView->setData(data);
        switchView(nextView);
    } else {
        loge("nextview is null, view type '%d' seem not found?", type);
    }
    traceout;
}

void MainWindow::switchView(BaseView *nextView, bool fromStack)
{
    tracein;
    dbg(LOG_DEBUG, "get view '%d' fromStack=%d",
        nextView?nextView->getViewType():-1, fromStack);
    if (nextView && mCurrentView != nextView) {
        if (mCurrentView && !fromStack) {
            dbg(LOG_DEBUG, "push curent view '%d' to stack",
                mCurrentView->getViewType());
            pushViewToStack(mCurrentView);
        }
        if (mCurrentView) {
            logd("hide currentl widget");
            mCurrentView->getWidget()->hide();
            mCurrentView->onPaused();
            ui->centralLayout->replaceWidget(mCurrentView->getWidget(), nextView->getWidget());
        }
        else{
            ui->centralLayout->addWidget(nextView->getWidget());
        }
        logd("show next");
        mCurrentView = nextView;
        mCurrentView->onShown();
        mCurrentView->getWidget()->show();
    } else {
        logw("%s!", nextView?"Same view":"Next view is NULL");
    }
    traceout;
}

BaseView *MainWindow::getView(ViewType type)
{
    tracein;
    BaseView *nextView = nullptr;
    logd("getView type %d", type);
    if (mViewList.contains(type)) {
        logd("view existed");
        nextView = mViewList.value(type);
    } else {
        logd("view not exist, create new one");
        nextView = UITableViewFactory::getView(type);
        if (nextView) {
            logi("Add view %d to view list as cache", type);
            mViewList.insert(type, nextView);
        } else {
            loge("invalid type '%d' or no memory", type);
        }
    }
    traceout;
    return nextView;
}

void MainWindow::onLoadController (Controller* ctl)
{
    tracein;
    if (ctl) {
        dbgd("on load ctrl %s", STR2CHA(ctl->getName()));
        if (mWaitDlg != nullptr) {
            mWaitDlg->setMessage(QString(STR_INIT_COMPONENT).arg(ctl->getDisplayName()));
        }
    } else {
        loge("invalid controller");
    }
    traceout;
}

void MainWindow::onUnloadController(Controller *ctl)
{
    tracein;
    UNUSED(ctl);
    // TODO: unload controller???
    traceout;
}

void MainWindow::doShowAddEditPerson(bool isSelfUpdate, Person *per, bool isNew)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgPerson* dlg = DlgPerson::buildDlg(this, per, isNew);
    if (dlg) {
        dlg->setIsSelfSave(isSelfUpdate);
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory to build edit person dlg?");
    }
    traceout;
}

void MainWindow::doShowAddEditCommunity(bool isSelfUpdate, const Community *com,
                                        CommonEditModelListener* listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCommunity* dlg = DlgCommunity::build(this, isSelfUpdate, KModelNameCommunity,
                                            (DbModel*)com, listener);
    if (dlg) {
        dlg->setListener(listener);
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory to build DlgCommunity?");
    }
    traceout;
}

ErrCode MainWindow::doShowImportPerson()
{
    tracein;
    ErrCode ret = ErrNone;
    dbg(LOG_VERBOSE, "show import person");
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
                                        this,
                                        STR_SELECT_TO_IMPORT,
                                        FileCtl::getAppDataDir(),
                                        tr("Excel (*.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        ret = showProcessingDialog(STR_IMPORT_DATA, nullptr,
            [this, fname](ErrCode* err, void* data, DlgWait* dlg) {
                UNUSED(data);
                UNUSED(dlg);
                ErrCode tmperr = ErrNone;
                QList<DbModel*> *list = new QList<DbModel*>();
                dbg(LOG_INFO, "Import from file %s", STR2CHA(fname));
                this->notifyMainWindownImportListenerStart(IMPORT_TARGET_PERSON);
                if (!list) {
                    loge("no memory to allocat list mode");
                    tmperr = ErrNoMemory;
                }
                if (tmperr == ErrNone) {
                    ImportType type = ImportFactory::importTypeFromExt(fname, true);
                    if (type == IMPORT_XLSX || type == IMPORT_CSV_LIST || type == IMPORT_CSV) {
                        tmperr = PERSONCTL->importFromFile(KModelNamePerson, type, fname, list);
                    } else {
                        tmperr = ErrNotSupport;
                        loge("Import type %d not support (fname = '%s'", type, STR2CHA(fname));
                    }
                }
                logife(tmperr, "import person failed");
                if (tmperr != ErrNone) {
                    delete list;
                    list = nullptr;
                }
                if (err) *err = tmperr;
                return list;
                },
            [this, fname](ErrCode err, void* data, void* result, DlgWait* dlg) {
                UNUSED(data);
                UNUSED(dlg);
                tracein;
                logd("Import result %d", err);
                if (err == ErrNone && result) {
                    QList<DbModel*>* list = static_cast<QList<DbModel*>*>(result);
                    logd("No of import item %lld", list->count());
                    emit this->importPeople(err, list);
                }
                traceout;
                return err;
            });
    } else {
        loge("no file selected to import person?");
    }
    traceret(ret);
    return ret;
}

ErrCode MainWindow::doShowImportCommunity()
{
    tracein;
    ErrCode err = ErrNone;
    dbg(LOG_VERBOSE, "show import community");
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
                                    this,
                                    STR_SELECT_TO_IMPORT,
                                    FileCtl::getAppDataDir(),
                                    tr("Excel (*.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        QList<DbModel*> list;
        dbg(LOG_VERBOSE, "Import community from file %s", STR2CHA(fname));
        notifyMainWindownImportListenerStart(IMPORT_TARGET_COMMUNITY);
        ErrCode ret = COMMUNITYCTL->importFromFile(KModelNameCommunity,
                                                   ImportType::IMPORT_XLSX,
                                                   fname, &list);
        dbg(LOG_DEBUG, "Import result %d, number item '%lld'",
            ret, list.count());
        DlgImportListResult* dlg = nullptr;
        err = DlgImportListResultFactory::getImportListResult(
                            IMPORT_TARGET_COMMUNITY, &dlg, this);
        if (dlg) {
            dlg->setup(list);
            dlg->exec();
            delete dlg;
        } else {
            err = ErrNoMemory;
            loge("no memory to craete result dialog");
        }
        notifyMainWindownImportListenerEnd(IMPORT_TARGET_COMMUNITY, err, nullptr);
        RELEASE_LIST_DBMODEL(list);
    } else {
        logw("no file selected to import community?");
    }
    traceret(err);
    return err;
}

ErrCode MainWindow::doShowCommonImport(ImportTarget target,
                                       ModelController *controller,
                                       const QString &modelName,
                                       const DbModel* targetModel)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> list;
    DlgImportListResult* dlg = nullptr;
    dbg(LOG_VERBOSE, "import for target %d", target);
    logd("model name '%s'", STR2CHA(modelName));
    // TODO: show dialog to select which type of file to be imported???
    QString fname;

    if (!controller) {
        loge("no controller");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        fname = QFileDialog::getOpenFileName(this,
                                            STR_SELECT_TO_IMPORT,
                                            FileCtl::getAppDataDir(),
                                            tr("Excel (*.xlsx)"));
        if (fname.isEmpty()) {
            loge("no file name selected");
            err = ErrNoFile;
        }
    }
    if (err == ErrNone) {
        dbg(LOG_VERBOSE, "Import from file %s", STR2CHA(fname));
        err = controller->importFromFile(modelName,
                                        ImportType::IMPORT_XLSX, fname, &list);
        logd("Import result %d", err);
    }
    if (err == ErrNone && list.size() == 0) {
        loge("no data is imported");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        logd("No of import item %lld", list.size());
        err = DlgImportListResultFactory::getImportListResult(target, &dlg, this);
    }
    if (err == ErrNone && !dlg) {
        loge("cannot create dialog obj, no memory?");
        err = ErrNoMemory;
    }
    if (err == ErrNone && targetModel) {
        logd("set target model '%s'", MODELSTR2CHA(targetModel));
        dlg->setTargetModel(targetModel);
    }
    if (err == ErrNone) {
        logd("Set up list, no. item %lld", list.size());
        err = dlg->setup(list);
    }
    if (err == ErrNone) {
        dlg->exec();
    }
    RELEASE_LIST_DBMODEL(list);
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

void MainWindow::doShowAddEditCommonModel(bool isSelfUpdate, const DbModel *model,
                                          CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d, model '%s' listener '%s'",
            isSelfUpdate,
             model?STR2CHA(model->toString()):"null",
             listener?STR2CHA(listener->getName()):"null");
    DlgEditModel* dlg = DlgEditModel::build(this, isSelfUpdate, nullptr, model, listener);
    if (dlg) {
        dlg->setListener(listener);
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory to create DlgEditModel");
    }
    traceout;
}

void MainWindow::doShowAddEditCourse(bool isSelfUpdate, const DbModel *model,
                                     CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCourse* dlg = DlgCourse::build(this, isSelfUpdate, nullptr, model, listener);
    if (dlg) {
        dlg->setListener(listener);
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory to create DlgCourse");
    }
    traceout;

}

void MainWindow::doShowAddEditCommDept(bool isSelfUpdate, const DbModel* comm,
                                       const DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCommDept* dlg = DlgCommDept::build(this, isSelfUpdate, KModelNameCommDept, model, listener);
    if (dlg) {
        dlg->setCommunity(comm);
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory to create DlgCommDept");
    }
    traceout;
}

void MainWindow::doShowAddEditArea(bool isSelfUpdate, const DbModel *model,
                                   CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgArea* dlg = DlgArea::build(this, isSelfUpdate, KModelNameArea, model, listener);
    if (dlg) {
        dlg->exec();
        delete dlg;
    } else {
        loge("failed to build dlgarea dialog, no memory?");
    }
    traceout;
}

void MainWindow::doShowAddEditEthnic(bool isSelfUpdate, const DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgEthnic* dlg = DlgEthnic::build(this, isSelfUpdate, KModelNameEthnic, model, listener);
    if (dlg) {
        dlg->exec();
        delete dlg;
    } else {
        loge("failed to build DlgEthnic dialog, no memory?");
    }
    traceout;
}

ErrCode MainWindow::doExportListItems(const QList<DbModel *> *items,
                                      const QString& datatype,
                                      ModelController *controller,
                                      const QString& title,
                                      quint64 exportTypeList,
                                      QString* outFpath)
{
    tracein;
    ErrCode err = ErrNone;
    QString fpath;
    ExportType type;
    DlgImportExportSelect* dlg = nullptr;
    if (err == ErrNone && !items) {
        loge("invalid list item");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && !controller) {
        loge("invalid lis controller");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        dlg = new DlgImportExportSelect(this);
        if (!dlg) {
            loge("fail to allocate DlgImportExportSelect");
            err = ErrNoMemory;
        }
    }
    dbg(LOG_DEBUG, "datatype '%s', exportTypeList %lld controller '%s'",
         STR2CHA(datatype), exportTypeList,
         controller?STR2CHA(controller->getName()):"null");
    if (err == ErrNone) {
        dlg->setTitle(title);
        dlg->setImportExport(true, title);
        dlg->setExportTypes(exportTypeList);
        dlg->exec();
        fpath = dlg->path();
        type = dlg->selectedExportType();
        if (type && !fpath.isEmpty()) {
            // TODO: show dialog???
            // TODO: should get export type list basing on controller???
            err = controller->exportToFile(items, datatype, type, &fpath);
        } else {
            loge("Not selected type nor fpath");
            err = ErrNotSelect;
        }
    }
    if (err == ErrNone && outFpath) {
        *outFpath = fpath;
    }
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

void MainWindow::loadHomePageFile()
{
    tracein;
    ErrCode ret = ErrNone;
    ret = FileCtl::checkAndUpdatePrebuiltFile(KPrebuiltHomeHtmlFileName, true);
    if (ret == ErrNone) {
        ret = FileCtl::checkAndUpdatePrebuiltFile(KPrebuiltLogoFileName, true);
    }
    logife(ret, "load home page file failed");
    // TODO: should log to file???
    traceout;
}

void MainWindow::loadOtherAddMenu()
{
    tracein;
    ErrCode err = ErrNone;
    QToolButton *otherButton = new QToolButton(this);
    QMenu *otherMenu = nullptr;
    // TODO: when those are freed?

    if (!otherButton) {
        err = ErrNoMemory;
        loge("no memory to create otherButton");
    }
    if (err == ErrNone) {
        // TODO: when other button be clear? check it
        otherButton->setText(tr("&Khác"));
        otherButton->setPopupMode(QToolButton::InstantPopup);
        otherButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QIcon icon;
        icon.addFile(ICON_ADD, QSize(), QIcon::Normal, QIcon::On);
        otherButton->setIcon(icon);
    }

    if (err == ErrNone) {
        otherMenu = new QMenu(otherButton);
        if (!otherMenu) {
            err = ErrNoMemory;
            loge("no memory to create otherMenu");
        }

    }

    if (err == ErrNone) {
        otherButton->setMenu(otherMenu);
        // actionDummy? stupid? but it works
        ui->toolBar->insertWidget(ui->actionDummyAdd, otherButton);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionNew_Community_triggered,
                        STR_ADD_COMMUNITY,
                        ICON_ADD_COMMUNITY);


        ADD_ACTION_ITEM(otherMenu,
                        on_actionNew_PersonEvent_triggered,
                        STR_ADD_PERSON_EVENT,
                        ICON_ADD_PERSON_EVENT);
    }
    logife(err, "failed to prepare other add memu");
    traceout;
}

void MainWindow::loadOtherMenu()
{
    tracein;
    ErrCode err = ErrNone;
    QToolButton *otherButton = new QToolButton(this);
    QMenu *otherMenu = nullptr;
    // TODO: when those are freed?

    if (!otherButton) {
        err = ErrNoMemory;
        loge("no memory otherButton");
    }

    if (err == ErrNone) {
        // TODO: when other button be clear? check it
        otherButton->setText(tr("&Khác"));
        otherButton->setPopupMode(QToolButton::InstantPopup);
        otherButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon/icons8-diversity-64"),
                     QSize(), QIcon::Normal, QIcon::On);
        otherButton->setIcon(icon);
    }

    if (err == ErrNone) {
        otherMenu = new QMenu(otherButton);
        if (!otherMenu) {
            loge("no memory to allocate otherMenu");
            err = ErrNoMemory;
        }
    }

    if (err == ErrNone) {
        otherButton->setMenu(otherMenu);
        // actionDummy? stupid? but it works
        ui->toolBar->insertWidget(ui->actionDummy, otherButton);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionDept_triggered,
                        STR_DEPARTMENT,
                        ICON_DEPT);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionSaints_2_triggered,
                        STR_HOLLYNAME,
                        ICON_SAINT);


        ADD_ACTION_ITEM(otherMenu,
                        on_actionRole_triggered,
                        STR_ROLE,
                        ICON_ROLE);


        ADD_ACTION_ITEM(otherMenu,
                        on_actionCountry_triggered,
                        STR_COUNTRY,
                        ICON_COUNTRY);


    #ifndef SKIP_PERSON_PROVINE
        ADD_ACTION_ITEM(otherMenu,
                        on_actionProvince_triggered,
                        "Tỉnh/Thành phố/Bang",
                        ICON_PATH("icons8-catholic-64"));
    #endif
        ADD_ACTION_ITEM(otherMenu,
                        on_actionMisson_triggered,
                        STR_NHIEM_VU_XA_HOI,
                        ICON_MISSION);


        ADD_ACTION_ITEM(otherMenu,
                        on_actionSpeclialist_triggered,
                        STR_CHUYEN_MON,
                        ICON_SPECIALIST);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionEducation_triggered,
                        STR_EDUCATION,
                        ICON_EDU);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionWork_triggered,
                        STR_WORK,
                        ICON_WORK);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionEthnic_triggered,
                        STR_ETHNIC,
                        ICON_ETHNIC);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionCourse_triggered,
                        STR_COURSE_TERM,
                        ICON_COURSE);

        ADD_ACTION_ITEM(otherMenu,
                        on_actionPersonStatus_triggered,
                        STR_PERSON_STATUS,
                        ICON_PERSON);
    }
    logife(err, "Faile to prepare other menu");
    traceout;
}

void MainWindow::loadImportMenu()
{
    tracein;
    ErrCode err = ErrNone;
    QMenu *importMenu = nullptr;
    mImportButton = new QToolButton(this);
    if (!mImportButton) {
        loge("no memory for mImportButton");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        // TODO: when import button be clear? check it
        mImportButton->setText(tr("&Nhập dữ liệu"));
        mImportButton->setPopupMode(QToolButton::InstantPopup);
        mImportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon/icons8-import-64"), QSize(), QIcon::Normal, QIcon::On);
        mImportButton->setIcon(icon);
    }

    if (err == ErrNone) {
        importMenu = new QMenu(mImportButton);
        if (!importMenu) {
            loge("no memory for importMenu");
            err = ErrNoMemory;
        }
    }

    if (err == ErrNone) {
        // TODO: when menu is clear???? check it careffully
        mImportButton->setMenu(importMenu);
        ui->toolBar->insertWidget(ui->action_New, mImportButton);

        ADD_MENU_ITEM(importMenu,
                      on_action_ImportPerson_triggered,
                      STR_DS_NU_TU,
                      ICON_PERSON);

        ADD_MENU_ITEM(importMenu,
                      on_action_ImportCommunityList_triggered,
                      STR_COMMUNITY_LIST,
                      ICON_ADD_COMMUNITY);

    }
    logife(err, "failed to prepare import menu");
    traceout;
}

void MainWindow::loadExportMenu()
{
    tracein;
    ErrCode err = ErrNone;
    QMenu *exportMenu = nullptr;
    mExportButton = new QToolButton(this);
    if (!mExportButton) {
        loge("no memory mExportButton");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        // TODO: when import button be clear? check it
        mExportButton->setText(tr("&Xuất dữ liệu"));
        mExportButton->setPopupMode(QToolButton::InstantPopup);
        mExportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon/icons8-export-100"), QSize(), QIcon::Normal, QIcon::On);
        mExportButton->setIcon(icon);
    }

    if (err == ErrNone) {
        exportMenu = new QMenu(mExportButton);
        if (!exportMenu) {
            loge("no memory exportMenu");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        // TODO: when menu is clear???? check it careffully
        mExportButton->setMenu(exportMenu);
        ui->toolBar->insertWidget(ui->action_New, mExportButton);

        ADD_MENU_ITEM(exportMenu,
                      on_action_ExportPersonList_triggered,
                      STR_DS_NU_TU,
                      ICON_PERSON);
        ADD_MENU_ITEM(exportMenu,
                      on_action_ExportCommunityList_triggered,
                      STR_COMMUNITY_LIST,
                      ICON_ADD_COMMUNITY);
    }
    logife(err, "failed to prepare loadExportMenu");

    traceout;

}

AppState MainWindow::appState() const
{
    return mAppState;
}

void MainWindow::setAppState(AppState newAppState)
{
    dbg(LOG_VERBOSE, "set app state %d --> %d", mAppState, newAppState);
    mAppState = newAppState;
}

BaseView* MainWindow::popViewFromStackAndShow()
{
    // TODO: implement his
    tracein;
    BaseView* view = nullptr;
    if (!mViewStack.empty()) {
        view = mViewStack.pop();
    } else {
        logd("view stack is empty");
    }
    traceout;
    return view;
}

void MainWindow::pushViewToStack(BaseView* view)
{
    // TODO: implement this
    // TODO: should limit max view, auto clean up old view
    tracein;
    if (view) {
        mViewStack.push(view);
    } else {
        logw("no view push to stack");
    }
    traceout;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    tracein;
    logi("Windonw Close event!");
    // TODO: call unload directly here instead of emit?
    // because QMainWindow::closeEvent may free resource before slot Unload is called....
    emit unload();
    logd("Close main window");
    QMainWindow::closeEvent(event);
    traceout;
}

void MainWindow::doAddMainWindownImportListener(MainWindownImportListener *listener)
{
    tracein;
    if (listener) {
        if (!mMainWindowImportListener.contains(listener)) {
            logd("Add main windown listener '%s'", STR2CHA(listener->getName()));
            mMainWindowImportListener.push_back(listener);
        } else {
            loge("main windown lister existed '%s'", STR2CHA(listener->getName()));
        }
    } else {
        loge("Invalid listener");
    }
    traceout;
}

void MainWindow::doRemoveMainWindownImportListener(MainWindownImportListener *listener)
{
    tracein;
    if (listener) {
        if (mMainWindowImportListener.contains(listener)) {
            logd("remove main windown listener '%s'", STR2CHA(listener->getName()));
            mMainWindowImportListener.removeAll(listener);
        } else {
            loge("main windown lister Not existed '%s'", STR2CHA(listener->getName()));
        }
    } else {
        loge("Invalid listener");
    }
    traceout;

}

void MainWindow::notifyMainWindownImportListenerStart(ImportTarget target)
{
    tracein;
    dbg(LOG_VERBOSE, "Notify import start, target '%d'", target);
    foreach (MainWindownImportListener* listener, mMainWindowImportListener) {
        if (listener) {
            dbg(LOG_DEBUG, "Call onMainWindownImportStart target %d name '%s'",
                target, STR2CHA(listener->getName()));
            listener->onMainWindownImportStart(target);
        }
    }
    traceout;
}

void MainWindow::notifyMainWindownImportListenerEnd(ImportTarget target, ErrCode err, void *importData)
{
    tracein;
    logi("Notify import end, target '%d' err '%d'", target, err);
    foreach (MainWindownImportListener* listener, mMainWindowImportListener) {
        if (listener) {
            dbg(LOG_DEBUG, "Call onMainWindownImportEnd target %d name '%s'",
                target, STR2CHA(listener->getName()));
            listener->onMainWindownImportEnd(target, err, importData);
        }
    }
    traceout;
}

void MainWindow::onLoad()
{
    tracein;
    setAppState(APP_STATE_LOADING);
    if (!mWaitDlg) {
        mWaitDlg = new DlgWait(this);
    }
    if (mWaitDlg) {
        if (isFirstInit) {
            mWaitDlg->setTitle(STR_INITING_FIRST);
        } else {
            mWaitDlg->setTitle(STR_INITING_LONG);
        }
        mWaitDlg->setAllowCancel(false);
        mWaitDlg->show(nullptr,
              nullptr,
              [](ErrCode* err, void* data, DlgWait* dlg) {
                UNUSED(err);
                UNUSED(data);
                UNUSED(dlg);
                logi("Call loaders on load");
                LoaderCtl::getInstance()->onLoad();
                return nullptr;
              },
            [this](ErrCode err, void* data, void* result, DlgWait* dlg) {
                UNUSED(err);
                UNUSED(data);
                UNUSED(result);
                UNUSED(dlg);
                logi("Finished on load, close wait dlg");
                this->setAppState(APP_STATE_READY);
                return ErrNone;
            }
        );
    } else {
        logi("no wait dialog");
    }
    traceout;
}

void MainWindow::onUnload()
{
    tracein;
    logi("unload from loader");
    LoaderCtl::getInstance()->onUnload();
    traceout;
}

void MainWindow::onImportPeople(ErrCode err, QList<DbModel *> *list)
{
    tracein;
    DlgImportPersonListResult* dlg = nullptr;
    if (err == ErrNone && (!list || (list->size() == 0))) {
        err = ErrNoData;
        loge("Import ok, but no data?");
    }
    if (err == ErrNone && (dlg = new DlgImportPersonListResult()) == nullptr) {
        err = ErrNoMemory;
        loge("no memory for dlg");
    }
    if (err == ErrNone) {
        dlg->setup(*list);
        dlg->exec();
        notifyMainWindownImportListenerEnd(IMPORT_TARGET_PERSON, err, list);
        FREE_PTR(dlg);
        if (list) {
            RELEASE_LIST_DBMODEL(*list);
            delete list;
        }
    }
    if (err != ErrNone) {
        loge("Import failed, err=%d", err);
        DialogUtils::showErrorBox(QString(STR_IMPORT_DATA_ERR_CODE).arg(err));
        notifyMainWindownImportListenerEnd(IMPORT_TARGET_PERSON, err, nullptr);
    }
    traceout;
}

void MainWindow::onShowMsgDlg(QString msg)
{
    tracein;
    DialogUtils::showMsgBox(msg, this);
    traceout;
}

void MainWindow::onShowErrorMsgDlg(ErrCode err, QString msg)
{
    tracein;
    DialogUtils::showErrorBox(err, msg);
    traceout;
}

void MainWindow::on_action_ImportPerson_triggered()
{
    tracein;
    doShowImportPerson();
    traceout;
}

void MainWindow::on_action_ImportPersonList_triggered()
{
    tracein;
    ErrCode ret = ErrNone;
    QList<DbModel*> list;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
                                    this,
                                    STR_SELECT_TO_IMPORT,
                                    FileCtl::getAppDataDir(),
                                    tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        dbg(LOG_INFO, "Import from file %s", STR2CHA(fname));
        ImportType type = ImportFactory::importTypeFromExt(fname, true);
        if (type == IMPORT_XLSX || type == IMPORT_CSV_LIST || type == IMPORT_CSV) {
            ret = INSTANCE(PersonCtl)->importFromFile(KModelNamePerson, type, fname, &list);
        } else {
            ret = ErrNotSupport;
            loge("Import type %d not support, fname = '%s'", type, STR2CHA(fname));
        }
        dbg(LOG_INFO, "Import result %d, No of import item %lld",
            ret, list.count());
        if (ret == ErrNone) {
            DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
            if (dlg) {
                dlg->setup(list);
                dlg->exec();
                delete dlg; // no clean list, as dlg will take over it
            } else {
                loge("failed to allocate memory for DlgImportPersonListResult");
                ret = ErrNoMemory;
            }
        }
    } else {
        logw("no file selected to import");
    }
    logife(ret, "import person list failed");
    RELEASE_LIST_DBMODEL(list);
    traceout;
}

void MainWindow::on_action_ImportCommunityList_triggered()
{
    tracein;
    MainWindow::showImportDlg(ImportTarget::IMPORT_TARGET_COMMUNITY);
    traceout;
}

void MainWindow::on_action_ImportCommunity_triggered()
{
    tracein;
    UNDER_DEV(tr("Nhập Cộng Đoàn từ tập tin"));
    // TODO: handle error case??
    traceout;
}

void MainWindow::on_action_New_triggered()
{
    tracein;
    dbg(LOG_VERBOSE, "new community click");
    DlgPerson *dlg = DlgPerson::buildDlg(this);
    if (dlg) {
        dlg->exec();
        delete dlg;
    } else {
        loge("no memory for DlgPerson");
    }
    traceout;
}


void MainWindow::on_actionNew_Community_triggered()
{
    tracein;
    dbg(LOG_VERBOSE, "new community click");
    DlgCommunity w;
    w.exec();
    traceout;
}


void MainWindow::on_actionCommunity_triggered()
{
    tracein;
    switchView(mCommunityView);
    traceout;
}


void MainWindow::on_actionSummarize_triggered()
{
    tracein;
    switchView(mSummarizeView);
    traceout;
}


void MainWindow::on_actionSaints_2_triggered()
{
    tracein;
    switchView(mSaintsView);
    traceout;

}


void MainWindow::on_actionHome_triggered()
{
    tracein;
    switchView(mHomeView);
    traceout;
}


void MainWindow::on_actionPerson_triggered()
{
    tracein;
    switchView(mPersonView);
    traceout;
}


void MainWindow::on_actionSearch_triggered()
{
    tracein;
    UNDER_DEV(tr("Tìm kiếm thông tin"));
    traceout;
}

void MainWindow::on_actionArea_triggered()
{
    tracein;
    switchView(mAreaView);
    traceout;
}


void MainWindow::on_actionRole_triggered()
{
    tracein;
    switchView(mRoleView);
    traceout;
}

void MainWindow::on_actionCourse_triggered()
{
    tracein;
    switchView(mCourseView);
    traceout;
}

void MainWindow::on_actionPersonStatus_triggered()
{
    tracein;
    switchView(mPersonStatusView);
    traceout;

}


MainWindow *MAINWIN
{
    return gInstance;
}


void MainWindow::on_actionDept_triggered()
{
    tracein;
    switchView(mDepartView);
    traceout;

}

void MainWindow::on_action_ExportPersonList_triggered()
{
    tracein;
    dbg(LOG_INFO, "export person");
    QList<DbModel*> list = PERSONCTL->getAllItemsFromDb();
    doExportListItems(&list, KModelNamePerson, PERSONCTL, STR_DS_NU_TU, ExportType::EXPORT_XLSX);
    RELEASE_LIST_DBMODEL(list);
    traceout;
}

void MainWindow::on_action_ExportCommunityList_triggered()
{
    tracein;
    dbg(LOG_INFO, "export community");
    QList<DbModel*> list = COMMUNITYCTL->getAllItemsFromDb();
    doExportListItems(&list, KModelNameCommunity, COMMUNITYCTL,
                      STR_COMMUNITY_LIST, ExportType::EXPORT_XLSX);
    RELEASE_LIST_DBMODEL(list);
    traceout;
}


void MainWindow::on_action_About_triggered()
{
    DlgAbout dlg(this);
    dlg.exec();
}


void MainWindow::on_action_Backup_triggered()
{
    tracein;
    QString fpath = DialogUtils::saveFileDialog(this, STR_BACKUP,
                                  QString("saoluu.zip"),
                                  QString("zip (*.zip)")
                                  );
    if (!fpath.isEmpty()) {
        logi("Backup file to '%s'", STR2CHA(fpath));
        ErrCode err = BACKUP->backup(fpath);
        logi("Backup file result=%d", err);
        if (err == ErrNone) {
            DialogUtils::showMsgBox(QString(STR_BACKUP_TO).arg(fpath));
        } else {
            DialogUtils::showErrorBox(QString(STR_BACKUP_ERRCODE).arg(err));
        }
    } else {
        logw("not file to backup");
    }
    traceout;
}


void MainWindow::on_actionRestore_triggered()
{
    tracein;
    UNDER_DEV(tr("Phục hồi dữ liệu đã sao lưu"));
    // TODO: implement it
    traceout;
}


void MainWindow::on_actionRevert_triggered()
{
    tracein;
    UNDER_DEV(tr("Khôi phục dữ liệu (khôi phục dữ liệu cũ, trong trường hợp dữ liệu hiện tại bị lỗi)"));
    // TODO: implement it
    traceout;
}


void MainWindow::on_action_Help_triggered()
{
    tracein;
    UNDER_DEV(tr("Hướng dẫn sử dụng phần mềm"));
    // TODO: implement it
    traceout;

}

void MainWindow::on_actionEthnic_triggered()
{
    tracein;
    switchView(VIEW_ETHNIC);
    traceout;
}

void MainWindow::on_actionWork_triggered()
{

    tracein;
    switchView(VIEW_WORK);
    traceout;
}

void MainWindow::on_actionEducation_triggered()
{

    tracein;
    switchView(VIEW_EDUCATION);
    traceout;
}

void MainWindow::on_actionSpeclialist_triggered()
{
    tracein;

    switchView(VIEW_SPECIALIST);
    traceout;
}

void MainWindow::on_actionMisson_triggered()
{
    tracein;
    switchView(VIEW_MISSION);
    traceout;
}

void MainWindow::on_actionProvince_triggered()
{
    tracein;
    UNDER_DEV(tr("Tỉnh/Thành phố/Bang"));
    // TODO: implement it
    traceout;
}

void MainWindow::on_actionCountry_triggered()
{
    tracein;
    switchView(VIEW_COUNTRY);
    traceout;
}


void MainWindow::on_actionBack_triggered()
{
    tracein;
    BaseView* view = popViewFromStackAndShow();
    if (view) {
        switchView(view, true);
    } else {
        logw("no view to back");
    }
    traceout;
}


void MainWindow::on_actionLogFile_triggered()
{
    QString logDir = Logger::logDirPath();
    logi("Open log dir=%s", STR2CHA(logDir));
    QDesktopServices::openUrl(QUrl::fromLocalFile(logDir));
}


void MainWindow::on_actionSummarizeReport_triggered()
{
    tracein;
    QString fpath;
    ErrCode err = STAT->exportGeneralStatistic(&fpath);
    dlgHtmlViewer* viewer = nullptr;

    if (err == ErrNone && ((viewer = new dlgHtmlViewer()) == nullptr)) {
        loge("faie to allocate viewer dialog");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        logd("show html viewer");
        viewer->setHtmlPath(fpath);
        viewer->setSubject(STR_GENERAL_STAT);
        viewer->exec();
    }
    QFile::remove(fpath);
    FREE_PTR(viewer);
    if (err != ErrNone) {
        loge("failed to show dlg err = %d", err);
        DialogUtils::showErrorBox(err, STR_GENERAL_STAT_ERR);
    }
    traceout;
}


void MainWindow::on_actionOrgInfo_triggered()
{
    // THONG TIN HOI DONG
    tracein;
    // TODO: implemen this
    UNDER_DEV(tr("Xem thông tin về hội dòng: lịch sử, số lượng Cộng đoàn, Nữ tu, Tổng phụ trách, v.v.v."));

    traceout;
}


void MainWindow::on_actionDong_bo_triggered()
{
    // DONG BO DU LIEU
    tracein;
    // TODO: implemen this
    UNDER_DEV(tr("Đồng bộ dữ liệu (với dữ liệu được lưu trên Google Drive, vvv.)"));

    traceout;
}

void MainWindow::on_actionNew_PersonEvent_triggered()
{
    tracein;
    doShowAddPersonEvent();
    traceout;
}

