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
 * Filename: idbspecialist.h
 * Author: Anh, Ngo Huy
 * Created date:8/8/2022
 * Brief:
 */
#ifndef IDBSPECIALIST_H
#define IDBSPECIALIST_H
#include <QList>
#include "errcode.h"


class Specialist;

class IDbSpecialist
{
public:

    virtual ErrCode addSpecialist(const Specialist* specialist) = 0;
    virtual bool exist(const Specialist* specialist) = 0;
    virtual QList<Specialist*> getAll() = 0;
};

#endif // IDBSPECIALIST_H
