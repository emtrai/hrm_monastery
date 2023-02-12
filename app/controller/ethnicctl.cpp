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
 * Filename: ethnicctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "ethnicctl.h"
#include "logger.h"
#include "country.h"
#include "ethnic.h"
#include "defs.h"
#include "dbctl.h"
EthnicCtl* EthnicCtl::gInstance = nullptr;

EthnicCtl::EthnicCtl():Controller(KModelHdlEthnic)
{
    traced;
}
// Format: country short name, name[, remark]
DbModel *EthnicCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Ethnic* item = new Ethnic();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setNameId(itemList->at(idx++));
    item->setCountryShortName(itemList->at(idx++));
    item->setName(itemList->at(idx++));
    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}

DbModelHandler *EthnicCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlEthnic);
}

const QList<Ethnic *>* EthnicCtl::getEthnicList(const QString &country)
{
    traced;
    if (mEthnicList.contains(country))
        return mEthnicList[country];
    return nullptr;
}

const QList<Ethnic *> EthnicCtl::getAllEthnics()
{
    QList<Ethnic *>listAll;
    foreach (QList<Ethnic*>* list,  mEthnicList.values()){
        listAll.append(*list);
    }
    return listAll;
}

const QHash<QString, QList<Ethnic*>*> EthnicCtl::getEthnicList()
{
    traced;
    return mEthnicList;
}

EthnicCtl *EthnicCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new EthnicCtl();
    }
    return gInstance;
}

void EthnicCtl::onLoad()
{

    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltEthnicCSVFileName, KFileTypeCSV);
    // TODO: should do lazyload???
    QList items = DB->getModelHandler(KModelHdlEthnic)->getAll(&Ethnic::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Ethnic* ethnic = (Ethnic*)model;
        if (!mEthnicList.contains(ethnic->countryShortName()))
            mEthnicList[ethnic->countryShortName()] = new QList<Ethnic*>();
        mEthnicList[ethnic->countryShortName()]->append(ethnic);
    }
    tracede;
}

