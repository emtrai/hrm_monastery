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


QList<Saint*> SaintCtl::getListSaints()
{
    traced;
    QList<Saint*> list;
    DbModelHandler* dbSaint = DbCtl::getDb()->getSaintModelHandler();
    if (dbSaint != nullptr){
        QList<DbModel*> lstModel = dbSaint->getAll(&Saint::builder);
        if (!lstModel.empty()) {
            foreach (DbModel* item, lstModel){
                list.append((Saint*)item);
            }
        }

    }
    else{
        loge("DbSaint not ready");
    }
    return list;
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

void SaintCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltSaintCSVFileName, KFileTypeCSV);
//    QString fname = Utils::getPrebuiltFileByLang(KPrebuiltSaintCSVFileName);
//    if (!FileCtl::checkPrebuiltDataFileHash(fname)){
//        logi("Hash file not match, seem need to update");
//        ret = check2UpdateDbFromFile(FileCtl::getPrebuiltDataFilePath(fname));
//        if (ret == ErrNone){
//            FileCtl::updatePrebuiltDataFileHash(fname);
//        }
//        else{
//            logi("Check to update db from file failed %d", ret);
//        }
//    }
//    else {
//        logi("Prebuilt saint file up-to-date");
////        getListSaints();//TODO: for test only;
//    }
//    check2UpdateDbFromFile(FileCtl::getPrebuiltDataFile(
//                            Utils::getPrebuiltFileByLang(KPrebuiltSaintCSVFileName)));
}

//ErrCode SaintCtl::check2UpdateDbFromFile(const QString &filePath)
//{
//    traced;
//    // TODO: add more checking????
//    ErrCode ret = Utils::parseCSVFile(filePath, &SaintCtl::oneCSVItemCallback,
//                                      this, nullptr, SPLIT);
//    logd("ret %d", ret);
//    return ret;
//}

//ErrCode SaintCtl::oneCSVItemCallback(const QStringList& items, void* caller, void*param)
//{
//    return getInstance()->doOneCSVItemCallback(items, param);
//}

//ErrCode SaintCtl::doOneCSVItemCallback(const QStringList& items, void*param)
//{
//    traced;
//    ErrCode ret = ErrNone;
//    (void) param;
//    if (!items.empty() && items.length() >= MAX_SPLIT_ITEMS) {
//        Saint saint;

//        qint32 idx = 0;
//        //name
//        if (ret == ErrNone)
//            saint.setName(items[idx++].simplified());

//        // full name
//        if (ret == ErrNone)
//            saint.setFullName(items[idx++].simplified());

//        // gender
//        if (ret == ErrNone)
//        {
//            Gender gender = Utils::genderFromString(items[idx++]);
//            if (gender != Gender::GENDER_UNKNOWN){
//                saint.setGender(gender);
//            }
//            else{
//                loge("Invalid gender");
//                ret = ErrInvalidData;
//            }

//        }

//        // memory date
//        if (ret == ErrNone)
//        {
//            qint64 date = Utils::dateFromString(items[idx++].simplified(),"M/D");
//            if (date > 0){
//                saint.setFeastDay(date);
//            }
//            else{
//                loge("Invalid date");
//                ret = ErrInvalidData;
//                // TODO: should break or continue???

//            }
//        }

//        // country
//        // TODO: check if country exist in db, then insert it????
//        if (ret == ErrNone)
//            saint.setCountry(items[idx++].simplified());

//        // brief info
//        if (ret == ErrNone)
//        {
//            QString brief;
//            for (;idx < items.length(); idx++){
//                if (!brief.isEmpty())
//                    brief += SPLIT;
//                brief += items[idx];
//            }
//        }

//        if (ret == ErrNone)
//        {
//            if (saint.isValid()){
//                saint.dump();
//                logi("Save saint '%s'", saint.toString().toStdString().c_str());
//                ret = saint.save();
//            }
//            else{
//                ret = ErrInvalidData;
//                loge("Saint data is invalid");
//            }
//        }

//    }
//    else {
//        loge("Invalid data");
//        // TODO: should break or continue???
//        ret = ErrInvalidData;
//    }
//    logd("ret %d", ret);
//    return ret;
//}

SaintCtl *SaintCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new SaintCtl();
    }

    return gInstance;
}
