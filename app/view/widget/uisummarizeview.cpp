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
 * Filename: uisummarizeview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/4/2022
 * Brief:
 */
#include "uisummarizeview.h"
#include "ui_uisummarizeview.h"
#include "filectl.h"
#include "utils.h"

#define GENERAL_STAT_FNAME "general_stat.html"

// TODO: Currently, skip this view
// TODO: implement this view, to show charts, etc.
UISummarizeView::UISummarizeView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UISummarizeView)
{
    ui->setupUi(this);
    ui->txtSummarize->clearHistory();
    ui->txtSummarize->clear();

    // TODO: load all memory, consume much memory and performance
    ui->txtSummarize->setSearchPaths(QStringList(FileCtl::getAppDataDir()));

    // get home.html file from install dir first, if not exist, load from prebuilt folder
    // which is copied from app's resource

    mGeneralStatFpath = FileCtl::getAppDataDir(GENERAL_STAT_FNAME);
    logd("mGeneralStatFpath '%s'", STR2CHA(mGeneralStatFpath));

//    QObject::connect(this, SIGNAL(load()), this, SLOT(onLoad()));
}

UISummarizeView::~UISummarizeView()
{
    delete ui;
}

void UISummarizeView::showEvent(QShowEvent *ev)
{
    tracein;
    QFrame::showEvent(ev);


    traceout;
}

void UISummarizeView::on_btnExport_clicked()
{

}

void UISummarizeView::onLoad()
{

    if (QFile::exists(mGeneralStatFpath)) {
        ui->txtSummarize->setHtml(Utils::readAll(mGeneralStatFpath));
    }
}

