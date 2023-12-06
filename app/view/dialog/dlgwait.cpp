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
#include "logger.h"
#include "utils.h"
#define WAIT_TIMEOUT (10 * 1000)

DlgWait::DlgWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWait),
    mWorker(nullptr),
    mFinishCallback(nullptr),
    mResult(ErrNone)
{
    ui->setupUi(this);
    connect(this, SIGNAL(closeDlg(ErrCode)), this, SLOT(handleCloseDlg(ErrCode)));
}

DlgWait::~DlgWait()
{
    tracein;
    if (mWorker && mWorker->isRunning()) {
        logw("thread is still running, waiting for a while");
        if (!mWorker->wait(WAIT_TIMEOUT)) {
            logw("thread wait timeout! (%d ms), terminate it", WAIT_TIMEOUT);
            mWorker->terminate();
            mWorker->wait(WAIT_TIMEOUT);
        }
    }
    if (mWorker) {
        mWorker->deleteLater();
    }
    mWorker = nullptr;

    delete ui;
    traceout;
}

ErrCode DlgWait::show(void* data, WaitPrepare_t prepare,
                      WaitRunt_t run, WaitFinished_t finish)
{
    tracein;
    // TODO: multiple threads can be created, but only on dialog should be created
    // to avoid flicking when wait dialog show multiple time for multiple purpose
    ErrCode err = ErrNone;
    mForceCancel = false;
    if (mWorker) {
        mWorker->deleteLater();
    }
    mWorker = new DlgWaitWorker(this, this, data);
    mWorker->setRunFunc(run);
    connect(mWorker, &DlgWaitWorker::runFinish, this, &DlgWait::handleResult, Qt::QueuedConnection);
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
        // don't try below one, it takes no affect, no waiting
        // this->setModal(true);
        // this->show();
        err = mResult;
        logd("thread completed, err %d", err);
    }
    emit closeDlg(err);
    // TODO: delete mWorker?
    traceret(err);
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
    tracein;
    logd("perc %d", perc);
    ui->lblProgress->setText(QString("%1 %").arg(perc));
}

void DlgWait::setProgress(int cur, int total)
{
    tracein;
    logd("cur %d total %d", cur, total);
    ui->lblProgress->setText(QString("%1 / %2")
                                 .arg(QString::number(cur), QString::number(total)));

}

void DlgWait::clearProgress()
{
    tracein;
    ui->lblProgress->setText("");
}

void DlgWait::forceClose(DlgWait* dlg)
{
    tracein;
    mForceCancel = true;
    if (dlg) {
        logd("accept dialog");
//        dlg->accept();
        emit accepted();
    }
    QDialog::close();
    traceout;
}

void DlgWait::accept()
{
    tracein;
    if (mAllowCancel || mForceCancel) QDialog::accept();

    traceout;
}

void DlgWait::done(int val)
{
    tracein;
    if (mAllowCancel || mForceCancel) QDialog::done(val);

    traceout;
}

void DlgWait::reject()
{
    tracein;
    if (mAllowCancel || mForceCancel) QDialog::reject();

    traceout;
}

void DlgWait::handleResult(ErrCode err, void *data, void *result, DlgWait* dlg)
{
    tracein;
    // TODO: should terminate thread???
    // TODO: callback is called when app is already closed???
    logd("err=%d", err);
    if (mFinishCallback) {
        err = mFinishCallback(err, data, result, dlg);
    }
    setErrResult(err);
    forceClose(dlg);
    traceout;
}

void DlgWait::handleCloseDlg(ErrCode err)
{
    logd("handle close dialog, err=%d", err);
    // TODO: implemen this
}

ErrCode DlgWait::errResult() const
{
    return mResult;
}

void DlgWait::setErrResult(ErrCode newResult)
{
    mResult = newResult;
}

DlgWaitWorker::DlgWaitWorker(QObject *parent, DlgWait* dlg, void* data):
    QThread(parent), mRunFunc(nullptr), mData(data), mDlg(dlg)
{
    traced;
}

void DlgWaitWorker::setRunFunc(const WaitRunt_t &newRunFunc)
{
    traced;
    mRunFunc = newRunFunc;
}

DlgWaitWorker::~DlgWaitWorker()
{
    traced;
}

void DlgWaitWorker::run()
{
    tracein;
    ErrCode ret = ErrNone;
    QThread::setTerminationEnabled(true);
    void* result = nullptr;
    if (mRunFunc) {
        logd("call run func");
        result = mRunFunc(&ret, mData, mDlg);
    } else {
        logw("no run function to execute");
    }
    logd("signal done, ret %d", ret);
    emit runFinish(ret, mData, result, mDlg);
    traceout;
}
