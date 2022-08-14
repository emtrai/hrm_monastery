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
 * Filename: eductl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "eductl.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "education.h"
#include "dbctl.h"
#include <QList>

#define SPLIT ','

EduCtl* EduCtl::gInstance = nullptr;

EduCtl::EduCtl(): mLoaded(false)
{

}
DbModel *EduCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Education* edu = new Education();
    QStringList* itemList = (QStringList*) items;
    QString item = itemList->join(SPLIT);
    logd("Add item %s to list", item.toStdString().c_str());
    edu->setName(item);
    return edu;
}
//ErrCode EduCtl::doOneCSVItemCallback(const QStringList &items, void *param)
//{
//    traced;
//    ErrCode ret = ErrNone;
//    (void) param;
//    if (!items.empty()) {
//        Education* edu = new Education();
//        QString item = items.join(SPLIT);
//        logd("Add item %s to list", item.toStdString().c_str());
//        edu->setName(item);
//        edu->save();// TODO handle error
////        mItemList.append(item);
//    }
//    else {
//        loge("Invalid data");
//        // TODO: should break or continue???
//        ret = ErrInvalidData;
//    }
//    logd("ret %d", ret);
//    return ret;
//}

//ErrCode EduCtl::oneCSVItemCallback(const QStringList &items, void *param)
//{
//    return getInstance()->doOneCSVItemCallback(items, param);
//}

QList<Education*> EduCtl::getListEdu()
{
    traced;
    // TODO: check if file update then reload??
    return mItemList;
}

EduCtl *EduCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new EduCtl();
    }

    return gInstance;
}

void EduCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltEduCSVFileName, KFileTypeCSV);
    // TODO: file should be from installed dir, rather than embedded inside bin??
//    QString fname = Utils::getPrebuiltFileByLang(KPrebuiltEduCSVFileName);
//    if (!FileCtl::checkPrebuiltDataFileHash(fname)){
//        ret = Utils::parseCSVFile(FileCtl::getPrebuiltDataFilePath(fname),
//                                  &EduCtl::oneCSVItemCallback);
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

    QList items = DB->getEduModelHandler()->getAll(&Education::builder);
//    mItemList.append();
    foreach (DbModel* model, items){
        mItemList.append((Education*)model);
    }

}
