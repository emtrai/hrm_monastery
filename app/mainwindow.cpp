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

#include "view/dialog/dlgimportpersonlistresult.h"
#include "personctl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSummarizeView(nullptr)
    , mCommunityView(nullptr)
    , mPersonView(nullptr)
    , mCurrentView(nullptr)
{
    ui->setupUi(this);
    QToolButton *importButton = new QToolButton(this);
    importButton->setText(tr("&Nhập"));
    importButton->setPopupMode(QToolButton::InstantPopup);
    importButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/icon/icons8-import-64"), QSize(), QIcon::Normal, QIcon::On);
    importButton->setIcon(icon);

    QMenu *importMenu = new QMenu(importButton);

    mActionImportPersonList = new QAction(this);
    mActionImportPersonList->setObjectName(QString::fromUtf8("action_ImportPersonList"));
    mActionImportPersonList->setText(tr("Danh sách Nữ tu"));
    QIcon mImportPersonListIcon;
    mImportPersonListIcon.addFile(QString::fromUtf8(":/icon/icon/icons8-nun-64.png"), QSize(), QIcon::Normal, QIcon::On);
    mActionImportPersonList->setIcon(mImportPersonListIcon);
    QObject::connect(mActionImportPersonList, SIGNAL(triggered()), this, SLOT(on_action_ImportPersonList_triggered()));
    importMenu->addAction(mActionImportPersonList);

    mActionImportPerson = new QAction(this);
    mActionImportPerson->setObjectName(QString::fromUtf8("action_ImportPerson"));
    mActionImportPerson->setText(tr("Nữ tu"));
    mActionImportPerson->setIcon(mImportPersonListIcon);
//    QObject::connect(mActionImportPerson, SIGNAL(triggered()), this, SLOT(on_action_ImportPersonList_triggered()));
    importMenu->addAction(mActionImportPerson);

    mActionImportCommunityList = new QAction(this);
    mActionImportCommunityList->setObjectName(QString::fromUtf8("action_ImportCommunityList"));
    mActionImportCommunityList->setText(tr("Danh sách Cộng đoàn"));
    QIcon mImportCommunityIcon;
    mImportCommunityIcon.addFile(QString::fromUtf8(":/icon/icon/icons8-community-64 (1).png"), QSize(), QIcon::Normal, QIcon::On);
    mActionImportCommunityList->setIcon(mImportCommunityIcon);


    importMenu->addAction(mActionImportCommunityList);
    importButton->setMenu(importMenu);
    ui->toolBar->addWidget(importButton);

    mSummarizeView = new UISummarizeView();

    mCommunityView = UITableViewFactory::getView(ViewType::COMMUNITY);
    mSaintsView = UITableViewFactory::getView(ViewType::SAINT);
    mPersonView = UITableViewFactory::getView(ViewType::PERSON);
    mHomeView = new QTextBrowser(this);
    mHomeView->clearHistory();
    mHomeView->clear();

//    mHomeView->setSource(QUrl(":/data/home"), QTextDocument::ResourceType::HtmlResource);
//    mHomeView->setHtml("<h1>Hello</h1>");
//    mHomeView->setHtml(Utils::readAll(":/data/home"));
    loadHomePageFile();
    // TODO: load all memory, consume much memory and performance
    mHomeView->setSearchPaths(QStringList(FileCtl::getOrCreatePrebuiltDataDir()));
    mHomeView->setHtml(Utils::readAll(
        FileCtl::getUpdatePrebuiltDataFilePath(KPrebuiltHomeHtmlFileName, false)));

    switchView(mHomeView);
//    summarize->setWindowState(Qt::WindowState::WindowMaximized);
//    summarize->show();

    logd("init config");
    Config::init();

    logd("connect");
    LoaderCtl* loader = LoaderCtl::getInstance();
    loader->setOnFinishLoadListener(MainWindow::onFinishLoading, this);
    QObject::connect(this, SIGNAL(load()), loader, SLOT(onLoad()));
}


void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    logd("On load");

    logd("emit load");
    emit load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFinishLoading(int ret, void* data){
    traced;

}

void MainWindow::switchView(QWidget *nextView)
{
    if (mCurrentView != nullptr) {
        mCurrentView->hide();
        ui->centralwidget->layout()->replaceWidget(mCurrentView, nextView);
    }
    else{
        ui->centralwidget->layout()->addWidget(nextView);
    }
    mCurrentView = nextView;
    mCurrentView->show();
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
        ErrCode ret = INSTANCE(PersonCtl)->importFromFile(nullptr, ImportType::IMPORT_CSV_LIST, fname, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
        dlg->setup(list);
        dlg->exec();
        delete dlg;
    }

}



void MainWindow::on_action_New_triggered()
{
    traced;
//    Person* person = TestCtl::genRandomPerson();
//    person->save();

//    delete person;
    DlgPerson w;
    //    w.setWindowState(Qt::WindowState::WindowMaximized);
    w.exec();
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


void MainWindow::on_actionImportComm_triggered()
{
    traced;
    CommunityCtl::getInstance()->loadFromFile(FileCtl::getFullFilePath("config"));

}


void MainWindow::on_actionCommunity_triggered()
{
    traced;
    switchView(mCommunityView);
}


void MainWindow::on_actionSummarize_triggered()
{
    traced;
    switchView(mSummarizeView);
}


void MainWindow::on_actionSaints_2_triggered()
{
    traced;
    switchView(mSaintsView);

}


void MainWindow::on_actionHome_triggered()
{
    traced;
    switchView(mHomeView);
}


void MainWindow::on_actionPerson_triggered()
{
    traced;
    switchView(mPersonView);
}


void MainWindow::on_actionSearch_triggered()
{
    traced;
}

