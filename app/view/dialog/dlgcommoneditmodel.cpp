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
#include "dialogutils.h"
#include "modeldefs.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include "dbctl.h"

DlgCommonEditModel::DlgCommonEditModel(QWidget *parent): QDialog(parent),
    mModel(nullptr), mIsNew(false), mIsSelfSave(false),
    mListener(nullptr),
    mCustomNameId(false),
    mAcceptResult(ErrNone)
{
    traced;
    this->setWindowTitle(STR_EDIT);

}

DlgCommonEditModel::~DlgCommonEditModel()
{
    tracein;
    FREE_PTR(mModel);
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

DbModel *DlgCommonEditModel::model(bool autoCreate)
{
    tracein;
    if (autoCreate) {
        if (!mModel) {
            logd("call newModel");
            mModel = newModel();
        }
        if (!mModel && mListener) {
            logd("call onNewModel from listener");
            mModel = mListener->onCreateDbModelObj(mModelName);
        }
    }
    logd("mModel=%s", MODELSTR2CHA(mModel));
    traceout;
    // TODO: copy or return value??? how will take over this object??? should use smart pointer????
    return mModel;
}

const DbModel *DlgCommonEditModel::getModel() const
{
    return mModel;
}

void DlgCommonEditModel::accept()
{
    tracein;
    QString errMsg;
    ErrCode ret = ErrNone;
    bool ok2Save = false;
    bool callListener = false;

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
                errMsg = STR_DATA_ERROR_CHECK_AGAIN; // TODO: should use "tr" or make it translatable???
            }
            loge("invalid data, err msg %s", STR2CHA(errMsg));
            ret = ErrInvalidData;
        }
    }
    if (ret == ErrNone && !onConfirmSave(mIsNew, item, errMsg)) {
        ret = ErrCancelled;
        loge("user cancel");
    }

    if (ret == ErrNone) {
        if (mIsSelfSave) {
            if (mIsNew) {
                logd("Save it");
                ret = item->save();
            } else {
                logd("Update it");
                ret = item->update();
            }
            logi("Save/Update result %d", ret);

            callListener = true;
        } else {
            logw("not mIsSelfSave, just call accept");
        }
    }
    logd("ret=%d", ret);
    mAcceptResult = ret;

    if (mIsSelfSave) {
        if (ret == ErrNone) {
            logi("Save/update '%s' ok , close dialog", STR2CHA(item->toString()));
            MAINWIN->showMessageBox(QString(tr("Đã lưu %1")).arg(item->name()));
        } else {
            MAINWIN->showErrorBox(QString("Lỗi, không thể lưu thông tin. %1").arg(errMsg), ret);
        }
    } else {
        MAINWIN->showIfErrorBox(errMsg, ret);
    }
    if (ret == ErrNone) {
        QDialog::accept();
    } else {
        loge("failed to save, error %d", ret);
    }
    if (callListener) {
        if (mListener) {
            logd("Call listener");
            mListener->onDbModelReady(ret, item, this);
        } else {
            logd("no listener to call");
        }
    }
    traceout;
}
bool DlgCommonEditModel::onValidateData(QString &msg)
{
    tracein;
    bool isValid = true;
    if (mModel) {
        if (mModel->nameId().isEmpty()) {
            msg += STR_LACK_NAMEID;
            isValid = false;
            logw("lack name id");
        }
        if (mModel->name().isEmpty()) {
            msg += STR_LACK_NAME;
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

bool DlgCommonEditModel::onConfirmSave(bool isNew, DbModel *model, QString& errMsg)
{
    bool accept = true;
    bool checkNameId = false;
    ErrCode err = ErrNone;
    UNUSED(isNew);
    UNUSED(model);
    UNUSED(errMsg);
    if (!model) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        if (isNew) { // if it's new, ok, check name id duplication
            checkNameId = true;
        } else if (model->isNameIdUpdated() && !model->nameId().isEmpty()) {
            // if not new, and name id is changed, so let's get current name id
            // if it's different, and check new name id if it's exists
            if (model->getDbModelHandler()) {
                dbgd("Get model '%s' from db to check", MODELSTR2CHA(model));
                DbModel* item = model->getDbModelHandler()->getByUid(model->uid());
                if (item && (item->nameId() != model->nameId())) {
                    checkNameId = true;
                } else {
                    logi("item in db '%s' is not found or name id is not change in model '%s'",
                         MODELSTR2CHA(item), MODELSTR2CHA(model));
                }
                FREE_PTR(item);
            }
        } else {
            dbgd("nameid is not update and/or empty, skip");
        }
    }
    if (err == ErrNone && checkNameId) {
        QString availNameId;
        if (!model->nameId().isEmpty()) {

            err = DB->getAvailableNameId(model->modelName(),
                                         model->nameId(), availNameId);
            if (err == ErrNone && model->nameId() != availNameId) {
                dbgd("change name id from '%s' to '%s'",
                     STR2CHA(model->nameId()), STR2CHA(availNameId));
                QString question =
                    QString("Tên định danh '%1' đã tồn tại, đổi sang '%2' ?")
                                   .arg(model->nameId(),
                                        availNameId);
                dbgd("confirm dialog '%s'", STR2CHA(question));
                accept = DialogUtils::showConfirmDialog(
                    this, STR_SAVE, question, nullptr);

                if (accept) {
                    logi("Accept to change nameid");
                    model->setNameId(availNameId);
                } else {
                    logw("Not accept to change name id");
                }
            }
        } else {
            logw("no name is set, skip checking '%s'", MODELSTR2CHA(model));
        }
    }
    return accept && (err == ErrNone);
}

ErrCode DlgCommonEditModel::loadList(QComboBox *cb, ModelController *ctrl, bool unknownItem)
{
    tracein;
    ErrCode err = ErrNone;
    cb->clear();
    QList<DbModel*> list = ctrl->getAllItems(true);
    logd("add unkown item %d", unknownItem);
    if (unknownItem) {
        cb->addItem(STR_UNKNOWN, KUidNone);
    }
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
    QString nameId = DialogUtils::showInputDialog(this, tr("Mã định danh"),
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

ErrCode DlgCommonEditModel::acceptResult() const
{
    return mAcceptResult;
}

void DlgCommonEditModel::setTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void DlgCommonEditModel::setModelName(const QString &newModelName)
{
    mModelName = newModelName;
}

void DlgCommonEditModel::setModel(DbModel *newModel)
{
    tracein;
    FREE_PTR(mModel);
    if (newModel) {
        logd("clone new model");
        mModel = CLONE_DBMODEL(newModel);
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
    dbgv("from model '%s'", MODELSTR2CHA(inModel));
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
