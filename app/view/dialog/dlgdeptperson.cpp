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
 * Filename: dlgdeptperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#include "dlgdeptperson.h"
#include "persondept.h"
#include "ui_dlgdeptperson.h"
#include "logger.h"
#include "personctl.h"
#include "utils.h"
#include "person.h"
#include "coursectl.h"
#include "rolectl.h"
#include "dialog/dlgsearchperson.h"

DlgDeptPerson::DlgDeptPerson(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgDeptPerson)
{
    tracein;
    ui->setupUi(this);
//    QStringList header;
//    // TODO: translation
//    header.append(tr("ID"));
//    header.append(tr("Tên Thánh"));
//    header.append(tr("Họ tên"));
//    header.append(tr("Năm sinh"));
//    header.append(tr("Nơi sinh"));
//    header.append(tr("Cộng đoàn"));
//    ui->tblList->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tblList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

//    ui->tblList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

//    ui->tblList->setShowGrid(true);
//    ui->tblList->setColumnCount(header.count());
////    ui->tblList->setMinimumHeight(500); // TODO: define default fix?

//    ui->tblList->setHorizontalHeaderLabels(header);

    loadCourse();
    loadRole();
    loadStatus();
}

DlgDeptPerson::~DlgDeptPerson()
{
    RELEASE_LIST_DBMODEL(mSelectedPersons);
    RELEASE_LIST_DBMODEL(mListPerson);
    delete ui;
}

void DlgDeptPerson::setupUI()
{
    traced;
}

ErrCode DlgDeptPerson::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    PersonDept* perdep = (PersonDept*) model;
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        perdep->setMarkModified(true); // start marking fields which are modified
    }
    perdep->setModelStatus(MODEL_ACTIVE);

//    if (mCommDept) perdep->setCommDeptUid(mCommDept->uid());
    SET_VAL_FROM_TEXTBOX(ui->txtSearch, KItemUid, perdep->setPersonUid, perdep->setPersonName);
    SET_VAL_FROM_CBOX(ui->cbRole, perdep->setRoleUid, perdep->setRoleName);
    SET_VAL_FROM_CBOX(ui->cbTerm, perdep->setCourseUid, perdep->setCourseName);
    SET_INT_VAL_FROM_CBOX(ui->cbStatus, perdep->setModelStatus, perdep->setModelStatusName);
    SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, perdep->setStartDate);
    SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, perdep->setEndDate);
    QString nameid = ui->txtNameId->text().trimmed();
    if (!nameid.isEmpty()) {
        perdep->setNameId(nameid);
    }
    traceret(err);
    return err;
}

ErrCode DlgDeptPerson::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNamePersonDept) {
        err = DlgCommonEditModel::fromModel(item);
        PersonDept* comm = (PersonDept*)model();
        if (err == ErrNone) {
            Utils::setSelectItemComboxByData(ui->cbTerm, comm->courseUid());
            Utils::setSelectItemComboxByData(ui->cbRole, comm->roleUid());
            Utils::setSelectItemComboxByData(ui->cbStatus, comm->modelStatus());
            ui->txtStartDate->setText(Utils::date2String(comm->startDate(), DEFAULT_FORMAT_YMD));
            ui->txtEndDate->setText(Utils::date2String(comm->endDate(), DEFAULT_FORMAT_YMD));
            ui->txtRemark->setPlainText(comm->remark());
            ui->btnSearch->setEnabled(false); // not allow to change person
            QString nameid = comm->nameId();
            if (!comm->personUid().isEmpty()) {
                logd("Search person uid '%s'", STR2CHA(comm->personUid()));
                Person* per = (Person*)PERSONCTL->getModelByUid(comm->personUid());
                if (per) {
                    SET_TEXTBOX_FROM_VALUE(ui->txtSearch, KItemUid,
                                           per->uid(), per->getFullName());
                    logd("found person '%s'", STR2CHA(per->toString()));
                    if (nameid.isEmpty()) {
                        nameid = QString("%1_%2").arg(comm->commDeptNameId(), per->nameId());
                        comm->setNameId(nameid);
                    }

                    delete per;
                } else {
                    loge("Not found person uid '%s'", STR2CHA(comm->personUid()));
                }
            } else {
                loge("No person uid");
            }
            ui->txtNameId->setText(comm->nameId());
        }
    } else {
        err = ErrInvalidArg;
        loge("null item or invalid type");
    }
    traceret(err);
    return err;
}

void DlgDeptPerson::loadCourse()
{
    tracein;
    ui->cbTerm->clear();
    QList<DbModel*> listCourse = COURSECTL->getAllItemsFromDb(); // TODO: should call getAllItem???
    foreach(DbModel* item, listCourse){
        ui->cbTerm->addItem(item->name(), item->uid());
    }

}

void DlgDeptPerson::loadRole()
{
    ui->cbRole->clear();
    QList<DbModel*> list = INSTANCE(RoleCtl)->getAllItemsFromDb(); // TODO: getAllItem???
    foreach(DbModel* item, list){
        ui->cbRole->addItem(item->name(), item->uid());
    }
}

void DlgDeptPerson::loadStatus()
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

//void DlgDeptPerson::showEvent(QShowEvent *event)
//{
//    tracein;
//    QDialog::showEvent(event);

//}

//void DlgDeptPerson::accept()
//{
//    tracein;
//    ErrCode ret = ErrNone;
//    // TODO: validate data
//    QTableWidget* tbl = ui->tblList;
//    QItemSelectionModel* selectionModel = tbl->selectionModel();
//    QModelIndexList selection = selectionModel->selectedRows();
//    logd("Selection count %d", (int)selection.count());
//    RELEASE_LIST_DBMODEL(mSelectedPersons);
//    // only select one
//    if (selection.count() > 0) {
//        for(int i=0; i< selection.count(); i++)
//        {
//            QModelIndex index = selection.at(i);
//            // TODO: validate value
//            Person* per =  (Person*)mListPerson.at(index.row());
//            PersonDept* perdep = (PersonDept*)PersonDept::build();
//            perdep->setPersonUid(per->uid());
//            perdep->setPersonName(per->getFullName());
//            SET_VAL_FROM_CBOX(ui->cbRole, perdep->setRoleUid, perdep->setRoleName);
//            SET_VAL_FROM_CBOX(ui->cbTerm, perdep->setCourseUid, perdep->setCourseName);
//            SET_INT_VAL_FROM_CBOX(ui->cbStatus, perdep->setModelStatus, perdep->setModelStatusName);
//            SET_DATE_VAL_FROM_WIDGET(ui->txtStartDate, perdep->setStartDate);
//            SET_DATE_VAL_FROM_WIDGET(ui->txtEndDate, perdep->setEndDate);
//            mSelectedPersons.append(perdep);
//        }
//        if (isSelfSave()) {
//            foreach (DbModel* item, mSelectedPersons) {
//                PersonDept* dept = (PersonDept*) item;
//                if (dept != nullptr) {
//                    dept->setModelStatus(MODEL_ACTIVE);
//                    dept->setCommDeptUid(mCommDeptUid);
//                    dept->dump();
//                    logd("Save item to db");

//                    if (mIsNew) {
//                        logd("Save it");
//                        ret = dept->save();
//                    } else {
//                        logd("Update it");
//                        ret = item->update();

//                    }
//                    logi("Save item result %d", ret); // TODO: check return value if one fail, what happend???
//                    if (ret != ErrNone){
//                        loge("Save '%s' to db failed", STR2CHA(dept->toString()));
//                        loge("Add per %s to dep %s failed %d", STR2CHA(dept->personName()),
//                             STR2CHA(dept->uid()), ret);
//                        break;
//                    }
//                }
//            }
//        } else {
//            logd("not mIsSelfSave, just call accept");
//        }

//        if (mListener) {
//            logd("Call listener");
//            mListener->onDbModelReady(ret, nullptr, this);
//        } else {
//            logd("no listener to call");
//        }
//    } else {
//        loge("Nothing to select");
//        // TODO:show warning dialog???
//    }
//    QDialog::accept();
//}

void DlgDeptPerson::on_btnSearch_clicked()
{
//    tracein;
//    ErrCode err = ErrNone;
//    QTableWidget* tbl = ui->tblList;
//    QString name = ui->txtSearch->text().trimmed();
//    RELEASE_LIST_DBMODEL(mSelectedPersons);
//    if (!name.isEmpty()) {
//        QList<DbModel*> list;
//        err = INSTANCE(PersonCtl)->search(name, &list);

//        if (err == ErrNone) {
//            logd("found %d item", list.size());
//            updatePeopleList(list);
//        } else {
//            loge("search failed, err=%d", err);
//            Utils::showErrorBox(QString(tr("Tìm kiếm lỗi, mã lỗi %1")).arg(err));
//        }
//    } else {
//        loge("Nothing to search");
//        Utils::showErrorBox(tr("Nhập tên để tìm")); // TODO: translation
//    }
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(false);

    if (dlg->exec() == QDialog::Accepted){
        Person* per = (Person*)dlg->selectedItem();
        if (per != nullptr) {
            ui->txtSearch->setText(per->getFullName());
            logd("setProperty %s", per->uid().toStdString().c_str());
            ui->txtSearch->setProperty(KItemUid, per->uid());
            QString nameid = QString("%1_%2").arg(mCommDeptNameId, per->nameId());
            ui->txtNameId->setText(nameid);
            delete per;
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}

void DlgDeptPerson::setCommDeptNameId(const QString &newCommDeptNameId)
{
    mCommDeptNameId = newCommDeptNameId;
}

void DlgDeptPerson::setCommDeptUid(const QString &newCommDeptUid)
{
    mCommDeptUid = newCommDeptUid;
}

//void DlgDeptPerson::setCommDept(DbModel *newCommDept)
//{
//    tracein;
//    if (mCommDept) {
//        delete mCommDept;
//        mCommDept = nullptr;
//    }
//    if (newCommDept) {
//        mCommDept = newCommDept->clone();
//    }
//    traceout;
//}

QList<DbModel *> DlgDeptPerson::selectedPersons() const
{
    QList<DbModel*> list;
    // Clone data, as data in mSelected will be deleted
    foreach (DbModel* item, mSelectedPersons) {
        list.append(item->clone());
    }
    return list;
}

DbModel *DlgDeptPerson::newModel()
{
    return PersonDept::build();
}

bool DlgDeptPerson::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        if (mModel->nameId().isEmpty()) {
            msg += tr("Thiếu mã định danh.");
            isValid = false;
            logw("lack name id");
        }
    } else {
        logw("no model to check");
        isValid = false;
    }
    logd("is valid %d", isValid);
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    traceout;
    return isValid;
}



//void DlgDeptPerson::on_btnAllPeople_clicked()
//{
//    tracein;
//    ErrCode err = ErrNone;
//    QList<DbModel*> list = INSTANCE(PersonCtl)->getAllItemsFromDb(DB_RECORD_ACTIVE, 0, 0, nullptr, &err);
//    logd("get all person err=%d", err);
//    if (err == ErrNone) {
//        logd("found %d item", list.size());
//        updatePeopleList(list);
//    } else {
//        Utils::showErrorBox(QString(tr("Lấy toàn bộ danh sách lỗi, mã lỗi %1")).arg(err));
//    }
//    traceout;
//}

//void DlgDeptPerson::updatePeopleList(const QList<DbModel *>& list)
//{
//    tracein;
//    QTableWidget* tbl = ui->tblList;
//    RELEASE_LIST_DBMODEL(mSelectedPersons);
//    tbl->clearContents();
//    tbl->model()->removeRows(0, tbl->rowCount());
//    RELEASE_LIST_DBMODEL(mListPerson);
//    mListPerson.append(list);
//    logd("Found %lld per", mListPerson.size());
//    if (mListPerson.count() > 0) {

//        qint32 col = 0;
//        qint32 idx = tbl->rowCount();

//        foreach(DbModel* item, mListPerson) {
//            Person* per = (Person*)item;
//            col = 0;
//            if (per == nullptr) {
//                continue;
//            }
//            logd("idx=%d", idx);
//            per->dump();
//            tbl->insertRow(idx);
//            tbl->setItem(idx, col++, new QTableWidgetItem(per->uid()));
//            tbl->setItem(idx, col++, new QTableWidgetItem(per->hollyName()));
//            tbl->setItem(idx, col++, new QTableWidgetItem(per->getFullName()));
//            tbl->setItem(idx, col++, new QTableWidgetItem(Utils::date2String(per->birthday())));
//            tbl->setItem(idx, col++, new QTableWidgetItem(per->birthPlace()));
//            idx++;
//        }
//    } else {
//        logi("No person");
//    }
//    traceout;
//}

//void DlgDeptPerson::setCommDeptUid(const QString &newCommDeptUid)
//{
//    mCommDeptUid = newCommDeptUid;
//}
