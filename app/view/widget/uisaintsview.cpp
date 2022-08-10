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
 * Filename: uisaintsview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/4/2022
 * Brief:
 */
#include "uisaintsview.h"
#include "ui_uisaintsview.h"
#include "logger.h"
#include "errcode.h"
#include "saintctl.h"
#include <QHash>
#include "utils.h"

UISaintsView::UISaintsView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UISaintsView)
{
    ui->setupUi(this);

    mHeader.append("ID");
    mHeader.append("Name");
    mHeader.append("Memory Day");
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
//    ui->tblList->horizontalHeader()->setStretchLastSection(true);
//    ui->tblList->verticalHeader()->hide();
    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(mHeader.count());
    ui->tblList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500);

    ui->tblList->setHorizontalHeaderLabels(mHeader);

}

UISaintsView::~UISaintsView()
{
    delete ui;
}

void UISaintsView::showEvent(QShowEvent *ev)
{
    QFrame::showEvent(ev);
    traced;
    QTableWidget* tbl = ui->tblList;
    // TODO: is it really remove all data?
    // Is there any risk of leakage memory here??
    tbl->clearContents();
    tbl->model()->removeRows(0, tbl->rowCount());
    // TODO: clear saints???
    QList<Saint*> saints = SaintCtl::getInstance()->getListSaints();
    int idx = tbl->rowCount();
    foreach (Saint* saint, saints){
        tbl->insertRow(idx);
        tbl->setItem(idx, 0, new QTableWidgetItem(QString("%1").arg(saint->dbId())));
        tbl->setItem(idx, 1, new QTableWidgetItem(saint->name()));
        tbl->setItem(idx, 2, new QTableWidgetItem(Utils::date2String(saint->feastDay(), "M/D")));
        idx ++;
    }
}
