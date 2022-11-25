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
//        QList<DbModel*> lstModel = dbSaint->getAll(&Saint::builder);
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

DbModel *SaintCtl::doImportOneItem(int importFileType, const QStringList &items, quint32 idx)
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
            ret = saint->onImportItem(importFileType, mImportFields[i++], item, idx);
        }
    }

    tracedr(ret);
    return saint;
}

SaintCtl::SaintCtl()
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
        ret = importFromFile(nullptr, ImportType::IMPORT_CSV_LIST, fpath, &list);
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

QList<DbModel *> SaintCtl::getItemFromDb()
{
    traced;
    return DB->getModelHandler(KModelHdlSaint)->getAll(&Saint::builder);
}

DbModelHandler *SaintCtl::getModelHandler()
{
    traced;
    return DB->getModelHandler(KModelHdlSaint);
}

ErrCode SaintCtl::getSaintUidListFromName(const QString &hollyName, QHash<QString, QString>* list)
{
    traced;
    DbModelHandler* saintHdlr = nullptr;
    ErrCode ret = ErrNone;
    QHash<QString, QString> uidList;
    saintHdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(KModelHdlSaint));
    if (saintHdlr == nullptr) {
        loge("Invalid Saint handler");
        ret = ErrFailed;
    }
    // TODO: read from cache?? there'll be a lot of saints
    if (ret == ErrNone) {
        if (!hollyName.isEmpty()) {
            QStringList names = hollyName.split(HOLLYNAME_SPLIT);
            DbModel* model = nullptr;
            QString hollyNotFound;
            foreach (QString name, names) {
                logd("Check saint name '%s'", name.toStdString().c_str());
                model = saintHdlr->getByName(name.trimmed());
                if (model) {
                    logd("update saint uid %s", model->uid().toStdString().c_str());
                    uidList.insert(model->uid(), model->name());
                    delete model;
                } else {
                    loge("Saint '%s' not found in db", name.toStdString().c_str());
                    ret = ErrNotFound;
                    break;
                }
            }
        }
    }

    if (ret == ErrNone && uidList.count() > 0 && list != nullptr) {
        logd("found %d saint from name %s", uidList.count(), hollyName.toStdString().c_str());
        list->insert(uidList);
    }
    tracedr(ret);
    return ret;
}

QString SaintCtl::getHollyNameFromSaintUidList(const QStringList &uidList)
{
    traced;
    DbModelHandler* saintHdlr = nullptr;
    ErrCode ret = ErrNone;
    QString hollyName;
    QStringList hollyNameList;
    saintHdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(KModelHdlSaint));
    if (saintHdlr == nullptr) {
        loge("Invalid Saint handler");
        ret = ErrFailed;
    }
    // TODO: read from cache?? there'll be a lot of saints
    if (ret == ErrNone) {
        if (!uidList.isEmpty()) {
            DbModel* model = nullptr;
            foreach (QString uid, uidList) {
                // TODO: cached it instead of reload from scratch??
                logd("Check saint uid '%s'", uid.toStdString().c_str());
                model = saintHdlr->getByUid(uid.trimmed());
                if (model) {
                    logd("update saint name %s", model->name().toStdString().c_str());
                    hollyNameList.append(model->name());
                    delete model;
                } else {
                    loge("Saint uid '%s' not found in db", uid.toStdString().c_str());
                    ret = ErrNotFound;
                    break;
                }
            }
        } else {
            logi("No saint uid to check");
        }
    }

    if (ret == ErrNone && hollyNameList.count() > 0) {
        logd("Found %d saint", hollyNameList.count());
        hollyName = hollyNameList.join(HOLLYNAME_SPLIT);
    }
    tracedr(ret);
    return hollyName;

}

QString SaintCtl::getHollyNameFromSaintUidList(const QString &uidList)
{
    traced;
    return getHollyNameFromSaintUidList(uidList.split(HOLLYNAME_SPLIT));
}

//void SaintCtl::onLoad()
//{
//    traced;
//    ErrCode ret = ErrNone;
//    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltSaintCSVFileName, KFileTypeCSV);

//}

SaintCtl *SaintCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new SaintCtl();
    }

    return gInstance;
}
