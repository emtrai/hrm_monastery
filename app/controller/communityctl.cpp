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
 * Filename: community.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "communityctl.h"
#include "logger.h"
#include "errcode.h"
#include "community.h"
#include "utils.h"
#include <QFile>
#include <QDate>
#include "defs.h"
#include "filectl.h"
#include "dbctl.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dbcommunitymodelhandler.h"

CommunityCtl* CommunityCtl::gInstance = nullptr;

CommunityCtl::CommunityCtl()
{

}

CommunityCtl *CommunityCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new CommunityCtl;
    }
    return gInstance;
}

Community* CommunityCtl::parseOneItem(const QJsonObject& jobj)
{
    Community* ret = new Community();
    traced;
    if (jobj.contains(JSON_ID)){
        QString tmp = jobj[JSON_ID].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setUid(tmp);
    }

    if (jobj.contains(JSON_NAME)){
        QString tmp = jobj[JSON_NAME].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setName(tmp);

    }

    if (jobj.contains(JSON_CHURCH)){
        QString tmp = jobj[JSON_CHURCH].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setChurch(tmp);
    }

    if (jobj.contains(JSON_ADDR)){
        QString tmp = jobj[JSON_ADDR].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setAddr(tmp);
    }

    if (jobj.contains(JSON_TEL)){
        QString tmp = jobj[JSON_TEL].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setTel(tmp);
    }

    if (jobj.contains(JSON_EMAIL)){
        QString tmp = jobj[JSON_EMAIL].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setEmail(tmp);

    }

    if (jobj.contains(JSON_FEASTDAY)){
        QString tmp = jobj[JSON_FEASTDAY].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setFeastDateFromString(tmp);
    }

    if (jobj.contains(JSON_ESTABLISH)){
        QString tmp = jobj[JSON_ESTABLISH].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setCreateDateFromString(tmp);
    }


    if (jobj.contains(JSON_AREA_UID)){
        QString tmp = jobj[JSON_AREA_UID].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setAreaUid(tmp);
    }

    return ret;

}
ErrCode CommunityCtl::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    if (ftype == KFileTypeJson) {

        logd("Load file %s", fpath.toStdString().c_str());
        QFile loadFile(fpath);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            loge("Couldn't open file %s", fpath.toStdString().c_str());
            ret = ErrFileRead;
        }

        if (ret == ErrNone){
            logd("Parse json");
            QByteArray saveData = loadFile.readAll();
            logd("saveData length %d", (int)saveData.length());

            QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);

            logd("loadDoc isEmpty %d", loadDoc.isEmpty());
            QJsonObject jRootObj = loadDoc.object();
            qWarning() << jRootObj;
            if (jRootObj.contains(JSON_COMMUNITYLIST) && jRootObj[JSON_COMMUNITYLIST].isArray()) {
                QJsonArray jlist = jRootObj[JSON_COMMUNITYLIST].toArray();
                for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                    QJsonObject jObj = jlist[levelIndex].toObject();
                    Community* community = parseOneItem(jObj);
                    if (community->isValid())
                        community->save();

                    delete community;

                }
            } else {
                loge("Invalid data, not found %s", JSON_COMMUNITYLIST);
                ret = ErrInvalidData;
            }
        }

    } else {
        ret = ErrNotSupport;
    }
    tracedr(ret);
    return ret;
}

void CommunityCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltCommunityJsonFileName, KFileTypeJson);
    // TODO: should do lazyload???
    loadFromDb();
    tracede;
}

ErrCode CommunityCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}

ErrCode CommunityCtl::loadFromDb()
{
    traced;
    ErrCode err = ErrNone;
    mListCommunity.clear(); // TODO: clear each item to avoid data leak?????
    QList items = DB->getModelHandler(KModelHdlCommunity)->getAll(&Community::builder);
    foreach (DbModel* model, items){
        model->dump();
        mListCommunity.append((Community*) model);
    }
    logd("load %d item from db", mListCommunity.count());
    tracedr(err);
    return err;
}

const QList<Community *> CommunityCtl::getCommunityList(bool reload)
{
//    QList<Community*> list;
//    traced;
//    DbModelHandler* modelHandler = DbCtl::getDb()->getCommunityModelHandler();
//    if (modelHandler != nullptr){
//        QList<DbModel*> lstModel = modelHandler->getAll(&Community::builder);
//        if (!lstModel.empty()) {
//            foreach (DbModel* item, lstModel){
//                list.append((Community*)item);
//            }
//        }

//    }
//    else{
//        loge("DbSaint not ready");
//    }
//    return list;
    traced;
    logd("reload %d", reload);
    if (reload) {
        loadFromDb();
    }
    return mListCommunity;
}

const QList<DbModel *> CommunityCtl::getPersonList(const QString &communityUid)
{
    traced;
    DbCommunityModelHandler* model =  dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
    QList<DbModel *> items = model->getListPerson(communityUid);
    return items;
}

ErrCode CommunityCtl::addPerson2Community(const Community *comm, const Person *per, int status, qint64 startdate, qint64 enddate, const QString &remark)
{
    traced;
    DbCommunityModelHandler* model =  dynamic_cast<DbCommunityModelHandler*>(DB->getModelHandler(KModelHdlCommunity));
    return model->addPerson2Community(comm, per, status, startdate, enddate, remark);
}

