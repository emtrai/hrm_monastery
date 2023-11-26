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
 * Filename: errcode.cpp
 * Author: Anh, Ngo Huy
 * Created date:11/25/2023
 * Brief:
 */
#include "errcode.h"

#define STR(s) #s
#define XSTR(s) STR(s)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const char* gErrString[] = {
    STR(ErrNone),
    STR(ErrFailed),
    STR(ErrNoMemory),
    STR(ErrInvalidArg),
    STR(ErrInvalidData),
    STR(ErrInvalidCond),
    STR(ErrInvalidState),
    STR(ErrInvalid),
    STR(ErrInvalidController),
    STR(ErrInvalidModel),
    STR(ErrNotSupport),
    STR(ErrNotFound),
    STR(ErrNotReady),
    STR(ErrNotAvail),
    STR(ErrFailSqlQuery),
    STR(ErrNotExist),
    STR(ErrFileOpen),
    STR(ErrFileWrite),
    STR(ErrFileRead),
    STR(ErrExisted),
    STR(ErrDbNotReady),
    STR(ErrUnknown),
    STR(ErrSkip),
    STR(ErrCancelled),
    STR(ErrNoData),
    STR(ErrNotImpl), // not implement
    STR(ErrBuildDataFailed),
    STR(ErrShortData),
    STR(ErrDenied),
    STR(ErrException),
    STR(ErrNoHandler),
    STR(ErrNoBuilder),
    STR(ErrDependency),
    STR(ErrSqlFailed),
    STR(ErrNoTable),
    STR(ErrFailedConvert),
    STR(ErrIncompatible),
    STR(ErrMigrationRequired),
    STR(ErrLoadFailed),
    STR(ErrOperatorFailed),
    STR(ErrSaveFailed),
    STR(ErrCreateDirFailed),
    STR(ErrFileOp),
    STR(ErrNotAllow),
    STR(ErrPermDenied),
    STR(ErrNoId),
    STR(ErrNoFile),
    STR(ErrInvalidView),
    STR(ErrNotSelect),
    STR(ErrAbort),
    STR(ErrInvalidQuery)
};

#define UNKNOWN_ERROR_CODE "Unknown error code"

const char *ErrMsg::errString(ErrCode err)
{
    if (err < ARRAY_SIZE(gErrString)) {
        return gErrString[err];
    } else {
        return UNKNOWN_ERROR_CODE;
    }
}
