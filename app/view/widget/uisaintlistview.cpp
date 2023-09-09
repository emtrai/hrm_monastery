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
#include "stringdefs.h"

UISaintListView::UISaintListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = false;
    mHasExportMenu = false;
    traceout;

}

UISaintListView::~UISaintListView()
{
    traced;
}


QList<DbModel *> UISaintListView::getListDbModels()
{
    return SAINTCTL->getAllItems(true);
}

void UISaintListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    mHeader.append(STR_FULLNAME);
    mHeader.append(STR_NGAY_BON_MANG);
    mHeader.append(STR_NOTE);
    traceout;
}

ErrCode UISaintListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        if (item && item->modelName() == KModelNameSaint) {
            Saint* saint = (Saint*)item;
            tblItem->addValue(item->nameId());
            tblItem->addValue(item->name());
            tblItem->addValue(saint->fullName());
            tblItem->addValue(DatetimeUtils::date2String(saint->feastDay(), DEFAULT_FORMAT_MD));
            tblItem->addValue(saint->remark());
        } else {
            loge("Invalid item");
            err = ErrInvalidModel;
        }
    }
    traceret(err);
    return err;

}

ModelController *UISaintListView::getController()
{
    return SAINTCTL;
}

QString UISaintListView::getTitle()
{
    return STR_HOLLYNAME;
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

QString UISaintListView::getMainModelName()
{
    return KModelNameSaint;
}

void UISaintListView::initFilterFields()
{
    tracein;
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}
