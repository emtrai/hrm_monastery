#include "controller.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "dbmodel.h"
#include "dbmodelhandler.h"
#include "iimporter.h"
#include "dbctl.h"

Controller::Controller()
{
    traced;
}

Controller::Controller(const QString& name):mName(name)
{
    traced;
}

const QString &Controller::getName()
{
    return mName;
}

DbModelHandler *Controller::getModelHandler()
{
    traced;
    logd("Get model handler for controller using name: %s", STR2CHA(mName));
    // TODO: fix me if this is not suitble
    return DB->getModelHandler(getName());
}

ErrCode Controller::addNew(DbModel *model)
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
                onLoad();
                // TODO: should emit???? if emit load, loader will catch and reload all
//                emit load();
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

QList<DbModel *> Controller::getItemFromDb()
{
    return QList<DbModel *>();
}

ErrCode Controller::reloadDb()
{
    traced; // TODO: implement observer to call all reload db when any changes in model
    return ErrNone;
}

ErrCode Controller::exportToFile(DbModel *model, ExportType type, QString *fpath)
{
    traced;
    ErrCode ret = ErrNone;
    QString fname = QString("%1.html").arg(model->uid());
    *fpath = FileCtl::getTmpDataDir(fname);
    ret = ExportFactory::exportTo(model->getExporter(),
                                  *fpath, type);
    // TODO: DbModel also has this function, redundant?????
    return ret;
}

ErrCode Controller::importFromFile(IImporter *importer, ImportType type, const QString& fpath, QList<DbModel*>*outList)
{
    traced;
    ErrCode ret = ErrNone;

    ret = ImportFactory::importFrom((importer != nullptr)?importer:this,fpath,type, outList);
    tracedr(ret);
    return ret;
}

ErrCode Controller::onImportItem(int importFileType, const QStringList &items, quint32 idx, void *tag)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* model = doImportOneItem(importFileType, items, idx);
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

ErrCode Controller::onImportItem(int importFileType, const QHash<QString, QString> &items, quint32 idx, void *tag)
{
    traced;
    ErrCode ret = ErrNone;
    DbModel* model = doImportOneItem(importFileType, items, idx);
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

DbModel* Controller::doImportOneItem(int importFileType, const QStringList &items, quint32 idx)
{
    traced;
    loge("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return nullptr;
}

DbModel *Controller::doImportOneItem(int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    traced;
    loge("DEFAULT doImportOneItem, MUST BE IMPLEMENTED IN DERIVED CLASS");
    // TODO: make it abstract????
    return nullptr;
}

int Controller::search(const QString &keyword, QList<DbModel *> *outList)
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

DbModel *Controller::getModelByName(const QString &name)
{
    traced;
    DbModel* model = nullptr;
    traced;
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

DbModel *Controller::getModelByUid(const QString &uid)
{
    traced;
    DbModel* model = nullptr;
    traced;
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

int Controller::filter(int catetoryid,
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

ErrCode Controller::loadFromDb()
{
    traced;
    return ErrNotSupport;
}


ErrCode Controller::getUidListFromName(const QString &name, QHash<QString, QString> *list, const char *hdlName)
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

QString Controller::getNameFromUidList(const QStringList &uidList, const char *hdlName)
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

QString Controller::getNameFromUidList(const QString &uidList, const char *hdlName)
{
    traced;
    return getNameFromUidList(uidList.split(NAME_SPLIT), hdlName);

}

ErrCode Controller::markModelDelete(DbModel *model)
{
    traced;
    ASSERT(false, "Derived class must implement markModelDelete");
    return ErrNotImpl;
}

ErrCode Controller::deleteModel(DbModel *model)
{
    traced;
    ASSERT(false, "Derived class must implement deleteModel");
    return ErrNotImpl;

}
DbModel *Controller::buildModel(void *items, const QString &fmt)
{
    traced;
    UNUSED(items);
    UNUSED(fmt);
    return nullptr;
}

const char *Controller::getPrebuiltFileName()
{
    traced;
    return nullptr;

}

const char *Controller::getPrebuiltFileType()
{
    traced;
    return nullptr;

}



ErrCode Controller::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
    // TODO: should we add meta field in beginning of file to know status of information???
    // i.e.: version, last update time.
    // This can be used to check/compare witl one stored in db
    if (ftype == KFileTypeCSV) {
        ret = Utils::parseCSVFile(fpath,
                                  &Controller::oneCSVItemCallback,
                                  this);
    } else {
        ret = ErrNotSupport;
    }
    return ret;
}
ErrCode Controller::check2UpdateDbFromPrebuiltFile(const QString &name, const QString &ftype)
{
    traced;
    ErrCode ret = ErrNone;

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
    return ret;
}

ErrCode Controller::doOneCSVItemCallback(const QStringList &items, void *param)
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

    }
    else {
        loge("Invalid data");
        // TODO: should break or continue???
        ret = ErrInvalidData;
    }
    logd("ret %d", ret);
    return ret;
}

ErrCode Controller::oneCSVItemCallback(const QStringList &items, void* caller, void *param, quint32 idx)
{
    traced;
    ErrCode ret = ErrFailed;
    if (caller != nullptr){
        ret = ((Controller*)caller)->doOneCSVItemCallback(items, param);
    }
    return ret;
}

void Controller::onLoad(){
    traced;
    logd("Onload %s", mName.toStdString().c_str());
    ErrCode ret = ErrNone;
    QString fname = getPrebuiltFileName();
    if (!fname.isEmpty()) {
        logi("Check & load from prebuilt file");
        ret = check2UpdateDbFromPrebuiltFile(fname, getPrebuiltFileType());
        logd("check2UpdateDbFromPrebuiltFile ret=%d", ret);
        // TODO: should do lazyload???
    } else {
        logi("Not load from prebuilt file");
    }

    ret = loadFromDb(); // TODO: handle error case????
    logd("load from db ret=%d", ret);
    tracede;

}
