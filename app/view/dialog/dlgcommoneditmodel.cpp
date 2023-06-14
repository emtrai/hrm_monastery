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
#include <QDialogButtonBox>
#include <QPushButton>
#include "stringdefs.h"


DlgCommonEditModel::DlgCommonEditModel(QWidget *parent): QDialog(parent),
    mModel(nullptr), mIsNew(false), mIsSelfSave(false),
    mCustomNameId(false)
{
    traced;

}

DlgCommonEditModel::~DlgCommonEditModel()
{
    tracein;
    if (mModel) {
        delete mModel;
        mModel = nullptr;
    }
    traceout;
}

void DlgCommonEditModel::setupUI()
{
    tracein;
    QPushButton* btn = nullptr;
    QDialogButtonBox* btnBox = buttonBox();
    if (btnBox) {
        btn = btnBox->button(QDialogButtonBox::Cancel);
        if (btn) btn->setText(STR_CANCEL);

        btn = btnBox->button(QDialogButtonBox::Save);
        if (btn) btn->setText(STR_SAVE);

        btn = btnBox->button(QDialogButtonBox::Ok);
        if (btn) btn->setText(STR_OK);

        btn = btnBox->button(QDialogButtonBox::Ignore);
        if (btn) btn->setText(STR_IGNORE);

        btn = btnBox->button(QDialogButtonBox::Help);
        if (btn) btn->setText(STR_HELP);
    }
    traceout;
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
    tracein;
    if (!mModel) {
        logd("call newModel");
        mModel = newModel();
    }
    if (!mModel && mListener) {
        logd("call onNewModel from listener");
        mModel = mListener->onNewModel(mModelName);
    }
    logd("mModel=%s", mModel?STR2CHA(mModel->toString()):"(null)");
    traceout;
    // TODO: copy or return value??? how will take over this object??? should use smart pointer????
    return mModel;
}

const DbModel *DlgCommonEditModel::getModel() const
{
    return mModel;
}

//void DlgCommonEditModel::accept()
//{
//    tracein;
//    QString errMsg;
//    ErrCode ret = ErrNone;
//    bool ok2Save = false;
//    logd("build model to save/return");
//    DbModel* item = model();
//    if (item) {
//        ret = buildModel(item, errMsg);
//    } else {
//        loge("accept failed, no memory??");
//        ret = ErrNoMemory;
//    }

//    if (ret == ErrNone) {
//        ok2Save = onValidateData(errMsg);
//        logd("ok2save %d", ok2Save);
//        if (!ok2Save) {
//            if (errMsg.isEmpty()) {
//                errMsg = QString("Có dữ liệu lỗi, vui lòng kiểm ra lại"); // TODO: should use "tr" or make it translatable???
//            }
//            loge("invalid data, err msg %s", STR2CHA(errMsg));
//            Utils::showErrorBox(errMsg);
//            ret = ErrInvalidData;
//        }
//    }

//    if (ret == ErrNone) {
//        if (mIsSelfSave) {
//            if (mIsNew) {
//                logd("Save it");
//                ret = item->save();
//            } else {
//                logd("Update it");
//                ret = item->update();
//            }
//            logi("Save/Update result %d", ret);

//            if (ret == ErrNone) {
//                logi("Save/update '%s' ok , close dialog", STR2CHA(item->toString()));
//                Utils::showMsgBox(QString(tr("Đã lưu %1")).arg(item->name()));
//            } else {
//                Utils::showErrorBox("Lỗi, không thể lưu thông tin");
//            }
//        } else {
//            logd("not mIsSelfSave, just call accept");
//        }
//        if (mListener) {
//            logd("Call listener");
//            mListener->onDbModelReady(ret, item, this);
//        } else {
//            logd("no listener to call");
//        }
//    } else {
//        Utils::showErrorBox("Lỗi, không thể tạo dữ liệu");
//        ret = ErrBuildDataFailed;
//    }
//    if (ret == ErrNone) {
//        QDialog::accept();
//    }
//    traceret(ret);
//}


void DlgCommonEditModel::accept()
{
    tracein;
    QString errMsg;
    ErrCode ret = ErrNone;
    bool ok2Save = false;

    logd("build model to save/return");
    DbModel* item = model();
    if (item) {
        ret = buildModel(item, errMsg);
    } else {
        loge("accept failed, no memory??");
        ret = ErrNoMemory;
    }

    if (ret == ErrNone) {
        ok2Save = onValidateData(errMsg);
        logd("ok2save %d", ok2Save);
        if (!ok2Save) {
            if (errMsg.isEmpty()) {
                errMsg = QString("Có dữ liệu lỗi, vui lòng kiểm ra lại"); // TODO: should use "tr" or make it translatable???
            }
            loge("invalid data, err msg %s", STR2CHA(errMsg));
            Utils::showErrorBox(errMsg);
            ret = ErrInvalidData;
        }
    }

    if (mIsSelfSave) {
        if (ret == ErrNone) {
            if (mIsNew) {
                logd("Save it");
                ret = item->save();
            } else {
                logd("Update it");
                ret = item->update();
            }
            logi("Save/Update result %d", ret);

            if (ret == ErrNone) {
                logi("Save/update '%s' ok , close dialog", STR2CHA(item->toString()));
                Utils::showMsgBox(QString(tr("Đã lưu %1")).arg(item->name()));
            } else {
                Utils::showErrorBox("Lỗi, không thể lưu thông tin");
            }
            if (mListener) {
                logd("Call listener");
                mListener->onDbModelReady(ret, item, this);
            } else {
                logd("no listener to call");
            }
        } else {
            Utils::showErrorBox("Lỗi, không thể tạo dữ liệu");
            ret = ErrBuildDataFailed;
        }
    } else {
        logd("not mIsSelfSave, just call accept");
    }
    logd("ret=%d", ret);
    if (ret == ErrNone) {
        QDialog::accept();
    }
    traceout;
}
bool DlgCommonEditModel::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        if (mModel->nameId().isEmpty()) {
            msg += tr("Thiếu mã định danh.");
            isValid = false;
            logw("lack name id");
        }
        if (mModel->name().isEmpty()) {
            msg += tr("Thiếu tên.");
            isValid = false;
            logw("lack name");
        }
    } else {
        logw("no model to check");
        isValid = false;
    }
    logd("is valid %d", isValid);
    // TODO: implement this????
    // TODO do we need this? or just implement on buildModel are enough??
    traceout;
    return isValid;
}

ErrCode DlgCommonEditModel::loadList(QComboBox *cb, ModelController *ctrl)
{
    tracein;
    ErrCode err = ErrNone;
    cb->clear();
    QList<DbModel*> list = ctrl->getAllItems(true);
    cb->addItem(STR_UNKNOWN, QString());
    foreach(DbModel* item, list){
        cb->addItem(item->name(), item->uid());
    }
    RELEASE_LIST_DBMODEL(list);
    traceret(err);
    return err;
}

void DlgCommonEditModel::onChangeNameIdTxt(QLineEdit *txtNameId, const QString &arg1, bool direct)
{
    if (!mCustomNameId) {
        bool ok = false;
        QString nameid;
        if (direct) {
            ok = true;
            nameid = arg1;
        } else {
            nameid = Utils::UidFromName(arg1, NO_VN_MARK_UPPER, &ok);
        }
        if (ok) {
            txtNameId->setText(nameid);
        } else {
            txtNameId->setText("");
        }
    }
}

void DlgCommonEditModel::onEditnameId(QLineEdit *txtNameId)
{
    tracein;
    QString txt = txtNameId->text().trimmed();
    bool ok = false;
    QString nameId = Utils::showInputDialog(this, tr("Mã định danh"),
                                            tr("Nhập mã định danh. \
\nSau khi nhập, mã định danh sẽ cố định, không tự động tao theo dữ liệu nhập.\
\nĐể mã định danh tự động tạo theo giá trị nhập, xóa toàn bộ và chọn Đồng ý"),
                                            txt, &ok);
    if (ok && !nameId.isEmpty()) {
        mCustomNameId = true;
        txtNameId->setText(nameId);
        logd("custom name id '%s'", STR2CHA(nameId));
    } else {
        logd("no name id (ok=%d) or name id is empty", ok);
        mCustomNameId = false;
        txtNameId->setText("");
    }
    traceout;
}

void DlgCommonEditModel::setModelName(const QString &newModelName)
{
    mModelName = newModelName;
}

void DlgCommonEditModel::setModel(DbModel *newModel)
{
    tracein;
    if (newModel) {
        if (mModel) {
            logd("delete old model");
            delete mModel;
        }
        logd("clone new model");
        mModel = newModel->clone();
    } else {
        loge("invalid new model");
    }
    traceout;
}

QDialogButtonBox *DlgCommonEditModel::buttonBox()
{
    return nullptr;
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
    tracein;
    ErrCode ret = ErrNone;
    if (inModel == nullptr) {
        loge("Invalid model info to clone");
        return ErrInvalidData;
    }
    DbModel* item = model();
    if (item) {
        mCustomNameId = true;
        item->clone(inModel);
        item->validateAllFields(); // TODO: should call validate here???
    } else {
        ret = ErrNoData;
        loge("No db model found");
    }
    traceret(ret);
    return ret;
}
