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

DlgSearchPerson::~DlgSearchPerson()
{
    tracein;
    clearAll();
    traceout;
}

DlgSearchPerson *DlgSearchPerson::build(QWidget *parent, bool isMulti)
{
    tracein;
    // TODO: create factory class and move this to factory???
    DlgSearchPerson* ret = new DlgSearchPerson(parent, isMulti);
    ret->setupUi();
    ret->enableGetAllSupport();
    traceout;
    return ret;
}

DlgSearchPerson::DlgSearchPerson(QWidget *parent, bool isMulti):
    DlgSearch(parent, isMulti)
{
    tracein;
}

QString DlgSearchPerson::getTitle()
{
    return tr("Tìm kiếm Nữ tu");
}


int DlgSearchPerson::onSearch(const QString &keyword)
{
    tracein;
    ErrCode err = ErrNone;
    clearAll();

    err = INSTANCE(PersonCtl)->search(keyword, &mListItems);
    if (err != ErrNone) {
        loge("Search person err=%d", err);
        Utils::showErrorBox(QString(tr("Tìm kiếm lỗi, mã lỗi %1")).arg(err));
    }
    traceout;
    return mListItems.count();
}

void DlgSearchPerson::clearAll()
{
    tracein;
    DlgSearch::clearAll();
    RELEASE_LIST_DBMODEL(mListItems);
    traceout;
}

int DlgSearchPerson::onGetAll()
{
    tracein;
    clearAll();
    int cnt = 0;
    ErrCode err = MainWindow::showProcessingDialog(tr("Đang truy vấn dữ liệu"), nullptr,
         [this, &cnt](ErrCode* err, void* data, DlgWait* dlg) {
            RELEASE_LIST_DBMODEL(this->mListItems);
            this->mListItems = PERSONCTL->getAllItemsFromDb();
            logd("get all cnt=%d", this->mListItems.count());
            cnt = this->mListItems.count();
            return nullptr;//nothing to return
         },
        [this](ErrCode err, void* data, void* result, DlgWait* dlg) {
            logd("Search result %d", err);

            return err;
        });

//    mListItems = PERSONCTL->getAllItemsFromDb();
//    logd("get all cnt=%d", mListItems.count());
    logd("found %d", cnt);
    traceout;
    return cnt;
}

DbModel *DlgSearchPerson::getItemAtIdx(int idx)
{
    tracein;
    DbModel* ret = nullptr;
    logd("get item at idx=%d", idx);
    if (idx >= 0 && idx < mListItems.count()) {
        ret = (DbModel*)mListItems[idx];
    } else {
        loge("invalid idx %d", idx);
    }
    traceout;
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
    tracein;
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
            val = per->nameId();
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
    traceout;
    return val;
}
