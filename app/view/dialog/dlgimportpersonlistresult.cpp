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
 * Filename: dlgimportpersonlistresult.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#include "dlgimportpersonlistresult.h"
#include "dbmodel.h"
#include "person.h"
#include "utils.h"
#include "datetimeutils.h"

DlgImportPersonListResult::DlgImportPersonListResult(QWidget *parent):DlgImportListResult(parent)
{
    tracein;
}


void DlgImportPersonListResult::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
    mHeader.append(tr("Năm sinh"));
    mHeader.append(tr("Nơi sinh"));
    mHeader.append(tr("Ngày bổn mạng"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    mHeader.append(tr("Căn cước công dân"));
    mHeader.append(tr("Nơi cấp CCCD"));
//    mHeader.append(tr("Khoá"));
}

ErrCode DlgImportPersonListResult::setWidgetItem(UIImportItem *wgitem, DbModel *item)
{
    tracein;
    Person* per = (Person*)item;
    wgitem->addValue(per->nameId());
    wgitem->addValue(per->hollyName());
    wgitem->addValue(per->fullName());
    wgitem->addValue(DatetimeUtils::date2String(per->birthday()));
    wgitem->addValue(per->birthPlace());
    wgitem->addValue(DatetimeUtils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

    wgitem->addValue(per->tel().join(";"));
    wgitem->addValue(per->email().join(";"));
    wgitem->addValue(per->idCard());
    wgitem->addValue(per->idCardIssuePlace());
    traceout;
    return ErrNone;
}
