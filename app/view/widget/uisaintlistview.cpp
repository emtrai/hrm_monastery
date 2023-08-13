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
 * Filename: uisaintlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uisaintlistview.h"
#include "saintctl.h"
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "saint.h"
#include "utils.h"
#include "datetimeutils.h"

UISaintListView::UISaintListView(QWidget *parent):
    UICommonListView(parent)
{


}

UISaintListView::~UISaintListView()
{
    tracein;
}

ErrCode UISaintListView::onLoad()
{

    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    mItemList = SAINTCTL->getAllItems();
    // TODO: loop to much, redundant, do something better?
//    foreach (Saint* item, items) {
//        mItemList.append(static_cast<DbModel*>(item));
//    }
    return ErrNone;
}

void UISaintListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    if (item && item->modelName() == KModelNameSaint) {
        Saint* saint = (Saint*)item;
        tblItem->addValue(item->nameId());
        tblItem->addValue(item->name());
        tblItem->addValue(saint->fullName());
        tblItem->addValue(DatetimeUtils::date2String(saint->feastDay(), DEFAULT_FORMAT_MD));
        tblItem->addValue(saint->remark());
    } else {
        logd("Invalid item");
    }
    traceout;

}

void UISaintListView::initHeader()
{
    tracein;
    mHeader.append(tr("Tên định danh"));
    mHeader.append(tr("Tên"));
    mHeader.append(tr("Tên đầy đủ"));
    mHeader.append(tr("Ngày bổn mạng"));
    mHeader.append(tr("Ghi chú"));
    traceout;
}

int UISaintListView::onFilter(int catetoryid, const QString &catetory, qint64 opFlags, const QString &keywords, const QVariant *value)
{
    tracein;
    QList<DbModel*> list;
    logd("Search %s", keywords.toStdString().c_str());
    ErrCode ret = SAINTCTL->search(keywords, &list);
    logd("Search ret %d", ret);
    RELEASE_LIST_DBMODEL(mItemList);
    // TODO: loop to much, redundant, do something better?
    if (ret == ErrNone) {
        foreach (DbModel* item, list) {
            mItemList.append(static_cast<DbModel*>(item));
            // TODO: should it be shared pointer or not???
//            mItemList.append(std::shared_ptr<DbModel>(item));
        }
    } else {
        logi("Nothing to add");
    }
    reload();
    return mItemList.size();
}

DbModel *UISaintListView::onCreateDbModelObj(const QString& modelName)
{
    return Saint::build();
}
