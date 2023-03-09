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
 * Filename: errreporterctl.h
 * Author: Anh, Ngo Huy
 * Created date:3/4/2023
 * Brief:
 */
#ifndef ERRREPORTERCTL_H
#define ERRREPORTERCTL_H

#include "utils.h"
#include "errcode.h"

#define REPORTERRCTL ErrReporterCtl::getInstance()

class ErrReporterCtl
{
    GET_INSTANCE_DECL(ErrReporterCtl);
private:
    ErrReporterCtl();
    virtual ~ErrReporterCtl();
public:
    void reportErr(const QString& errMsg, ErrCode err,
                  bool showErrDlg = false, // show show error dialog
                  bool delay = false // delay true: not show dialog right now until delay = false
                  );
};

#endif // ERRREPORTERCTL_H
