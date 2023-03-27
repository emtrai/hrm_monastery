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
 * Filename: dlgimportcommunitylistresult.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/24/2023
 * Brief:
 */
#include "dlgimportcommunitylistresult.h"
#include "community.h"
#include "dlgimportlistresult.h"

DlgImportCommunityListResult::DlgImportCommunityListResult(QWidget *parent):DlgImportListResult(parent)
{
    tracein;
}

void DlgImportCommunityListResult::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Địa chỉ"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
}

QList<UIImportItem *> *DlgImportCommunityListResult::getItems()
{
    tracein;
    QList<UIImportItem *> *items = new QList<UIImportItem *>();
    foreach (DbModel* item, mList) {
        Community* comm = (Community*)item;
        comm->dump();
        UIImportItem* wgitem = UIImportItem::build(item);
        wgitem->addValue(comm->nameId());
        wgitem->addValue(comm->name());
        wgitem->addValue(comm->addr());
        wgitem->addValue(comm->tel());
        wgitem->addValue(comm->email());
        // TODO: add more information??
        items->append(wgitem);
    }
    traceout;
    return items;
}
