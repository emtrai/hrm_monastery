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
 * Filename: dlgsearchperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/20/2022
 * Brief:
 */
#include "dlgsearchperson.h"
#include "ui_dlgsearchperson.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "person.h"

DlgSearchPerson::DlgSearchPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSearchPerson),
    mPerson(nullptr)
{
    traced;
    ui->setupUi(this);
    QStringList header;
    // TODO: translation
    header.append(tr("ID"));
    header.append(tr("Tên Thánh"));
    header.append(tr("Họ tên"));
    header.append(tr("Năm sinh"));
    header.append(tr("Nơi sinh"));
    header.append(tr("Cộng đoàn"));
    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(header.count());
    //    ui->tblCommunityList->setMinimumWidth(500);
    ui->tblList->setMinimumHeight(500); // TODO: define default fix?

    ui->tblList->setHorizontalHeaderLabels(header);

}

DlgSearchPerson::~DlgSearchPerson()
{
    traced;
    delete ui;
}



void DlgSearchPerson::on_btnSearch_clicked()
{
    traced;
    QTableWidget* tbl = ui->tblList;
    QString name = ui->txtName->text().trimmed();
    mPerson = nullptr;
    if (!name.isEmpty()) {
        tbl->clearContents();
        tbl->model()->removeRows(0, tbl->rowCount());
        mListPerson.clear();
        mListPerson = INSTANCE(PersonCtl)->searchPersonByName(name);
        logd("Found %d per", mListPerson.count());
        if (mListPerson.count() > 0) {

            qint32 col = 0;
            qint32 idx = tbl->rowCount();

            foreach(Person* per, mListPerson) {
                col = 0;
                if (per == nullptr) {
                    continue;
                }
                logd("idx=%d", idx);
                per->dump();
                tbl->insertRow(idx);
                tbl->setItem(idx, col++, new QTableWidgetItem(per->uid()));
                tbl->setItem(idx, col++, new QTableWidgetItem(per->hollyName()));
                tbl->setItem(idx, col++, new QTableWidgetItem(per->getFullName()));
                tbl->setItem(idx, col++, new QTableWidgetItem(Utils::date2String(per->birthday())));
                tbl->setItem(idx, col++, new QTableWidgetItem(per->birthPlace()));
                idx++;
            }
        } else {
            logi("No person");
        }
    } else {
        loge("Nothing to search");
        Utils::showErrorBox(tr("Nhập tên để tìm")); // TODO: translation
    }
}

Person *DlgSearchPerson::person() const
{
    return mPerson;
}

void DlgSearchPerson::accept()
{
    traced;
    // TODO: validate data
    QTableWidget* tbl = ui->tblList;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    logd("Selection count %d", selection.count());
    // only select one
    if (selection.count() > 0) {
        QModelIndex index = selection.at(0);
        mPerson = mListPerson.at(index.row());
        if (mPerson == nullptr) {
            loge("No person found, what's wrong???");
            // TODO: handle this error special case
        }
    } else {
        loge("Nothing to select");
        // TODO:show warning dialog???
    }

}

