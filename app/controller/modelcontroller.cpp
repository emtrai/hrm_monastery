#include "modelcontroller.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "dbmodel.h"
#include "dbmodelhandler.h"
#include "idataimporter.h"
#include "dbctl.h"
#include <QFile>
#include "defs.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "exporttype.h"
#include "filectl.h"

ModelController::ModelController():
    mEnableCache(true)
{
    traced;
    mCacheItemList.clear();
}

ModelController::~ModelController()
{
    traced;
    clearCacheItemList();
    tracede;
}

ModelController::ModelController(const QString& name):mName(name),
    mEnableCache(true)
{
    traced;
    logd("construct with name=%s", STR2CHA(name));
    mCacheItemList.clear();
}

QString ModelController::getName()
{
    return mName;
}

DbModelHandler *ModelController::getModelHandler()
{
    traced;
    logd("Get model handler for controller using name: %s", STR2CHA(mName));
    // TODO: fix me if this is not suitble
    return DB->getModelHandler(getName());
}

ErrCode ModelController::addNew(DbModel *model)
{
    ErrCode ret = ErrNone;
    traced;
    DbModelHandler* hdl = getModelHandler();
    if (hdl != nullptr){
        if (!hdl->exist(model)) {
            logd("all well to save");
            ret = model->save();
            if (ret == ErrNone) {
                logi("Save ok, let's reload");
                reloadDb(); // TODO: overhead, when add new is called multiple time!!! (i.e. when import data)
                // TODO: should emit???? if emit load, loader will catch and reload all
            }
        } else {
            ret = ErrExisted;
            loge("Alerady exist");
        }
    } else {
        ret = ErrNotExist;
        loge("not found suitable model handler");
    }
    return ret;
}

QList<DbModel *> ModelController::getAllItemsFromDb(qint64 status, int from, int noItems, int* total)
{
    traced;
    DbModelHandler* hdl = getModelHandler();
    DbModelBuilder builder = getMainBuilder();
    QList<DbModel *> list;
    if (hdl && builder) {
        list = hdl->getAll(builder,
                           status,
                           nullptr, // model name
                           from, noItems, total);
    } else {
        loge("not found default handler or builder");
    }
    logd("item counter = %lld", list.size());
    tracede;
    return list; // caller must free data after use, to avoid memory leakage
}

ErrCode ModelController::reloadDb()
{
    traced;
    ErrCode err = ErrNone;
    (void)getAllItems(true); // reload data to cache
    tracedr(err);
    return err;
}

QList<DbModel *> ModelController::getAllItems(bool readFromDb, int from, int noItems, int *total)
{
    traced;
    logd("reload %d", readFromDb);
    logd("from %d", from);
    logd("noItems %d", noItems);
    // TODO: use share pointer to avoid overhead???
    QList<DbModel*> list;
    if (mEnableCache) {
        logd("get data from cache if any");
        // reload data from db, or current cached be empty
        if (readFromDb || mCacheItemList.empty()) {
            // TODO: reload data from db when add/delete/modify item from db???
            logd("reload from db");
            clearCacheItemList();
            QList<DbModel*> dbItems = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total);
            ErrCode err = ErrNone;
            foreach (DbModel* item, dbItems) {
                err = insertModelToCache(item, false); // don't clone, so we don't need to clear dbItems
                if (err != ErrNone && item) {
                    loge("Add '%s' to cache failed, delete data", STR2CHA(item->toString()));
                    delete item;
                }
            }
        }
        logd("Copy data from cache");
        foreach (DbModel* item, mCacheItemList.values()) {
            if (item) {
                DbModel* cloneItem = item->clone();
                if (cloneItem) {
                    list.append(cloneItem);
                } else {
                    loge("Clone failed, no data/memory???");
                }
            }
        }
    } else {
        logd("not support cache, get from db directly");
        list = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total);
    }
    logd("got %lld item", list.size());
    tracede;
    return list;
}

quint64 ModelController::getExportTypeList()
{
    // default supported one
    return ExportType::EXPORT_CSV_LIST | ExportType::EXPORT_XLSX;

}

DbModel *ModelController::getModelByUid(const QString &uid)
{
    traced;
    DbModel* model = nullptr;
    if (!uid.isEmpty()) {
        logd("get model by uid '%s'", STR2CHA(uid));
        if (mEnableCache) {
            logd("Try to get from cache");
            if (mCacheItemList.contains(uid)) {
                DbModel* cacheModel = mCacheItemList.value(uid);
                if (cacheModel) {
                    logd("Found '%s' from cache", STR2CHA(cacheModel->toString()));
                    // clone it, not get directly from cache
                    model = cacheModel->clone();
                } else {
                    loge("Cach contain null value!!!");
                }
            }
        }
        if (!model) {
            logd("not enable cached (%d) or not found?s, try to get from db", mEnableCache);
            model = getModelByUidFromDb(uid);
            if (model && mEnableCache) {
                logd("Save back to cache");
                ErrCode err = insertModelToCache(model, true); // clone it, as model will be returned back
                // when fall to this branch, mean getting from cache failed (no data)
                // so save back to cache
                if (err != ErrNone) {
                    loge("Add model '%s' to cache failed, ret=%d",
                         STR2CHA(model->toString()), err);
                } else {
                    logd("Add model '%s' to cache, ret=%d", STR2CHA(model->toString()), err);
                }
            }
        }
    } else {
        loge("request data from empty uid, seem something wrong");
    }

    tracede;
    return model;
}


int ModelController::search(const QString &keyword, QList<DbModel *> *outList)
{
    int ret = 0;
    traced;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->search(keyword, outList);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = 0;
        // TODO: should throw exception???
    }

    tracedr(ret);
    return ret;
}

DbModel *ModelController::getModelByName(const QString &name)
{
    traced;
    DbModel* model = nullptr;
    DbModelHandler *hdl = getModelHandler();
    logd("name %s", name.toStdString().c_str());
    if (hdl != nullptr) {
        model = hdl->getByName(name);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        model = nullptr;
        // TODO: should throw exception???
    }

    tracede;
    return model;
}

DbModel *ModelController::getModelByUidFromDb(const QString &uid)
{
    traced;
    DbModel* model = nullptr;
    DbModelHandler *hdl = getModelHandler();
    logd("uid %s", uid.toStdString().c_str());
    if (hdl != nullptr) {
        model = hdl->getByUid(uid);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        model = nullptr;
        // TODO: should throw exception???
    }

    tracede;
    return model;
}

DbModel *ModelController::getModelByNameId(const QString &nameId)
{
    traced;
    DbModel* model = nullptr;
    if (!nameId.isEmpty()) {
        logd("get model by name id '%s'", STR2CHA(nameId));
        if (mEnableCache) {
            logd("Try to get from cache");
            foreach (DbModel* item, mCacheItemList.values()) {
                if (item) {
                    logd("Check name id '%s'", STR2CHA(item->nameId()));
                    if (item->nameId() == nameId) {
                        logd("Found name id '%s'", STR2CHA(nameId));
                        model = item->clone();
                        break;
                    }
                } else {
                    // seem something wrong
                    logw("null item in cache may something wrong happen!!!");
                }
            }
        }
        if (!model) {
            logd("not enable cached (%d) or not found, try to get from db", mEnableCache);
            model = getModelByNameIdFromDb(nameId);
            if (model && mEnableCache) {
                logd("Save back to cache");
                ErrCode err = insertModelToCache(model, true); // clone it, as model will be returned back
                // when fall to this branch, mean getting from cache failed (no data)
                // so save back to cache
                if (err != ErrNone) {
                    loge("Add model '%s' to cache failed, ret=%d",
                         STR2CHA(model->toString()), err);
                } else {
                    logd("Add model '%s' to cache, ret=%d", STR2CHA(model->toString()), err);
                }
            }
        }
    } else {
        loge("request data from empty name, seem something wrong");
    }

    tracede;
    return model;
}

DbModel *ModelController::getModelByNameIdFromDb(const QString &nameId)
{
    traced;
    logd("get model from name id '%s", STR2CHA(nameId));
    DbModel* model = nullptr;
    DbModelHandler *hdl = getModelHandler();
    logd("uid %s", nameId.toStdString().c_str());
    if (hdl != nullptr) {
        model = hdl->getByNameId(nameId);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        model = nullptr;
        // TODO: should throw exception???
    }

    tracede;
    return model;

}

ErrCode ModelController::importFromFile(const QString &importName, ImportType type, const QString &fpath, QList<DbModel *> *outList)
{
    traced;
    ErrCode ret = ErrNone;

    ret = ImportFactory::importFrom(importName, this,fpath,type, outList);
    tracedr(ret);
    return ret;
}

void ModelController::clearCacheItemList()
{
    traced;
    logd("clear %lld item in cache", mCacheItemList.size());
    foreach(DbModel* item, mCacheItemList.values()) {
        if (item) delete item;
    }
    mCacheItemList.clear();
    tracede;
}

int ModelController::filter(int catetoryid,
                       const QString &catetory,
                       qint64 opFlags,
                       const QString &keywords,
                       QList<DbModel*>* outList)
{
    int ret = 0;
    traced;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->filter(catetoryid, opFlags, keywords, outList);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = 0;
        // TODO: should throw exception???
    }

    tracedr(ret);
    return ret;
}


ErrCode ModelController::getUidListFromName(const QString &name, QHash<QString, QString> *list, const char *hdlName)
{
    traced;
    DbModelHandler* hdlr = nullptr;
    ErrCode ret = ErrNone;
    QHash<QString, QString> uidList;
    if (hdlName != nullptr) {
        logd("model hdl name %s", hdlName);
        hdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(hdlName));
    } else {
        logd("Use default model hdl");
        hdlr = getModelHandler();
    }

    if (hdlr == nullptr) {
        loge("Invalid handler");
        ret = ErrFailed;
    }
    // TODO: read from cache?? there'll be a lot of items
    if (ret == ErrNone) {
        if (!name.isEmpty()) {
            QStringList names = name.split(HOLLYNAME_SPLIT);
            DbModel* model = nullptr;
            QString hollyNotFound;
            foreach (QString name, names) {
                logd("Check name '%s'", name.toStdString().c_str());
                model = hdlr->getByName(name.trimmed());
                if (model) {
                    logd("update uid %s", model->uid().toStdString().c_str());
                    uidList.insert(model->uid(), model->name());
                    delete model;
                } else {
                    loge("Name '%s' not found in db", name.toStdString().c_str());
                    ret = ErrNotFound;
                    break;
                }
            }
        }
    }

    if (ret == ErrNone && uidList.count() > 0 && list != nullptr) {
        logd("found %d model from name %s", uidList.count(), name.toStdString().c_str());
        list->insert(uidList);
    }
    tracedr(ret);
    return ret;
}

QString ModelController::getNameFromUidList(const QStringList &uidList, const char *hdlName)
{
    traced;
    DbModelHandler* hdlr = nullptr;
    ErrCode ret = ErrNone;
    QString name;
    QStringList nameList;
    if (hdlName != nullptr) {
        logd("model hdl name %s", hdlName);
        hdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(hdlName));
    } else {
        logd("Use default model hdl");
        hdlr = getModelHandler();
    }

    if (hdlr == nullptr) {
        loge("Invalid handler");
        ret = ErrFailed;
    }
    // TODO: read from cache?? there'll be a lot of saints
    if (ret == ErrNone) {
        if (!uidList.isEmpty()) {
            DbModel* model = nullptr;
            foreach (QString uid, uidList) {
                // TODO: cached it instead of reload from scratch??
                logd("Check uid '%s'", uid.toStdString().c_str());
                model = hdlr->getByUid(uid.trimmed());
                if (model) {
                    logd("update name %s", model->name().toStdString().c_str());
                    nameList.append(model->name());
                    delete model;
                } else {
                    loge("uid '%s' not found in db", uid.toStdString().c_str());
                    ret = ErrNotFound;
                    break;
                }
            }
        } else {
            logi("No saint uid to check");
        }
    }

    if (ret == ErrNone && nameList.count() > 0) {
        logd("Found %d saint", nameList.count());
        name = nameList.join(NAME_SPLIT);
    }
    tracedr(ret);
    return name;
}

QString ModelController::getNameFromUidList(const QString &uidList, const char *hdlName)
{
    traced;
    return getNameFromUidList(uidList.split(NAME_SPLIT), hdlName);

}

const char *ModelController::getPrebuiltFileName()
{
    traced;
    return nullptr;

}

const char *ModelController::getPrebuiltFileType()
{
    traced;
    return KFileTypeCSV; // default one
}



ErrCode ModelController::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    // TODO: should we add meta field in beginning of file to know status of information???
    // i.e.: version, last update time.
    // This can be used to check/compare witl one stored in db
    logi("Parse prebuilt file %s, fpath '%s', ftype '%s'",
         STR2CHA(getName()), STR2CHA(fpath), STR2CHA(ftype));

    if (fpath.isEmpty() || ftype.isEmpty()) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }

    if (ret == ErrNone) {
        if (ftype == KFileTypeCSV) {
            ret = Utils::parseCSVFile(fpath,
                                      &ModelController::onCsvParseOneItemCallback,
                                      this);
        } else if (ftype == KFileTypeJson) {
            logd("Load file %s", fpath.toStdString().c_str());
            QFile loadFile(fpath);

            if (!loadFile.open(QIODevice::ReadOnly)) {
                loge("Couldn't open file %s", fpath.toStdString().c_str());
                ret = ErrFileRead;
            }

            if (ret == ErrNone){
                logd("Load file %s", fpath.toStdString().c_str());
                QFile loadFile(fpath);
                QByteArray importData;

                if (!loadFile.open(QIODevice::ReadOnly)) {
                    loge("Couldn't open file %s", fpath.toStdString().c_str());
                    ret = ErrFileRead;
                }

                if (ret == ErrNone){
                    logd("Parse json");
                    importData = loadFile.readAll();
                    logd("importData length %d", (int)importData.length());
                    // TODO: too big data????
                    if (importData.size() == 0) {
                        ret = ErrNoData;
                        loge("No data to import");
                    }
                }

                if (ret == ErrNone) {
                    QJsonDocument loadDoc = QJsonDocument::fromJson(importData);

                    logd("loadDoc isEmpty %d", loadDoc.isEmpty());
                    QJsonObject jRootObj = loadDoc.object();

                    if (jRootObj.contains(JSON_ITEMS) && jRootObj[JSON_ITEMS].isArray()) {
                        QJsonArray jlist = jRootObj[JSON_ITEMS].toArray();
                        for (int levelIndex = 0; levelIndex < jlist.size() && (ret == ErrNone); ++levelIndex) {
                            logd("Import community idx=%d", levelIndex);
                            QJsonObject jObj = jlist[levelIndex].toObject();
                            bool ok = false;
                            DbModel* model = onJsonParseOneItem(jObj, &ok);
                            if (model) {
                                if (ok && model->isValid()) {
                                    ret = model->save();
                                    if (ret == ErrExisted) { // already exist, mean ok
                                        ret = ErrNone;
                                        logw("'%s' already exist", STR2CHA(model->toString()));
                                    }
                                } else {
                                    loge("Invalid '%s'", STR2CHA(model->toString()));
                                    ret = ErrInvalidData;
                                }
                                delete model;
                            } else {
                                ret = ErrFailed;
                                loge("parse json one item failed");
                            }

                        }
                    } else {
                        loge("Invalid json data, not found %s", JSON_ITEMS);
                        ret = ErrInvalidData;
                    }
                }
                loadFile.close();
            }
        } else {
            ret = ErrNotSupport;
        }
    }
    tracedr(ret);
    return ret;
}
ErrCode ModelController::check2UpdateDbFromPrebuiltFile(const QString &name, const QString &ftype)
{
    traced;
    ErrCode ret = ErrNone;
    logd("check update for '%s', ftype '%s'", STR2CHA(name), STR2CHA(ftype));
    // TODO: file should be from installed dir, rather than embedded inside bin??
    QString fname = Utils::getPrebuiltFileByLang(name);
    if (!FileCtl::checkPrebuiltDataFileHash(fname)){
        ret = parsePrebuiltFile(FileCtl::getPrebuiltDataFilePath(fname), ftype);
        if (ret == ErrNone){
            FileCtl::updatePrebuiltDataFileHash(fname);
        }
        else{
            logi("Check to update db from file failed %d", ret);
        }
    }
    else {
        logi("Prebuilt saint file up-to-date");
    }
    tracedr(ret);
    return ret;
}

ErrCode ModelController::check2UpdateDbFromPrebuiltFile()
{
    traced;
    logd("check 2 update for controller %s", mName.toStdString().c_str());
    ErrCode ret = ErrNone;
    QString fname = getPrebuiltFileName();
    if (!fname.isEmpty()) {
        logi("Check & load from prebuilt file");
        ret = check2UpdateDbFromPrebuiltFile(fname, getPrebuiltFileType());
        logd("check2UpdateDbFromPrebuiltFile ret=%d", ret);
        // TODO: should do lazyload???
    } else {
        logi("Not load from prebuilt file, no prebuilt filename");
    }
    tracedr(ret);
    return ret;
}

ErrCode ModelController::doCsvParseOneItem(const QStringList &items, void *param)
{
    traced;
    ErrCode ret = ErrNone;
    (void) param;
    if (!items.empty()) {
        DbModel* model = buildModel((void*)&items, KDataFormatStringList);
        if ((model != nullptr) && model->isValid()){
            model->dump();
            logi("Save model '%s'", model->name().toStdString().c_str());
            ret = model->save();
        }
        else{
            ret = ErrInvalidData;
            loge("Model data is invalid");
        }
        if (model) delete model;
    }
    else {
        loge("Invalid data");
        // TODO: should break or continue???
        ret = ErrInvalidData;
    }
    logd("ret %d", ret);
    return ret;
}

DbModel *ModelController::onJsonParseOneItem(const QJsonObject &jobj, bool *ok)
{
    traced;
    ErrCode err = ErrNone;
    DbModel* model = allocMainModel();
    if (!model) {
        err = ErrNoMemory;
        loge("no memory");
    }
    if (err == ErrNone) {
        JSON_GET_TO_SET_STR(jobj, JSON_NAMEID, model->setNameId);
        JSON_GET_TO_SET_STR(jobj, JSON_NAME, model->setName);
        JSON_GET_TO_SET_STR(jobj, JSON_REMARK, model->setRemark);
    }
    logd("Parse result %d", err);
    if (ok) *ok = (err == ErrNone);
    tracede;
    return model;
}

ErrCode ModelController::onCsvParseOneItemCallback(const QStringList &items, void* caller, void *param, quint32 idx)
{
    traced;
    ErrCode ret = ErrFailed;
    if (caller != nullptr){
        ret = ((ModelController*)caller)->doCsvParseOneItem(items, param);
    }
    return ret;
}

ErrCode ModelController::onLoad(){
    traced;
    logd("Onload %s", mName.toStdString().c_str());
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile();
    if (ret != ErrNone) {
        // TODO: report error/issue???
        loge("Check & updaet db from prebuild failed, ret=%d", ret);
    }
    ret = reloadDb(); // TODO: handle error case????
    logd("load from db ret=%d", ret);
    tracedr(ret);
    return ret;
}


ErrCode ModelController::getListExportKeywords(Exporter* exporter,
                                         QHash<QString, QString>& outMap) const
{
    traced;
    ErrCode err = ErrNone;

    QString fpath;
    QFile loadFile;
    QByteArray fileData;

    if (exporter) {
        fpath = exportTemplatePath(exporter);
    } else {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone && fpath.isEmpty()) {
        err = ErrInvalidArg;
        loge("not found template file");
    }
    if (err == ErrNone){
        logd("Load file %s", fpath.toStdString().c_str());
        loadFile.setFileName(fpath);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            loge("Couldn't open file %s", fpath.toStdString().c_str());
            err = ErrFileRead;
        }
    }

    if (err == ErrNone){
        // TODO: ASSUME only JSON, how about other????
        logd("Parse json");
        fileData = loadFile.readAll();
        if (!fileData.isEmpty()) {
            logd("fileData length %d", (int)fileData.length());
        } else {
            err = ErrNoData;
            loge("file '%s' is empty?", STR2CHA(fpath));
        }
    }

    if (err == ErrNone) {
        QJsonDocument loadDoc = QJsonDocument::fromJson(fileData);

        logd("loadDoc isEmpty %d", loadDoc.isEmpty());
        QJsonObject jRootObj = loadDoc.object();
        if (jRootObj.contains(JSON_ITEMS) && jRootObj[JSON_ITEMS].isArray()) {
            QJsonArray jlist = jRootObj[JSON_ITEMS].toArray();
            for (int levelIndex = 0; levelIndex < jlist.size(); ++levelIndex) {
                QJsonObject jObj = jlist[levelIndex].toObject();
                QString id;
                QString name;
                // id
                if (jObj.contains(JSON_ID)){
                    id = jObj[JSON_ID].toString().trimmed();
                } else {
                    loge("lack of id field %s", JSON_ID);
                }
                if (id.isEmpty()){
                    loge("invalid template, id not found/empty");
                    err = ErrInvalidData;
                    break;
                }
                //name
                if (jObj.contains(JSON_NAME)){
                    name = jObj[JSON_NAME].toString().trimmed();
                }
                // not found name, get default name???
                if (name.isEmpty()) {
                    name = exportItem2Name(id);
                }
                logd("id=%s, name=%s", STR2CHA(id), STR2CHA(name));
                if (!id.isEmpty() && !name.isEmpty()) {
                    outMap.insert(id, name);
                } else {
                    err = ErrInvalidData;
                    loge("Export template lacks field of id or name");
                    break;
                }
            }
        } else {
            loge("Invalid data, not found %s", JSON_ITEMS);
            err = ErrInvalidData;
        }
    }
    loadFile.close();
    tracedr(err);
    return err;
}

ErrCode ModelController::insertModelToCache(DbModel* model, bool clone)
{
    traced;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidData;
        loge("Invalid argument");
    }
    if (err == ErrNone && !mEnableCache) {
        err = ErrNotSupport;
        logw("cach is not enable");
    }
    if (err == ErrNone) {
        logd("Save '%s' back to cache, clone: %d", STR2CHA(model->toString()), clone);
        // when fall to this branch, mean getting from cache failed (no data)
        // so save back to cache
        DbModel* saveModel = nullptr;
        if (clone) {
            model->clone();
        } else {
            saveModel = model;
        }
        if (saveModel && !saveModel->uid().isEmpty()) {
            if (mCacheItemList.contains(saveModel->uid())) {
                logw("'%s' already existed in cache, delete it first", STR2CHA(saveModel->toString()));
                DbModel* exist = mCacheItemList.value(saveModel->uid());
                if (exist) {
                    delete exist;
                } else {
                    logw("uid exist in cached, but no data, strange info!");
                }
                mCacheItemList.remove(saveModel->uid());
            }
            logd("Add model '%s' to cache", STR2CHA(saveModel->toString()));
            mCacheItemList.insert(saveModel->uid(), saveModel);
        } else {
            logw("%s", saveModel?"no uid":"run out of memory");
            err = saveModel?ErrInvalidData:ErrNoMemory;
        }
    }
    tracedr(err);
    return err;
}


const QString ModelController::exportTemplatePath(Exporter *exporter) const
{
    traced;
    QString fpath;
    if (exporter) {
        logd("export type %d", exporter->getExportType());
        switch (exporter->getExportType()) {
        case EXPORT_CSV_LIST:
        case EXPORT_XLSX:
            // TODO: docx, text???
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltDefaultExportTemplateName);
            break;
        default:
            loge("invalid export type %d", exporter->getExportType());
            break;
        };
    } else {
        loge("invalid exporter");
        // TODO: report or raise exception???
    }
    logd("fpath '%s'", STR2CHA(fpath));
    tracede;
    return fpath;
}

ErrCode ModelController::getExportFileName(ExportType type, QString fnameNoExt, QString *fpath)
{
    traced;
    ErrCode ret = ErrNone;
    QString ext;
    QString fname;
    bool isExtOk = false;
    if (!fpath) {
        ret = ErrInvalidArg;
        loge("invalid arg");
    }
    if (ret == ErrNone) {
        ext = typeToExt(type, &isExtOk);
        if (!isExtOk) {
            ret = ErrInvalidArg;
            loge("invalid export type %d", type);
        }
    }
    if (ret == ErrNone) {
        fname = QString("%1.%2").arg(fnameNoExt, ext);
        if (fpath && fpath->isEmpty()) {
            *fpath = FileCtl::getTmpDataDir(fname);
        }
    }
    tracedr(ret);
    return ret;
}

ErrCode ModelController::exportToFile(DbModel *model, ExportType type, QString *fpath)
{
    traced;
    ErrCode ret = ErrNone;
    QString ext;
    QString fname;
    bool isExtOk = false;
    if (!model) {
        ret = ErrInvalidArg;
        loge("export to file failed, invalid arg");
    }
    if (ret == ErrNone) {
        ret = getExportFileName(type, model->uid(), fpath);
    }
    if (ret == ErrNone) {
        ret = ExportFactory::exportTo(model->getExporter(),
                                      *fpath, type);
    }
    // TODO: DbModel also has this function, redundant?????
    tracedr(ret);
    return ret;
}

ErrCode ModelController::exportToFile(const QList<DbModel *>* listModel, ExportType type, QString *fpath)
{
    traced;
    ErrCode ret = ErrNone;
    QString ext;
    QString fname;
    bool isExtOk = false;
    if (!listModel) {
        ret = ErrInvalidArg;
        loge("export to file failed, invalid arg");
    }
    if (ret == ErrNone) {
        ret = getExportFileName(type, getName(), fpath);
    }
    if (ret == ErrNone) {
        logd("export file path %s", STR2CHA((*fpath)));
        ret = ExportFactory::exportTo(this, *listModel,
                                      *fpath, type);
    }
    // TODO: DbModel also has this function, redundant?????
    tracedr(ret);
    return ret;
}


ErrCode ModelController::onImportItem(const QString& importName, int importFileType,
                                const QStringList &items, quint32 idx, void *tag)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* model = doImportOneItem(importName, importFileType, items, idx);
    QList<DbModel*> *list = (QList<DbModel*>*)tag;
    if (model != nullptr) {
        if (list != nullptr) {
            list->append(model);
        } else {
            delete model;
        }
    } else {
        // do nothing
        // TODO: handle error case
        // be ware the case that fist item is header!!
    }
    // TODO: handle error case
    tracedr(ret);
    return ret;


}

ErrCode ModelController::onImportItem(const QString& importName, int importFileType,
                                const QHash<QString, QString> &items, quint32 idx, void *tag)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* model = doImportOneItem(importName, importFileType, items, idx);
    QList<DbModel*> *list = (QList<DbModel*>*)tag;
    if (model != nullptr) {
        if (list != nullptr) {
            list->append(model);
        } else {
            delete model;
        }
    } else {
        // do nothing
        // TODO: handle error case
        // be ware the case that fist item is header!!
    }
    // TODO: handle error case
    tracedr(ret);
    return ret;

}


DbModel* ModelController::doImportOneItem(const QString& importName, int importFileType, const QStringList &items, quint32 idx)
{
    traced;
    loge("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return nullptr;
}

DbModel *ModelController::doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    traced;
    loge("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return nullptr;
}

DbModel* ModelController::allocMainModel()
{
    traced;
    DbModel* model = NULL;
    DbModelBuilder builder = getMainBuilder();
    if (builder) {
        model = builder();
        if (!model) {
            loge("No memory");
        }
    } else {
        loge("No main builder");
    }
    tracede;
    return model;
}

// default format: name id, name, remark
DbModel *ModelController::buildModel(void *items, const QString &fmt)
{
    traced;
    ErrCode err = ErrNone;
    DbModel* item = nullptr;
    QStringList* itemList = nullptr;
    qint32 noItem = 0;
    qint32 idx = 0;
    QString nameId;
    QString name;
    QString remark;
    if (!items) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone && fmt != KDataFormatStringList) {
        err = ErrNotSupport;
        loge("invalid data format '%s", STR2CHA(fmt));
    }
    if (err == ErrNone) {
        itemList = (QStringList*) items;
        noItem = itemList->length();
        logd("noItem %d", noItem);
    }

    if (err == ErrNone && (noItem < 2)) { // require name id, name
        err = ErrShortData;
        loge("Not enouth field, %d < 3", noItem);
    }

    if (err == ErrNone) {
        nameId = itemList->at(idx++);
        name = itemList->at(idx++);
        if (noItem > idx) {
            remark = itemList->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
        }

        logd("nameId '%s'", STR2CHA(nameId));
        logd("name '%s'", STR2CHA(name));
        if (nameId.isEmpty() || name.isEmpty()) {
            err = ErrInvalidData;
            loge("data is invalid, nameid/countrynameid or name is empty");
        }
    }
    if (err == ErrNone) {
        DbModelBuilder builder = getMainBuilder();
        if (builder) {
            item = builder();
            if (!item) {
                loge("No memory");
                err = ErrNoMemory;
            }
        } else {
            loge("No main builder");
            err = ErrNotAvail;
        }
    }

    if (err == ErrNone) {
        item->setNameId(nameId);
        item->setName(name);
        if (!remark.isEmpty()) {
            item->setRemark(remark);
        }
    } else {
        loge("failed to import/build model, err=%d", err);
        if (item) {
            delete item;
            item = nullptr;
        }
    }
    tracede;
    return item;
}
