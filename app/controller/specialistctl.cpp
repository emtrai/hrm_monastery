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
 * Filename: specialistctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#include "specialistctl.h"

#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "specialist.h"
#include "dbctl.h"
#include "dbspecialistmodelhandler.h"

#define SPLIT ','

SpecialistCtl* SpecialistCtl::gInstance = nullptr;

SpecialistCtl::SpecialistCtl(): mLoaded(false)
{

}

DbModel *SpecialistCtl::buildModel(void *items, const QString &fmt)
{
    Specialist* specialist = new Specialist();
    QStringList* itemList = (QStringList*) items;
    QString item = itemList->join(SPLIT);
    qint32 idx = 0;
    logd("Add item %s to list", item.toStdString().c_str());
    specialist->setNameId(itemList->at(idx++));
    specialist->setName(itemList->at(idx++));
    tracede;
    return specialist;
}

DbModelHandler *SpecialistCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlSpecialist);

}

//ErrCode SpecialistCtl::doOneCSVItemCallback(const QStringList &items, void *param)
//{
//    traced;
//    ErrCode ret = ErrNone;
//    (void) param;
//    if (!items.empty()) {
//        Specialist* specialist = new Specialist();
//        QString item = items.join(SPLIT);
//        logd("Add item %s to list", item.toStdString().c_str());
//        specialist->setName(item);
//        specialist->save();// TODO handle error
//        //        mItemList.append(item);
//    }
//    else {
//        loge("Invalid data");
//        // TODO: should break or continue???
//        ret = ErrInvalidData;
//    }
//    logd("ret %d", ret);
//    return ret;
//}

//ErrCode SpecialistCtl::oneCSVItemCallback(const QStringList &items, void* caller, void *param)
//{
//    return getInstance()->doOneCSVItemCallback(items, param);
//}

QList<Specialist*> SpecialistCtl::getAll()
{
    traced;
    // TODO: check if file update then reload??
    return mItemList;
}

SpecialistCtl *SpecialistCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new SpecialistCtl();
    }

    return gInstance;
}

QList<DbModel *> SpecialistCtl::getListPerson(const QString &specialistUid)
{
    logd("get list person of specialist uid %s", specialistUid.toStdString().c_str());
    DbSpecialistModelHandler* hdl = dynamic_cast<DbSpecialistModelHandler*>(DB->getSpecialistModelHandler());
    QList<DbModel*> list =  hdl->getListPerson(specialistUid);
    logd("No. item: %d", list.count());
    return list;
}


void SpecialistCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltSpecialistCSVFileName, KFileTypeCSV);
//    // TODO: file should be from installed dir, rather than embedded inside bin??
//    QString fname = Utils::getPrebuiltFileByLang(KPrebuiltSpecialistCSVFileName);
//    if (!FileCtl::checkPrebuiltDataFileHash(fname)){
//        ret = Utils::parseCSVFile(FileCtl::getPrebuiltDataFilePath(fname),
//                                  &SpecialistCtl::oneCSVItemCallback, this);
//        if (ret == ErrNone){
//            mLoaded = true;
//            FileCtl::updatePrebuiltDataFileHash(fname);
//        }
//        else{
//            logi("Check to update db from file failed %d", ret);
//        }
//    }
//    else {
//        logi("Prebuilt saint file up-to-date");
//    }
    QList<DbModel*> items = DB->getSpecialistModelHandler()->getAll(&Specialist::builder);
    foreach (DbModel* item, items)
        mItemList.append((Specialist*)item);

}

