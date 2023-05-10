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
 * Filename: uipersoneventlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:5/7/2023
 * Brief:
 */
#include "uipersoneventlistview.h"

#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "dbpersonmodelhandler.h"
#include "person.h"
#include "personctl.h"
#include "utils.h"
#include "dialog/dlgaddpersonevent.h"
#include "stringdefs.h"

UIPersonEventListView::UIPersonEventListView(QWidget *parent):
    UICommonListView(parent),
    mPerson(nullptr)
{
    tracein;
}


UIPersonEventListView::~UIPersonEventListView()
{
    tracein;
    if (mPerson) {
        delete mPerson;
        mPerson = nullptr;
    }
    traceout;
}

void UIPersonEventListView::loadPersonEvent(const QString &personUid, bool isActive)
{
    tracein;

}

ErrCode UIPersonEventListView::onLoad()
{
    tracein;
    ErrCode err = ErrNone;
    if (mPerson != nullptr) {
        setTitle(getTitle());
        logd("Load person list of mPerson '%s'", MODELSTR2CHA(mPerson));
        QList<DbModel*> items;
        err = PERSONCTL->getListEvents(mPerson->uid(), items);
        if (err != ErrNone) {
            loge("Falied to get list event of person '%s', err=%d",
                 MODELSTR2CHA(mPerson), err);
        }
        RELEASE_LIST_DBMODEL(mItemList);
        mItemList.append(items);
    } else {
        loge("Nothing to load");
    }
    return err;
}

Person *UIPersonEventListView::person() const
{
    return mPerson;
}

void UIPersonEventListView::setPerson(const Person *person)
{
    tracein;
    if (mPerson) {
        logd("Delete old person");
        delete mPerson;
        mPerson = nullptr;
    }
    if (person) {
        mPerson = (Person*)((DbModel*)person)->clone();
    } else {
        loge("No person");
    }
    traceout;
}

void UIPersonEventListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_EVENT);
    mHeader.append(STR_TITLE);
    mHeader.append(STR_DATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_NOTE);
    traceout;
}

QString UIPersonEventListView::getTitle()
{

    return QString(tr("Danh sách sự kiện của nữ tu: %1")).arg(
        mPerson?mPerson->displayName():tr("Không rõ"));
}

void UIPersonEventListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    loge("updateItem '%s'", item?STR2CHA(item->modelName()):"");
    if (item && item->modelName() == KModelNamePersonEvent) {
        PersonEvent* per = (PersonEvent*) item;
        tblItem->addValue(per->nameId());
        tblItem->addValue(per->eventName());
        tblItem->addValue(per->name());
        tblItem->addValue(Utils::date2String(per->date()));
        tblItem->addValue(Utils::date2String(per->endDate()));
        tblItem->addValue(per->remark());
    } else {
        loge("No item found, or not expected model '%s'", MODELSTR2CHA(item));
    }
    traceout;
}

void UIPersonEventListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    DlgAddPersonEvent * dlg = DlgAddPersonEvent::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        return;
    }
    dlg->setPerson(mPerson);

    if (dlg->exec() == QDialog::Accepted){
        logd("reload data");
        reload();
    }
    delete dlg;
    traceout;
}

void UIPersonEventListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    DbModel* model = nullptr;
    ErrCode err = ErrNone;
    if (!item) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        model = item->itemData();
        if (!model) {
            err = ErrNoData;
            loge("No item data");
        }
    }
    if (err == ErrNone) {
        DlgAddPersonEvent * dlg = DlgAddPersonEvent::build(this, true, KModelNamePersonEvent, model, this);
        if (dlg == nullptr) {
            loge("Open dlg DlgAddPersonEvent fail, No memory");
            return;
        }
        dlg->setPerson(mPerson);

        if (dlg->exec() == QDialog::Accepted){
            logd("reload data");
            reload();
        }
        delete dlg;
    }
    if (err != ErrNone) {
        Utils::showErrorBox((int)err, QString(tr("Không thể chỉnh sửa sự kiện")));
    }
    traceout;
}
