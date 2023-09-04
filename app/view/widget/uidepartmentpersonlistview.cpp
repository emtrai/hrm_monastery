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
#include "communitydept.h"
#include "communitydeptctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "dialog/dlgdeptperson.h"
#include "persondept.h"
#include "stringdefs.h"

UIDepartmentPersonListView::UIDepartmentPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = true;
    traceout;
}


UIDepartmentPersonListView::~UIDepartmentPersonListView()
{
    traced;
}

DbModel *UIDepartmentPersonListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return PersonDept::build();
}

QList<DbModel *> UIDepartmentPersonListView::getListDbModels()
{
    tracein;
    QList<DbModel*> items;
    CommunityDept* dept = communityDept();
    if (dept) {
        items = COMMUNITYDEPTCTL->getListPerson(dept->uid());
        logd("found %lld item", items.size());
    } else {
        loge("no dept model");
    }
    traceout;
    return items;
}

QString UIDepartmentPersonListView::getTitle()
{
    tracein;
    QString title;
    CommunityDept* dept = communityDept();
    if (dept) {
        title = QString(tr("Thành viên phòng ban %1 cộng đoàn %2")).arg(dept->departmentName(), dept->communityName());
    } else {
        loge("no common dept");
        title = QString(tr("Thành viên phòng ban"));
    }
    traceout;
    return title;
}

ErrCode UIDepartmentPersonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    ErrCode err = ErrNone;
    DlgDeptPerson* dlg = nullptr;
    CommunityDept* dept = communityDept();
    if (!dept) {
        loge("Unknown depart");
        err = ErrInvalidModel;
    }
    if (err == ErrNone) {
        dlg = DlgDeptPerson::build(this, true, KModelNamePersonDept, nullptr, this);
        if (!dlg) {
            err = ErrNoMemory;
            loge("No memory");
        }
    }
    if (err == ErrNone) {
        dlg->setCommDeptUid(dept->uid());
        dlg->setCommDeptNameId(dept->nameId());
        dlg->exec();
    }
    FREE_PTR(dlg);
    traceout;
    return ErrNone;
}

ErrCode UIDepartmentPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = nullptr;
    PersonDept* per = nullptr;
    DlgDeptPerson* dlg = nullptr;
    CommunityDept* dept = communityDept();
    if (!item) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    if (err == ErrNone && !dept) {
        loge("Unknown depart");
        err = ErrInvalidModel;
    }
    if (err == ErrNone) {
        model = item->itemData();
        if (model && IS_MODEL_NAME(model, KModelNamePersonDept)) {
            per = (PersonDept*)model;
        } else {
            loge("no or invalid model data '%s'", MODELSTR2CHA(model));
            err = ErrNoData;
        }
    }
    if (err == ErrNone){
        dlg = DlgDeptPerson::build(this, true, KModelNamePersonDept, per, this);
        if (!dlg) {
            loge("no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        dlg->setCommDeptUid(dept->uid());
        dlg->setCommDeptNameId(dept->nameId());
        dlg->exec();
    }
    FREE_PTR(dlg);
    traceret(err);
    return err;
}

CommunityDept *UIDepartmentPersonListView::communityDept() const
{
    return (CommunityDept*)parentModel();
}

ErrCode UIDepartmentPersonListView::setCommDept(const CommunityDept *commDept)
{
    traced;
    // parent model is area, as this listview will show all contact people of an area
    return setParentModel(commDept);
}

QString UIDepartmentPersonListView::getMainModelName()
{
    return KModelNamePersonDept;
}


void UIDepartmentPersonListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_PERSON_NAMEID);
    mHeader.append(STR_HOLLYNAME);
    mHeader.append(STR_NU_TU);
    mHeader.append(STR_ROLE);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_TEL);
    mHeader.append(STR_STATUS);
    mHeader.append(STR_TERM);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
    mHeader.append(STR_NOTE);
}

ModelController *UIDepartmentPersonListView::getController()
{
    return COMMUNITYDEPTCTL;
}

ErrCode UIDepartmentPersonListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        if (IS_MODEL_NAME(item, KModelNamePersonDept)) {
            PersonDept* model = (PersonDept*) item;
            tblItem->addValue(model->nameId());
            tblItem->addValue(model->personNameId());
            tblItem->addValue(model->personHollyName());
            tblItem->addValue(model->personName());
            tblItem->addValue(model->roleName());
            tblItem->addValue(model->personEmail());
            tblItem->addValue(model->personTel());
            tblItem->addValue(DbModel::modelStatus2Name((DbModelStatus)model->modelStatus()));
            tblItem->addValue(model->courseName());
            tblItem->addValue(DatetimeUtils::date2String(model->startDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(DatetimeUtils::date2String(model->endDate(), DEFAULT_FORMAT_YMD));
            tblItem->addValue(model->remark());
        } else {
            loge("invalid model '%s'", MODELSTR2CHA(item));
            err = ErrInvalidModel;
        }
    }
    traceret(err);
    return err;
}
