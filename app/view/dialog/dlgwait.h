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
 * Filename: dlgwait.h
 * Author: Anh, Ngo Huy
 * Created date:12/6/2022
 * Brief:
 */
#ifndef DLGWAIT_H
#define DLGWAIT_H

#include <QDialog>
#include "errcode.h"
#include <QThread>

#define MAX_WAIT_TIME_MS (5*60*1000)

namespace Ui {
class DlgWait;
}


class DlgWait;
typedef std::function<ErrCode(void* data, DlgWait* dlg)> WaitPrepare_t;
/**
 * @brief run callback for progress dialog
 * @param err: output error code. Run function will return result via this param
 * @param data: input data for processing run
 * @param dlg: progress dialog
 */
typedef std::function<void*(ErrCode* err, void* data, DlgWait* dlg)> WaitRunt_t;
typedef std::function<ErrCode(ErrCode err, void* data, void* result, DlgWait* dlg)> WaitFinished_t;

class DlgWaitWorker: public QThread {
    Q_OBJECT
public:
    DlgWaitWorker(QObject *parent = nullptr,
                  DlgWait* dlg = nullptr,
                  void* data = nullptr);

    void setRunFunc(const WaitRunt_t &newRunFunc);
    virtual ~DlgWaitWorker();
public slots:
    void run() override;

signals:
    void runFinish(ErrCode err, void* data, void* result, DlgWait* dlg);
private:
    WaitRunt_t mRunFunc;
    void* mData;
    DlgWait* mDlg;
};

class DlgWait : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWait(QWidget *parent = nullptr);
    ~DlgWait();

    ErrCode show(void* data = nullptr,
                 WaitPrepare_t prepare = nullptr,
                 WaitRunt_t run = nullptr,
                 WaitFinished_t finish = nullptr);

    bool allowCancel() const;
    void setAllowCancel(bool newAllowCancel);

    const QString &title() const;
    void setTitle(const QString &newTitle);

    const QString &message() const;
    void setMessage(const QString &newMessage);

    void setProgress(int newProgress);
    void setProgress(int cur, int total);
    void clearProgress();
    void forceClose(DlgWait* dlg);
    ErrCode errResult() const;
    void setErrResult(ErrCode newResult);

protected:
    virtual void accept();
    virtual void done(int);
    virtual void reject();
signals:
    void closeDlg(ErrCode err);
public slots:
    void handleResult(ErrCode err, void* data, void* result, DlgWait* dlg);
    void handleCloseDlg(ErrCode err);
private:
    Ui::DlgWait *ui;
    bool mAllowCancel;
    bool mForceCancel;
    QString mTitle;
    QString mMessage;
    DlgWaitWorker* mWorker;
    WaitFinished_t mFinishCallback;
    ErrCode mResult;
};


#endif // DLGWAIT_H
