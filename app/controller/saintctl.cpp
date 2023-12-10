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
 * Filename: saintctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/29/2022
 * Brief:
 */
#include "saintctl.h"
#include "logger.h"
#include "errcode.h"
#include "config.h"
#include "saint.h"
#include "utils.h"
#include "datetimeutils.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "filectl.h"
#include "dbctl.h"
#include "prebuiltdefs.h"
#include "controllerdefs.h"
#include "modeldefs.h"

SaintCtl *SaintCtl::gInstance = nullptr;

#define MAX_SPLIT_ITEMS (6)

#define SPLIT ','

//QList<Saint*> SaintCtl::getListSaints()
//{
//    tracein;
//    QList<Saint*> list;
//    DbModelHandler* dbSaint = DbCtl::getDb()->getSaintModelHandler();
//    if (dbSaint != nullptr){
//        QList<DbModel*> lstModel = dbSaint->getAll(&Saint::build);
//        if (!lstModel.empty()) {
//            foreach (DbModel* item, lstModel){
//                list.append((Saint*)item);
//            }
//        }

//    }
//    else{
//        loge("DbSaint not ready");
//    }
//    return list;
//}

DbModel *SaintCtl::doImportOneItem(const QString& importName, int importFileType, const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Saint* saint = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %d", items.count());
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            mImportFields.append(item.trimmed());
        }
    } else {

        saint = (Saint*)Saint::build();
        foreach (QString item, items) {
            ret = saint->onImportParseDataItem(importName, importFileType, mImportFields[i++], item, idx);
        }
    }

    traceret(ret);
    return saint;
}

SaintCtl::SaintCtl():
    ModelController(KControllerSaint, KModelHdlSaint)
{

}

DbModel *SaintCtl::buildModel(void *params, const QString &fmt)
{
    tracein;
    ErrCode ret = ErrNone;
    int idx = 0;
    qint32 noItem = 0;
    Saint* saint = dynamic_cast<Saint*>(Saint::build());
    QStringList* items = (QStringList*)params;
    noItem = items->length();
    UNUSED(fmt);
    saint->setNameId(items->at(idx++));
    //name
    if (ret == ErrNone)
        saint->setName(items->at(idx++).simplified());

    // full name
    if (ret == ErrNone)
        saint->setFullName(items->at(idx++).simplified());

    // full name
    if (ret == ErrNone)
        saint->setOriginName(items->at(idx++).simplified());


    // gender
    if (ret == ErrNone) {
        Gender gender = Utils::genderFromString(items->at(idx++));
        if (gender != Gender::GENDER_UNKNOWN){
            saint->setGender(gender);
        } else{
//            loge("Invalid gender");

        }

    }

    // memory date
    if (ret == ErrNone) {
        qint64 date = DatetimeUtils::dateFromString(items->at(idx++).simplified(),DEFAULT_FORMAT_MD);
        if (date > 0){
            saint->setFeastDay(date);
        }
        else{
            loge("Invalid date");
            // TODO: should break or continue???

        }
    }

    if (noItem > idx) {
        QString remark = items->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
        saint->setRemark(remark);
    }
    return saint;
}

const char *SaintCtl::getPrebuiltFileName()
{
    return KPrebuiltSaintCSVFileName;
}

const char *SaintCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

char SaintCtl::getCsvSplitItem()
{
    return CSV_LIST_ITEM_SPLIT;
}

DbModelBuilder SaintCtl::getMainBuilder()
{
    return &Saint::build;
}

ErrCode SaintCtl::getSaintUidListFromName(const QString &hollyName, QHash<QString, QString>* list)
{
    tracein;

    return getUidListFromName(hollyName, list);
}

QString SaintCtl::getHollyNameFromSaintUidList(const QStringList &uidList)
{
    tracein;

    return getNameFromUidList(uidList);

}

QString SaintCtl::getHollyNameFromSaintUidList(const QString &uidList)
{
    tracein;
    return getNameFromUidList(uidList);
}

SaintCtl *SaintCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new SaintCtl();
    }

    return gInstance;
}
