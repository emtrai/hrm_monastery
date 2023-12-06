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
#include "datetimeutils.h"
#include "dialog/dlgaddpersonevent.h"
#include "stringdefs.h"
#include "dialogutils.h"

UIPersonEventListView::UIPersonEventListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;
}


UIPersonEventListView::~UIPersonEventListView()
{
    tracein;
    traceout;
}

QString UIPersonEventListView::getName()
{
    return "UIPersonEventListView";
}

Person *UIPersonEventListView::person() const
{
    return (Person*) parentModel();
}

ErrCode UIPersonEventListView::setPerson(const Person *person)
{
    traced;
    return setParentModel(person);
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
    Person* per = person();
    return QString(tr("Danh sách sự kiện của nữ tu: %1")).arg(
        per?per->displayName():tr("Không rõ"));
}

ModelController *UIPersonEventListView::getController()
{
    return PERSONCTL;
}

QString UIPersonEventListView::getMainModelName()
{
    return KModelNamePersonEvent;
}

DbModel *UIPersonEventListView::onCreateDbModelObj(const QString &modelName)
{
    UNUSED(modelName);
    return PersonEvent::build();
}

ErrCode UIPersonEventListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNamePersonEvent)) {
        loge("No item found, or not expected model '%s'", MODELSTR2CHA(item));
        err = ErrInvalidModel;
    }
    if (err == ErrNone) {
        logd("updateItem '%s'", item?STR2CHA(item->modelName()):"");
        PersonEvent* per = (PersonEvent*) item;
        tblItem->addValue(per->nameId());
        tblItem->addValue(per->eventName());
        tblItem->addValue(per->name());
        tblItem->addValue(per->dateString());
        tblItem->addValue(per->endDateString());
        tblItem->addValue(per->remark());
    }
    traceret(err);
    return err;
}

ErrCode UIPersonEventListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    ErrCode err = ErrNone;
    Person* per = person();
    DlgAddPersonEvent * dlg = DlgAddPersonEvent::build(this, true);
    if (!dlg) {
        loge("Open dlg DlgAddPersonEvent fail, No memory");
        err = ErrNoMemory;
    }
    if (err == ErrNone && !per){
        loge("invalid per model");
        err = ErrInvalidModel;
    }
    if (err == ErrNone) {
        dlg->setPerson(per);

        if (dlg->exec() == QDialog::Accepted){
            logd("reload data");
            requestReload();
        }
    }
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

ErrCode UIPersonEventListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    const DbModel* model = nullptr;
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
            return ErrNoMemory;
        }
        dlg->setPerson(person());

        if (dlg->exec() == QDialog::Accepted){
            logd("reload data");
            requestReload();
        }
        delete dlg;
    }
    if (err != ErrNone) {
        DialogUtils::showErrorBox((int)err, QString(tr("Không thể chỉnh sửa sự kiện")));
    }
    traceret(err);
    return err;
}

QList<DbModel *> UIPersonEventListView::getListDbModels()
{
    tracein;
    QList<DbModel*> items;
    ErrCode err = ErrNone;
    Person* per = person();
    if (!per) {
        err = ErrInvalidModel;
        loge("invalid per model");
    }
    if (err == ErrNone) {
        err = PERSONCTL->getListEvents(per->uid(), items);
    }
    if (err != ErrNone) {
        loge("Falied to get list event of person '%s', err=%d",
             MODELSTR2CHA(per), err);
    }
    traceout;
    return items;
}
