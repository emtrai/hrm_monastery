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
#include "stringdefs.h"
#include "communityperson.h"

UICommunityPersonListView::UICommunityPersonListView(QWidget *parent):
    UICommonListView(parent),
    mCommunity(nullptr)
{
    tracein;
}


UICommunityPersonListView::~UICommunityPersonListView()
{
    tracein;
    if (mCommunity) delete mCommunity;
    traceout;
}

void UICommunityPersonListView::loadCommunityPerson(const QString &communityUid, bool isActive)
{
    tracein;

}

ErrCode UICommunityPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return ErrNone; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(true);

    if (dlg->exec() == QDialog::Accepted){
        QList<DbModel *>  list = dlg->selectedItems();
        logd("Selected %d per ", list.count());
        int cnt = 0;
        if (list.count() > 0) {
            foreach(DbModel* item, list) {
                Person* per = (Person*) item;
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
    tracein;
    // TODO: handle it
    return ErrNone;
}

ErrCode UICommunityPersonListView::onMenuActionView(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    // TODO: handle it
    return ErrNone;
}


ErrCode UICommunityPersonListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;

    return ErrNone;

}

ErrCode UICommunityPersonListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    return ErrNone;

}

QList<UITableMenuAction *> UICommunityPersonListView::getMenuMultiSelectedItemActions(const QMenu *menu,
                                                                              const QList<UITableItem *>& items)
{
    tracein;
//    logd("idx %d", idx);
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiSelectedItemActions(menu, items);

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
    tracein;
    ErrCode err = ErrNone;
    if (mCommunity != nullptr) {
        setTitle(getTitle());
        logd("Load person list of community '%s'", STR2CHA(mCommunity->toString()));
        QList<CommunityPerson*> items;
        err = COMMUNITYCTL->getListCommunityPerson(mCommunity->uid(), items);
        RELEASE_LIST_DBMODEL(mItemList);
        foreach (CommunityPerson* per, items) {
            mItemList.append((DbModel*)per); // TODO: convert it to Person????
        }
    } else {
        loge("Nothing to load");
    }
    return err;
}

Community *UICommunityPersonListView::community() const
{
    return mCommunity;
}

void UICommunityPersonListView::setCommunity(const Community *newCommunity)
{
    tracein;
    if (mCommunity) {
        logd("Delete old community");
        delete mCommunity;
        mCommunity = nullptr;
    }
    if (newCommunity) {
        mCommunity = (Community*)((DbModel*)newCommunity)->clone();
    } else {
        loge("No community");
    }
    traceout;
}

void UICommunityPersonListView::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
    mHeader.append(tr("Cộng đoàn hiện tại"));
    mHeader.append(STR_STATUS);
    mHeader.append(tr("Năm sinh"));
    mHeader.append(tr("Nơi sinh"));
    mHeader.append(tr("Ngày bổn mạng"));
    mHeader.append(tr("Ngày Nhập Dòng"));
    mHeader.append(tr("Ngày Tiên Khấn"));
    mHeader.append(tr("Ngày Vĩnh Khấn"));
    mHeader.append(tr("Lớp khấn"));
    mHeader.append(tr("Ngày an nghỉ"));
    mHeader.append(tr("Chuyên môn"));
    mHeader.append(tr("Công tác xã hội"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    traceout;
}

QString UICommunityPersonListView::getTitle()
{

    return QString(tr("Danh sách nữ tu của cộng đoàn: %1")).arg(mCommunity?mCommunity->name():tr("Không rõ"));
}

void UICommunityPersonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    CommunityPerson* commper = nullptr;
    Person* per  = nullptr;
    ErrCode err = ErrNone;
    if (!item || item->modelName() != KModelNameCommPerson) {
        err = ErrInvalidArg;
        loge("invalid item '%s'", MODELSTR2CHA(item));
    }
    loge("updateItem '%s'", item?STR2CHA(item->modelName()):"");
    if (err == ErrNone) {
        commper = (CommunityPerson*) item;
        per = (Person*) commper->person();
        if (!per) {
            loge("no person object in communitypersom");
            err = ErrInvalidData;
        }
    }
    if (err == ErrNone) {
        tblItem->addValue(per->nameId());
        tblItem->addValue(per->hollyName());
        tblItem->addValue(per->getFullName());
        tblItem->addValue(per->communityName());
        tblItem->addValue(commper->modelStatusName());
        tblItem->addValue(Utils::date2String(per->birthday()));
        tblItem->addValue(per->birthPlace());
        tblItem->addValue(Utils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

        tblItem->addValue(Utils::date2String(per->joinDate()));
        tblItem->addValue(Utils::date2String(per->vowsDate()));
        tblItem->addValue(Utils::date2String(per->eternalVowsDate()));
        tblItem->addValue(per->courseName());
        tblItem->addValue(Utils::date2String(per->deadDate()));
        tblItem->addValue(per->tel().join(";"));
        tblItem->addValue(per->email().join(";"));
        tblItem->addValue(per->specialistNameList().join(","));
        tblItem->addValue(per->currentWorkName());
    }
    logd("err = %d", err);
    // TODO: show dialog/report issue when invalid info?

    traceout;
}
