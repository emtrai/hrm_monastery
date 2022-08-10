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
 * Filename: idbsaint.h
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#ifndef IDBSAINT_H
#define IDBSAINT_H

#include "errcode.h"
#include <QHash>
class Saint;

class IDbSaint
{
public:

    virtual ErrCode addSaint(const Saint* saint) = 0;
    virtual bool exist(const Saint* saint) = 0;
    virtual QHash<QString, Saint*> getListSaint() = 0;
};

#endif // IDBSAINT_H
