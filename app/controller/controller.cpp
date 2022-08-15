#include "controller.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "filectl.h"
#include "dbmodel.h"

Controller::Controller()
{
    traced;
}

Controller::Controller(const QString& name):mName(name)
{
    traced;
}

DbModel *Controller::buildModel(void *items, const QString &fmt)
{
    traced;
    UNUSED(items);
    UNUSED(fmt);
    return nullptr;
}



ErrCode Controller::parsePrebuiltFile(const QString &fpath, const QString &ftype)
{
    ErrCode ret = ErrNone;
    traced;
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
//        if (ftype == KFileTypeCSV) {
//            ret = Utils::parseCSVFile(FileCtl::getPrebuiltDataFilePath(fname),
//                                      &Controller::oneCSVItemCallback,
//                                      this);
//        } else {
//            ret = ErrNotSupport;
//        }

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

ErrCode Controller::oneCSVItemCallback(const QStringList &items, void* caller, void *param)
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
}
