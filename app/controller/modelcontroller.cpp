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
#include "filter.h"
#include "jsondefs.h"
#include "prebuiltdefs.h"
#include "dbmodelfactory.h"


ModelController::ModelController():
    mEnableCache(false), // TODO: when enable cache, data is not sort, so temporary disable now
    mReloadDb(false)
{
    tracein;
    mCacheItemList.clear();
    traceout;
}

ModelController::~ModelController()
{
    tracein;
    clearCacheItemList();
    traceout;
}

ModelController::ModelController(const QString& name, const QString& hdlName):ModelController()
{
    tracein;
    mName = name;
    mMainModelHandlerName = hdlName;
    logd("construct with name=%s", STR2CHA(name));
    traceout;
}

void ModelController::init()
{
    tracein;
    logd("Init controller '%s'", STR2CHA(getName()));
    logd("Register signal/slots");
    QObject::connect(this, &ModelController::dataUpdate,
                     this, &ModelController::onModelControllerDataUpdated,
                     Qt::QueuedConnection);

    DbModelHandler* hdl = getModelHandler();
    if (hdl) {
        hdl->addListener(this);
    }
    traceout;
}

QString ModelController::getName() const
{
    return mName;
}

QString ModelController::getMainModelHandlerName()
{
    return mMainModelHandlerName;
}

DbModelHandler *ModelController::getModelHandler()
{
    tracein;
    logd("Get model handler for controller using name: %s", STR2CHA(mName));
    // TODO: fix me if this is not suitble
    return DB->getModelHandler(getMainModelHandlerName());
}

void ModelController::addListener(OnModelControllerListener *listener)
{
    tracein;
    if (listener) {
        if (!mListeners.contains(listener)) {
            logd("Add listener '%s'", STR2CHA(listener->getName()));
            mListeners.append(listener);
        } else {
            loge("Listener '%s' already existed", STR2CHA(listener->getName()));
        }
    } else {
        loge("Invalid listener");
    }
    traceout;
}

void ModelController::delListener(OnModelControllerListener *listener)
{
    tracein;
    if (listener) {
        logd("Remove listener '%s'", STR2CHA(listener->getName()));
        mListeners.removeAll(listener);
    } else {
        loge("Invalid listener");
    }
    traceout;
}

QList<DbModel *> ModelController::getAllItemsFromDb(qint64 status, int from, int noItems, int* total, ErrCode* ret)
{
    tracein;
    ErrCode err = ErrNone;
    DbModelHandler* hdl = nullptr;
    DbModelBuilder builder = nullptr;
    QList<DbModel *> list;

    hdl = getModelHandler();
    if (!hdl) {
        err = ErrNotFound;
        loge("not found model handler");
    }
    if (err == ErrNone) {
        builder = getMainBuilder();
        if (!builder) {
            err = ErrNotFound;
            loge("Not found main builder");
        }
    }
    if (err == ErrNone) {
        list = hdl->getAll(builder,
                           status,
                           nullptr, // model name
                           from, noItems, total);
        // TODO: add error code return???
    }

    if (ret) {
        *ret = err;
    }
    logd("item counter = %lld", list.size());
    traceout;
    return list; // caller must free data after use, to avoid memory leakage
}

QList<DbModel *> ModelController::getAllItems(bool readFromDb, int from, int noItems, int *total)
{
    tracein;
    logd("reload %d", readFromDb);
    logd("from %d", from);
    logd("noItems %d", noItems);
    // TODO: use share pointer to avoid overhead???
    QList<DbModel*> list;
    ErrCode err = ErrNone;
    if (mEnableCache) {
        logd("get data from cache if any");
        // reload data from db, or current cached be empty
        if (mReloadDb || readFromDb || mCacheItemList.empty()) {
            // TODO: reload data from db when add/delete/modify item from db???
            logd("reload from db");
            clearCacheItemList();
            QList<DbModel*> dbItems = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total, &err);
            ErrCode err = ErrNone;
            foreach (DbModel* item, dbItems) {
                err = insertModelToCache(item, false); // don't clone, so we don't need to clear dbItems
                if (err != ErrNone && item) {
                    loge("Add '%s' to cache failed, delete data", STR2CHA(item->toString()));
                    delete item;
                }
            }
            if (err == ErrNone) {
                mReloadDb = false;
            }
        }
        logd("Copy data from cache");
        // Cache is hash map, so data is not sorted, take care
        foreach (DbModel* item, mCacheItemList.values()) {
            if (item) {
                DbModel* cloneItem = item->clone();
                if (cloneItem) {
                    list.append(cloneItem);
                } else {
                    loge("Clone failed, no data/memory???");
                    err = ErrFailed;
                    break;
                }
            }
        }
        if (err != ErrNone) {
            logd("Failed to query from cache, get directly from db");
            list = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total, &err);
        }
    } else {
        logd("not support cache, get from db directly");
        list = getAllItemsFromDb(DB_RECORD_ACTIVE, from, noItems, total, &err);
    }
    logd("got %lld item", list.size());
    traceout;
    return list;
}

int ModelController::getTotalItems(qint32 modelstatus)
{
    tracein;
    int ret = 0;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->getTotalItemCount(modelstatus);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = -(ErrUnknown);
        // TODO: should throw exception???
    }
    if (ret < 0) {
        loge("get total item failed, err=%d", ret);
    }
    traceout;
    return ret;
}

int ModelController::getTotalActiveItems()
{
    return getTotalItems(MODEL_STATUS_ACTIVE);
    
}

quint64 ModelController::getExportTypeList()
{
    // default supported one
    return ExportType::EXPORT_CSV_LIST | ExportType::EXPORT_XLSX;

}

const QString ModelController::exportListPrebuiltTemplateName(const QString& modelName) const
{
    return KPrebuiltDefaultExportTemplateName;
}

const QString ModelController::exportHtmlPrebuiltTemplateName(const QString &modelName) const
{
    return KPrebuiltCommonTemplateFileName;
}

DbModel *ModelController::getModelByUid(const QString &uid)
{
    tracein;
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

    traceout;
    return model;
}

ErrCode ModelController::search(const QString &keyword, QList<DbModel *> *outList,
                            int from,
                            int noItems,
                            int* total)
{
    ErrCode ret = ErrNone;
    tracein;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->search(keyword, outList, DB_RECORD_ACTIVE, from, noItems, total);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = ErrNoHandler;
        // TODO: should throw exception???
    }

    traceret(ret);
    return ret;
}

DbModel *ModelController::getModelByUidFromDb(const QString &uid)
{
    tracein;
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

    traceout;
    return model;
}

DbModel *ModelController::getModelByNameId(const QString &nameId)
{
    tracein;
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

    traceout;
    return model;
}

DbModel *ModelController::getModelByNameIdFromDb(const QString &nameId)
{
    tracein;
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

    traceout;
    return model;

}

ErrCode ModelController::importFromFile(const QString &importName, ImportType type,
                                        const QString &fpath, QList<DbModel *> *outList)
{
    tracein;
    ErrCode ret = ErrNone;
    logd("Import name '%s'", STR2CHA(importName));
    logd("Import fpath '%s'", STR2CHA(fpath));
    logd("ImportType %d", type);
    ret = ImportFactory::importFrom(importName, this, fpath,type, outList);
    traceret(ret);
    return ret;
}

void ModelController::clearCacheItemList()
{
    tracein;
    logd("clear %lld item in cache", mCacheItemList.size());
    foreach(DbModel* item, mCacheItemList.values()) {
        if (item) delete item;
    }
    mCacheItemList.clear();
    traceout;
}

ErrCode ModelController::filter(int fieldId,
                               qint64 opFlags,
                               const QString &keywords,
                               const char* targetModelName,
                               const DbModel* parentModel,
                               QList<DbModel*>* outList,
                               int from,
                               int noItems,
                               int* total)
{
    ErrCode ret = ErrNone;
    tracein;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->filter(fieldId, opFlags, keywords,
                          targetModelName, parentModel,
                          outList,
                          DB_RECORD_ACTIVE, from, noItems, total);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = ErrNoHandler;
    }

    traceret(ret);
    return ret;
}

ErrCode ModelController::filter(const QList<FilterKeyworkItem *> &filters,
                                const char *targetModelName, const DbModel *parentModel, QList<DbModel *> *outList, int from, int noItems, int *total)
{
    ErrCode ret = ErrNone;
    tracein;
    DbModelHandler *hdl = getModelHandler();
    if (hdl != nullptr) {
        ret = hdl->filter(filters, targetModelName, parentModel,
                          outList, DB_RECORD_ACTIVE, from, noItems, total);
    } else {
        loge("Unknown handler, DERIVED class should implement this");
        ret = ErrNoHandler;
    }

    traceret(ret);
    return ret;
}


ErrCode ModelController::getUidListFromName(const QString &name, QHash<QString, QString> *list, const char *hdlName)
{
    tracein;
    DbModelHandler* hdlr = nullptr;
    ErrCode ret = ErrNone;
    if (name.isEmpty()) {
        ret = ErrInvalidArg;
        loge("inavlid argument");
    }
    if (ret == ErrNone) {
        if (hdlName != nullptr) {
            logd("model hdl name %s", hdlName);
            hdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(hdlName));
        } else {
            logd("Use default model hdl");
            hdlr = getModelHandler();
        }

        if (hdlr == nullptr) {
            loge("Invalid handler");
            ret = ErrNoHandler;
        }
    }

    // TODO: read from cache?? there'll be a lot of items
    if (ret == ErrNone) {
        QStringList names = name.split(NAME_SPLIT);
        foreach (QString name, names) {
            QList<DbModel*> modellist;
            logd("Check name '%s'", name.toStdString().c_str());
            ret = hdlr->filter(FILTER_FIELD_NAME, FILTER_OP_EQUAL, name.trimmed(), nullptr, nullptr, &modellist);
            if (ret == ErrNone) {
                if (modellist.size()) {
                    foreach(DbModel* model, modellist) {
                        if (model) {
                            logd("add uid '%s', name '%s'", STR2CHA(model->uid()),
                                 STR2CHA(model->name()));
                            if (list) list->insert(model->uid(), model->name());
                            delete model;
                        } else {
                            loge("Something went wrong, null model in output list");
                        }
                    }
                } else {
                    logd("Name '%s' not found in db", name.toStdString().c_str());
                }
            } else {
                loge("filter failed, ret=%d", ret);
                break;
            }
        }
    }

    traceret(ret);
    return ret;
}

QString ModelController::getNameFromUidList(const QStringList &uidList, const char *hdlName)
{
    tracein;
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
        ret = ErrNoHandler;
    }

    // TODO: read from cache?? there'll be a lot of saints
    if (ret == ErrNone) {
        if (!uidList.isEmpty()) {
            DbModel* model = nullptr;
            foreach (QString uid, uidList) {
                // TODO: cached it instead of reload from scratch??
                logd("Check uid '%s'", STR2CHA(uid));
                model = hdlr->getByUid(uid.trimmed());
                if (model) {
                    logd("update name %s", STR2CHA(model->name()));
                    nameList.append(model->name());
                    delete model;
                } else {
                    loge("uid '%s' not found in db", STR2CHA(uid));
                    ret = ErrNotFound;
                    break;
                }
            }
        } else {
            logi("No saint uid to check");
        }
    }

    if (ret == ErrNone && nameList.count() > 0) {
        logd("Found %lld saint", nameList.size());
        name = nameList.join(NAME_SPLIT);
        logd("name: '%s'", STR2CHA(name));
    } else {
        loge("get name list for uid '%s' failed or no data, ret=%d",
             STR2CHA(uidList.join(NAME_SPLIT)), ret);
    }
    traceret(ret);
    return name;
}

QString ModelController::getNameFromUidList(const QString &uidList, const char *hdlName)
{
    tracein;
    logd("get name from uid '%s'", STR2CHA(uidList));
    return getNameFromUidList(uidList.split(NAME_SPLIT), hdlName);

}

const char *ModelController::getPrebuiltFileName()
{
    tracein;
    return nullptr;

}

const char *ModelController::getPrebuiltFileType()
{
    tracein;
    return KFileTypeCSV; // default one
}

char ModelController::getCsvSplitItem()
{
    return DEFAULT_CSV_ITEM_SPLIT;
}



ErrCode ModelController::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    tracein;
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
                                      this, nullptr, getCsvSplitItem());
        } else if (ftype == KFileTypeJson) {
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
        } else {
            ret = ErrNotSupport;
        }
    }
    traceret(ret);
    return ret;
}

void ModelController::onDbModelHandlerDataUpdate(DbModel *model, int type, ErrCode err)
{
    tracein;
    logd("controller '%s'", STR2CHA(getName()));
    reloadDb();
    emit dataUpdate(CLONE_DBMODEL(model));
    traceout;
}

void ModelController::reloadDb()
{
    tracein;
    mReloadDb = true;
    // TODO: notify/emit event???
    traceout;
}

ErrCode ModelController::check2UpdateDbFromPrebuiltFile(const QString &name, const QString &ftype)
{
    tracein;
    ErrCode ret = ErrNone;
    QString fpath;
    logd("check update for '%s', ftype '%s'", STR2CHA(name), STR2CHA(ftype));
    if (name.isEmpty() || ftype.isEmpty()) {
        ret = ErrInvalidArg;
        loge("invalid argument");
    }

    if (ret == ErrNone) {
    // TODO: file should be from installed dir, rather than embedded inside bin??
        fpath = Utils::getPrebuiltFileByLang(name);
        if (fpath.isEmpty()) {
            ret = ErrFailed;
            loge("failed to get prebuilt full path by name");
        }
    }
    if (ret == ErrNone) {
        if (!FileCtl::checkPrebuiltDataFileHash(fpath)){
            ret = parsePrebuiltFile(FileCtl::getPrebuiltDataFilePath(fpath), ftype);
            if (ret == ErrNone){
                FileCtl::updatePrebuiltDataFileHash(fpath);
            }
            else{
                logi("Check to update db from file failed %d", ret);
            }
        }
        else {
            logi("Prebuilt saint file up-to-date");
        }
    }
    traceret(ret);
    return ret;
}

ErrCode ModelController::check2UpdateDbFromPrebuiltFile()
{
    tracein;
    logd("check 2 update for controller %s", mName.toStdString().c_str());
    ErrCode ret = ErrNone;
    QString fname = getPrebuiltFileName();
    if (!fname.isEmpty()) {
        logi("Check & load from prebuilt file");
        ret = check2UpdateDbFromPrebuiltFile(fname, getPrebuiltFileType());
        logd("check2UpdateDbFromPrebuiltFile ret=%d", ret);
    } else {
        logi("Not load from prebuilt file, no prebuilt filename");
    }
    traceret(ret);
    return ret;
}

ErrCode ModelController::doCsvParseOneItem(const QStringList &items, void *param)
{
    tracein;
    ErrCode ret = ErrNone;
    (void) param;
    if (!items.empty()) {
        DbModel* model = buildModel((void*)&items, KDataFormatStringList);
        if ((model != nullptr) && model->isValid()) {
            logi("Save model '%s'", STR2CHA(model->toString()));
            ret = model->save();
            logd("save ret = %d", ret);
            if (ret == ErrExisted) {
                logw("'%s' already exist, skip", STR2CHA(model->toString()));
                ret = ErrNone;
            }
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
    tracein;
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
    traceout;
    return model;
}

ErrCode ModelController::onCsvParseOneItemCallback(const QStringList &items, void* caller, void *param, quint32 idx)
{
    tracein;
    ErrCode ret = ErrFailed;
    if (caller != nullptr){
        ret = ((ModelController*)caller)->doCsvParseOneItem(items, param);
    }
    return ret;
}

void ModelController::onModelControllerDataUpdated(DbModel* model)
{
    tracein;
    logd("controller '%s'", STR2CHA(getName()));
    mReloadDb = true;
    foreach (OnModelControllerListener* listener, mListeners) {
        if (listener) {
            listener->onModelControllerDataUpdated(model);
        }
    }
    FREE_PTR(model);
    traceout;
}

ErrCode ModelController::onLoad(){
    tracein;
    logd("Onload %s", mName.toStdString().c_str());
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile();
    if (ret != ErrNone) {
        // TODO: report error/issue???
        loge("Check & updaet db from prebuild failed, ret=%d", ret);
    }

    traceret(ret);
    return ret;
}

void ModelController::onUnload()
{
    traced;
    logd("unload something?");
    // TODO: unload something?
}


ErrCode ModelController::insertModelToCache(DbModel* model, bool clone)
{
    tracein;
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
            saveModel = model->clone();
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
    traceret(err);
    return err;
}


ErrCode ModelController::exportTemplatePath(
                            FileExporter *exporter,
                            const QString& name,
                            QString& fpath,
                            QString* ftype) const
{
    tracein;
    QString templateName;
    ErrCode err = ErrNone;
    if (exporter) {
        logd("export type %d", exporter->getExportType());
        switch (exporter->getExportType()) {
        case EXPORT_CSV_LIST:
        case EXPORT_XLSX:
            templateName = exportListPrebuiltTemplateName(name);
            if (ftype) *ftype = KFileTypeJson;
            break;
        case EXPORT_HTML:
            templateName= exportHtmlPrebuiltTemplateName(name);
            if (ftype) *ftype = KFileTypeHtml;
            break;
            // TODO: docx, text???
        default:
            loge("invalid export type %d", exporter->getExportType());
            err = ErrInvalidArg;
            break;
        };
    } else {
        loge("invalid exporter");
        err = ErrInvalidArg;
        // TODO: report or raise exception???
    }
    if (err == ErrNone) {
        if (!templateName.isEmpty()) {
            fpath = FileCtl::getPrebuiltDataFilePath(templateName);
        } else {
            loge("NOT support export type %d", exporter->getExportType());
            err = ErrNotSupport;
        }
    }
    logd("fpath '%s'", STR2CHA(fpath));
    traceout;
    return err;
}

ErrCode ModelController::getExportDataString(const QString &item,
                                             const FileExporter* fileexporter,
                                             const QString& datatype,
                                             const DbModel *data,
                                             QString *exportData) const
{
    tracein;
    ErrCode err = ErrNone;
    logd("item '%s'", STR2CHA(item));
    if (!data || !exportData || item.isEmpty()) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }
    /* item may not found in DbModel, need extra checking by controller */
    if (err == ErrNone) {
        err = data->getExportDataString(item, fileexporter, datatype, exportData);
    }
    logd("Expoted data '%s", exportData?STR2CHA((*exportData)):"(null)");
    traceret(err);
    return err;
}

ErrCode ModelController::exportToFile(DbModel *model,
                                      ExportType type, QString *fpath)
{
    tracein;
    ErrCode ret = ErrNone;
    logi("Export model '%s' to file, type %d",
         model?STR2CHA(model->toString()):"(unknown)", type);
    if (!model) {
        ret = ErrInvalidArg;
        loge("export to file failed, invalid arg");
    }
    if (ret == ErrNone) {
        ret = getExportFileName(type,
                                model->nameId().isEmpty()?model->uid():model->nameId(),
                                fpath);
        logd("fpath '%s'", fpath?STR2CHA(*fpath):"(unknown)");
    }
    if (ret == ErrNone) {
        ret = ExportFactory::exportTo(model->getExporter(),
                                      model->modelName(),
                                      *fpath, type);
    }
    // TODO: DbModel also has this function, redundant?????
    traceret(ret);
    return ret;
}

ErrCode ModelController::exportToFile(const QList<DbModel *>* listModel,
                                      const QString& datatype,
                                      ExportType type, QString *fpath)
{
    tracein;
    ErrCode ret = ErrNone;
    if (!listModel) {
        ret = ErrInvalidArg;
        loge("export to file failed, invalid arg");
    }
    if (ret == ErrNone) {
        ret = getExportFileName(type, getName(), fpath);
    }
    if (ret == ErrNone) {
        logd("export file path %s datatype '%s'",
             fpath?STR2CHA((*fpath)):"(unknown)", STR2CHA(datatype));
        ret = ExportFactory::exportTo(this, datatype, *listModel,
                                      *fpath, type);
    }
    // TODO: DbModel also has this function, redundant?????
    traceret(ret);
    return ret;
}


ErrCode ModelController::onImportParseDataItem(const QString& importName, int importFileType,
                                const QStringList &items, quint32 idx, QList<DbModel *>* outList)
{
    tracein;
    ErrCode ret = ErrNone;
    DbModel* model = doImportOneItem(importName, importFileType, items, idx);
    if (model != nullptr) {
        logd("found one item %s", MODELSTR2CHA(model));
        if (outList != nullptr) {
            outList->append(model);
        } else {
            delete model;
        }
    } else {
        // do nothing
        // TODO: handle error case
        // be ware the case that fist item is header!!
    }
    // TODO: handle error case
    traceret(ret);
    return ret;


}

ErrCode ModelController::onImportParseDataItem(const QString& importName, int importFileType,
                                const QHash<QString, QString> &items, quint32 idx, QList<DbModel *>* outList)
{
    tracein;
    ErrCode ret = ErrNone;
    DbModel* model = nullptr;
    model = doImportOneItem(importName, importFileType, items, idx);
    if (model != nullptr) {
        logd("found one item %s", MODELSTR2CHA(model));
        if (outList != nullptr) {
            outList->append(model);
        } else {
            delete model;
        }
    } else {
        // do nothing
        // TODO: handle error case
        // be ware the case that fist item is header!!
    }
    // TODO: handle error case
    traceret(ret);
    return ret;

}


DbModel* ModelController::doImportOneItem(const QString& importName, int importFileType, const QStringList &items, quint32 idx)
{
    tracein;
    loge("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
//    FAIL("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return nullptr;
}

DbModel *ModelController::doImportOneItem(const QString& importName,
                                          int importFileType,
                                          const QHash<QString, QString> &items,
                                          quint32 idx)
{

    ErrCode err = ErrNone;
    DbModel* model = nullptr;
    logd("idx = %d", idx);
    logd("importName = %s", STR2CHA(importName));
    DbModelBuilder builder = DbModelFactory::getBuilder(importName);
    if (builder) {
        model = builder();
        if (!model) {
            err = ErrNoMemory;
            loge("cannot allocate model, no mem?");
        }
    } else {
        err = ErrNotSupport;
        loge("import '%s' not support", STR2CHA(importName));
    }
    if (err == ErrNone) {
        foreach (QString field, items.keys()) {
            QString value = items.value(field);
            logd("Import field %s", field.toStdString().c_str());
            logd("Import value %s", value.toStdString().c_str());
            err = model->onImportParseDataItem(importName,
                                               importFileType,
                                               field, value, idx);
            if (err != ErrNone) {
                loge("on import item failed, %d", err);
                break;
            }
        }
        if (err == ErrNone && model->nameId().isEmpty() && !model->name().isEmpty()) {
            bool ok = false;
            QString nameid = model->buildNameId(nullptr, &ok);
            logd("auto buid nameid '%s'", STR2CHA(nameid));
            // TODO: check if nameid exist
            if (ok) {
                model->setNameId(nameid);
            } else {
                err = ErrNoId;
                loge("Failed to build nameid");
            }
            // TODO: numer is increased, but not save --> may cause much dummy code?
        }
    }

    if (err != ErrNone) {
        FREE_PTR(model);
    }
    traceout;
    return model;
}

DbModelBuilder ModelController::getBuilder(const QString &modelName)
{
    return DbModelFactory::getBuilder(modelName);
}

DbModel* ModelController::allocMainModel()
{
    tracein;
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
    traceout;
    return model;
}

// default format: name id, name, remark
DbModel *ModelController::buildModel(void *items, const QString &fmt)
{
    tracein;
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
    traceout;
    return item;
}
