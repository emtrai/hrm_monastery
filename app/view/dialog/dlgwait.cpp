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
 * Filename: dlgwait.cpp
 * Author: Anh, Ngo Huy
 * Created date:12/6/2022
 * Brief:
 */
#include "dlgwait.h"
#include "ui_dlgwait.h"

DlgWait::DlgWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWait),
    mWorker(nullptr),
    mFinishCallback(nullptr)
{
    ui->setupUi(this);
}

DlgWait::~DlgWait()
{
    delete ui;
}

ErrCode DlgWait::show(void* data, WaitPrepare_t prepare,
                      WaitRunt_t run, WaitFinished_t finish)
{
    traced;
    ErrCode err = ErrNone;
    mForceCancel = false;
    mWorker = new DlgWaitWorker(this, this, data);
    mWorker->setRunFunc(run);
    connect(mWorker, &DlgWaitWorker::done, this, &DlgWait::handleResult);
    connect(mWorker, &DlgWaitWorker::finished, mWorker, &QObject::deleteLater);
    if (prepare) {
        logd("Call prepare");
        err = prepare(data, this);
        logd("ret=%d", err);
    } else {
        logd("no prepare func to call");
    }
    mFinishCallback = finish;
    if (err == ErrNone) {
        logd("start worker thread");
        mWorker->start();
        this->exec();
    }
    tracedr(err);
    return err;
}

bool DlgWait::allowCancel() const
{
    return mAllowCancel;
}

void DlgWait::setAllowCancel(bool newAllowCancel)
{
    mAllowCancel = newAllowCancel;
    if (!mAllowCancel) {
        ui->btnBox->hide();
    }
}

const QString &DlgWait::title() const
{
    return mTitle;
}

void DlgWait::setTitle(const QString &newTitle)
{
    mTitle = newTitle;
    ui->lblTitle->setText(newTitle);
}

const QString &DlgWait::message() const
{
    return mMessage;
}

void DlgWait::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
    ui->lblInfo->setText(newMessage);
}

void DlgWait::setProgress(int perc)
{
    traced;
    logd("perc %d", perc);
    ui->lblProgress->setText(QString("%1 %").arg(perc));
}

void DlgWait::setProgress(int cur, int total)
{
    traced;
    logd("cur %d total %d", cur, total);
    ui->lblProgress->setText(QString("%1 / %2").arg(cur, total));

}

void DlgWait::clearProgress()
{
    traced;
    ui->lblProgress->setText("");
}

void DlgWait::forceClose()
{
    traced;
    mForceCancel = true;
    QDialog::close();
    tracede;
}

void DlgWait::accept()
{
    traced;
    if (mAllowCancel || mForceCancel) QDialog::accept();

    tracede;
}

void DlgWait::done(int val)
{
    traced;
    if (mAllowCancel || mForceCancel) QDialog::done(val);

    tracede;
}

void DlgWait::reject()
{
    traced;
    if (mAllowCancel || mForceCancel) QDialog::reject();

    tracede;
}

void DlgWait::handleResult(ErrCode err, void *data, void *result)
{
    traced;
    // TODO: should terminate thread???
    // TODO: callback is called when app is already closed???
    logd("err=%d", err);
    if (mFinishCallback) {
        mFinishCallback(err, data, result, this);
    }
    tracede;
}

DlgWaitWorker::DlgWaitWorker(QObject *parent, DlgWait* dlg, void* data):
    QThread(parent), mDlg(dlg), mRunFunc(nullptr), mData(data)
{
    traced;
}

void DlgWaitWorker::setRunFunc(const WaitRunt_t &newRunFunc)
{
    mRunFunc = newRunFunc;
}

void DlgWaitWorker::run()
{
    traced;
    ErrCode ret = ErrNone;
    void* result = nullptr;
    if (mRunFunc) {
        result = mRunFunc(&ret, mData, mDlg);
    }
    logd("signal done");
    emit done(ret, mData, result, mDlg);
    tracede;
}