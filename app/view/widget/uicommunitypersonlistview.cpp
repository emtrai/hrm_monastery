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
 * Filename: uicommunitypersonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/10/2022
 * Brief:
 */
#include "uicommunitypersonlistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbcommunitymodelhandler.h"
#include "community.h"
#include "communityctl.h"
#include "utils.h"
#include "dialog/dlgsearchperson.h"

UICommunityPersonListView::UICommunityPersonListView(QWidget *parent):
    UICommonListView(parent),
    mCommunity(nullptr)
{
    traced;
}


UICommunityPersonListView::~UICommunityPersonListView()
{
    traced;
}

void UICommunityPersonListView::loadCommunityPerson(const QString &communityUid, bool isActive)
{
    traced;

}

ErrCode UICommunityPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    DlgSearchPerson * dlg = new DlgSearchPerson();
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return ErrNone; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(true);

    if (dlg->exec() == QDialog::Accepted){
        QList<Person *>  list = dlg->personList();
        logd("Selected %d per ", list.count());
        int cnt = 0;
        if (list.count() > 0) {
            foreach(Person* per, list) {
                logd("Add per %s to comm %s ", per->getFullName().toStdString().c_str(),
                     community()->name().toStdString().c_str());
                ErrCode ret = CommunityCtl::getInstance()->addPerson2Community(community(), per);
                logd("Add comm vs per ret=%d", ret);
                if (ret == ErrNone){
                    cnt ++;
                } else {
                    loge("Add per %s to comm %s failed %d", per->getFullName().toStdString().c_str(),
                         community()->name().toStdString().c_str(), ret);
                }
            }
            logd("Add %d per", cnt);
            if (cnt > 0) {
                logd("reload data");
                reload();
            }
        } else {
            logi("Nothing to add to community");
        }

    }
    delete dlg;
    return ErrNone;
}

ErrCode UICommunityPersonListView::onMenuActionDelete(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}

ErrCode UICommunityPersonListView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    traced;
    // TODO: handle it
    return ErrNone;
}


ErrCode UICommunityPersonListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    traced;

    return ErrNone;

}

ErrCode UICommunityPersonListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    traced;
    return ErrNone;

}

QList<UITableMenuAction *> UICommunityPersonListView::getMenuMultiItemActions(const QMenu *menu,
                                                                              const QList<UITableItem *>& items)
{
    traced;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);

//    actionList.append(UITableMenuAction::build(tr("Danh sách nữ tu"), this)
//                                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                               return this->onMenuActionListPerson(m, a);
//                                           }));
//    actionList.append(UITableMenuAction::build(tr("Danh sách ban"), this)
//                                          ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
//                                              return this->onMenuActionListDepartment(m, a);
//                                          }));
    return actionList;

}

ErrCode UICommunityPersonListView::onLoad()
{
//    QList<Community*> items = COMMUNITYCTL->getCommunityList();
    traced;
    if (mCommunity != nullptr) {
        logd("Load person list of ocmmunity");
        mCommunity->dump();
        QList<DbModel*> items = COMMUNITYCTL->getPersonList(mCommunity->uid());
        mItemList.clear(); // TODO: clean up item data
        mItemList.append(items);
    } else {
        loge("Nothing to load");
    }
    // TODO: loop to much, redundant, do something better?
//    foreach (Community* item, items) {
//        mItemList.append(static_cast<DbModel*>(item));
//    }
    return ErrNone;
}

Community *UICommunityPersonListView::community() const
{
    return mCommunity;
}

void UICommunityPersonListView::setCommunity(Community *newCommunity)
{
    mCommunity = newCommunity;
}

const QString &UICommunityPersonListView::communityUid() const
{
    return mCommunityUid;
}

void UICommunityPersonListView::setCommunityUid(const QString &newCommunityUid)
{
    mCommunityUid = newCommunityUid;
}

void UICommunityPersonListView::initHeader()
{
    traced;
    mHeader.append(tr("ID"));
    mHeader.append(tr("Tên"));
}

void UICommunityPersonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    Person* per = (Person*) item;
    tblItem->addValue(QString("%1").arg(item->dbId()));
    tblItem->addValue(per->getFullName());
}
