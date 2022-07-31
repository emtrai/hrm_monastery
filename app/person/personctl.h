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
 * Filename: personctl.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef PersonCTL_H
#define PersonCTL_H

#include <QObject>
#include "errcode.h"
#include "controller.h"
class Person;


// TODO: observer Person change?
class PersonCtl: public Controller
{
public:
    virtual ~PersonCtl();
    ErrCode_t addPerson(const Person* person);
private:
    PersonCtl();

public:
    static PersonCtl* getInstance();

private:
    static PersonCtl* gInstance;

public slots:
    virtual void onLoad();
};

#endif // PersonCTL_H
