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
 * Filename: dlgimportareapersonlistresult.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2023
 * Brief:
 */
#include "dlgimportareapersonlistresult.h"

#include "dbmodel.h"
#include "area.h"
#include "areaperson.h"
#include "utils.h"
#include "datetimeutils.h"
#include "stringdefs.h"
DlgImportAreaPersonListResult::DlgImportAreaPersonListResult(QWidget *parent):DlgImportListResult(parent)
{
    traced;
}

ErrCode DlgImportAreaPersonListResult::setup(const QList<DbModel *> &newList)
{
    ErrCode err = ErrNone;
    tracein;
    err = DlgImportListResult::setup(newList);
    if (err == ErrNone &&
        (!IS_MODEL_NAME(mTargetModel,  KModelNameArea))) {
        err = ErrInvalidData;
        loge("invalid target model '%s'", MODELSTR2CHA(mTargetModel));
    }
    if (err == ErrNone && mList.size() == 0){
        loge("no data to import");
        err = ErrNoData;
    }
    if (err == ErrNone) {
        Area* targetArea = (Area*) mTargetModel;
        logd("targetArea '%s'", MODELSTR2CHA(targetArea));
        foreach (DbModel* model, mList) {
            logd("model in list '%s'", MODELSTR2CHA(model));
            if (IS_MODEL_NAME(model, KModelNameAreaPerson)) {
                bool ok = false;
                AreaPerson* per = (AreaPerson*)model;
                per->setArea(targetArea);
                QString nameid = per->buildNameId(nullptr, &ok);
                if (ok) {
                    per->setNameId(nameid);
                } else {
                    err = ErrInvalidData;
                    loge("cannot build nameid for model '%s'", MODELSTR2CHA(model));
                    break;
                }
            } else {
                logw("invalid model in list '%s'", MODELSTR2CHA(model));
            }
        }
    }
    traceret(err);
    return err;
}

void DlgImportAreaPersonListResult::initHeader()
{
    traced;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_PERSON_NAMEID);
    mHeader.append(STR_NU_TU);
    mHeader.append(STR_AREA);
    mHeader.append(STR_ROLE);
    mHeader.append(STR_TERM);
    mHeader.append(STR_STATUS);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_STARTDATE);
    mHeader.append(STR_ENDDATE);
}

ErrCode DlgImportAreaPersonListResult::setWidgetItem(UIImportItem *wgitem, DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!wgitem || !item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && (item->modelName() != KModelNameAreaPerson)) {
        loge("invalid model '%s'", MODELSTR2CHA(item));
        err = ErrInvalidData;
    }
    if (err == ErrNone) {
        logd("set wigetimte for model '%s'", MODELSTR2CHA(item));
        AreaPerson* per = (AreaPerson*)item;
        wgitem->addValue(per->nameId());
        wgitem->addValue(per->personNameId());
        wgitem->addValue(per->personName());
        wgitem->addValue(per->areaName());
        wgitem->addValue(per->roleName());
        wgitem->addValue(per->courseName());
        wgitem->addValue(per->modelStatusName());
        wgitem->addValue(per->personTel());
        wgitem->addValue(per->personEmail());
        wgitem->addValue(DatetimeUtils::date2String(per->startDate()));
        wgitem->addValue(DatetimeUtils::date2String(per->endDate()));
    }
    traceout;
    return err;
}
