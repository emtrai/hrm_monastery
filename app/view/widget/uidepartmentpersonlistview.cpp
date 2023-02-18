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
#include "dialog/dlgdeptmgr.h"
#include "person.h"
#include "persondept.h"

UIDepartmentPersonListView::UIDepartmentPersonListView(QWidget *parent):
    UICommonListView(parent),
    mCommDept(nullptr)
{
    traced;
}


UIDepartmentPersonListView::~UIDepartmentPersonListView()
{
    traced;
}


ErrCode UIDepartmentPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    DlgDeptMgr * dlg = new DlgDeptMgr();
    if (dlg == nullptr) {
        loge("Open dlg DlgDeptMgr fail, No memory");
        return ErrNone; // TODO: open dlg??
    }

    if (dlg->exec() == QDialog::Accepted){
        QList<PersonDept*> list = dlg->selectedPersons();
        ErrCode ret = ErrNone;
        int cnt = 0;
        if (list.count() > 0) {
            foreach (PersonDept* item, list) {
                if (item != nullptr) {
                    item->setCommDeptUid(mCommDept->uid());
                    item->dump();
                    logd("Save item to db");
                    ret = item->save();
                    logi("Save item result %d", ret); // TODO: check return value if one fail, what happend???
                    if (ret == ErrNone){
                        cnt ++;
                    } else {
                        loge("Add per %s to dep %s failed %d", item->personName().toStdString().c_str(),
                             item->uid().toStdString().c_str(), ret);
                    }
                }
            }
        } else {
            logi("Nothing selecetd");
        }


        logd("Add %d per", cnt);
        if (cnt > 0) {
            logd("reload data");
            reload();
        }

//        QList<Person *>  list = dlg->personList();
//        logd("Selected %d per ", list.count());
//        int cnt = 0;
//        if (list.count() > 0) {
//            foreach(Person* per, list) {
//                logd("Add per %s to comm %s ", per->getFullName().toStdString().c_str(),
//                     community()->name().toStdString().c_str());
//                ErrCode ret = CommunityCtl::getInstance()->addPerson2Community(community(), per);
//                logd("Add comm vs per ret=%d", ret);
//                if (ret == ErrNone){
//                    cnt ++;
//                } else {
//                    loge("Add per %s to comm %s failed %d", per->getFullName().toStdString().c_str(),
//                         community()->name().toStdString().c_str(), ret);
//                }
//            }
//            logd("Add %d per", cnt);
//            if (cnt > 0) {
//                logd("reload data");
//                reload();
//            }
        } else {
            logi("Nothing to add to department");
        }

//    }
    delete dlg;
    return ErrNone;
}

ErrCode UIDepartmentPersonListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UIDepartmentPersonListView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

DbModel *UIDepartmentPersonListView::onNewModel()
{
    // TODO: handle it
    return nullptr;
}


ErrCode UIDepartmentPersonListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;

    return ErrNone;

}

ErrCode UIDepartmentPersonListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    traced;
    return ErrNone;

}

QList<UITableMenuAction *> UIDepartmentPersonListView::getMenuMultiItemActions(const QMenu *menu,
                                                                                const QList<UITableItem *>& items)
{
    traced;
    //    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);

    //    actionList.append(UITableMenuAction::build(tr("Danh sách ban"), this)
    //                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
    //                                              return this->onMenuActionListDepartment(m, a);
    //                                          }));
    return actionList;

}

ErrCode UIDepartmentPersonListView::onLoad()
{
    //    QList<Community*> items = COMMUNITYCTL->getCommunityList();
    traced;
    if (mCommDept != nullptr) {
        logd("Load person list of department");
        mCommDept->dump();

        QList<DbModel*> items = COMMUNITYDEPTCTL->getPersonList(mCommDept->uid());
        mItemList.clear(); // TODO: clean up item data
        mItemList.append(items);
    } else {
        loge("Nothing to load");
    }
    // TODO: loop to much, redundant, do something better?
    //    foreach (Community* item, items) {
    //        mItemList.append(static_cast<DbModel*>(item));
    //    }
    tracede;
    return ErrNone;
}


void UIDepartmentPersonListView::setCommDept(CommunityDept *commDept)
{
    mCommDept = commDept;
}


void UIDepartmentPersonListView::initHeader()
{
    traced;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Vai trò"));
    mHeader.append(tr("Nhiệm kỳ"));
}

void UIDepartmentPersonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    PersonDept* model = (PersonDept*) item;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(model->personName());
    tblItem->addValue(model->roleName());
    tblItem->addValue(model->courseName());
    tracede;
}
