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
 * Filename: errcode.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef ERRCODE_H
#define ERRCODE_H

#include <QString>

typedef enum ErrCode {
    ErrNone = 0,
    ErrFailed,
    ErrNoMemory,
    ErrInvalidArg,
    ErrInvalidData,
    ErrInvalidCond,
    ErrInvalidState,
    ErrInvalid,
    ErrInvalidController,
    ErrInvalidModel,
    ErrNotSupport,
    ErrNotFound,
    ErrNotReady,
    ErrNotAvail,
    ErrFailSqlQuery,
    ErrNotExist,
    ErrFileOpen,
    ErrFileWrite,
    ErrFileRead,
    ErrExisted,
    ErrDbNotReady,
    ErrUnknown,
    ErrSkip,
    ErrCancelled,
    ErrNoData,
    ErrNotImpl, // not implement
    ErrBuildDataFailed,
    ErrShortData,
    ErrDenied,
    ErrException,
    ErrNoHandler,
    ErrNoBuilder,
    ErrDependency,
    ErrSqlFailed,
    ErrNoTable,
    ErrFailedConvert,
    ErrIncompatible,
    ErrMigrationRequired,
    ErrLoadFailed,
    ErrOperatorFailed,
    ErrSaveFailed,
    ErrFileOp,
    ErrNotAllow,
    ErrPermDenied,
    ErrNoId,
    ErrNoFile,
    ErrInvalidView,
    ErrNotSelect,
    ErrAbort,
    ErrInvalidQuery,
    ErrMax
    } ErrCode_t;

class ErrMsg {
public:
    ErrMsg(ErrCode code, const QString& msg) {
        mCode = code;
        mMsg = msg;
    }

    ErrMsg(ErrCode code) {
        mCode = code;
    }

    inline ErrCode code() const {
        return mCode;
    }
    inline void setCode(ErrCode newCode)
    {
        mCode = newCode;
    }

    inline const QString &msg() const
    {
        return mMsg;
    }

    inline void setMsg(const QString &newMsg)
    {
        mMsg = newMsg;
    }


private:
    ErrCode mCode;
    QString mMsg;
};



#endif // ERRCODE_H
