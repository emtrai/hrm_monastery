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

#include "errreporterctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "person.h"
#include "community.h"
#include "communityperson.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "dialog/dlgpersoncomm.h"
#include "stringdefs.h"
#include "dialogutils.h"
#include "modeldefs.h"

UICommunitiesOfPersonListView::UICommunitiesOfPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    // there is not much items, so skipping import
    // TODO: support to import?
    mHasImportMenu = false;
    mHasExportMenu = true;
    traceout;
}


UICommunitiesOfPersonListView::~UICommunitiesOfPersonListView()
{
    tracein;
    traceout;
}

QString UICommunitiesOfPersonListView::getName()
{
    return "UICommunitiesOfPersonListView";
}

ErrCode UICommunitiesOfPersonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = DlgPersonCommunity::build(this);
    const Person* per = person();
    if (!dlg) {
        loge("Cannot create/build dialog, no memory?");
        err = ErrNoMemory;
    }

    if (!per) {
        loge("no person");
        err = ErrNoData;
    }

    if (err == ErrNone) {
        err = dlg->appendPerson(per);
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
    const DbModel* model = nullptr;
    const Person* per = person();
    tracein;
    if (!item) {
        loge("Edit failed, null item");
        err = ErrInvalidArg;
    }

    if (err == ErrNone && !per) {
        loge("no person");
        err = ErrNoData;
    }

    // TODO: change this like one in UIPeopleInCommunityListView
    if (err == ErrNone) {
        model = item->itemData();
        if (!model || model->modelName() != KModelNameCommPerson) {
            loge("Edit failed, null comm per '%s'", MODELSTR2CHA(model));
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

    if (err == ErrNone && dlg) {
        if (model) model->dump();
        dlg->exec();
    }
    if (err != ErrNone) {
        DialogUtils::showErrorBox(err, tr("Lỗi chỉnh sửa dữ liệu Cộng đoàn"));
    }
    if (dlg) delete dlg;
    traceret(err);
    return err;
}

const Person *UICommunitiesOfPersonListView::person() const
{
    return (Person*) parentModel();
}

ErrCode UICommunitiesOfPersonListView::setPerson(const Person *per)
{
    traced;
    return setParentModel(per);
}

QString UICommunitiesOfPersonListView::getMainModelName()
{
    return KModelNameCommPerson;
}

DbModel *UICommunitiesOfPersonListView::onCreateDbModelObj(const QString &modelName)
{
    UNUSED(modelName);
    return CommunityPerson::build();
}

ModelController *UICommunitiesOfPersonListView::getController()
{
    return PERSONCTL;
}

void UICommunitiesOfPersonListView::initHeader()
{
    tracein;
    mHeader.append(STR_COMMUNITY_NAMEID);
    mHeader.append(STR_COMMUNITY);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_MODELSTATUS);
    traceout;
}

QString UICommunitiesOfPersonListView::getTitle()
{
    const Person* per = person();
    return QString(tr("Danh sách Cộng đoàn của Nữ tu : %1"))
        .arg(per?per->displayName():STR_UNKNOWN);
}

ErrCode UICommunitiesOfPersonListView::fillValueTableRowItem(DbModel *item,
                                                             UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;

    if (!item || !tblItem) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }

    loge("updateItem '%s'", MODELSTR2CHA(item));
    if (err == ErrNone && item->modelName() == KModelNameCommPerson) {
        CommunityPerson* commper = ((CommunityPerson*) item);
        Community* comm = commper->community();
        logd("commper'%s'", MODELSTR2CHA(commper));
        if (comm) {
            logd("community '%s'", MODELSTR2CHA(comm));
            tblItem->addValue(comm->nameId());
            tblItem->addValue(comm->name());
            tblItem->addValue(DatetimeUtils::date2String(commper->startDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(DatetimeUtils::date2String(commper->endDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(commper->modelStatusName());
        } else {
            loge("Invalid community");
            err = ErrInvalidData;
        }
    } else {
        loge("No item found, or not expected model '%s'", MODELSTR2CHA(item));
        err = ErrInvalidModel;
    }
    traceret(err);
    return err;
}

QList<DbModel *> UICommunitiesOfPersonListView::getListDbModels()
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> ret;
    QString uid;
    const Person* per = person();
    if (!per) {
        loge("No per, nothing to load");
        err = ErrNoData;
    }

    if (err == ErrNone) {
        uid = per->uid();
        if (uid.isEmpty()) {
            err = ErrInvalidData;
            loge("Invalid per info, uid is null, comm '%s'", MODELSTR2CHA(per));
        }
    }
    if (err == ErrNone) {
        logd("Load comm list of per '%s'", MODELSTR2CHA(per));

        QList<CommunityPerson*> items;
        err = PERSONCTL->getListCommunityOfPerson(uid, items);
        if (err == ErrNone && items.size() > 0) {
            foreach (CommunityPerson* item, items) {
                ret.append((DbModel*)item);
            }
        }
    }

    if (err != ErrNone) {
        loge("Faild to get list item, err=%d", err);
        REPORTERRCTL->reportErr(
            tr("Lỗi truy vấn thông tin danh sách Cộng đoàn của Nữ tu"), err);
    }

    traceout;
    return ret;
}
