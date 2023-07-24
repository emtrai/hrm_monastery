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
#include "uipeopleincommunitylistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbcommunitymodelhandler.h"
#include "community.h"
#include "communityctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "dialog/dlgsearchperson.h"
#include "dialog/dlgpersoncomm.h"
#include "stringdefs.h"
#include "communityperson.h"
#include "dialogutils.h"

UIPeopleInCommunityListView::UIPeopleInCommunityListView(QWidget *parent):
    UICommonListView(parent),
    mCommunity(nullptr)
{
    tracein;
}


UIPeopleInCommunityListView::~UIPeopleInCommunityListView()
{
    tracein;
    FREE_PTR(mCommunity);
    traceout;
}

//ErrCode UICommunityPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
//{
//    tracein;
//    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
//    if (dlg == nullptr) {
//        loge("Open dlg DlgAddPersonEvent fail, No memory");
//        return ErrNone; // TODO: open dlg??
//    }
//    dlg->setIsMultiSelection(true);

//    if (dlg->exec() == QDialog::Accepted){
//        QList<DbModel *>  list = dlg->selectedItems();
//        logd("Selected %d per ", list.count());
//        int cnt = 0;
//        if (list.count() > 0) {
//            foreach(DbModel* item, list) {
//                Person* per = (Person*) item;
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
//        } else {
//            logi("Nothing to add to community");
//        }

//    }
//    delete dlg;
//    return ErrNone;
//}

ErrCode UIPeopleInCommunityListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = nullptr;
    CommunityPerson* commPer = nullptr;

    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        logd("selected idx=%d", item->idx());
        model = item->itemData();
        if (model) {
            logd("model %s", MODELSTR2CHA(model));
        } else {
            err = ErrInvalidData;
            loge("Invalid data");
        }
    }
    if (err == ErrNone) {
        if (model->modelName() == KModelNameCommPerson) {
            commPer = (CommunityPerson*) model;
        } else {
            err = ErrIncompatible;
            loge("Invalid model name, model '%s'", MODELSTR2CHA(model));
        }
    }

    if (err == ErrNone) {
        logd("open dialog to edit model '%s'", MODELSTR2CHA(model));
        err = DlgPersonCommunity::updateCommunityPerson(this, commPer);
    }
    if (err == ErrNone) {
        reload();
    } else if (err != ErrCancelled) {
        DialogUtils::showErrorBox(err, tr("Lỗi cập nhật Cộng đoàn - Nữ tu"));
    } // cancel, do nothing
    traceret(err);
    return err;
}

ErrCode UIPeopleInCommunityListView::onDeleteItem(const QList<UITableItem *> &selectedItems)
{
    // TODO: implement it
    UNDER_DEV("Delete Community Person");
    return ErrNotImpl;
}

ErrCode UIPeopleInCommunityListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    QList<Person*> perList;
    Community* comm = community();
    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        err = ErrNoMemory;
    } else {
        dlg->setIsMultiSelection(true);
    }
    if (!comm) {
        loge("No community");
        err = ErrInvalidData;
    }
    if ((err == ErrNone) && (dlg->exec() == QDialog::Accepted)){
        QList<DbModel *>  list = dlg->selectedItems();
        logd("Selected %d per ", list.count());
        if (list.count() > 0) {
            perList = CLONE_LIST_FROM_DBMODEL(list, Person);
        } else {
            logi("Nothing to add to community");
            err = ErrNoData;
        }
    } else {
        err = ErrCancelled;
    }

    if (err == ErrNone) {
        logd("Add %d people to community '%s'", perList.size(), MODELSTR2CHA(community()));
        err = DlgPersonCommunity::addListPeopleToCommunity(this,
                                                           community(),
                                                           perList);
    }

    if (err == ErrNone) {
        reload();
    } else if (err != ErrCancelled) {
        loge("failed to add item %d", err);
        DialogUtils::showErrorBox(err, tr("Lỗi thêm mục Cộng đoàn - Nữ tu"));
    }

    FREE_PTR(dlg);
    RELEASE_LIST(perList, Person);
    traceout;
    return err;
}

QList<UITableMenuAction *>
UIPeopleInCommunityListView::getMenuMultiSelectedItemActions(const QMenu *menu,
                                                           const QList<UITableItem *>& items)
{
    tracein;
    QList<UITableMenuAction*> actionList =
        UITableView::getMenuMultiSelectedItemActions(menu, items);

    return actionList;

}

ErrCode UIPeopleInCommunityListView::onLoad()
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

Community *UIPeopleInCommunityListView::community() const
{
    return mCommunity;
}

void UIPeopleInCommunityListView::setCommunity(const Community *newCommunity)
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

void UIPeopleInCommunityListView::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
    mHeader.append(tr("Năm sinh"));
    mHeader.append(tr("Nơi sinh"));
    mHeader.append(tr("Lớp khấn"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    mHeader.append(tr("Cộng đoàn hiện tại"));
    mHeader.append(STR_STATUS);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    traceout;
}

QString UIPeopleInCommunityListView::getTitle()
{

    return QString(tr("Danh sách nữ tu của cộng đoàn: %1")).arg(mCommunity?mCommunity->name():tr("Không rõ"));
}

void UIPeopleInCommunityListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
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
        tblItem->addValue(DatetimeUtils::date2String(per->birthday()));
        tblItem->addValue(per->birthPlace());
        tblItem->addValue(per->courseName());
        tblItem->addValue(per->tel().join(";"));
        tblItem->addValue(per->email().join(";"));
        tblItem->addValue(commper->modelStatusName());
        tblItem->addValue(per->communityName());
        tblItem->addValue(DatetimeUtils::date2String(commper->startDate()));
        tblItem->addValue(DatetimeUtils::date2String(commper->endDate()));
    }
    logd("err = %d", err);
    // TODO: show dialog/report issue when invalid info?

    traceout;
}
