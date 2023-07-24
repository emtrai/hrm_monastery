/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: uipersoncommunitylistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:5/20/2023
 * Brief:
 */
#include "uicommunitiesofpersonlistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbcommunitymodelhandler.h"
#include "person.h"
#include "community.h"
#include "communityperson.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "dialog/dlgpersoncomm.h"
#include "stringdefs.h"
#include "mainwindow.h"
#include "dialogutils.h"

UICommunitiesOfPersonListView::UICommunitiesOfPersonListView(QWidget *parent):
    UICommonListView(parent),
    mPerson(nullptr)
{
    tracein;
}


UICommunitiesOfPersonListView::~UICommunitiesOfPersonListView()
{
    tracein;
    FREE_PTR(mPerson);
    traceout;
}

void UICommunitiesOfPersonListView::loadCommunityPerson(const QString &communityUid, bool isActive)
{
    tracein;

}

ErrCode UICommunitiesOfPersonListView::onMenuActionListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;

    return ErrNone;

}

ErrCode UICommunitiesOfPersonListView::onMenuActionListDepartment(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    return ErrNone;

}

ErrCode UICommunitiesOfPersonListView::onViewItem(UITableCellWidgetItem *item)
{
    return ErrNoData;
}

ErrCode UICommunitiesOfPersonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = DlgPersonCommunity::build(this);
    if (!dlg) {
        loge("Cannot create/build dialog, no memory?");
        err = ErrNoMemory;
    }

    if (err == ErrNone) {
        err = dlg->appendPerson(mPerson);
    }
    dlg->exec();
    if (err != ErrNone) {
        DialogUtils::showErrorBox(err, tr("Lỗi thêm dữ liệu Cộng đoàn"));
    }
    if (dlg) delete dlg;
    traceout;
    return err;
}

ErrCode UICommunitiesOfPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = nullptr;
    DbModel* model = nullptr;
    tracein;
    if (!item) {
        loge("Edit failed, null item");
        err = ErrInvalidArg;
    }
    // TODO: change this like one in UIPeopleInCommunityListView
    if (err == ErrNone) {
        model = item->itemData();
        if (!model || model->modelName() != KModelNameCommPerson) {
            loge("Edit failed, null course '%s'", MODELSTR2CHA(model));
            err = ErrInvalidData;
        }
    }
    if (err == ErrNone) {
        dlg = DlgPersonCommunity::build(this, true, KModelNameCommPerson, model);
        if (!dlg) {
            loge("Cannot create/build dialog, no memory?");
            err = ErrNoMemory;
        }
    }

    if (err == ErrNone) {
        err = dlg->appendPerson(mPerson);
    }
    dlg->exec();
    if (err != ErrNone) {
        DialogUtils::showErrorBox(err, tr("Lỗi chỉnh sửa dữ liệu Cộng đoàn"));
    }
    if (dlg) delete dlg;
    traceret(err);
    return err;
}

QList<UITableMenuAction *> UICommunitiesOfPersonListView::getMenuMultiSelectedItemActions(const QMenu *menu,
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

ErrCode UICommunitiesOfPersonListView::onLoad()
{
    tracein;
    ErrCode err = ErrNone;
    if (mPerson != nullptr) {
        setTitle(getTitle());
        logd("Load comm list of person '%s'", MODELSTR2CHA(mPerson));
        QList<CommunityPerson*> items;
        err = PERSONCTL->getListCommunityOfPerson(mPerson->uid(), items);
        setListItem<CommunityPerson>(items);
//        RELEASE_LIST_DBMODEL(mItemList);
//        mItemList.append(items);
    } else {
        loge("Nothing to load");
    }
    return err;
}

const Person *UICommunitiesOfPersonListView::person() const
{
    return mPerson;
}

ErrCode UICommunitiesOfPersonListView::setPerson(const Person *per)
{
    tracein;
    ErrCode err = ErrNone;
    logd("Delete old one if exist");
    FREE_PTR(mPerson);

    if (per) {
        logd("clone new person '%s'", MODELSTR2CHA(per));
        mPerson = (Person*)((DbModel*)per)->clone();
    } else {
        loge("No person");
        err = ErrInvalidArg;
    }
    traceout;
    return err;
}

void UICommunitiesOfPersonListView::initHeader()
{
    tracein;
    mHeader.append(STR_COMMUNITY);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_MODELSTATUS);
    traceout;
}

QString UICommunitiesOfPersonListView::getTitle()
{

    return QString(tr("Danh sách Cộng đoàn của Nữ tu : %1"))
        .arg(mPerson?mPerson->displayName():STR_UNKNOWN);
}

void UICommunitiesOfPersonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    loge("updateItem '%s'", MODELSTR2CHA(item));
    if (item && item->modelName() == KModelNameCommPerson) {
        CommunityPerson* commper = ((CommunityPerson*) item);
        Community* comm = commper->community();
        logd("commper'%s'", MODELSTR2CHA(commper));
        if (comm) {
            logd("community to to show '%s'", MODELSTR2CHA(comm));
            tblItem->addValue(comm->nameId());
            tblItem->addValue(DatetimeUtils::date2String(commper->startDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(DatetimeUtils::date2String(commper->endDate(), DEFAULT_FORMAT_MD));
            tblItem->addValue(commper->modelStatusName());
        } else {
            loge("Invalid community");
        }
    } else {
        loge("No item found, or not expected model '%s'", MODELSTR2CHA(item));
    }
    traceout;
}
