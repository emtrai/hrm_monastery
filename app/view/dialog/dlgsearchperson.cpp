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


DlgSearchPerson::~DlgSearchPerson()
{
    traced;
}

DlgSearchPerson *DlgSearchPerson::build(QWidget *parent, bool isMulti)
{
    traced;
    // TODO: create factory class and move this to factory???
    DlgSearchPerson* ret = new DlgSearchPerson(parent, isMulti);
    ret->setupUi();
    tracede;
    return ret;
}

DlgSearchPerson::DlgSearchPerson(QWidget *parent, bool isMulti):
    DlgSearch(parent, isMulti)
{
    traced;
}

QString DlgSearchPerson::getTitle()
{
    return tr("Tìm kiếm Nữ tu");
}


int DlgSearchPerson::onSearch(const QString &keyword)
{
    traced;
    clearAll();
    mListItems = INSTANCE(PersonCtl)->searchPerson(keyword);
    tracede;
    return mListItems.count();
}

void DlgSearchPerson::clearAll()
{
    traced;
    DlgSearch::clearAll();
    // TODO: clear each element of list????
    mListItems.clear();
    tracede;
}

DbModel *DlgSearchPerson::getItemAtIdx(int idx)
{
    traced;
    DbModel* ret = nullptr;
    logd("get item at idx=%d", idx);
    if (idx >= 0 && idx < mListItems.count()) {
        ret = (DbModel*)mListItems[idx];
    } else {
        loge("invalid idx %d", idx);
    }
    tracede;
    return ret;
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

QString DlgSearchPerson::getValueOfItemAt(int idx, int col, QString header, DbModel* item)
{
    traced;
    QString val;
    logd("idx = %d, col = %d", idx, col);
    if (item != nullptr) {
        logd("item is nullptr, get from index");
        item = getItemAtIdx(idx);
    }
    if (item != nullptr){
        Person* per = (Person*)item;

        // TODO: improve this, i.e. app callback to mHeader?
        switch (col) {
        case 0:
            val = per->personCode();
            break;
        case 1:
            val = per->hollyName();
            break;
        case 2:
            val = per->getFullName();
            break;
        case 3:
            val = Utils::date2String(per->birthday());
            break;
        case 4:
            val = per->birthPlace();
            break;
        case 5:
            val = per->communityName();
            break;
        }
    }
    tracede;
    return val;
}
