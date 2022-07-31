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

#define ERR_NONE        (0)
#define ERR_FAILED      (-1)
#define ERR_INVALID     (-2)
#define ERR_NOT_SUPPORT (-3)
#define ERR_NOT_FOUND   (-4)
#define ERR_NOT_READY   (-5)
#define ERR_NOT_AVAIL   (-6)

typedef enum ErrCode{
    ErrNone = 0,
    ErrFailed,
    ErrInvalidArg,
    ErrInvalidData,
    ErrInvalidState,
    ErrInvalid,
    ErrNotSupport,
    ErrNotFound,
    ErrNotReady,
    ErrNotAvail,
    ErrFailSqlQuery,
    ErrNotExist,
    ErrFileRead,
    ErrExisted,
    ErrDbNotReady,

    ErrMax
    } ErrCode_t;
#endif // ERRCODE_H
