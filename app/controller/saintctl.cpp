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
#include <QFile>
#include <QDate>
#include "defs.h"
#include "filectl.h"
#include "dbctl.h"

SaintCtl *SaintCtl::gInstance = nullptr;

#define MAX_SPLIT_ITEMS (6)

#define SPLIT ','

//QList<Saint*> SaintCtl::getListSaints()
//{
//    traced;
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
            ret = saint->onImportItem(importName, importFileType, mImportFields[i++], item, idx);
        }
    }

    tracedr(ret);
    return saint;
}

SaintCtl::SaintCtl():
    ModelController(KModelHdlSaint)
{

}

DbModel *SaintCtl::buildModel(void *params, const QString &fmt)
{
    traced;
    ErrCode ret = ErrNone;
    int idx = 0;
    Saint* saint = new Saint();
    QStringList* items = (QStringList*)params;
    UNUSED(fmt);
    saint->setNameId(items->at(idx++));
    //name
    if (ret == ErrNone)
        saint->setName(items->at(idx++).simplified());

    // full name
    if (ret == ErrNone)
        saint->setFullName(items->at(idx++).simplified());

    // gender
    if (ret == ErrNone)
    {
        Gender gender = Utils::genderFromString(items->at(idx++));
        if (gender != Gender::GENDER_UNKNOWN){
            saint->setGender(gender);
        }
        else{
            loge("Invalid gender");
            ret = ErrInvalidData;
        }

    }

    // memory date
    if (ret == ErrNone)
    {
        qint64 date = Utils::dateFromString(items->at(idx++).simplified(),"M/D");
        if (date > 0){
            saint->setFeastDay(date);
        }
        else{
            loge("Invalid date");
            ret = ErrInvalidData;
            // TODO: should break or continue???

        }
    }

    // country
    // TODO: check if country exist in db, then insert it????
    if (ret == ErrNone)
        saint->setCountry(items->at(idx++).simplified());

    // brief info
//    if (ret == ErrNone)
//    {
//        QString brief;
//        for (;idx < items.length(); idx++){
//            if (!brief.isEmpty())
//                brief += SPLIT;
//            brief += items->at(idx++);
//        }
//    }

    return saint;
}

ErrCode SaintCtl::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    // TODO: should we add meta field in beginning of file to know status of information???
    // i.e.: version, last update time.
    // This can be used to check/compare witl one stored in db
    if (ftype == KFileTypeCSV) {

        QList<DbModel*> list;
        logd("Import saint file %s", fpath.toStdString().c_str());
        ret = importFromFile(KModelHdlSaint, ImportType::IMPORT_CSV_LIST, fpath, &list);
        logd("Import result %d", ret);
        logd("No of import item %d", list.count());
        if (ret == ErrNone) {
            if (list.count() > 0) {
                foreach (DbModel* item, list) {
                    logd("Save item");
                    item->dump();
                    item->save();
                }
            } else {
                logi("Nothing to import saint");
            }
        }
    } else {
        ret = ErrNotSupport;
    }
    return ret;
}

const char *SaintCtl::getPrebuiltFileName()
{
    return KPrebuiltSaintCSVFileName;
}

const char *SaintCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

DbModelBuilder SaintCtl::getMainBuilder()
{
    return &Saint::build;
}

ErrCode SaintCtl::getSaintUidListFromName(const QString &hollyName, QHash<QString, QString>* list)
{
    traced;

    return getUidListFromName(hollyName, list);
}

QString SaintCtl::getHollyNameFromSaintUidList(const QStringList &uidList)
{
    traced;

    return getNameFromUidList(uidList);

}

QString SaintCtl::getHollyNameFromSaintUidList(const QString &uidList)
{
    traced;
    return getNameFromUidList(uidList);
}

SaintCtl *SaintCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new SaintCtl();
    }

    return gInstance;
}
