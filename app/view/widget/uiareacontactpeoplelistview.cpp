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
 * Filename: uiareacontactpeople.cpp
 * Author: Anh, Ngo Huy
 * Created date:4/16/2023
 * Brief:
 */
#include "uiareacontactpeoplelistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbareamodelhandler.h"
#include "community.h"
#include "areactl.h"
#include "utils.h"
#include "dialog/dlgsearchperson.h"
#include "dialog/dlgareaperson.h"
#include "area.h"
#include "areaperson.h"
#include "errreporterctl.h"
#include "stringdefs.h"

UIAreaContactPeopleListView::UIAreaContactPeopleListView(QWidget *parent):
    UICommonListView(parent),
    mArea(nullptr)
{
    tracein;
}


UIAreaContactPeopleListView::~UIAreaContactPeopleListView()
{
    tracein;
    if (mArea) delete mArea;
    traceout;
}

Area *UIAreaContactPeopleListView::area() const
{
    return mArea;
}

void UIAreaContactPeopleListView::setArea(const Area *newArea)
{
    tracein;
    if (mArea) {
        logd("Delete old area");
        delete mArea;
        mArea = nullptr;
    }
    if (newArea) {
        mArea = (Area*)((DbModel*)newArea)->clone();
        setTitle(getTitle());
    } else {
        loge("No area");
    }
    traceout;
}

void UIAreaContactPeopleListView::initHeader()
{
    tracein;
    mHeader.append(STR_PERSON_NAMEID);
    mHeader.append(STR_HOLLYNAME);
    mHeader.append(STR_FULLNAME);
    mHeader.append(STR_ROLE);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_COURSE);
    traceout;
}

QString UIAreaContactPeopleListView::getTitle()
{

    return QString(tr("Danh sách Nữ tu liên lạc của khu vực: %1"))
            .arg(mArea?mArea->name():STR_UNKNOWN);
}

void UIAreaContactPeopleListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    loge("updateItem '%s'", item?STR2CHA(item->modelName()):"");
    if (item && item->modelName() == KModelNameAreaPerson) {
        AreaPerson* per = (AreaPerson*) item;
        tblItem->addValue(per->personNameId());
        tblItem->addValue(per->hollyName());
        tblItem->addValue(per->personName());
        tblItem->addValue(per->roleName());
        tblItem->addValue(per->personTel());
        tblItem->addValue(per->personEmail());
        tblItem->addValue(per->courseName());
    } else {
        loge("No item found, or not expected model '%s'", item?STR2CHA(item->modelName()):"");
    }
    traceout;
}

QList<DbModel *> UIAreaContactPeopleListView::getListItem()
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> items;
    if (mArea != nullptr) {
        logd("Load person list of area '%s'", STR2CHA(mArea->toString()));
        err = AREACTL->getContactPeopleList(mArea->uid(), items);
        if (err != ErrNone) {
            loge("Failed to get contact people list for area '%s', err=%d",
                 STR2CHA(mArea->toString()), err);
            REPORTERRCTL->reportErr(tr("Lỗi truy vấn thông tin người liên lạc cho khu vực"), err);
        }
    } else {
        loge("Nothing to load");
    }
    traceout;
    return items;
}

void UIAreaContactPeopleListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    DlgAreaPerson* dlg = DlgAreaPerson::build(this, true, nullptr, this);
    dlg->setArea(mArea);
    dlg->exec();
    delete dlg;
    traceout;
}

void UIAreaContactPeopleListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    if (item) {
        DbModel* model = item->itemData();
        if (model) {
            DlgAreaPerson* dlg = DlgAreaPerson::build(this, true, model, this);
            dlg->setArea(mArea);
            dlg->exec();
            delete dlg;
        } else {
            loge("Edit failed, null area");
        }
    } else {
        loge("Edit failed, null item");
    }
    traceout;
}

void UIAreaContactPeopleListView::onDeleteItem(const QList<UITableItem *> &selectedItems)
{

}

void UIAreaContactPeopleListView::onViewItem(UITableCellWidgetItem *item)
{

}

