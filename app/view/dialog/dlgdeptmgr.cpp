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
 * Filename: dlgdeptmgr.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "dlgdeptmgr.h"
#include "persondept.h"
#include "ui_dlgdeptmgr.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "person.h"
#include "coursectl.h"
#include "rolectl.h"

DlgDeptMgr::DlgDeptMgr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeptMgr)
{
    tracein;
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

    ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    ui->tblList->setShowGrid(true);
    ui->tblList->setColumnCount(header.count());
//    ui->tblList->setMinimumHeight(500); // TODO: define default fix?

    ui->tblList->setHorizontalHeaderLabels(header);

    loadCourse();
    loadRole();
    loadStatus();
}

DlgDeptMgr::~DlgDeptMgr()
{
    RELEASE_LIST_DBMODEL(mSelectedPersons);
    RELEASE_LIST_DBMODEL(mListPerson);
    delete ui;
}

void DlgDeptMgr::loadCourse()
{
    tracein;
    ui->cbTerm->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItemsFromDb(); // TODO: should call getAllItem???
    foreach(DbModel* item, listCourse){
        ui->cbTerm->addItem(item->name(), item->uid());
    }

}

void DlgDeptMgr::loadRole()
{
    ui->cbRole->clear();
    QList<DbModel*> list = INSTANCE(RoleCtl)->getAllItemsFromDb(); // TODO: getAllItem???
    foreach(DbModel* item, list){
        ui->cbRole->addItem(item->name(), item->uid());
    }
}

void DlgDeptMgr::loadStatus()
{
    tracein;
    ui->cbStatus->clear();
    const QHash<int, QString>* statuses = DbModel::getModelStatusIdNameMap();
    logd("the number of status %lld", statuses->count());
    foreach (int key, statuses->keys()) {
        ui->cbStatus->addItem(statuses->value(key), key);
    }
    traceout;
}

void DlgDeptMgr::showEvent(QShowEvent *event)
{
    tracein;
    QDialog::showEvent(event);

}

void DlgDeptMgr::accept()
{
    tracein;
    // TODO: validate data
    QTableWidget* tbl = ui->tblList;
    QItemSelectionModel* selectionModel = tbl->selectionModel();
    QModelIndexList selection = selectionModel->selectedRows();
    logd("Selection count %d", (int)selection.count());
    // only select one
    if (selection.count() > 0) {
        for(int i=0; i< selection.count(); i++)
        {
            QModelIndex index = selection.at(i);
            // TODO: validate value
            Person* per =  (Person*)mListPerson.at(index.row());
            PersonDept* perdep = (PersonDept*)PersonDept::build();
            perdep->setPersonUid(per->uid());
            perdep->setPersonName(per->getFullName());
//            perdep->setPerson(per);
            // TODO: implement t
//            perdep->setCourseUid()
            mSelectedPersons.append(perdep);
            SET_VAL_FROM_CBOX(ui->cbRole, perdep->setRoleUid, perdep->setRoleName);
            SET_VAL_FROM_CBOX(ui->cbTerm, perdep->setCourseUid, perdep->setCourseName);
            SET_INT_VAL_FROM_CBOX(ui->cbStatus, perdep->setModelStatus, perdep->setModelStatusName);
            SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, perdep->setStartDate);
            SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, perdep->setEndDate);
        }
    } else {
        loge("Nothing to select");
        // TODO:show warning dialog???
    }
    QDialog::accept();
}

void DlgDeptMgr::on_btnSearch_clicked()
{
    tracein;
    ErrCode err = ErrNone;
    QTableWidget* tbl = ui->tblList;
    QString name = ui->txtSearch->text().trimmed();
    RELEASE_LIST_DBMODEL(mSelectedPersons);
    if (!name.isEmpty()) {
        tbl->clearContents();
        tbl->model()->removeRows(0, tbl->rowCount());
        RELEASE_LIST_DBMODEL(mListPerson);
        err = INSTANCE(PersonCtl)->search(name, &mListPerson);
        if (err == ErrNone) {
            logd("Found %d per", mListPerson.count());
            if (mListPerson.count() > 0) {

                qint32 col = 0;
                qint32 idx = tbl->rowCount();

                foreach(DbModel* item, mListPerson) {
                    Person* per = (Person*)item;
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
            loge("search failed, err=%d", err);
            Utils::showErrorBox(QString(tr("Tìm kiếm lỗi, mã lỗi %1")).arg(err));
        }
    } else {
        loge("Nothing to search");
        Utils::showErrorBox(tr("Nhập tên để tìm")); // TODO: translation
    }
}

QList<DbModel *> DlgDeptMgr::selectedPersons() const
{
    QList<DbModel*> list;
    // Clone data, as data in mSelected will be deleted
    foreach (DbModel* item, mSelectedPersons) {
        list.append(item->clone());
    }
    return list;
}

