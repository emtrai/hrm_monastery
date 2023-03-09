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
 * Filename: dlgcommonmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/29/2022
 * Brief:
 */
#include "dlgcommoneditmodel.h"
#include "logger.h"
#include "dbmodel.h"
#include <QDialog>
#include <QMessageBox>
#include "utils.h"
#include <QString>
#include "modelcontroller.h"

DlgCommonEditModel::DlgCommonEditModel(QWidget *parent): QDialog(parent),
    mModel(nullptr), mIsNew(false), mIsSelfSave(false)
{
    traced;
}

DlgCommonEditModel::~DlgCommonEditModel()
{
    traced;
}

bool DlgCommonEditModel::isNew() const
{
    return mIsNew;
}

void DlgCommonEditModel::setIsNew(bool newIsNew)
{
    mIsNew = newIsNew;
}

bool DlgCommonEditModel::isSelfSave() const
{
    return mIsSelfSave;
}

void DlgCommonEditModel::setIsSelfSave(bool newIsSelfSave)
{
    mIsSelfSave = newIsSelfSave;
}

DbModel *DlgCommonEditModel::model()
{
    if (!mModel) {
        mModel = newModel();
    }
    if (!mModel && mListener) {
        mModel = mListener->onNewModel();
    }
    // TODO: copy or return value??? how will take over this object??? should use smart pointer????
    return mModel;
}

void DlgCommonEditModel::accept()
{
    traced;
    QString errMsg;
    ErrCode ret = ErrNone;
    bool ok2Save = onValidateData(errMsg);
    logd("ok2save %d", ok2Save);
    if (!ok2Save) {
        if (errMsg.isEmpty()) {
            errMsg = QString("Có dữ liệu lỗi, vui lòng kiểm ra lại"); // TODO: should use "tr" or make it translatable???
        }
        logd("invalid data, err msg %s", STR2CHA(errMsg));
        Utils::showErrorBox(errMsg);
        ret = ErrInvalidData;
    } else {
        logd("build model to save/return");
        DbModel* item = model();
        if (item) {
            ret = buildModel(item, errMsg);
            if (ret == ErrNone) {
                if (mIsSelfSave) {
                    if (mIsNew) {
                        logd("Save it");
                        ret = item->save();
                    } else {
                        logd("Update it");
                        ret = item->update();
                    }
                    logi("Save/Update person result %d", ret);

                    if (ret == ErrNone) {
                        logd("Save/update ok, close dialog");
                    } else {
                        Utils::showErrorBox("Lỗi, không thể lưu thông tin");
                    }
                } else {
                    logd("not mIsSelfSave, just call accept");
                }
                if (mListener) {
                    logd("Call listener");
                    mListener->onDbModelReady(ErrNone, item, this);
                } else {
                    logd("no listener to call");
                }
            } else {
                Utils::showErrorBox("Lỗi, không thể tạo dữ liệu");
                ret = ErrBuildDataFailed;
            }
        } else {
            loge("accept failed, no memory??");
            ret = ErrNoMemory;
        }
    }
    if (ret == ErrNone) {
        QDialog::accept();
    }
    tracedr(ret);
}

ErrCode DlgCommonEditModel::loadList(QComboBox *cb, ModelController *ctrl)
{
    traced;
    ErrCode err = ErrNone;
    cb->clear();
    QList<DbModel*> list = ctrl->getAllItems(true);
    foreach(DbModel* item, list){
        cb->addItem(item->name(), item->uid());
    }
    tracedr(err);
    return err;
}

void DlgCommonEditModel::setModel(DbModel *newModel)
{
    // TODO: clone or assign????
    // TODO: change to use smart pointer??????
    mModel = newModel;
}

CommonEditModelListener *DlgCommonEditModel::listener() const
{
    return mListener;
}

void DlgCommonEditModel::setListener(CommonEditModelListener *newListener)
{
    mListener = newListener;
}

ErrCode DlgCommonEditModel::fromModel(const DbModel *inModel)
{
    traced;
    ErrCode ret = ErrNone;
    if (inModel == nullptr) {
        loge("Invalid model info to clone");
        return ErrInvalidData;
    }
    DbModel* item = model();
    if (item) {
        item->clone(inModel);
        item->validate(); // TODO: should call validate here???
        if (item == nullptr){
            ret = ErrInvalidArg; // TODO: should raise assert instead???
        }
    } else {
        ret = ErrNoData;
        loge("No db model found");
    }
    tracedr(ret);
    return ret;
}
