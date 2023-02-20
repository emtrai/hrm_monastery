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

CommunityCtl::CommunityCtl():CommonCtl(KModelHdlCommunity)
{
    traced;
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

    if (jobj.contains(JSON_CODE)){
        QString tmp = jobj[JSON_CODE].toString().trimmed();
        if (!tmp.isEmpty())
            ret->setCommunityCode(tmp);
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

    if (jobj.contains(JSON_STATUS)){
        QString tmp = jobj[JSON_STATUS].toString().trimmed();
        if (!tmp.isEmpty()) {
            int status = QString(tmp).toInt();
            // TODO: handle return value
            ret->setStatus(status);
        }
    }

    return ret;

}

const QString CommunityCtl::exportTemplatePath(Exporter *exporter) const
{
    traced;
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_CSV_LIST:
        case EXPORT_XLSX:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltCommunityExportTemplateName);
        };
    }

    return QString();
}

ErrCode CommunityCtl::getExportDataString(const QString &keyword, const DbModel *data, QString *exportData) const
{
    traced;
    return data->getExportDataString(keyword, exportData);
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

//void CommunityCtl::onLoad()
//{
//    traced;
//    ErrCode ret = ErrNone;
//    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltCommunityJsonFileName, KFileTypeJson);
//    // TODO: should do lazyload???
//    loadFromDb();
//    tracede;
//}

ErrCode CommunityCtl::addNew(DbModel *model)
{
    traced;
    // TODO: auto add management board department, to contain list of managmenet member and role
    // at least, we need to have CEO for community
    return Controller::addNew(model);
}

ErrCode CommunityCtl::loadFromFile(const QString &path)
{
    traced;
    logd("load config from file %s", path.toStdString().c_str());
    return ErrNone;
}

//ErrCode CommunityCtl::loadFromDb()
//{
//    traced;
//    ErrCode err = ErrNone;
//    mListCommunity.clear(); // TODO: clear each item to avoid data leak?????
//    QList items = DB->getModelHandler(KModelHdlCommunity)->getAll(&Community::build);
//    foreach (DbModel* model, items){
//        model->dump();
//        mListCommunity.append((Community*) model);
//    }
//    logd("load %d item from db", mListCommunity.count());
//    tracedr(err);
//    return err;
//}

//const QList<Community *> CommunityCtl::getCommunityList(bool reload)
//{
////    QList<Community*> list;
////    traced;
////    DbModelHandler* modelHandler = DbCtl::getDb()->getCommunityModelHandler();
////    if (modelHandler != nullptr){
////        QList<DbModel*> lstModel = modelHandler->getAll(&Community::build);
////        if (!lstModel.empty()) {
////            foreach (DbModel* item, lstModel){
////                list.append((Community*)item);
////            }
////        }

////    }
////    else{
////        loge("DbSaint not ready");
////    }
////    return list;
//    traced;
//    logd("reload %d", reload);
//    if (reload) {
//        loadFromDb();
//    }
//    return mListCommunity;
//}

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

const char *CommunityCtl::getPrebuiltFileName()
{
    return KPrebuiltCommunityJsonFileName;
}

const char *CommunityCtl::getPrebuiltFileType()
{
    return KFileTypeJson;
}

QList<DbModel *> CommunityCtl::getItemFromDb()
{
    return getModelHandler()->getAll(&Community::build);
}

DbModelHandler *CommunityCtl::getModelHandler()
{
    return DB->getModelHandler(KModelHdlCommunity);
}

ErrCode CommunityCtl::addPerson(Community* comm, Person *per)
{
    traced;
    ErrCode ret = ErrNone;
    logd("add person to community");
    per->dump();
    ret = addPerson2Community(comm, per);
    tracedr(ret);
    return ret;
}

QHash<int, QString>* CommunityCtl::getStatusIdNameMap()
{
    traced;
    static bool isInitiedStatusNameMap = false;
    static QHash<int, QString> map;
    if (!isInitiedStatusNameMap) {
        map.insert(INACTIVE, "Không hoạt động");
        map.insert(ACTIVE, "Đang hoạt động");
        map.insert(BUILDING, "Đang xây dựng");
        isInitiedStatusNameMap = true;
    }
    // TODO: make it as static to load once only???
    tracede;
    return &map;
}

QString CommunityCtl::status2Name(CommunityStatus status)
{
    QHash<int, QString>* statuses = getStatusIdNameMap();
    QString ret;
    traced;
    logd("Status to get name %d", status);
    if (statuses->contains(status)){
        ret = statuses->value(status);
    } else {
        loge("invalid status %d", status);
        ret = "Không rõ"; // TODO: translate???
    }
    tracede;
    return ret;
}

ErrCode CommunityCtl::markModelDelete(DbModel *model)
{
    traced;
    // TODO: implement delete
    // TODO: don't delete root community
    ASSERT(false, "implement markModelDelete");
    return ErrNotImpl;
}

ErrCode CommunityCtl::deleteModel(DbModel *model)
{
    traced;
    // TODO: implement delete
    // TODO: don't delete root community, judge basing on level
    ASSERT(false, "implement deleteModel");
    return ErrNotImpl;
}

DbModel *CommunityCtl::doImportOneItem(int importFileType, const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Community* comm = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
        }
    } else {

        comm = (Community*)Community::build();
        foreach (QString item, items) {
            QString field = mImportFields[i++];
            logd("Import field %s", field.toStdString().c_str());
            ret = comm->onImportItem(importFileType, field, item, idx);
        }
    }

    tracedr(ret);
    return comm;
}

DbModel *CommunityCtl::doImportOneItem(int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Community* comm = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    comm = (Community*)Community::build();
    foreach (QString field, items.keys()) {
        QString value = items.value(field);
        logd("Import field %s", field.toStdString().c_str());
        logd("Import value %s", value.toStdString().c_str());
        ret = comm->onImportItem(importFileType, field, value, idx);
    }

    tracedr(ret);
    return comm;
}

