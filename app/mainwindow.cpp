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
#include "view/dialog/dlgimportcommunitylistresult.h"
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
    tracein;
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

    mHomeView = new UITextBrowser(this);
    mHomeView->clearHistory();
    mHomeView->clear();

    loadHomePageFile();
    // TODO: load all memory, consume much memory and performance
    mHomeView->setSearchPaths(QStringList(FileCtl::getOrCreatePrebuiltDataDir()));
    mHomeView->setHtml(Utils::readAll(
        FileCtl::getUpdatePrebuiltDataFilePath(KPrebuiltHomeHtmlFileName, false)));


    mViewList.insert(VIEW_SUMMARY, (BaseView*)mSummarizeView);
    mViewList.insert(VIEW_SAINT, (BaseView*)mSaintsView);
    mViewList.insert(VIEW_PERSON, (BaseView*)mPersonView);
    mViewList.insert(VIEW_AREA, (BaseView*)mAreaView);
    mViewList.insert(VIEW_TEXT_BROWSER, (BaseView*)mHomeView);
    mViewList.insert(VIEW_COMMUNITY, (BaseView*)mCommunityView);
    mViewList.insert(VIEW_DEPARTMENT, (BaseView*)mDepartView);
    mViewList.insert(VIEW_ROLE, (BaseView*)mRoleView);
    mViewList.insert(VIEW_COURSE, (BaseView*)mCourseView);

    switchView(mHomeView);

    logd("init config");
    Config::init();

    logd("connect");
//    QObject::connect(this, SIGNAL(load()), loader, SLOT(onLoad()));
    QObject::connect(this, SIGNAL(load()), this, SLOT(onLoad()));
    QObject::connect(this, SIGNAL(unload()), this, SLOT(onUnload()));
    if (!QObject::connect(this,
                          SIGNAL(showMsgDlgSignal(QString)),
                          this,
                          SLOT(onShowMsgDlg(QString)))) {
        loge("Failed to connect showMsgDlgSignal to onShowMsgDlg");
    }
    if (!QObject::connect(this,
                          SIGNAL(showErrorDlgSignal(ErrCode,QString)),
                          this,
                          SLOT(onShowErrorMsgDlg(ErrCode,QString)))) {
        loge("Failed to connect showErrorDlgSignal to onShowErrorMsgDlg");
    }
    traceout;
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
    logd("import target %d", target);
    switch (target) {
    case IMPORT_TARGET_PERSON:
        getInstance()->doShowImportPerson();
        break;
    case IMPORT_TARGET_COMMUNITY:
        getInstance()->doShowImportCommunity();
        break;
    default:
        logw("import target %d not support, use commone one", target);
        err = getInstance()->doShowCommonImport(target, controller, modelName, targetModel);
        break;
    }
    // TODO: return value to handle error case???
    // TODO: use function pointer instead?
    traceret(err);
    return err;
}

ErrCode MainWindow::showOnHtmlViewer(DbModel *model, const QString& subject)
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
        logd("export to html file");
        ret = model->exportToFile(ExportType::EXPORT_HTML, &fpath);
        logd("exported file '%s'", STR2CHA(fpath));
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
        logd("show html viewer");
        viewer->setHtmlPath(fpath);
        viewer->setSubject(subject);
        viewer->exec();
    }
    FREE_PTR(viewer);
    traceret(ret);
    return ret;
}

void MainWindow::showAddEditCommonModel(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCommonModel(isSelfUpdate, model, listener);
    traceout;

}

void MainWindow::showAddEditCourse(bool isSelfUpdate, DbModel *com, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCourse(isSelfUpdate, com, listener);
    traceout;

}

void MainWindow::showAddEditCommDept(bool isSelfUpdate, DbModel* comm, DbModel *dept, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditCommDept(isSelfUpdate, comm, dept, listener);
    traceout;

}

void MainWindow::showAddEditArea(bool isSelfUpdate, DbModel *com, CommonEditModelListener *listener)
{
    tracein;
    getInstance()->doShowAddEditArea(isSelfUpdate, com, listener);
    traceout;
}

void MainWindow::showAddEditEthnic(bool isSelfUpdate, DbModel *com, CommonEditModelListener *listener)
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
    err = getInstance()->doExportListItems(items, datatype, controller, title, exportTypeList);
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
    emit showErrorDlgSignal(err, msg);
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
    traceout;
    return err;
}

ErrCode MainWindow::doShowAddPersonEvent(bool isSelfUpdate, DbModel* person, DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    logd("Set person '%s'", MODELSTR2CHA(person));
    logd("Set model '%s'", MODELSTR2CHA(model));
    DlgAddPersonEvent* dlg = DlgAddPersonEvent::build(this, isSelfUpdate,
                                                      KModelNamePersonEvent, model, listener);
    if (person) {
        dlg->setPerson((Person*)person);
    }
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::switchView(ViewType type, void* data)
{
    tracein;
    BaseView *nextView = getView(type);
    if (nextView != nullptr) {
        nextView->setData(data);
        switchView(nextView);
    }
    traceout;
}

void MainWindow::switchView(BaseView *nextView, bool fromStack)
{
    tracein;
    logd("fromStack=%d", fromStack);
    if (mCurrentView != nextView) {
        if (mCurrentView && !fromStack) {
            logd("push view to stack");
            pushViewToStack(mCurrentView);
        }
        if (mCurrentView != nullptr) {
            logd("hide currentl widget");
            mCurrentView->getWidget()->hide();
            mCurrentView->onPaused();
            ui->centralLayout->replaceWidget(mCurrentView->getWidget(), nextView->getWidget());
    //        ui->centralwidget->layout()->replaceWidget(mCurrentView, nextView);
            // TODO: make this as stack????
    //        if (!mViewList.contains(mCurrentView)) {
    //            logd("Not in cached view, remove");
    //            delete mCurrentView;
                // TODO: delete here cause something terrible, i.e UICommDeptListView --> uidepartmentpersonlistview.cpp
                // UICommDeptListView is deleted, but its function is still called, because
                // uidepartmentpersonlistview is created in menu UICommDeptListView::onMenuActionListPerson
                // cause use-after-free issue
                // Need to rethink this again!!!!!
    //            mCurrentView = nullptr;
    //        } else {
    //            logd("In cache view, keep current, just hide");
    //        }
        }
        else{
    //        ui->centralwidget->layout()->addWidget(nextView);
            ui->centralLayout->addWidget(nextView->getWidget());

        }
        logd("show next");
        mCurrentView = nextView;
        mCurrentView->onShown();
        mCurrentView->getWidget()->show();
    } else {
        logd("Same view!");
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
//    switch (type) {
//    case ViewType::VIEW_DEPARTMENT:
//        nextView = mDepartView;
//        break;
//    case ViewType::VIEW_PERSON:
//        nextView = mPersonView;
//        break;
//    default:
//        {
//            BaseView* view = UITableViewFactory::getView(type);
//            if (view) {
//                nextView = (BaseView*) view;
//            } else {
//                loge("Unknown type %d", type);
//            }
//        }
//        break;
//    }
    traceout;
    return nextView;
}

void MainWindow::onLoadController (Controller* ctl)
{
    tracein;
    logd("on load ctrl %s", ctl->getName().toStdString().c_str());
    if (mWaitDlg != nullptr) {
        mWaitDlg->setMessage(QString(tr("Khởi tạo %1")).arg(ctl->getName()));
    }
//    QThread::msleep(500);
    traceout;
}

void MainWindow::onUnloadController(Controller *ctl)
{
    tracein;
    // TODO: unload controller???
    traceout;
}

void MainWindow::doShowAddEditPerson(bool isSelfUpdate, Person *per, bool isNew)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgPerson* dlg = DlgPerson::buildDlg(this, per, isNew);
    dlg->setIsSelfSave(isSelfUpdate);
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::doShowAddEditCommunity(bool isSelfUpdate, const Community *com, CommonEditModelListener* listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCommunity* dlg = DlgCommunity::build(this, isSelfUpdate, KModelNameCommunity, (DbModel*)com, listener);
    dlg->setListener(listener);
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::doShowImportPerson()
{
//    tracein;
//    ErrCode err = ErrNone;
//    // TODO: show dialog to select which type of file to be imported???
//    QString fname = QFileDialog::getOpenFileName(
//        this,
//        tr("Open file"),
//        FileCtl::getAppDataDir(),
//        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
//    // TODO: this is duplicate code, make it common please
//    if (!fname.isEmpty()){
//        logd("File %s is selected", fname.toStdString().c_str());
//        QList<DbModel*> list;
//        logd("Import from file %s", fname.toStdString().c_str());
//        ErrCode ret = INSTANCE(PersonCtl)->importFromFile(KModelHdlPerson, ImportType::IMPORT_CSV, fname, &list);
//        logd("Import result %d", ret);
//        logd("No of import item %d", list.count());
//        DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
//        dlg->setup(list);
//        dlg->exec();
//        delete dlg;
//    }
//    traceout;
    tracein;
    ErrCode ret = ErrNone;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("Excel (*.xlsx))"));
//        tr("Excel (*.xlsx);;CSV Files (*.csv);;All Files (*.*)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
//        QList<DbModel*> list;
        connect(this, &MainWindow::importPeople, this, &MainWindow::onImportPeople);
        notifyMainWindownImportListenerStart(IMPORT_TARGET_PERSON);
        ret = showProcessingDialog(tr("Nhập dữ liệu"), nullptr,
            [this, fname](ErrCode* err, void* data, DlgWait* dlg) {
                QList<DbModel*>*list = new QList<DbModel*>();
                logd("Import from file %s", STR2CHA(fname));
                ImportType type = ImportFactory::importTypeFromExt(fname, true);
                if (type == IMPORT_XLSX || type == IMPORT_CSV_LIST || type == IMPORT_CSV) {
                    *err = PERSONCTL->importFromFile(KModelHdlPerson, type, fname, list);
                } else {
                    *err = ErrNotSupport;
                    loge("Import type %d not support (fname = '%s'", type, STR2CHA(fname));
                }
                return list;
                },
            [this, fname](ErrCode err, void* data, void* result, DlgWait* dlg) {
                logd("Import result %d", err);
                QList<DbModel*>* list = (QList<DbModel*>*)(result);
                if (result) {
                    logd("No of import item %d", list->count());
                }
//                dlg->forceClose();
                if (err == ErrNone) {
                    emit this->importPeople(err, list);
                }
                return err;
            });
        if (ret != ErrNone) {
            loge("Failed to import, notify listener err = %d", ret);
            notifyMainWindownImportListenerEnd(IMPORT_TARGET_PERSON, ret, nullptr);
        }
    }
    traceout;
}

void MainWindow::doShowImportCommunity()
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("Excel (*.xlsx)"));
//        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        logd("File %s is selected", fname.toStdString().c_str());
        QList<DbModel*> list;
        logd("Import from file %s", fname.toStdString().c_str());
        ErrCode ret = COMMUNITYCTL->importFromFile(KModelHdlCommunity, ImportType::IMPORT_XLSX, fname, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        DlgImportListResult* dlg = nullptr;
        err = DlgImportListResultFactory::getImportListResult(
                            IMPORT_TARGET_COMMUNITY, &dlg, this);
        dlg->setup(list);
        dlg->exec();
        delete dlg;
    }
    traceout;
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
    logd("import for target %d", target);
    logd("model name '%s'", STR2CHA(modelName));
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("Excel (*.xlsx)"));
    if (fname.isEmpty()) {
        loge("no file name selected");
        err = ErrNoFile;
    }
    if (err == ErrNone) {
        logd("Import from file %s", STR2CHA(fname));
        err = controller->importFromFile(modelName,
                                        ImportType::IMPORT_XLSX, fname, &list);
        logd("Import result %d", err);
    }
    if (err == ErrNone && list.size() == 0) {
        loge("no data is imported");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        logd("No of import item %d", list.size());
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
        logd("Set up list, no. item %d", list.size());
        err = dlg->setup(list);
    }
    if (err == ErrNone) {
        dlg->exec();
    }
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

void MainWindow::doShowAddEditCommonModel(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d, model '%s' listener '%s'",
            isSelfUpdate,
             model?STR2CHA(model->toString()):"null",
             listener?STR2CHA(listener->getName()):"null");
    DlgEditModel* dlg = DlgEditModel::build(this, isSelfUpdate, nullptr, model, listener);
    dlg->setListener(listener);
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::doShowAddEditCourse(bool isSelfUpdate, DbModel *model,
                                     CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCourse* dlg = DlgCourse::build(this, isSelfUpdate, nullptr, model, listener);
    dlg->setListener(listener);
    dlg->exec();
    delete dlg;
    traceout;

}

void MainWindow::doShowAddEditCommDept(bool isSelfUpdate, DbModel* comm,
                                       DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgCommDept* dlg = DlgCommDept::build(this, isSelfUpdate, KModelNameCommDept, model, listener);
    dlg->setCommunity(comm);
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::doShowAddEditArea(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgArea* dlg = DlgArea::build(this, isSelfUpdate, KModelNameArea, model, listener);
    dlg->exec();
    delete dlg;
    traceout;
}

void MainWindow::doShowAddEditEthnic(bool isSelfUpdate, DbModel *model, CommonEditModelListener *listener)
{
    tracein;
    logd("isSelfUpdate %d", isSelfUpdate);
    DlgEthnic* dlg = DlgEthnic::build(this, isSelfUpdate, KModelNameEthnic, model, listener);
    dlg->exec();
    delete dlg;
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
    logd("datatype '%s', exportTypeList %d controller '%s'",
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
    if (ret == ErrNone)
        ret = FileCtl::checkAndUpdatePrebuiltFile(KPrebuiltLogoFileName, true);

    logd("Load home page file ret %d", ret);
    // TODO: should log to file???
}

#define ADD_ACTION_ITEM(menu, func, name, iconPath) \
    do { \
        QAction* act = nullptr;\
        act = menu->addAction(QIcon(QString::fromUtf8(iconPath)), \
                                   name); \
        QObject::connect(act, SIGNAL(triggered()), this, SLOT(func())); \
    } while (0)


void MainWindow::loadOtherAddMenu()
{
    tracein;

    QToolButton *otherButton = new QToolButton(this);
    // TODO: when other button be clear? check it
    otherButton->setText(tr("&Khác"));
    otherButton->setPopupMode(QToolButton::InstantPopup);
    otherButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(ICON_ADD, QSize(), QIcon::Normal, QIcon::On);
    otherButton->setIcon(icon);

    QMenu *otherMenu = new QMenu(otherButton);

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

void MainWindow::loadOtherMenu()
{
    tracein;

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

//    ADD_ACTION_ITEM(otherMenu,
//                    on_actionArea_triggered,
//                    "Khu vực/Vùng",
//                    ICON_PATH("icons8-earth-planet-80"));

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
    tracein;

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
                  "Danh sách Nữ Tu",
                  ":/icon/icon/icons8-nun-64.png");

    ADD_MENU_ITEM(importMenu,
                  on_action_ImportCommunityList_triggered,
                  "Danh sách Cộng đoàn",
                  ":/icon/icon/icons8-community-64.png");

//    ADD_MENU_ITEM(importMenu,
//                  on_action_ImportCommunity_triggered,
//                  "Cộng đoàn",
//                  ":/icon/icon/icons8-community-64.png");

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

    traceout;
}

void MainWindow::loadExportMenu()
{
    tracein;

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


    traceout;

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
    mViewStack.push(view);
    traceout;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    tracein;
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
    foreach (MainWindownImportListener* listener, mMainWindowImportListener) {
        if (listener) {
            logd("Call onMainWindownImportStart target %d name '%s'", target, STR2CHA(listener->getName()));
            listener->onMainWindownImportStart(target);
        }
    }
    traceout;
}

void MainWindow::notifyMainWindownImportListenerEnd(ImportTarget target, ErrCode err, void *importData)
{
    tracein;
    foreach (MainWindownImportListener* listener, mMainWindowImportListener) {
        if (listener) {
            logd("Call onMainWindownImportEnd target %d name '%s'", target, STR2CHA(listener->getName()));
            listener->onMainWindownImportEnd(target, err, importData);
        }
    }
    traceout;
}

void MainWindow::onLoad()
{
    tracein;
    mAppState = APP_STATE_LOADING;
    if (!mWaitDlg)
        mWaitDlg = new DlgWait(this);
    mWaitDlg->setMessage(tr("Đang khởi tạo"));
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
//            dlg->forceClose();
            this->setAppState(APP_STATE_READY);
            return ErrNone;
        }
        );
//    LoaderCtl::getInstance()->onLoad();
//    this->setAppState(APP_STATE_READY);
    traceout;
}

void MainWindow::onUnload()
{
    tracein;
    logd("unload from loader");
    LoaderCtl::getInstance()->onUnload();
    traceout;
}

void MainWindow::onImportPeople(ErrCode err, QList<DbModel *> *list)
{
    tracein;
    if (err == ErrNone && (!list || (list->size() == 0))) {
        err = ErrNone;
        loge("Import ok, but no data?");
    }
    if (err == ErrNone) {
        DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
        dlg->setup(*list);
        dlg->exec();
        notifyMainWindownImportListenerEnd(IMPORT_TARGET_PERSON, err, list);
        delete list;
        delete dlg; // no clean list, as dlg will take over it
    } else {
        loge("Import failed, err=%d", err);
        DialogUtils::showErrorBox(QString(tr("Nhập dữ liệu lỗi, mã lỗi %1")).arg(err));
        notifyMainWindownImportListenerEnd(IMPORT_TARGET_PERSON, err, nullptr);
    }
    traceout;
}

void MainWindow::onShowMsgDlg(QString msg)
{
    tracein;
    DialogUtils::showMsgBox(msg);
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
    // TODO: show dialog to select which type of file to be imported???
    QString fname = QFileDialog::getOpenFileName(
        this,
        tr("Open file"),
        FileCtl::getAppDataDir(),
        tr("CSV Files (*.csv);;Excel (*.xls *.xlsx)"));
    // TODO: this is duplicate code, make it common please
    if (!fname.isEmpty()){
        QList<DbModel*> list;
        logd("Import from file %s", STR2CHA(fname));
        ImportType type = ImportFactory::importTypeFromExt(fname, true);
        if (type == IMPORT_XLSX || type == IMPORT_CSV_LIST || type == IMPORT_CSV) {
            ret = INSTANCE(PersonCtl)->importFromFile(KModelHdlPerson, type, fname, &list);
        } else {
            ret = ErrNotSupport;
            loge("Import type %d not support (fname = '%s'", type, STR2CHA(fname));
        }
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        if (ret == ErrNone) {
            DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
            dlg->setup(list);
            dlg->exec();
            delete dlg; // no clean list, as dlg will take over it
        }
    }
    traceout;
}

void MainWindow::on_action_ImportCommunityList_triggered()
{
    tracein;
    MainWindow::showImportDlg(ImportTarget::IMPORT_TARGET_COMMUNITY);
    // TODO: implement it
    traceout;
}

void MainWindow::on_action_ImportCommunity_triggered()
{
    tracein;
    UNDER_DEV(tr("Nhập Cộng Đoàn từ tập tin"));
    // TODO: handle error case??

}

void MainWindow::on_action_New_triggered()
{
    tracein;
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
    tracein;
//    Community* comm = TestCtl::genRandomCommunity();
//    comm->save();

//    delete comm;
    DlgCommunity w;
//    w.setWindowState(Qt::WindowState::WindowMaximized);
    w.exec();
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
    QList<DbModel*> list = PERSONCTL->getAllItemsFromDb();
//    UNDER_DEV(tr("Xuất danh sách nữ tu ra tập tin"));
//    doExportListItems(&list, PERSONCTL, "Danh sách nữ tu", ExportType::EXPORT_XLSX | ExportType::EXPORT_CSV_LIST);
    doExportListItems(&list, KModelNamePerson, PERSONCTL, "Danh sách nữ tu", ExportType::EXPORT_XLSX);
    RELEASE_LIST_DBMODEL(list);
}

void MainWindow::on_action_ExportCommunityList_triggered()
{
    tracein;
    QList<DbModel*> list = COMMUNITYCTL->getAllItemsFromDb();
//    UNDER_DEV(tr("Xuất danh sách nữ tu ra tập tin"));
    doExportListItems(&list, KModelNameCommunity, COMMUNITYCTL, "Danh sách cộng đoàn", ExportType::EXPORT_XLSX);
    RELEASE_LIST_DBMODEL(list);
}


void MainWindow::on_action_About_triggered()
{
    DlgAbout* dlg = new DlgAbout(this);
    dlg->exec();
    delete dlg;
}


void MainWindow::on_action_Backup_triggered()
{
    tracein;
//    UNDER_DEV(tr("Sao lưu dữ liệu"));
    // TODO: implement it
    QString fpath = DialogUtils::saveFileDialog(this, tr("Sao lưu dữ liệu"),
                                  QString("saoluu.zip"),
                                  QString("zip (*.zip)")
                                  );
    if (!fpath.isEmpty()) {
        logi("Backup file to '%s'", STR2CHA(fpath));
        ErrCode err = BACKUP->backup(fpath);
        logi("Backup file result=%d", err);
        if (err == ErrNone) {
            DialogUtils::showMsgBox(QString(tr("Sao lưu file vào: %1")).arg(fpath));
        } else {
            DialogUtils::showErrorBox(QString(tr("Sao lưu file thất bại, mã lỗi %1")).arg(err));
        }
    }
    traceout;
}


void MainWindow::on_actionRestore_triggered()
{
    tracein;
    UNDER_DEV(tr("Phục hồi dữ liệu đã sao lưu"));
    // TODO: implement it

}


void MainWindow::on_actionRevert_triggered()
{
    tracein;
    UNDER_DEV(tr("Khôi phục dữ liệu (khôi phục dữ liệu cũ, trong trường hợp dữ liệu hiện tại bị lỗi)"));
    // TODO: implement it

}


void MainWindow::on_action_Help_triggered()
{
    tracein;
    UNDER_DEV(tr("Hướng dẫn sử dụng phần mềm"));
    // TODO: implement it

}

void MainWindow::on_actionEthnic_triggered()
{
    tracein;
//    UNDER_DEV(tr("Dân tộc"));
    // TODO: implement it
    switchView(VIEW_ETHNIC);
}

void MainWindow::on_actionWork_triggered()
{

    tracein;
    switchView(VIEW_WORK);
    // TODO: implement it

}

void MainWindow::on_actionEducation_triggered()
{

    tracein;
//    UNDER_DEV(tr("Giáo dục"));
    // TODO: implement it
    switchView(VIEW_EDUCATION);
}

void MainWindow::on_actionSpeclialist_triggered()
{
    tracein;
//    UNDER_DEV(tr("Chuyên môn"));

    switchView(VIEW_SPECIALIST);
}

void MainWindow::on_actionMisson_triggered()
{
    tracein;
//    UNDER_DEV(tr("Nhiệm vụ"));
    // TODO: implement it
    switchView(VIEW_MISSION);
    traceout;
}

void MainWindow::on_actionProvince_triggered()
{
    tracein;
    UNDER_DEV(tr("Tỉnh/Thành phố/Bang"));
    // TODO: implement it
}

void MainWindow::on_actionCountry_triggered()
{
    tracein;
//    UNDER_DEV(tr("Quốc gia"));
    // TODO: implement it

    switchView(VIEW_COUNTRY);
}


void MainWindow::on_actionBack_triggered()
{
    tracein;
    // TODO: implemen this
    // TODO: how about forward/redo???? show we support it?
//    popViewFromStackAndShow();
//    UNDER_DEV(tr("Quay lại màn hình trước"));
    BaseView* view = popViewFromStackAndShow();
    if (view) {
        switchView(view, true);
    }
    traceout;
}


void MainWindow::on_actionLogFile_triggered()
{
    QString logDir = Logger::logDirPath();
    logd("Open log dir=%s", STR2CHA(logDir));
    QDesktopServices::openUrl(QUrl::fromLocalFile(logDir));
}


void MainWindow::on_actionSummarizeReport_triggered()
{
    // BAO CAO TONG QUAN HOI DONG
    tracein;
    // TODO: implemen this
    UNDER_DEV(tr("Báo cáo tổng quan Hội dòng, xuất báo cáo ra tập tin"));

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

