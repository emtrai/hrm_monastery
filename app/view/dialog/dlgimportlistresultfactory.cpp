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
 * Filename: dlgimportlistresultfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:6/27/2023
 * Brief:
 */
#include "dlgimportlistresultfactory.h"
#include "logger.h"
#include "errcode.h"
#include "dlgimportlistresult.h"
#include "dlgimportcommunitylistresult.h"
#include "dlgimportareapersonlistresult.h"
#include <QWidget>

ErrCode DlgImportListResultFactory::getImportListResult(int target,
                                                        DlgImportListResult ** newdlg,
                                                        QWidget *parent)
{
    tracein;
    ErrCode err = ErrNone;
    DlgImportListResult* dlg = nullptr;
    if (!newdlg) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        switch (target) {
        case IMPORT_TARGET_COMMUNITY:
            dlg = new DlgImportCommunityListResult(parent);
            break;
        case IMPORT_TARGET_AREA_CONTACT:
            dlg = new DlgImportAreaPersonListResult(parent);
            break;
        default:
            logw("target %d not support, use default one", target);
            dlg = new DlgImportListResult(parent);
            break;
        }
    }

    if (err == ErrNone && !dlg) {
        err = ErrNoMemory;
        loge("cannot allocate new dlg, no memory?");
    }

    if (err == ErrNone) {
        *newdlg = dlg;
    }

    traceout;
    return err;
}
