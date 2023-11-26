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
 * Filename: dlgsearchperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/9/2022
 * Brief:
 */
#include "dlgsearchperson.h"
#include "logger.h"
#include "person.h"
#include "personctl.h"
#include "mainwindow.h"
#include "utils.h"
#include "datetimeutils.h"
#include "dialogutils.h"
#include "communityctl.h"
#include "communitymanager.h"
#include "stringdefs.h"
#include "person.h"

DlgSearchPerson *DlgSearchPerson::build(QWidget *parent, bool isMulti)
{
    tracein;
    // TODO: create factory class and move this to factory???
    DlgSearchPerson* ret = new DlgSearchPerson(parent, isMulti);
    ret->setupUi();
    ret->enableGetAllSupport();
    ret->enableGetManagerSupport();
    traceout;
    return ret;
}

DlgSearchPerson::DlgSearchPerson(QWidget *parent, bool isMulti):
    DlgSearch(parent, isMulti)
{
    traced;
}

DlgSearchPerson::~DlgSearchPerson()
{
    traced;
}

QString DlgSearchPerson::getTitle()
{
    return STR_SEARCH_PERSON;
}


ErrCode DlgSearchPerson::doSearch(const QString& keyword, QList<DbModel*>& items)
{
    tracein;
    ErrCode err = ErrNone;
    err = INSTANCE(PersonCtl)->search(keyword, &items);
    traceout;
    return err;
}

ErrCode DlgSearchPerson::doGetAll(QList<DbModel*>& items)
{
    tracein;
    items = PERSONCTL->getAllItemsFromDb();
    logd("get all people cnt=%lld", items.count());
    traceout;
    return ErrNone;
}

ErrCode DlgSearchPerson::doGetManager(QList<DbModel*>& items)
{
    tracein;
    QList<DbModel*> managerList;
    ErrCode err = ErrNone;
    err = COMMUNITYCTL->getAllManagersList(managerList);
    logd("get manager err = %d cnt=%lld", err, managerList.size());
    if (err == ErrNone && managerList.size() > 0) {
        foreach (DbModel* item, managerList) {
            if (IS_MODEL_NAME(item, KModelNameCommManager)) {
                CommunityManager* mgr = static_cast<CommunityManager*>(item);
                Person* per = CLONE_MODEL(mgr->person(), Person);
                if (per) {
                    items.append(per);
                }
            } else {
                loge("invalid model '%s'", MODELSTR2CHA(item));
            }
        }
    }
    RELEASE_LIST_DBMODEL(managerList);
    traceout;
    return err;

}


void DlgSearchPerson::initHeader()
{
    // TODO: translation
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
    mHeader.append(tr("Năm sinh"));
    mHeader.append(tr("Nơi sinh"));
    mHeader.append(tr("Cộng đoàn"));
    // WARNING: any change on this, must update to getValueOfItemAt
}

QString DlgSearchPerson::getValueOfItemCol(int col, const DbModel* item)
{
    tracein;
    QString val;
    if (IS_MODEL_NAME(item, KModelNamePerson)) {
        const Person* per = static_cast<const Person*>(item);
        switch (col) {
        case 0:
            val = per->nameId();
            break;
        case 1:
            val = per->hollyName();
            break;
        case 2:
            val = per->fullName();
            break;
        case 3:
            val = DatetimeUtils::date2String(per->birthday());
            break;
        case 4:
            val = per->birthPlace();
            break;
        case 5:
            val = per->communityName();
            break;
        default:
            loge("invalid col %d", col);
            break;
        }
    } else {
        loge("invalid item '%s'", MODELSTR2CHA(item));
    }
    logd("value of col %d is '%s'", col, STR2CHA(val));
    traceout;
    return val;
}
