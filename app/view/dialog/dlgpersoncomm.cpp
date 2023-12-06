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
 * Filename: DlgPersonCommunity.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/15/2022
 * Brief:
 */
#include "dlgpersoncomm.h"
#include "ui_dlgpersoncomm.h"
#include "dbmodel.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "communityctl.h"
#include "community.h"
#include "communityperson.h"
#include "person.h"
#include "dlgsearchperson.h"
#include "exception.h"
#include "datetimeutils.h"
#include "dialogutils.h"


DlgPersonCommunity::DlgPersonCommunity(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgPersonCommunity),
    mCommunity(nullptr),
    mSkipStartDate(false)
{
    tracein;
    ui->setupUi(this);
    loadCommunity();
    loadModelStatus();
//    ui->txtStartDate->setText(DatetimeUtils::currentTimeToDatestring());
//    ui->txtEndDate->setText(DatetimeUtils::currentTimeToDatestring());
    traceout;
}

DlgPersonCommunity::~DlgPersonCommunity()
{
    tracein;
    delete ui;
    FREE_PTR(mCommunity);
    RELEASE_LIST(mPersonList, Person);
    RELEASE_LIST(mCommunityPersonList, CommunityPerson);
    traceout;
}

void DlgPersonCommunity::setupUI()
{
    tracein;
    DlgCommonEditModel::setupUI();
    setModelStatus(MODEL_STATUS_ACTIVE);
    traceout;
}

ErrCode DlgPersonCommunity::buildModel(DbModel *model, QString &errMsg)
{

    tracein;
    ErrCode err = ErrNotSupport;
    THROWEX("not support build model here");
    traceret(err);
    return err;
}

ErrCode DlgPersonCommunity::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;;
    CommunityPerson* comm = (CommunityPerson*) item;
    if (!comm) {
        err = ErrInvalidArg;
        loge("invalid item");
    }
    if (err == ErrNone) {
        if (comm->startDate() > 0) {
            ui->txtStartDate->setText(DatetimeUtils::date2String(comm->startDate()));
        }
        if (comm->endDate() > 0) {
            ui->txtEndDate->setText(DatetimeUtils::date2String(comm->endDate()));
        }
        setModelStatus(comm->modelStatus());
        appendCommunityPerson(comm);
    }
    traceret(err);
    return err;
}

void DlgPersonCommunity::loadCommunity()
{
    tracein;
    loadList(ui->cbCommunity, COMMUNITYCTL, true);
    traceout;
}

void DlgPersonCommunity::loadModelStatus(bool skipActive)
{
    tracein;
    ui->cbModelStatus->clear();
    const QHash<int, QString>* statuses = DbModel::getModelStatusIdNameMap();
    logd("the number of status %lld", statuses->count());
    foreach (int key, statuses->keys()) {
        if (skipActive && key == MODEL_STATUS_ACTIVE) {
            // changing status will cause some complicated processing, so not allow to set here
            // TODO: support to change model status
            continue;
        }
        ui->cbModelStatus->addItem(statuses->value(key), key);
    }
    traceout;
}

ErrCode DlgPersonCommunity::setModelStatus(int modelStatus, bool allowChange)
{
    tracein;
    ErrCode err = ErrNone;
    logd("modelStatus 0x%x, allowChange %d", modelStatus, allowChange);
    int index = -1;
    err = Utils::setSelectItemComboxByData(ui->cbModelStatus, modelStatus, &index);
    logd("index %d", index);
    if (err == ErrNone && index >= 0) {
        updateModelStatus(modelStatus);
    } else {
        loge("set model status failed, err = %d", err);
    }
    ui->cbModelStatus->setEnabled(allowChange);
    traceret(err);
    return err;
}

int DlgPersonCommunity::getModelStatus(bool *ok)
{
    tracein;
    int modelStatus = 0;
    int index = ui->cbModelStatus->currentIndex();
    logd("index=%d", index);
    if (index >= 0) {
        QVariant value = ui->cbModelStatus->itemData(index);
        if (value.isValid()) {
            modelStatus = value.toInt(ok);
        } else {
            loge("value is invalid at index=%d", index);
        }
    } else {
        loge("invalid index=%d", index);
    }
    logd("modelStatus=0x%x", modelStatus);
    traceout;
    return modelStatus;
}

ErrCode DlgPersonCommunity::appendPerson(const Person *person)
{
    tracein;
    ErrCode err = ErrNone;
    Person* clone = nullptr;
    if (!person) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && person->modelName() != KModelNamePerson) {
        err = ErrInvalidData;
        loge("item '%s' is not person model", MODELSTR2CHA(person));
    }
    if (err == ErrNone) {
        clone = CLONE_MODEL(person, Person);
        if (!clone) {
            err = ErrNoMemory;
            loge("No memory, cloned failed");
        }
    }
    if (err == ErrNone) {
        logd("add '%s' to person list", MODELSTR2CHA(clone));
        mPersonList.append(clone);
        ui->txtPeople->appendPlainText(clone->displayName());
        ui->btnSearchPeople->setEnabled(false);
    }
    traceret(err);
    return err;
}

void DlgPersonCommunity::clearPersonList()
{
    tracein;
    RELEASE_LIST(mPersonList, Person);
    ui->txtPeople->clear();
    traceout;
}

void DlgPersonCommunity::accept()
{
    tracein;
    QString errMsg;
    ErrCode err = ErrNone;
    bool ok = false;
    int modelStatus = getModelStatus(&ok);
    QString remark = ui->txtRemark->toPlainText();
    int startDate = DatetimeUtils::dateFromString(ui->txtStartDate->text());
    int endDate = DatetimeUtils::dateFromString(ui->txtEndDate->text());
    bool callListener = false;
    logd("model status = 0x%x", modelStatus);
    logd("community '%s'", MODELSTR2CHA(mCommunity));
    logd("startDate = 0x%x", startDate);
    logd("endDate = 0x%x", endDate);
    if (err == ErrNone && (modelStatus == 0 || !ok)) {
        loge("invalid model status");
        err = ErrInvalidData;
        errMsg = tr("Trạng thái không đúng");
    }


    if (mIsNew) {
        logd("build model to save/return");

        if (!mCommunity) {
            logd("get selected community");
            bool isOk = false;
            QString communityUid = Utils::getCurrentComboxDataString(ui->cbCommunity, &isOk);
            logd("search community uid '%s'", STR2CHA(communityUid));
            if (!communityUid.isEmpty() && ok) {
                DbModel* model = COMMUNITYCTL->getModelByUid(communityUid);
                if (model) {
                    mCommunity = (Community*) model;
                } else {
                    loge("not found community with uid '%s'", STR2CHA(communityUid));
                    err = ErrNotFound;
                }
            } else {
                loge("community uid '%s' is empty or get cbox value failed", STR2CHA(communityUid));
                err = ErrInvalidData;
            }
        }

        if (err == ErrNone && !mCommunity) {
            loge("no community");
            err = ErrNoData;
            errMsg = tr("Không có thông tin cộng đoàn");
        }
        if (err == ErrNone) {
            RELEASE_LIST(mCommunityPersonList, CommunityPerson);
            foreach (Person* per, mPersonList) {
                if (!per) {
                    logw("something strange, null value in list");
                    continue;
                }
                CommunityPerson* commPer = (CommunityPerson*) CommunityPerson::build();
                if (!commPer) {
                    err = ErrNoMemory;
                    loge("failed to allocate memory");
                    errMsg = tr("Hết bộ nhớ");
                    break;
                }
                logd("per '%s'", MODELSTR2CHA(per));
                commPer->setCommunity(mCommunity);
                commPer->setPerson(per);
                if (!mSkipStartDate) commPer->setStartDate(startDate);
                commPer->setEndDate(endDate);
                commPer->setModelStatus(modelStatus);
                commPer->setRemark(remark);
                mCommunityPersonList.append(commPer);
            }
        }
    } else {
        foreach (CommunityPerson* commPer, mCommunityPersonList) {
            if (!commPer) {
                logw("something strange, null value in list");
                continue;
            }
            logd("commper '%s'", MODELSTR2CHA(commPer));
            if (!mSkipStartDate) commPer->setStartDate(startDate);
            commPer->setEndDate(endDate);
            commPer->setModelStatus(modelStatus);
            commPer->setRemark(remark);
        }
    }

    if (err == ErrNone && mIsSelfSave) {
        if (mCommunityPersonList.size() > 0) {
            foreach (CommunityPerson* commper, mCommunityPersonList) {
                if (mIsNew) {
                    logd("Save it");
                    err = commper->save();
                } else {
                    commper->setMarkModified(true);
                    if (!mSkipStartDate) commper->setStartDate(startDate);
                    commper->setEndDate(endDate);
                    commper->setModelStatus(modelStatus);
                    commper->setRemark(remark);
                    logd("Update it");
                    err = commper->update();
                }
                logi("Save/Update result %d", err);
                if (err != ErrNone) {
                    errMsg = QString(tr("Lỗi không thể lưu/cập nhật cho Nữ tu '%1'"))
                                 .arg(MODELSTR2CHA(commper->person()));
                    break;
                }

            }
            if (err == ErrNone) {
                logi("Save/update ok , close dialog");
                DialogUtils::showMsgBox(tr("Lưu thành công"));
            } else {
                DialogUtils::showErrorBox(err,
                                          QString(tr("Lỗi, không thể lưu thông tin. %1"))
                                        .arg(errMsg));
            }
            callListener = true;
        }
    } else {
        logd("not mIsSelfSave, just call accept");
    }
    logd("ret=%d", err);

    mAcceptResult = err;
    if (err == ErrNone) {
        QDialog::accept();
    }
    if (callListener) {
        if (mListener) {
            logd("Call listener");
            mListener->onDbModelReady(err, nullptr, this);// TODO: nullptr is ok?
        } else {
            logd("no listener to call");
        }
    }
    traceout;
}

void DlgPersonCommunity::updateModelStatus(int status)
{
    tracein;
    logd("status 0x%x", status);
    if (status & MODEL_STATUS_ACTIVE) {
        ui->lblStartDateNote->setText(tr("Ngày bắt đầu ở cộng đoàn mới"));
        ui->lblEndDateNote->setText(tr("Ngày kết thúc ở cộng đoàn hiện tại"));
        ui->lblStartDateNote->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
        ui->lblEndDateNote->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
    } else {
        ui->lblStartDateNote->setText(tr("Ngày bắt đầu ở cộng đoàn"));
        ui->lblEndDateNote->setText(tr("Ngày kết thúc ở cộng đoàn"));
        ui->lblStartDateNote->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
        ui->lblEndDateNote->setStyleSheet(QStringLiteral("QLabel{color: rgb(0, 0, 0);}"));
    }
//    ui->cbModelStatus->setDisabled(true);
    traceout;
}

void DlgPersonCommunity::setDate(QLineEdit *txt, qint64 timeInMs)
{
    tracein;
    logd("set date timeInMs %lld", timeInMs);
    if (!timeInMs) {
        timeInMs = DatetimeUtils::currentTimeMs();
    }
    QString timeString = DatetimeUtils::timeMsToDatestring(timeInMs, QT_DATE_FORMAT_DMY);
    logd("timeString '%s'", STR2CHA(timeString));
    if (!timeString.isEmpty()) {
        txt->setText(timeString);
    }
    traceout;
}

QDialogButtonBox *DlgPersonCommunity::buttonBox()
{
    return ui->buttonBox;
}

DbModel *DlgPersonCommunity::newModel()
{
    return CommunityPerson::build();
}

void DlgPersonCommunity::on_btnSearchPeople_clicked()
{
    tracein;
    DlgSearchPerson * dlg = DlgSearchPerson::build(this, true);
    if (dlg == nullptr) {
        loge("Open dlg DlgPersonCommunity fail, No memory");
        return; // TODO: open dlg??
    }
    dlg->setIsMultiSelection(true);

    if (dlg->exec() == QDialog::Accepted){
        QList<DbModel *> list = dlg->selectedItems();
        if (list.size() > 0) {
            setPersonList<DbModel>(list);
        } else {
            logi("No person selected");
        }
    }
    delete dlg;
    traceout;
}


void DlgPersonCommunity::on_cbModelStatus_currentIndexChanged(int index)
{
    tracein;
    logd("index=%d", index);
    if (index >= 0) {
        int status;
        bool ok = false;
        QVariant value = ui->cbModelStatus->itemData(index);
        if (!value.isNull()) {
            status = value.toInt(&ok);
        } else {
            loge("not item data");
        }
        logd("status=0x%x", status);
        if (ok) {
            updateModelStatus(status);
        } else {
            loge("unknown status");
        }
    }
    traceout;
}

void DlgPersonCommunity::setSkipStartDate(bool newSkipStartDate)
{
    tracein;
    logd("newSkipStartDate %d", newSkipStartDate);
    mSkipStartDate = newSkipStartDate;
    ui->txtStartDate->setEnabled(!newSkipStartDate);
    traceout;
}

void DlgPersonCommunity::setEndDate(qint64 timeInMs)
{
    tracein;
    logd("set end date timeInMs %lld", timeInMs);
    setDate(ui->txtEndDate, timeInMs);
    traceout;
}

void DlgPersonCommunity::setStartDate(qint64 timeInMs)
{
    tracein;
    logd("set start date timeInMs %lld", timeInMs);
    setDate(ui->txtStartDate, timeInMs);
    traceout;
}

void DlgPersonCommunity::setCommunity(const Community *newCommunity)
{
    tracein;
    if (newCommunity) {
        int index = -1;
        FREE_PTR(mCommunity);
        mCommunity = CLONE_MODEL(newCommunity, Community);
        logd("newCommunity '%s'", MODELSTR2CHA(newCommunity));
        Utils::setSelectItemComboxByData(ui->cbCommunity, mCommunity->uid(), &index);
        logd("index = %d", index);
        ui->cbCommunity->setEnabled(false);
    } else {
        loge("Invalid input community");
    }
    traceout;
}

const QList<CommunityPerson *> &DlgPersonCommunity::communityPersonList()
{
    return mCommunityPersonList;
}

ErrCode DlgPersonCommunity::appendCommunityPerson(const CommunityPerson *commPer,
                                                  bool ignoreCommunity)
{
    tracein;
    ErrCode err = ErrNone;
    CommunityPerson* newCommPer = nullptr;
    logd("commper '%s'", MODELSTR2CHA(commPer));
    if (mIsNew) {
        loge("not allow to set if dialog is created for new item");
        err = ErrNotAllow;
    }
    if (err == ErrNone && !commPer) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        newCommPer = CLONE_MODEL(commPer, CommunityPerson);
        if (newCommPer) {
            mCommunityPersonList.append(newCommPer);
        } else {
            loge("no memory");
            err = ErrNoMemory;
        }
    }

    if (err == ErrNone) {
        if (!ignoreCommunity) {
            logd("set community");
            setCommunity(newCommPer->community());
        }
        logd("append person");
        appendPerson(commPer->person());
    } else {
        FREE_PTR(newCommPer);
    }

    // not allow to change persone nor community, just edit other data
    ui->btnSearchPeople->setEnabled(false);
    ui->cbCommunity->setEnabled(false);
    traceret(err);
    return err;
}

ErrCode DlgPersonCommunity::setCommunityPersonList(const QList<CommunityPerson *> &commPer, bool ignoreCommunity)
{
    tracein;
    ErrCode err = ErrNone;
    logd("commPer size %lld, ignoreCommunity %d", commPer.size(), ignoreCommunity);
    RELEASE_LIST(mCommunityPersonList, CommunityPerson);
    if (commPer.size() > 0) {
        foreach (CommunityPerson* item, commPer) {
            if (item) {
                err = appendCommunityPerson(item, ignoreCommunity);
            } else {
                loge("Invalid data");
                err = ErrInvalidData;
            }
            if (err != ErrNone) {
                break;
            }
        }
    } else {
        loge("No data");
        err = ErrNoData;
    }
    traceret(err);
    return err;
}


ErrCode DlgPersonCommunity::addListPeopleToCommunity(QWidget *parent,
                                                     const Community *comm,
                                                     const QList<Person *> &perList)
{
    tracein;
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = nullptr;
    if (!comm || perList.empty()) {
        err = ErrInvalidArg;
        loge("cannot add comm history, invalid arg");
    }
    if (err == ErrNone && (dlg = DlgPersonCommunity::build(parent)) == nullptr) {
        loge("Cannot create/build dialog, no memory?");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        err = dlg->setPersonList<Person>(perList);
    }
    if (err == ErrNone) {
        logd("set comm '%s'", MODELSTR2CHA(comm));
        dlg->setCommunity(comm);
    }

    if (err == ErrNone) {
        dlg->exec();
    }

    if (err != ErrNone) {
        logd("Add community history failed, err=%d", err);
        DialogUtils::showErrorBox(err, tr("Lỗi thêm dữ liệu Cộng đoàn"));
    }
    if (dlg) delete dlg;
    traceret(err);
    return err;
}

ErrCode DlgPersonCommunity::updateCommunityPerson(QWidget *parent,
                                                  const CommunityPerson *model)
{
    tracein;
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = nullptr;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }

    if (err == ErrNone) {
        dlg = DlgPersonCommunity::build(parent, true, KModelNameCommPerson, model);
        if (!dlg) {
            loge("Cannot create/build dialog, no memory?");
            err = ErrNoMemory;
        }
    }
    if (err == ErrNone) {
        if (dlg->exec() != QDialog::Accepted) {
            err = ErrCancelled;
            logd("cancel");
        }
    } else {
        DialogUtils::showErrorBox(err, tr("Lỗi chỉnh sửa dữ liệu Cộng đoàn - Nữ tu"));
    }
    FREE_PTR(dlg);
    traceret(err);
    return err;
}


