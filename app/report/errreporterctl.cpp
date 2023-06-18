/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: errreporterctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/4/2023
 * Brief:
 */
#include "errreporterctl.h"
#include "mainwindow.h"
#include "utils.h"
#include "dialogutils.h"

GET_INSTANCE_IMPL(ErrReporterCtl)

ErrReporterCtl::ErrReporterCtl()
{
    tracein;
}

ErrReporterCtl::~ErrReporterCtl()
{
    tracein;
}

void ErrReporterCtl::reportErr(const QString &errMsg, ErrCode err,
                               bool showErrDlg,
                               bool delay)
{
    // TODO: report err, show public dialog????
    QString msg = QString("%1. Mã lỗi = %2").arg(errMsg).arg(err);
    loge("%s", STR2CHA(msg));
    logd("showErrDlg %d", showErrDlg);
    logd("delay %d", delay);
    if (showErrDlg) {
        DialogUtils::showErrorBox(msg);
    }
}
