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
 * Filename: uidepartmentpersonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/17/2022
 * Brief:
 */
#include "uidepartmentpersonlistview.h"


#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbcommunitymodelhandler.h"
#include "communitydept.h"
#include "communitydeptctl.h"
#include "utils.h"
#include "dialog/dlgdeptperson.h"
#include "person.h"
#include "persondept.h"

UIDepartmentPersonListView::UIDepartmentPersonListView(QWidget *parent):
    UICommonListView(parent),
    mCommDept(nullptr)
{
    tracein;
}


UIDepartmentPersonListView::~UIDepartmentPersonListView()
{
    tracein;
    if (mCommDept) {
        delete mCommDept;
        mCommDept = nullptr;
    }
}

DbModel *UIDepartmentPersonListView::onNewModel()
{
    // TODO: handle it
    return nullptr;
}

QString UIDepartmentPersonListView::getTitle()
{
    tracein;
    QString title;
    if (mCommDept) {
        title = QString(tr("Thành viên phòng ban %1 cộng đoàn %2")).arg(mCommDept->departmentName(), mCommDept->communityName());
    } else {
        loge("no common dept");
        title = QString(tr("Thành viên phòng ban"));
    }
    traceout;
    return title;
}

void UIDepartmentPersonListView::onViewItem(UITableCellWidgetItem *item)
{

}

void UIDepartmentPersonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    DlgDeptPerson* dlg = DlgDeptPerson::build(this, true, nullptr, this);
    dlg->setCommDeptUid(mCommDept->uid());
    dlg->setCommDeptNameId(mCommDept->nameId());
//    if (mCommDept) {
//        dlg->setCommDept(mCommDept);
//        dlg->setCommDeptUid(mCommDept->uid());
//    }
    dlg->exec();
    delete dlg;
    traceout;
//    tracein;
//    DlgDeptPerson * dlg = new DlgDeptPerson();
//    if (dlg == nullptr) {
//        loge("Open dlg DlgDeptPerson fail, No memory");
//    }

//    if (dlg->exec() == QDialog::Accepted){
//        QList<DbModel*> list = dlg->selectedPersons();
//        ErrCode ret = ErrNone;
//        int cnt = 0;
//        if (list.count() > 0) {
//            foreach (DbModel* item, list) {
//                PersonDept* dept = (PersonDept*) item;
//                if (dept != nullptr) {
//                    if (mCommDept){
//                        dept->setCommDeptUid(mCommDept->uid());
//                    }
//                    dept->setModelStatus(MODEL_ACTIVE);
//                    dept->dump();
//                    logd("Save item to db");
//                    ret = dept->save();
//                    logi("Save item result %d", ret); // TODO: check return value if one fail, what happend???
//                    if (ret == ErrNone){
//                        cnt ++;
//                    } else {
//                        loge("Add per %s to dep %s failed %d", dept->personName().toStdString().c_str(),
//                             dept->uid().toStdString().c_str(), ret);
//                    }
//                }
//            }
//        } else {
//            logi("Nothing selecetd");
//        }


//        logd("Add %d per", cnt);
//        if (cnt > 0) {
//            logd("reload data");
//            reload();
//        }

////        QList<Person *>  list = dlg->personList();
////        logd("Selected %d per ", list.count());
////        int cnt = 0;
////        if (list.count() > 0) {
////            foreach(Person* per, list) {
////                logd("Add per %s to comm %s ", per->getFullName().toStdString().c_str(),
////                     community()->name().toStdString().c_str());
////                ErrCode ret = CommunityCtl::getInstance()->addPerson2Community(community(), per);
////                logd("Add comm vs per ret=%d", ret);
////                if (ret == ErrNone){
////                    cnt ++;
////                } else {
////                    loge("Add per %s to comm %s failed %d", per->getFullName().toStdString().c_str(),
////                         community()->name().toStdString().c_str(), ret);
////                }
////            }
////            logd("Add %d per", cnt);
////            if (cnt > 0) {
////                logd("reload data");
////                reload();
////            }
//        } else {
//            logi("Nothing to add to department");
//        }

////    }
//    delete dlg;
}

void UIDepartmentPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    DbModel* model = item->itemData();
    if (model){
        PersonDept* per = (PersonDept*)model;
        DlgDeptPerson* dlg = DlgDeptPerson::build(this, true, per, this);
        dlg->setCommDeptUid(mCommDept->uid());
        dlg->setCommDeptNameId(mCommDept->nameId());
//        dlg->setCommDeptUid(mCommDept->uid());
//        dlg->setCommDept(mCommDept);
        dlg->exec();
        delete dlg;

    } else {
        loge("Invalid item data");
        // TODO: popup message???
    }
    traceout;
}


ErrCode UIDepartmentPersonListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;

    return ErrNone;

}

ErrCode UIDepartmentPersonListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    return ErrNone;

}

QList<UITableMenuAction *> UIDepartmentPersonListView::getMenuMultiSelectedItemActions(const QMenu *menu,
                                                                                const QList<UITableItem *>& items)
{
    tracein;
    //    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiSelectedItemActions(menu, items);

    //    actionList.append(UITableMenuAction::build(tr("Danh sách ban"), this)
    //                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
    //                                              return this->onMenuActionListDepartment(m, a);
    //                                          }));
    return actionList;

}

ErrCode UIDepartmentPersonListView::onLoad()
{
    //    QList<Community*> items = COMMUNITYCTL->getCommunityList();
    tracein;
    if (mCommDept != nullptr) {
        logd("Load person list of department");
        mCommDept->dump();

        QList<DbModel*> items = COMMUNITYDEPTCTL->getListPerson(mCommDept->uid());
        RELEASE_LIST_DBMODEL(mItemList);
        mItemList.append(items);
    } else {
        loge("Nothing to load");
    }
    // TODO: loop to much, redundant, do something better?
    //    foreach (Community* item, items) {
    //        mItemList.append(static_cast<DbModel*>(item));
    //    }
    traceout;
    return ErrNone;
}


void UIDepartmentPersonListView::setCommDept(CommunityDept *commDept)
{
    tracein;
    if (mCommDept) {
        delete mCommDept;
        mCommDept = nullptr;
    }
    if (commDept) {
        logd("clone new one");
        mCommDept = (CommunityDept*)commDept->clone();
    }
    traceout;
}


void UIDepartmentPersonListView::initHeader()
{
    tracein;
    mHeader.append(tr("Mã Định Danh"));
    mHeader.append(tr("Mã Định Danh Nữ Tu"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Vai trò"));
    mHeader.append(tr("Trạng thái"));
    mHeader.append(tr("Nhiệm kỳ"));
    mHeader.append(tr("Ngày bắt đầu"));
    mHeader.append(tr("Ngày kết thúc"));
}

void UIDepartmentPersonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    PersonDept* model = (PersonDept*) item;
    tblItem->addValue(model->nameId());
    tblItem->addValue(model->personNameId());
    tblItem->addValue(model->personName());
    tblItem->addValue(model->roleName());
    tblItem->addValue(DbModel::modelStatus2Name((DbModelStatus)model->modelStatus()));
    tblItem->addValue(model->courseName());
    tblItem->addValue(Utils::date2String(model->startDate(), DEFAULT_FORMAT_YMD));
    tblItem->addValue(Utils::date2String(model->endDate(), DEFAULT_FORMAT_YMD));
    traceout;
}
